/* SHA512-based Unix crypt implementation.
   Released into the Public Domain by Ulrich Drepper <drepper@redhat.com>.  */
// Ported from https://www.akkadia.org/drepper/SHA-crypt.txt

#include "sha512.h"
#ifdef _WIN32
#pragma warning(push, 0)
#endif

#include <malloc.h>

#ifdef _WIN32
typedef unsigned __int64 UW64;
#define SNPRINTF _snprintf
#define ALIGN8 _declspec(align(8))
#else
typedef uint64_t UW64;
#define SNPRINTF snprintf
#define ALIGN8 __attribute__((aligned(8)))
#endif


#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)

/* Structure to save state of computation between the single steps.  */
struct sha512_ctx {
	UW64 H[8];

	UW64 total[2];
	UW64 buflen;
	char buffer[256];   /* NB: always correctly aligned for UW64.  */
};


#if __BYTE_ORDER == __LITTLE_ENDIAN
# define SWAP(n) \
  (((n) << 56)                  \
   | (((n) & 0xff00) << 40)         \
   | (((n) & 0xff0000) << 24)           \
   | (((n) & 0xff000000) << 8)          \
   | (((n) >> 8) & 0xff000000)          \
   | (((n) >> 24) & 0xff0000)           \
   | (((n) >> 40) & 0xff00)         \
   | ((n) >> 56))
#else
# define SWAP(n) (n)
#endif


/* This array contains the bytes used to pad the buffer to the next
   64-byte boundary.  (FIPS 180-2:5.1.2)  */
static const unsigned char fillbuf[128] = { 0x80, 0 /* , 0, 0, ...  */ };


/* Constants for SHA512 from FIPS 180-2:4.2.3.  */
static const UW64 K[80] = {
	UW64 (0x428a2f98d728ae22), UW64 (0x7137449123ef65cd),
	UW64 (0xb5c0fbcfec4d3b2f), UW64 (0xe9b5dba58189dbbc),
	UW64 (0x3956c25bf348b538), UW64 (0x59f111f1b605d019),
	UW64 (0x923f82a4af194f9b), UW64 (0xab1c5ed5da6d8118),
	UW64 (0xd807aa98a3030242), UW64 (0x12835b0145706fbe),
	UW64 (0x243185be4ee4b28c), UW64 (0x550c7dc3d5ffb4e2),
	UW64 (0x72be5d74f27b896f), UW64 (0x80deb1fe3b1696b1),
	UW64 (0x9bdc06a725c71235), UW64 (0xc19bf174cf692694),
	UW64 (0xe49b69c19ef14ad2), UW64 (0xefbe4786384f25e3),
	UW64 (0x0fc19dc68b8cd5b5), UW64 (0x240ca1cc77ac9c65),
	UW64 (0x2de92c6f592b0275), UW64 (0x4a7484aa6ea6e483),
	UW64 (0x5cb0a9dcbd41fbd4), UW64 (0x76f988da831153b5),
	UW64 (0x983e5152ee66dfab), UW64 (0xa831c66d2db43210),
	UW64 (0xb00327c898fb213f), UW64 (0xbf597fc7beef0ee4),
	UW64 (0xc6e00bf33da88fc2), UW64 (0xd5a79147930aa725),
	UW64 (0x06ca6351e003826f), UW64 (0x142929670a0e6e70),
	UW64 (0x27b70a8546d22ffc), UW64 (0x2e1b21385c26c926),
	UW64 (0x4d2c6dfc5ac42aed), UW64 (0x53380d139d95b3df),
	UW64 (0x650a73548baf63de), UW64 (0x766a0abb3c77b2a8),
	UW64 (0x81c2c92e47edaee6), UW64 (0x92722c851482353b),
	UW64 (0xa2bfe8a14cf10364), UW64 (0xa81a664bbc423001),
	UW64 (0xc24b8b70d0f89791), UW64 (0xc76c51a30654be30),
	UW64 (0xd192e819d6ef5218), UW64 (0xd69906245565a910),
	UW64 (0xf40e35855771202a), UW64 (0x106aa07032bbd1b8),
	UW64 (0x19a4c116b8d2d0c8), UW64 (0x1e376c085141ab53),
	UW64 (0x2748774cdf8eeb99), UW64 (0x34b0bcb5e19b48a8),
	UW64 (0x391c0cb3c5c95a63), UW64 (0x4ed8aa4ae3418acb),
	UW64 (0x5b9cca4f7763e373), UW64 (0x682e6ff3d6b2b8a3),
	UW64 (0x748f82ee5defb2fc), UW64 (0x78a5636f43172f60),
	UW64 (0x84c87814a1f0ab72), UW64 (0x8cc702081a6439ec),
	UW64 (0x90befffa23631e28), UW64 (0xa4506cebde82bde9),
	UW64 (0xbef9a3f7b2c67915), UW64 (0xc67178f2e372532b),
	UW64 (0xca273eceea26619c), UW64 (0xd186b8c721c0c207),
	UW64 (0xeada7dd6cde0eb1e), UW64 (0xf57d4f7fee6ed178),
	UW64 (0x06f067aa72176fba), UW64 (0x0a637dc5a2c898a6),
	UW64 (0x113f9804bef90dae), UW64 (0x1b710b35131c471b),
	UW64 (0x28db77f523047d84), UW64 (0x32caab7b40c72493),
	UW64 (0x3c9ebe0a15c9bebc), UW64 (0x431d67c49c100d4c),
	UW64 (0x4cc5d4becb3e42b6), UW64 (0x597f299cfc657e2a),
	UW64 (0x5fcb6fab3ad6faec), UW64 (0x6c44198c4a475817)
};


/* Process LEN bytes of BUFFER, accumulating context into CTX.
   It is assumed that LEN % 128 == 0.  */
static void
sha512_process_block (const void *buffer, size_t len, struct sha512_ctx *ctx)
{
	const UW64 *words = (UW64*) buffer;
	size_t nwords = len / sizeof (UW64);
	UW64 a = ctx->H[0];
	UW64 b = ctx->H[1];
	UW64 c = ctx->H[2];
	UW64 d = ctx->H[3];
	UW64 e = ctx->H[4];
	UW64 f = ctx->H[5];
	UW64 g = ctx->H[6];
	UW64 h = ctx->H[7];

	/* First increment the byte count.  FIPS 180-2 specifies the possible
	   length of the file up to 2^128 bits.  Here we only compute the
	   number of bytes.  Do a double word increment.  */
	ctx->total[0] += len;
	if (ctx->total[0] < len)
		++ctx->total[1];

	/* Process all bytes in the buffer with 128 bytes in each round of
	   the loop.  */
	while (nwords > 0) {
		UW64 W[80];
		UW64 a_save = a;
		UW64 b_save = b;
		UW64 c_save = c;
		UW64 d_save = d;
		UW64 e_save = e;
		UW64 f_save = f;
		UW64 g_save = g;
		UW64 h_save = h;

		/* Operators defined in FIPS 180-2:4.1.2.  */
#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define S0(x) (CYCLIC (x, 28) ^ CYCLIC (x, 34) ^ CYCLIC (x, 39))
#define S1(x) (CYCLIC (x, 14) ^ CYCLIC (x, 18) ^ CYCLIC (x, 41))
#define R0(x) (CYCLIC (x, 1) ^ CYCLIC (x, 8) ^ (x >> 7))
#define R1(x) (CYCLIC (x, 19) ^ CYCLIC (x, 61) ^ (x >> 6))

		/* It is unfortunate that C does not provide an operator for
		cyclic rotation.  Hope the C compiler is smart enough.  */
#define CYCLIC(w, s) ((w >> s) | (w << (64 - s)))

		/* Compute the message schedule according to FIPS 180-2:6.3.2 step 2.  */
		for (unsigned int t = 0; t < 16; ++t) {
			W[t] = SWAP (*words);
			++words;
		}
		for (unsigned int t = 16; t < 80; ++t)
			W[t] = R1 (W[t - 2]) + W[t - 7] + R0 (W[t - 15]) + W[t - 16];

		/* The actual computation according to FIPS 180-2:6.3.2 step 3.  */
		for (unsigned int t = 0; t < 80; ++t) {
			UW64 T1 = h + S1 (e) + Ch (e, f, g) + K[t] + W[t];
			UW64 T2 = S0 (a) + Maj (a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		/* Add the starting values of the context according to FIPS 180-2:6.3.2
		step 4.  */
		a += a_save;
		b += b_save;
		c += c_save;
		d += d_save;
		e += e_save;
		f += f_save;
		g += g_save;
		h += h_save;

		/* Prepare for the next round.  */
		nwords -= 16;
	}

	/* Put checksum in context given as argument.  */
	ctx->H[0] = a;
	ctx->H[1] = b;
	ctx->H[2] = c;
	ctx->H[3] = d;
	ctx->H[4] = e;
	ctx->H[5] = f;
	ctx->H[6] = g;
	ctx->H[7] = h;
}


/* Initialize structure containing state of computation.
   (FIPS 180-2:5.3.3)  */
static void
sha512_init_ctx (struct sha512_ctx *ctx)
{
	ctx->H[0] = UW64 (0x6a09e667f3bcc908);
	ctx->H[1] = UW64 (0xbb67ae8584caa73b);
	ctx->H[2] = UW64 (0x3c6ef372fe94f82b);
	ctx->H[3] = UW64 (0xa54ff53a5f1d36f1);
	ctx->H[4] = UW64 (0x510e527fade682d1);
	ctx->H[5] = UW64 (0x9b05688c2b3e6c1f);
	ctx->H[6] = UW64 (0x1f83d9abfb41bd6b);
	ctx->H[7] = UW64 (0x5be0cd19137e2179);

	ctx->total[0] = ctx->total[1] = 0;
	ctx->buflen = 0;
}


/* Process the remaining bytes in the internal buffer and the usual
   prolog according to the standard and write the result to RESBUF.

   IMPORTANT: On some systems it is required that RESBUF is correctly
   aligned for a 32 bits value.  */
static void *
sha512_finish_ctx (struct sha512_ctx *ctx, void *resbuf)
{
	/* Take yet unprocessed bytes into account.  */
	UW64 bytes = ctx->buflen;
	size_t pad;

	/* Now count remaining bytes.  */
	ctx->total[0] += bytes;
	if (ctx->total[0] < bytes)
		++ctx->total[1];

	pad = bytes >= 112 ? 128 + 112 - bytes : 112 - bytes;
	memcpy (&ctx->buffer[bytes], fillbuf, pad);

	/* Put the 128-bit file length in *bits* at the end of the buffer.  */
	*(UW64 *) &ctx->buffer[bytes + pad + 8] = SWAP (ctx->total[0] << 3);
	*(UW64 *) &ctx->buffer[bytes + pad] = SWAP ((ctx->total[1] << 3) |
	                                      (ctx->total[0] >> 61));

	/* Process last bytes.  */
	sha512_process_block (ctx->buffer, bytes + pad + 16, ctx);

	/* Put result from CTX in first 64 bytes following RESBUF.  */
	for (unsigned int i = 0; i < 8; ++i)
		((UW64 *) resbuf)[i] = SWAP (ctx->H[i]);

	return resbuf;
}


static void
sha512_process_bytes (const void *buffer, size_t len, struct sha512_ctx *ctx)
{
	/* When we already have some bits in our internal buffer concatenate
	   both inputs first.  */
	if (ctx->buflen != 0) {
		size_t left_over = ctx->buflen;
		size_t add = 256 - left_over > len ? len : 256 - left_over;

		memcpy (&ctx->buffer[left_over], buffer, add);
		ctx->buflen += add;

		if (ctx->buflen > 128) {
			sha512_process_block (ctx->buffer, ctx->buflen & ~127, ctx);

			ctx->buflen &= 127;
			/* The regions in the following copy operation cannot overlap.  */
			memcpy (ctx->buffer, &ctx->buffer[(left_over + add) & ~127],
			        ctx->buflen);
		}

		buffer = (const char *) buffer + add;
		len -= add;
	}

	/* Process available complete blocks.  */
	if (len >= 128) {
#if !_STRING_ARCH_unaligned
		/* To check alignment gcc has an appropriate operator.  Other
		   compilers don't.  */
# if __GNUC__ >= 2
#  define UNALIGNED_P(p) (((uintptr_t) p) % sizeof (UW64) != 0)
# else
#  define UNALIGNED_P(p) (((uintptr_t) p) % sizeof (UW64) != 0)
# endif
		if (UNALIGNED_P (buffer))
			while (len > 128) {
				sha512_process_block (memcpy (ctx->buffer, buffer, 128), 128,
				                      ctx);
				buffer = (const char *) buffer + 128;
				len -= 128;
			}
		else
#endif
		{
			sha512_process_block (buffer, len & ~127, ctx);
			buffer = (const char *) buffer + (len & ~127);
			len &= 127;
		}
	}

	/* Move remaining bytes into internal buffer.  */
	if (len > 0) {
		size_t left_over = ctx->buflen;

		memcpy (&ctx->buffer[left_over], buffer, len);
		left_over += len;
		if (left_over >= 128) {
			sha512_process_block (ctx->buffer, 128, ctx);
			left_over -= 128;
			memcpy (ctx->buffer, &ctx->buffer[128], left_over);
		}
		ctx->buflen = left_over;
	}
}


/* Define our magic string to mark salt for SHA512 "encryption"
   replacement.  */
static const char sha512_salt_prefix[] = "$6$";

/* Prefix for optional rounds specification.  */
static const char sha512_rounds_prefix[] = "rounds=";

/* Maximum salt string length.  */
#define SALT_LEN_MAX 16
/* Default number of rounds if not explicitly specified.  */
#define ROUNDS_DEFAULT 5000
/* Minimum number of rounds.  */
#define ROUNDS_MIN 1000
/* Maximum number of rounds.  */
#define ROUNDS_MAX 999999999

/* Table with characters for base64 transformation.  */
static const char b64t[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";


static char* sha512_crypt_r (const char *key, const char *salt, char *buffer, int buflen)
{
	ALIGN8 unsigned char alt_result[64];
	ALIGN8 unsigned char temp_result[64];
	struct sha512_ctx ctx;
	struct sha512_ctx alt_ctx;
	size_t salt_len;
	size_t key_len;
	size_t cnt;
	char *cp;
	char *copied_key = NULL;
	char *copied_salt = NULL;
	char *p_bytes;
	char *s_bytes;
	/* Default number of rounds.  */
	size_t rounds = ROUNDS_DEFAULT;
	bool rounds_custom = false;

	/* Find beginning of salt string.  The prefix should normally always
	   be present.  Just in case it is not.  */
	if (strncmp (sha512_salt_prefix, salt, sizeof (sha512_salt_prefix) - 1) == 0)
		/* Skip salt prefix.  */
		salt += sizeof (sha512_salt_prefix) - 1;

	if (strncmp (salt, sha512_rounds_prefix, sizeof (sha512_rounds_prefix) - 1)
	        == 0) {
		const char *num = salt + sizeof (sha512_rounds_prefix) - 1;
		char *endp;
		unsigned long int srounds = strtoul (num, &endp, 10);
		if (*endp == '$') {
			salt = endp + 1;
			rounds = MAX (ROUNDS_MIN, MIN (srounds, ROUNDS_MAX));
			rounds_custom = true;
		}
	}

	salt_len = MIN (strcspn (salt, "$"), SALT_LEN_MAX);
	key_len = strlen (key);


	if ((key - (char *) 0) % sizeof (UW64) != 0) {
		char *tmp = (char *) alloca (key_len + sizeof (UW64));
		key = copied_key = (char*)  memcpy (tmp + sizeof (UW64) - (tmp - (char *) 0) % sizeof (UW64),  key, key_len);
	}

	if ((salt - (char *) 0) % sizeof (UW64) != 0) {
		char *tmp = (char *) alloca (salt_len + sizeof (UW64));
		salt = copied_salt = (char*) memcpy (tmp + sizeof (UW64) - (tmp - (char *) 0) % sizeof (UW64), salt, salt_len);
	}


	/* Prepare for the real work.  */
	sha512_init_ctx (&ctx);

	/* Add the key string.  */
	sha512_process_bytes (key, key_len, &ctx);

	/* The last part is the salt string.  This must be at most 16
	   characters and it ends at the first `$' character (for
	   compatibility with existing implementations).  */
	sha512_process_bytes (salt, salt_len, &ctx);


	/* Compute alternate SHA512 sum with input KEY, SALT, and KEY.  The
	   final result will be added to the first context.  */
	sha512_init_ctx (&alt_ctx);

	/* Add key.  */
	sha512_process_bytes (key, key_len, &alt_ctx);

	/* Add salt.  */
	sha512_process_bytes (salt, salt_len, &alt_ctx);

	/* Add key again.  */
	sha512_process_bytes (key, key_len, &alt_ctx);

	/* Now get result of this (64 bytes) and add it to the other
	   context.  */
	sha512_finish_ctx (&alt_ctx, alt_result);

	/* Add for any character in the key one byte of the alternate sum.  */
	for (cnt = key_len; cnt > 64; cnt -= 64)
		sha512_process_bytes (alt_result, 64, &ctx);
	sha512_process_bytes (alt_result, cnt, &ctx);

	/* Take the binary representation of the length of the key and for every
	   1 add the alternate sum, for every 0 the key.  */
	for (cnt = key_len; cnt > 0; cnt >>= 1)
		if ((cnt & 1) != 0)
			sha512_process_bytes (alt_result, 64, &ctx);
		else
			sha512_process_bytes (key, key_len, &ctx);

	/* Create intermediate result.  */
	sha512_finish_ctx (&ctx, alt_result);

	/* Start computation of P byte sequence.  */
	sha512_init_ctx (&alt_ctx);

	/* For every character in the password add the entire password.  */
	for (cnt = 0; cnt < key_len; ++cnt)
		sha512_process_bytes (key, key_len, &alt_ctx);

	/* Finish the digest.  */
	sha512_finish_ctx (&alt_ctx, temp_result);

	/* Create byte sequence P.  */
	cp = p_bytes = (char*) alloca (key_len);
	for (cnt = key_len; cnt >= 64; cnt -= 64)
		cp = (char*) memcpy (cp, temp_result, 64) + 64;
	memcpy (cp, temp_result, cnt);

	/* Start computation of S byte sequence.  */
	sha512_init_ctx (&alt_ctx);

	/* For every character in the password add the entire password.  */
	for (cnt = 0; cnt < 16 + alt_result[0]; ++cnt)
		sha512_process_bytes (salt, salt_len, &alt_ctx);

	/* Finish the digest.  */
	sha512_finish_ctx (&alt_ctx, temp_result);

	/* Create byte sequence S.  */
	cp = s_bytes = (char*) alloca (salt_len);
	for (cnt = salt_len; cnt >= 64; cnt -= 64)
		cp = (char*) memcpy (cp, temp_result, 64) + 64;
	memcpy (cp, temp_result, cnt);

	/* Repeatedly run the collected hash value through SHA512 to burn
	   CPU cycles.  */
	for (cnt = 0; cnt < rounds; ++cnt) {
		/* New context.  */
		sha512_init_ctx (&ctx);

		/* Add key or last result.  */
		if ((cnt & 1) != 0)
			sha512_process_bytes (p_bytes, key_len, &ctx);
		else
			sha512_process_bytes (alt_result, 64, &ctx);

		/* Add salt for numbers not divisible by 3.  */
		if (cnt % 3 != 0)
			sha512_process_bytes (s_bytes, salt_len, &ctx);

		/* Add key for numbers not divisible by 7.  */
		if (cnt % 7 != 0)
			sha512_process_bytes (p_bytes, key_len, &ctx);

		/* Add key or last result.  */
		if ((cnt & 1) != 0)
			sha512_process_bytes (alt_result, 64, &ctx);
		else
			sha512_process_bytes (p_bytes, key_len, &ctx);

		/* Create intermediate result.  */
		sha512_finish_ctx (&ctx, alt_result);
	}

	/* Now we can construct the result string.  It consists of three
	   parts.  */
	cp = strncpy (buffer, sha512_salt_prefix, MAX (0, buflen));
	cp+=sizeof (sha512_salt_prefix) - 1;
	buflen -= sizeof (sha512_salt_prefix) - 1;

	if (rounds_custom) {
		int n = SNPRINTF (cp, MAX (0, buflen), "%s%u$",
		                  sha512_rounds_prefix, rounds);
		cp += n;
		buflen -= n;
	}

	cp = strncpy (cp, salt, MIN ((size_t) MAX (0, buflen), salt_len));
	buflen -= MIN ((size_t) MAX (0, buflen), salt_len);
	cp+=MIN ((size_t) MAX (0, buflen), salt_len);

	if (buflen > 0) {
		*cp++ = '$';
		--buflen;
	}

#define b64_from_24bit(B2, B1, B0, N)                         \
  do {                                        \
    unsigned int w = ((B2) << 16) | ((B1) << 8) | (B0);               \
    int n = (N);                                  \
    while (n-- > 0 && buflen > 0)                         \
      {                                       \
    *cp++ = b64t[w & 0x3f];                           \
    --buflen;                                 \
    w >>= 6;                                  \
      }                                       \
  } while (0)

	b64_from_24bit (alt_result[0], alt_result[21], alt_result[42], 4);
	b64_from_24bit (alt_result[22], alt_result[43], alt_result[1], 4);
	b64_from_24bit (alt_result[44], alt_result[2], alt_result[23], 4);
	b64_from_24bit (alt_result[3], alt_result[24], alt_result[45], 4);
	b64_from_24bit (alt_result[25], alt_result[46], alt_result[4], 4);
	b64_from_24bit (alt_result[47], alt_result[5], alt_result[26], 4);
	b64_from_24bit (alt_result[6], alt_result[27], alt_result[48], 4);
	b64_from_24bit (alt_result[28], alt_result[49], alt_result[7], 4);
	b64_from_24bit (alt_result[50], alt_result[8], alt_result[29], 4);
	b64_from_24bit (alt_result[9], alt_result[30], alt_result[51], 4);
	b64_from_24bit (alt_result[31], alt_result[52], alt_result[10], 4);
	b64_from_24bit (alt_result[53], alt_result[11], alt_result[32], 4);
	b64_from_24bit (alt_result[12], alt_result[33], alt_result[54], 4);
	b64_from_24bit (alt_result[34], alt_result[55], alt_result[13], 4);
	b64_from_24bit (alt_result[56], alt_result[14], alt_result[35], 4);
	b64_from_24bit (alt_result[15], alt_result[36], alt_result[57], 4);
	b64_from_24bit (alt_result[37], alt_result[58], alt_result[16], 4);
	b64_from_24bit (alt_result[59], alt_result[17], alt_result[38], 4);
	b64_from_24bit (alt_result[18], alt_result[39], alt_result[60], 4);
	b64_from_24bit (alt_result[40], alt_result[61], alt_result[19], 4);
	b64_from_24bit (alt_result[62], alt_result[20], alt_result[41], 4);
	b64_from_24bit (0, 0, alt_result[63], 2);

	if (buflen <= 0) {

		buffer = NULL;
	}
	else
		*cp = '\0';     /* Terminate the string.  */

	/* Clear the buffer for the intermediate result so that people
	   attaching to processes or reading core dumps cannot get any
	   information.  We do it in this way to clear correct_words[]
	   inside the SHA512 implementation as well.  */
	sha512_init_ctx (&ctx);
	sha512_finish_ctx (&ctx, alt_result);
	memset (temp_result, '\0', sizeof (temp_result));
	memset (p_bytes, '\0', key_len);
	memset (s_bytes, '\0', salt_len);
	memset (&ctx, '\0', sizeof (ctx));
	memset (&alt_ctx, '\0', sizeof (alt_ctx));
	if (copied_key != NULL)
		memset (copied_key, '\0', key_len);
	if (copied_salt != NULL)
		memset (copied_salt, '\0', salt_len);

	return buffer;
}


/* This entry point is equivalent to the `crypt' function in Unix
   libcs.  */
char *
sha512_crypt (const char *key, const char *salt)
{
	/* We don't want to have an arbitrary limit in the size of the
	   password.  We can compute an upper bound for the size of the
	   result in advance and so we can prepare the buffer we pass to
	   `sha512_crypt_r'.  */
	char *buffer=NULL;
	int buflen=0;
	int needed = (sizeof (sha512_salt_prefix) - 1
	              + sizeof (sha512_rounds_prefix) + 9 + 1
	              + strlen (salt) + 1 + 86 + 1);

	if (buflen < needed) {
		char *new_buffer = (char *) realloc (buffer, needed);
		if (new_buffer == NULL)
			return NULL;

		buffer = new_buffer;
		buflen = needed;
	}

	return sha512_crypt_r (key, salt, buffer, buflen);
}

#ifdef _WIN32
#pragma warning(pop)
#endif

bool sha512 (WCSTR input, WCSTR salt, WString& output)
{
	char* ret = sha512_crypt ((WCSTR) input, (WCSTR) salt);
	if (ret) {
		output = (WCSTR) ret;
		free (ret);
		return true;
	}
	return false;
}

bool sha512_check (WCSTR input, WCSTR enc)
{
	WString check;
	sha512 (input, enc, check);
	return !strcmp (enc, check);
}


void* sha512_init ()
{
	sha512_ctx* ctx = (sha512_ctx*) malloc (sizeof (sha512_ctx));
	if (ctx) {
		sha512_init_ctx (ctx);
	}
	return ctx;
}

void sha512_update (void* ctx, void* buf, unsigned int len)
{
	sha512_process_bytes (buf, len, (sha512_ctx*) ctx);
}

void sha512_finish (void* ctx, void* hash)
{
	sha512_finish_ctx ((sha512_ctx*) ctx, hash);
	free (ctx);
}

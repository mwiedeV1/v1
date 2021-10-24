#ifndef _BASE64_H_
#define _BASE64_H_

#ifdef __cplusplus
extern "C" {
#endif

int Base64encode_len(int len);
int Base64encode(char * coded_dst, const char *plain_src,int len_plain_src /* , int breakAfter=-1 */);

int Base64decode_len(const char * coded_src);
int Base64decode(char * plain_dst, const char *coded_src);

#ifdef __cplusplus
}
#endif

#endif //_BASE64_H_

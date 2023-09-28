#################################################################
# This makefile create V1 on Linux systems
# You need g++ and make
#
# sudo apt-get install g++ make
#
# Usage:
# make v1-core
#
# Optional:
# make v1-lib
#
# Compile extensions:
# Read details: https://v1-script.net/compile-extensions.html
# make v1-extensions
#
# Create archives and install library:
# sudo make install
# make clean
#################################################################

PROGRAM_NAME        = v1
PROGRAM_VERSION     = 0.96
SYSTEM              = $(shell uname)
ARCH                = $(shell uname -m)
CC                  = gcc
LIB_DIR             = /usr/local/lib

# Definitions for extensions
# Read details in README.txt 
EXTENSION_DIR       = extensions

# Open SSL dependencies
OPENSSL_DIR         = $(EXTENSION_DIR)/ssl/openssl-1.0.2u
LIBSSL              = $(OPENSSL_DIR)/libssl.a
LIBCRYPTO           = $(OPENSSL_DIR)/libcrypto.a

# PCRE dependencies
PCRE_DIR            = $(EXTENSION_DIR)/regex/pcre-8.43
LIBPCRE             = $(PCRE_DIR)/.libs/libpcre.a

# GDLib dependencies
# Read details: https://v1-script.net/compile-extension-gd.html
GDLIB_DIR           = $(EXTENSION_DIR)/gd/libgd-2.3.0
LIBGD               = -lgd
LIBJPEG             =
LIBTIFF             = 
LIBXPM              = 
LIBZ                = 
LIBFREETYPE         = 
LIBPNG              =	
LIBWEBP             = 

# DynCall dependencies
DYNCALL_DIR         = dep/dyncall-1.2/dyncall
DYNCALL_OBJECTS     = dyncall_api.o dyncall_callvm.o dyncall_vector.o dyncall_callf.o dyncall_callvm_base.o

# Main definitions
INST_DIR            = install
MAIN_DEFINES        = -D_REENTRANT -D_LINUX -D_GNU_SOURCE -std=c++0x -Wall -fPIC -Ofast
DEFINES             = $(MAIN_DEFINES) 

ifeq (x86_64,$(ARCH))
EXTRA_FLAGS         = -mpreferred-stack-boundary=4
DYNCALL_OBJECTS     =
EXTRA_FLAGS         = -I$(DYNCALL_DIR)
# DYNCALL_OBJECTS  := $(DYNCALL_OBJECTS) dyncall_call_x64.o
# EXTRA_FLAGS       = -I$(DYNCALL_DIR) -D_WITH_DYNCALL 
else ifeq (i686,$(ARCH))
DYNCALL_OBJECTS    := $(DYNCALL_OBJECTS) dyncall_call_x86.o
EXTRA_FLAGS         = -I$(DYNCALL_DIR) -D_WITH_DYNCALL -DDC_UNIX
else ifeq (i386,$(ARCH))
DYNCALL_OBJECTS    := $(DYNCALL_OBJECTS) dyncall_call_x86.o
EXTRA_FLAGS         = -I$(DYNCALL_DIR) -D_WITH_DYNCALL -DDC_UNIX
else ifeq (armv7l,$(ARCH))
DYNCALL_OBJECTS    := $(DYNCALL_OBJECTS) dyncall_call_arm32_arm_armhf.o
EXTRA_FLAGS         = -I$(DYNCALL_DIR) -D_WITH_DYNCALL -DDC_UNIX
else ifeq (armv6l,$(ARCH))
DYNCALL_OBJECTS    := $(DYNCALL_OBJECTS) dyncall_call_arm32_arm_armhf.o
EXTRA_FLAGS         = -I$(DYNCALL_DIR) -D_WITH_DYNCALL -DDC_UNIX
else ifeq (aarch64,$(ARCH))
EXTRA_FLAGS         = -mpreferred-stack-boundary=4
DYNCALL_OBJECTS    := $(DYNCALL_OBJECTS) dyncall_call_arm64.o
EXTRA_FLAGS         = -I$(DYNCALL_DIR) -D_WITH_DYNCALL -DDC_UNIX
else
EXTRA_FLAGS         = -I$(DYNCALL_DIR) -D_WITH_DYNCALL
endif

C_FLAGS             = $(OS_C_FLAGS) $(EXTRA_FLAGS) $(DEFINES) -I$(WLIB_DIR) -w 
LD_FLAGS            = $(OS_LD_FLAGS) -s -L./ -lstdc++ -lpthread -ldl -lm -lrt

WLIB_DIR            = wlib
WLIB_DEPENDS        = $(WLIB_DIR)/constants.h $(WLIB_DIR)/types.h $(WLIB_DIR)/wbase.h $(WLIB_DIR)/wlib.h
WLIB_BASE_OBJECTS   = WString.o WException.o WMemory.o WSystem.o WDate.o
WLIB_OBJECTS        = $(WLIB_BASE_OBJECTS) WFile.o WSocket.o WThread.o Process.o WStringTokenizer.o WInitFile.o URL.o CGIUtils.o HTTPClient.o

V1_DEPENDS          = wscript.h ast.h v1.h
V1_OBJECTS          = ast.o global.o interpret.o wscript.flex.o wscript.tab.o base64.o blowfish.o des.o md5.o sha256.o sha512.o $(WLIB_OBJECTS)

$(PROGRAM_NAME): main.o $(V1_OBJECTS) $(DYNCALL_OBJECTS)
	$(CC) $(C_FLAGS) main.o $(V1_OBJECTS) $(DYNCALL_OBJECTS) -o $(PROGRAM_NAME) $(LD_FLAGS)
	
libv1.so: v1lib.o $(V1_OBJECTS) $(DYNCALL_OBJECTS)
	$(CC) $(C_FLAGS) v1lib.o $(V1_OBJECTS) $(DYNCALL_OBJECTS) -shared -o lib$(PROGRAM_NAME).so $(LD_FLAGS)

crypt.so: $(EXTENSION_DIR)/crypt/crypt.o blowfish.o des.o md5.o sha256.o sha512.o sha1.o $(WLIB_BASE_OBJECTS)
	$(CC) $(C_FLAGS) $(WLIB_BASE_OBJECTS) sha1.o sha512.o sha256.o md5.o des.o blowfish.o $(EXTENSION_DIR)/crypt/crypt.o -shared -o crypt.so $(LD_FLAGS)

ssl.so: $(EXTENSION_DIR)/ssl/ssl.o $(WLIB_BASE_OBJECTS)
	$(CC) $(C_FLAGS) $(WLIB_BASE_OBJECTS) $(EXTENSION_DIR)/ssl/ssl.o $(LIBSSL) $(LIBCRYPTO) -shared -o ssl.so $(LD_FLAGS)

regex.so: $(EXTENSION_DIR)/regex/regex.o $(WLIB_BASE_OBJECTS)
	$(CC) $(C_FLAGS) $(WLIB_BASE_OBJECTS) $(EXTENSION_DIR)/regex/regex.o $(LIBPCRE) -shared -o regex.so $(LD_FLAGS)
	
gd.so: $(EXTENSION_DIR)/gd/gd.o $(WLIB_BASE_OBJECTS) $(DYNCALL_OBJECTS)
	$(CC) $(C_FLAGS) $(WLIB_BASE_OBJECTS) $(DYNCALL_OBJECTS) $(EXTENSION_DIR)/gd/gd.o $(LIBGD) $(LIBJPEG) $(LIBTIFF) $(LIBXPM) $(LIBZ) $(LIBFREETYPE) $(LIBPNG) $(LIBWEBP) -shared -o gd.so $(LD_FLAGS)

all:
	@make v1-core
	@make v1-lib
	@make v1-extensions

# V1 Core
main.o: main.cpp $(V1_DEPENDS) $(WLIB_DEPENDS)
	$(CC) -c $(C_FLAGS) $<
ast.o: ast.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
global.o: global.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
interpret.o: interpret.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
wscript.flex.o: wscript.flex.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
wscript.tab.o: wscript.tab.cpp wscript.tab.h $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	

# V1 Lib
v1lib.o: v1lib.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<

# DynCall
dyncall_call.o: $(DYNCALL_DIR)/dyncall_call.S
	$(CC) -c $(C_FLAGS) $<
dyncall_api.o: $(DYNCALL_DIR)/dyncall_api.c
	$(CC) -c $(C_FLAGS) $<
dyncall_callvm_base.o: $(DYNCALL_DIR)/dyncall_callvm_base.c
	$(CC) -c $(C_FLAGS) $<
dyncall_callvm.o: $(DYNCALL_DIR)/dyncall_callvm.c
	$(CC) -c $(C_FLAGS) $<
dyncall_vector.o: $(DYNCALL_DIR)/dyncall_vector.c
	$(CC) -c $(C_FLAGS) $<
dyncall_callf.o: $(DYNCALL_DIR)/dyncall_callf.c
	$(CC) -c $(C_FLAGS) $<
dyncall_struct.o: $(DYNCALL_DIR)/dyncall_struct.c
	$(CC) -c $(C_FLAGS) $<
dyncall_call_arm32_arm_armhf.o: $(DYNCALL_DIR)/dyncall_call_arm32_arm_armhf.S
	$(CC) -c $(C_FLAGS) $<
dyncall_call_arm64.o: $(DYNCALL_DIR)/dyncall_call_arm64.S
	$(CC) -c $(C_FLAGS) $<
dyncall_call_x64.o: $(DYNCALL_DIR)/dyncall_call_x64.S
	$(CC) -c $(C_FLAGS) $<
dyncall_call_x86.o: $(DYNCALL_DIR)/dyncall_call_x86.S
	$(CC) -c $(C_FLAGS) $<

	
# Extensions
$(EXTENSION_DIR)/crypt/crypt.o: $(EXTENSION_DIR)/crypt/crypt.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $< -o $(EXTENSION_DIR)/crypt/crypt.o

$(EXTENSION_DIR)/ssl/ssl.o: $(EXTENSION_DIR)/ssl/ssl.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) -I$(OPENSSL_DIR)/include $< -o $(EXTENSION_DIR)/ssl/ssl.o

$(EXTENSION_DIR)/regex/regex.o: $(EXTENSION_DIR)/regex/regex.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $< -o $(EXTENSION_DIR)/regex/regex.o

$(EXTENSION_DIR)/gd/gd.o: $(EXTENSION_DIR)/gd/gd.cpp $(DEPENDS_V1) $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $< -o $(EXTENSION_DIR)/gd/gd.o

# WLIB
WException.o: $(WLIB_DIR)/WException.cpp $(WLIB_DIR)/WException.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WMemory.o: $(WLIB_DIR)/WMemory.cpp $(WLIB_DIR)/WMemory.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WSystem.o: $(WLIB_DIR)/WSystem.cpp $(WLIB_DIR)/WSystem.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WString.o: $(WLIB_DIR)/WString.cpp $(WLIB_DIR)/WString.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WDate.o: $(WLIB_DIR)/WDate.cpp $(WLIB_DIR)/WDate.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WFile.o: $(WLIB_DIR)/WFile.cpp $(WLIB_DIR)/WFile.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WSocket.o: $(WLIB_DIR)/WSocket.cpp $(WLIB_DIR)/WSocket.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WThread.o: $(WLIB_DIR)/WThread.cpp $(WLIB_DIR)/WThread.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
Process.o: $(WLIB_DIR)/utils/Process.cpp $(WLIB_DIR)/utils/Process.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WStringTokenizer.o: $(WLIB_DIR)/utils/WStringTokenizer.cpp $(WLIB_DIR)/utils/WStringTokenizer.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
WInitFile.o: $(WLIB_DIR)/utils/WInitFile.cpp $(WLIB_DIR)/utils/WInitFile.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<
URL.o: $(WLIB_DIR)/utils/URL.cpp $(WLIB_DIR)/utils/URL.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
CGIUtils.o: $(WLIB_DIR)/utils/CGIUtils.cpp $(WLIB_DIR)/utils/CGIUtils.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
HTTPClient.o: $(WLIB_DIR)/utils/HTTPClient.cpp $(WLIB_DIR)/utils/HTTPClient.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
base64.o: $(WLIB_DIR)/hash/base64.cpp $(WLIB_DIR)/hash/base64.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<		
blowfish.o: $(WLIB_DIR)/hash/blowfish.cpp $(WLIB_DIR)/hash/blowfish.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<		
des.o: $(WLIB_DIR)/hash/des.cpp $(WLIB_DIR)/hash/des.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<		
md5.o: $(WLIB_DIR)/hash/md5.cpp $(WLIB_DIR)/hash/md5.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<		
sha256.o: $(WLIB_DIR)/hash/sha256.cpp $(WLIB_DIR)/hash/sha256.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<		
sha512.o: $(WLIB_DIR)/hash/sha512.cpp $(WLIB_DIR)/hash/sha512.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<	
sha1.o: $(WLIB_DIR)/hash/sha1.cpp $(WLIB_DIR)/hash/sha1.h $(DEPENDS_WLIB)
	$(CC) -c $(C_FLAGS) $<		

v1-core:
	@make OS_C_FLAGS="" OS_LD_FLAGS="";

v1-lib:
	@make libv1.so OS_C_FLAGS="-fvisibility=hidden -D_V1_DLL_EXPORT"  OS_LD_FLAGS="-rdynamic";
	
v1-extensions:
	@make crypt
	@make ssl
	@make regex
	@make gd
	
crypt:
	@make crypt.so OS_C_FLAGS="-fvisibility=hidden -DDLL_EXPORT" OS_LD_FLAGS="-rdynamic";

ssl:
	@make ssl.so OS_C_FLAGS="-fvisibility=hidden -D_USE_SSL -DDLL_EXPORT" OS_LD_FLAGS="-rdynamic";

regex:
	@make regex.so OS_C_FLAGS="-fvisibility=hidden -I$(PCRE_DIR) -DDLL_EXPORT" OS_LD_FLAGS="-rdynamic";

gd:
	@make gd.so OS_C_FLAGS="-fvisibility=hidden -DDLL_EXPORT" OS_LD_FLAGS="-rdynamic";

clean:
	(rm $(PROGRAM_NAME) || true) && (rm *.so || true) && (rm *.o || true) && (find ./extensions -name '*.o' -exec rm {} \; || true)
	
.PHONY: install
install:
	(cp $(PROGRAM_NAME) $(INST_DIR)/$(PROGRAM_NAME)/ || true) && (cp *.so $(INST_DIR)/$(PROGRAM_NAME)/ || true) && (rm $(INST_DIR)/$(PROGRAM_NAME)/lib$(PROGRAM_NAME).so || true) && (cp lib$(PROGRAM_NAME).so $(INST_DIR)/$(PROGRAM_NAME)-lib/ || true) && (cp v1.h $(INST_DIR)/$(PROGRAM_NAME)-lib/ || true)
	cd $(INST_DIR) && tar cvfz $(PROGRAM_NAME)-$(PROGRAM_VERSION)-$(SYSTEM)-$(ARCH).tar.gz --transform s/$(PROGRAM_NAME)/$(PROGRAM_NAME)-$(PROGRAM_VERSION)/ --format=gnu --exclude='$(PROGRAM_NAME)/*.ttf' --exclude='$(PROGRAM_NAME)/lib/*.conf' --exclude='$(PROGRAM_NAME)/experiments*' --exclude='$(PROGRAM_NAME)/cert/*.crt' --exclude='$(PROGRAM_NAME)/cert/*.key' --exclude='$(PROGRAM_NAME)/app/chat*' --exclude='$(PROGRAM_NAME)/x11-examples/xt-*' --owner=1000 --group=1000 $(PROGRAM_NAME)
	cd $(INST_DIR) && tar cvfz $(PROGRAM_NAME)-$(PROGRAM_VERSION)-lib-$(SYSTEM)-$(ARCH).tar.gz --transform s/$(PROGRAM_NAME)-lib/$(PROGRAM_NAME)-lib-$(PROGRAM_VERSION)/ --format=gnu --exclude='$(PROGRAM_NAME)-lib/dev/testfile*' --owner=1000 --group=1000 $(PROGRAM_NAME)-lib
	if [ -d "/usr/lib64" ]; then cp lib$(PROGRAM_NAME).so /usr/lib64/ && chmod 0755 /usr/lib64/lib$(PROGRAM_NAME).so && ln -sf /usr/lib64/libv1.so /usr/lib/lib$(PROGRAM_NAME).so && ldconfig; else cp lib$(PROGRAM_NAME).so /usr/lib/ && chmod 0755 /usr/lib/lib$(PROGRAM_NAME).so && ldconfig; fi

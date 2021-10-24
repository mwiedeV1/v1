# sudo make all
# will build a test application which include V1

PROGRAM_NAME        = v1-test
PROGRAM_VERSION     = 0.96
CURR_USER           = `whoami`
CURR_GROUP          = `id -g`
SYSTEM              = `uname`
ARCH                = `uname -m`
MAIN_DEFINES        =-D_REENTRANT -Wall -w -fPIC -std=c++0x -D_LINUX
DEFINES             = $(MAIN_DEFINES)
C_FLAGS             = $(OS_C_FLAGS) $(DEFINES) 
LD_FLAGS            = -s -L./ -lstdc++ -lpthread -ldl -lm -lrt -lv1
DEPENDS_V1          = v1.h

$(PROGRAM_NAME): v1-test.o
	$(CC) $(C_FLAGS) $(DEFINES) *.o -o $(PROGRAM_NAME) $(LD_FLAGS)

v1-test.o: v1-test.cpp $(DEPENDS_V1)
	$(CC) -c $(C_FLAGS) $<

all:
	@make
	@make install

install:
	if [ -d "/usr/lib64" ]; then cp libv1.so /usr/lib64/ && chmod 0755 /usr/lib64/libv1.so && ln -sf /usr/lib64/libv1.so /usr/lib/libv1.so && ldconfig; else cp libv1.so /usr/lib/ && chmod 0755 /usr/lib/libv1.so && ldconfig; fi

clean:
	(rm $(PROGRAM_NAME) || true) && (rm *.o || true)

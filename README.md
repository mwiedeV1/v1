# V1 Scripting Language V 0.96 Development System

You need g++ in newest version and make.

sudo apt-get install g++ make

Usage:
make v1-core

Optional:
make v1-lib

Compile the extensions:
-----------------------

1. Download: https://mwiede.de/v1-script/download/v1-dev-0.96-extensions.tar.gz
and extract into the v1-dev-0.96 directory.

2. Compile PCRE
cd extensions/regex/pcre-8.43/
./configure  --enable-static --with-pic --enable-utf8
make

3. Compile OpenSSL
cd extensions/ssl/openssl-1.0.2u/
./config -fPIC no-shared
make

4. Compile GDLib
cd extensions/gd/libgd-2.3.0/
./configure 
make 
sudo make install

Note! libgd require other libraries such as libjpeg, libpng ... you have to install manually on your system.

5. Compile the V1 extensions
make v1-extensions

---------------------

Create archives and install library:
sudo make install
make clean

After installing a tar.gz is located in install directory.
You can test directly in install/v1 sub directory.

See also: https://www.v1-script.net

V1 is a project from Matthias Wiede 2016-2021.

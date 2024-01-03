#!/bin/bash

set -ex

apt-get install autoconf libtool gperf -y

export LIBSECCOMP_VERSION=2.5.5

sed -i "s/0.0.0/${LIBSECCOMP_VERSION}/g" configure.ac

GOARCH=amd64

./autogen.sh
./configure \
  --prefix="$PWD/.tmp/$GOARCH" --libdir="$PWD/.tmp/$GOARCH/lib" \
  --enable-static --enable-shared
make install
make clean

GOARCH=arm64
export CC=aarch64-linux-gnu-gcc
export STRIP=aarch64-linux-gnu-strip
./autogen.sh
./configure --host "aarch64-linux-gnu" \
  --prefix="$PWD/.tmp/$GOARCH" --libdir="$PWD/.tmp/$GOARCH/lib" \
  --enable-static --enable-shared
make install
make clean

GOARCH=ppc64le
export CC=powerpc64le-linux-gnu-gcc
export STRIP=powerpc64le-linux-gnu-strip
./autogen.sh
./configure --host "powerpc64le-linux-gnu" \
  --prefix="$PWD/.tmp/$GOARCH" --libdir="$PWD/.tmp/$GOARCH/lib" \
  --enable-static --enable-shared
make install
make clean

GOARCH=mips64le
export CC=mips64el-linux-gnuabi64-gcc
export STRIP=mips64el-linux-gnuabi64-strip
./autogen.sh
./configure --host "mips64el-linux-gnu" \
  --prefix="$PWD/.tmp/$GOARCH" --libdir="$PWD/.tmp/$GOARCH/lib" \
  --enable-static --enable-shared
make install
make clean

sed -i "s/${LIBSECCOMP_VERSION}/0.0.0/g" configure.ac

#!/bin/bash

set -ex

apt-get install pkg-config build-essential autoconf libtool gperf -y

cp -r /usr/include/asm-generic/ /usr/include/asm

export LIBSECCOMP_VERSION=2.5.5

sed -i "s/0.0.0/${LIBSECCOMP_VERSION}/g" configure.ac

GOARCH=loong64
./autogen.sh && \
./configure \
  --prefix="$PWD/.tmp/$GOARCH" --libdir="$PWD/.tmp/$GOARCH/lib" \
  --enable-static --enable-shared
make install
make clean

sed -i "s/${LIBSECCOMP_VERSION}/0.0.0/g" configure.ac

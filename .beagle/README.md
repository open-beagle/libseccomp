# seccomp/libseccomp

<https://github.com/seccomp/libseccomp>

```bash
git remote add upstream git@github.com:seccomp/libseccomp.git

git fetch upstream

# 2023.04.24
git merge upstream/main
```

## build

```bash
# x86_64
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.19 \
bash -c '
apt-get install autoconf libtool gperf -y && \
./autogen.sh && \
./configure \
  --prefix="$PWD/.tmp/loong64" --libdir="$PWD/.tmp/loong64/lib" \
  --enable-static --enable-shared
make install
make clean
'

# arm64
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.19 \
bash -c '
apt-get install autoconf libtool gperf -y && \
./autogen.sh && \
export LIBSECCOMP_VERSION=2.5.4 && \
export CC=aarch64-linux-gnu-gcc && \
export STRIP=aarch64-linux-gnu-strip && \
./configure --host "aarch64-linux-gnu" \
  --prefix="$PWD/.tmp/arm64" --libdir="$PWD/.tmp/arm64/lib" \
  --enable-static --enable-shared && \
make install && \
make clean
'

# mips64le
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.19-loongnix \
bash -c '
apt-get install autoconf libtool gperf -y && \
./autogen.sh && \
export CC=mips64el-linux-gnuabi64-gcc && \
export STRIP=mips64el-linux-gnuabi64-strip && \
./configure --host "mips64el-linux-gnuabi64" \
  --prefix="$PWD/.tmp/mips64le" --libdir="$PWD/.tmp/mips64le/lib" \
  --enable-static --enable-shared  && \
make install  && \
make clean
'

# Loong64
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.19-loongnix \
bash -c '
apt-get install autoconf libtool gperf -y && \
./autogen.sh && \
export CC=loongarch64-linux-gnu-gcc && \
export STRIP=loongarch64-linux-gnu-strip && \
./configure --host "loongarch64-linux-gnu" \
  --prefix="$PWD/.tmp/loong64" --libdir="$PWD/.tmp/loong64/lib" \
  --enable-static --enable-shared && \
make install && \
make clean
'

docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry.cn-qingdao.aliyuncs.com/wod/debian:bookworm \
bash

# x86_64
# /go/src/gitlab.wodcloud.com/cloud/runc/Dockerfile

## 首先打个补丁，再编译runc-build
git apply .beagle/v1.1-add-mips64el-support.patch

docker build \
  --build-arg GO_VERSION=1.19 \
  --tag registry-vpc.cn-qingdao.aliyuncs.com/wod/runc-build:1.1.5 \
  --file ./.beagle/runc-build.dockerfile .

docker push registry-vpc.cn-qingdao.aliyuncs.com/wod/runc-build:1.1.5
```

## cache

```bash
# tar.gz
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.19-loongnix \
bash -c '
apt-get install autoconf libtool gperf -y && \
./autogen.sh && \
cp -r ./ /tmp/libseccomp-2.5.5 && \
tar -zcvf libseccomp-2.5.5.tar.gz -C /tmp libseccomp-2.5.5
'

# cache
mc cp ./libseccomp-2.5.5.tar.gz cache/kubernetes/k8s/libseccomp/
rm -rf libseccomp-2.5.5.tar.gz
```

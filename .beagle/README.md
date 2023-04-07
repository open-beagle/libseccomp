# seccomp/libseccomp

<https://github.com/seccomp/libseccomp>

```bash
git remote add upstream git@github.com:seccomp/libseccomp.git

git fetch upstream

# 2022.11.01
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
  --prefix="$PWD/.tmp/amd64" --libdir="$PWD/.tmp/amd64/lib" \
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

# Loong64
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.19 \
bash -c '
apt-get install autoconf libtool gperf -y && \
./autogen.sh && \
export CC=loongarch64-linux-gnu-gcc && \
export STRIP=loongarch64-linux-gnu-strip && \
./configure --host "loongarch64-linux-gnu" \
  --prefix="$PWD/.tmp/loong64" --libdir="$PWD/.tmp/loong64/lib" \
  --enable-static --enable-shared
make install
make clean
'

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
# 构建缓存-->推送缓存至服务器
docker run --rm \
  -e PLUGIN_REBUILD=true \
  -e PLUGIN_ENDPOINT=$PLUGIN_ENDPOINT \
  -e PLUGIN_ACCESS_KEY=$PLUGIN_ACCESS_KEY \
  -e PLUGIN_SECRET_KEY=$PLUGIN_SECRET_KEY \
  -e PLUGIN_PATH="/cache/open-beagle/runc" \
  -e PLUGIN_MOUNT="./.git" \
  -v $(pwd):$(pwd) \
  -w $(pwd) \
  registry.cn-qingdao.aliyuncs.com/wod/devops-s3-cache:1.0

# 读取缓存-->将缓存从服务器拉取到本地
docker run --rm \
  -e PLUGIN_RESTORE=true \
  -e PLUGIN_ENDPOINT=$PLUGIN_ENDPOINT \
  -e PLUGIN_ACCESS_KEY=$PLUGIN_ACCESS_KEY \
  -e PLUGIN_SECRET_KEY=$PLUGIN_SECRET_KEY \
  -e PLUGIN_PATH="/cache/open-beagle/runc" \
  -v $(pwd):$(pwd) \
  -w $(pwd) \
  registry.cn-qingdao.aliyuncs.com/wod/devops-s3-cache:1.0
```

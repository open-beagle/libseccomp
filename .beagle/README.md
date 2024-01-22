# seccomp/libseccomp

<https://github.com/seccomp/libseccomp>

```bash
git remote add upstream git@github.com:seccomp/libseccomp.git

git fetch upstream

# 2023.12.21
git merge v2.5.5
```

## build

```bash
rm -rf .tmp

# cross compile
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.21 \
bash .beagle/build.sh

# loong64
docker run -it --rm \
-v $PWD/:/go/src/github.com/open-beagle/libseccomp \
-w /go/src/github.com/open-beagle/libseccomp \
cr.loongnix.cn/library/debian:buster \
bash .beagle/build-loong64.sh

docker build \
  --build-arg BASE=registry.cn-qingdao.aliyuncs.com/wod/alpine:3-amd64 \
  --build-arg AUTHOR=mengkzhaoyun@gmail.com \
  --build-arg VERSION=v2.5.5 \
  --build-arg TARGETOS=linux \
  --build-arg TARGETARCH=loong64 \
  --tag registry-vpc.cn-qingdao.aliyuncs.com/wod/libseccomp:v2.5.5-loong64 \
  --file ./.beagle/dockerfile .

docker push registry-vpc.cn-qingdao.aliyuncs.com/wod/libseccomp:v2.5.5-loong64

# loong64 Copy From Loongarch64 4.19 Linux
docker build \
  --build-arg BASE=registry.cn-qingdao.aliyuncs.com/wod/alpine:3-amd64 \
  --build-arg AUTHOR=mengkzhaoyun@gmail.com \
  --build-arg VERSION=v2.3.3 \
  --build-arg TARGETOS=linux \
  --build-arg TARGETARCH=loong64 \
  --tag registry-vpc.cn-qingdao.aliyuncs.com/wod/libseccomp:v2.3.3-loong64 \
  --file ./.beagle/dockerfile .

docker push registry-vpc.cn-qingdao.aliyuncs.com/wod/libseccomp:v2.3.3-loong64
```

## cache

```bash
# 构建缓存-->推送缓存至服务器
docker run --rm \
  -e PLUGIN_REBUILD=true \
  -e PLUGIN_ENDPOINT=$PLUGIN_ENDPOINT \
  -e PLUGIN_ACCESS_KEY=$PLUGIN_ACCESS_KEY \
  -e PLUGIN_SECRET_KEY=$PLUGIN_SECRET_KEY \
  -e DRONE_REPO_OWNER="open-beagle" \
  -e DRONE_REPO_NAME="libseccomp" \
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
  -e DRONE_REPO_OWNER="open-beagle" \
  -e DRONE_REPO_NAME="libseccomp" \
  -v $(pwd):$(pwd) \
  -w $(pwd) \
  registry.cn-qingdao.aliyuncs.com/wod/devops-s3-cache:1.0
```

## debug

```bash
# # x86 cross compile loong64 do not work
# docker run -it --rm \
# -v $PWD/:/go/src/github.com/open-beagle/libseccomp \
# -w /go/src/github.com/open-beagle/libseccomp \
# -e LIBSECCOMP_VERSION=2.5.5 \
# registry-vpc.cn-qingdao.aliyuncs.com/wod/golang:1.21-loongnix \
# bash -c '
# apt-get install build-essential autoconf libtool gperf -y && \
# ./autogen.sh && \
# export CC=loongarch64-linux-gnu-gcc && \
# export STRIP=loongarch64-linux-gnu-strip && \
# ./configure --host "loongarch64-linux-gnu" \
#   --prefix="$PWD/.tmp/loong64" --libdir="$PWD/.tmp/loong64/lib" \
#   --enable-static --enable-shared && \
# make install && \
# make clean
# '

docker build \
-f .beagle/dockerfile \
--build-arg BASE=registry.cn-qingdao.aliyuncs.com/wod/alpine:3 \
--build-arg AUTHOR=mengkzhaoyun@gmail.com \
--build-arg VERSION=v2.5.5 \
-t registry.cn-qingdao.aliyuncs.com/wod/libseccomp:v2.5.5 \
. 

docker run -it --rm \
registry.cn-qingdao.aliyuncs.com/wod/libseccomp:v2.5.5 \
ls -ll /opt/libseccomp
```

FROM ubuntu:latest
RUN apt-get update && apt-get install -y apt-utils && apt-get update&& apt-get install -y clang-9 curl python3 python3-pip && pip install requests && ln -s /usr/bin/clang++-9 /usr/bin/clang++

#sent over slack, tar -xvf criterion.tgz
ADD criterion /usr/



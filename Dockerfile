FROM ubuntu:16.04

ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

RUN apt update && \
    apt install -y software-properties-common build-essential && \
    add-apt-repository -y ppa:deadsnakes/ppa && \
    apt update -y && \
    apt install -y python3.7 && \
    apt clean && \
    update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.7 0 && \
    apt install -y python3-pip && \
    pip3 install --upgrade pip && \
    pip3 install -U platformio 

WORKDIR /agent

CMD PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote agent start



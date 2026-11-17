FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    python3 \
    python3-pip \
    git \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    libsfml-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY . /workspace

RUN git submodule update --init --recursive || true

RUN chmod +x build.sh

CMD ["./build.sh", "--target", "raspberry_pico_w"]

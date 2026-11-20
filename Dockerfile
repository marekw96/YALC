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

# Configure global git safe.directory to prevent dubious ownership errors in CI volume mounts
RUN git config --global --add safe.directory '*'

WORKDIR /workspace

COPY . /workspace

RUN git submodule update --init --recursive || true

RUN chmod +x build.sh

CMD ["./build.sh", "--target", "raspberry_pico_w"]

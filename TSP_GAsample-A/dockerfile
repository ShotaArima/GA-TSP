FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    gdb \
    git \
    clang-format \
    valgrind \
    cmake \
    clang \
    cppcheck \
    lcov \
    wget \
    curl \
    ca-certificates && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

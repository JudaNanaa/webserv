FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y \
        clang \
        lldb \
        build-essential \
        sudo \
        valgrind \
        bc \
        libbsd-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

COPY Makefile .

CMD ["/bin/bash"]


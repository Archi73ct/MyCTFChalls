# docker build -t canarybirds .
# docker run -p 1024:1024 --rm -it canarybirds
FROM ubuntu:20.04 as build

RUN apt-get update
RUN apt-get -y install build-essential

RUN useradd -d /home/ctf/ -m -p ctf -s /bin/bash ctf
RUN echo "ctf:ctf" | chpasswd

WORKDIR /home/ctf

COPY build/canary .
COPY canaries.cpp .
COPY flag .
COPY ynetd .

RUN chmod +x ynetd

RUN chown -R root:root /home/ctf

RUN mkdir /build



USER ctf
EXPOSE 1024
CMD ./ynetd -p 1024 ./canary
#CMD c++ canaries.cpp -o canary -no-pie -fstack-protector && cp canary /build

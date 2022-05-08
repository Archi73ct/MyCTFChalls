FROM ubuntu:20.04

RUN apt-get update && apt-get install -y --reinstall  build-essential python3 socat

RUN useradd -d /home/ctf/ -m -p ctf -s /bin/bash ctf
RUN echo "ctf:ctf" | chpasswd


WORKDIR /home/ctf

COPY src/ .
COPY flag .

USER ctf

ENTRYPOINT socat tcp-l:1024,reuseaddr,fork exec:"python3 challenge.py"
FROM ubuntu:18.04

RUN apt-get update
RUN apt-get -y install gcc
RUN useradd -d /home/ctf/ -m -p ctf -s /bin/bash ctf
RUN echo "ctf:ctf" | chpasswd

WORKDIR /home/ctf

COPY hobc.c .
COPY flag .
COPY ynetd .

RUN chmod +x ynetd
#RUN chmod +x hobc

RUN chown -R root:root /home/ctf
RUN gcc  -no-pie -Wl,-z,norelro hobc.c -o hobc
USER ctf
EXPOSE 1024
CMD ./ynetd -p 1024 ./hobc

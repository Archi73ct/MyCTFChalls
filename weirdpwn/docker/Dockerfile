FROM alpine:latest

RUN apk update
RUN apk add qemu-system-arm bash cpio gzip socat

WORKDIR /challenge

RUN mkdir fs


COPY launch.sh .
COPY zImage .

RUN chmod +x ./launch.sh
EXPOSE 1024
COPY fs fs/

CMD socat tcp-l:1024,reuseaddr,fork exec:./launch.sh
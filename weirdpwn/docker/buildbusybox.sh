#!/bin/bash
export BUSYBOX_VERSION=1.35.0
if ! [ -f busybox-$BUSYBOX_VERSION.tar.bz2 ]; then
    echo "[+] Downloading busybox..."
    wget -q -c https://busybox.net/downloads/busybox-$BUSYBOX_VERSION.tar.bz2
    [ -e busybox-$BUSYBOX_VERSION ] || tar xjf busybox-$BUSYBOX_VERSION.tar.bz2
fi

echo "[+] Building busybox..."
./dockcross make -C busybox-$BUSYBOX_VERSION defconfig
sed -i 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/g' busybox-$BUSYBOX_VERSION/.config
./dockcross make -C busybox-$BUSYBOX_VERSION -j16
./dockcross make -C busybox-$BUSYBOX_VERSION install

echo "[+] Building filesystem..."
pushd fs
mkdir -p bin sbin etc proc sys usr/bin usr/sbin root home/ctf
popd
cp -a busybox-$BUSYBOX_VERSION/_install/* fs

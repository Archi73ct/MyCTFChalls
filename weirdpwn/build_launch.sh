./dockcross bash -c "\$CC -static challenge.c -o challenge"
cp challenge docker/fs/challenge
pushd docker
docker build -t weirdpwn . && docker run -p 1024:1024 -it weirdpwn
popd
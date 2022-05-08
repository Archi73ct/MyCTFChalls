docker build -t canarybirds .
docker run -p 1024:1024 -v $PWD/build:/build --rm -it canarybirds


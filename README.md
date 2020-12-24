# Embedded V8 Docker Image

* Builds the Google JavaScript V8 engine and allows to use the D8 shell
* Contains D8 (Debug8) is V8's own minimalist debug shell (https://v8.dev/docs/d8)
* Leaves also v8 headers and libraries to allow easy building C++ programs that uses v8 under the hood
* Image is preequipped also with g++ and cmake

# Usage

I'll skip a pure js usage of this image, it's described in original repo: https://github.com/andreburgaud/docker-v8

Sample usage in dockerfile, assuming you have C++ app in cxx dir in same root as Dockerfile.

```
FROM dervan/embedded-v8:latest

COPY ./cxx /app

WORKDIR /app
RUN mkdir -p build && cd build && cmake .. && make

ENTRYPOINT /app/build/run_app
```

Inside a container headers are placed in `/v8/include` dir and obj files in `/v8/out.gn/x64.release.sample/obj`. For a complete example of an application see `./examples/app/`. You can build it with:

```
cd ./examples/app
docker build -t example-v8-app -f Dockerfile .
docker run -i example-v8-app <<< "() => Math.random()"
```

# Build Local Image

```
$ git clone https://github.com/dervan/docker-embedded-v8.git
$ cd docker-embedded-v8
$ docker build -t embedded-v8 .
```

**Note**: the compilation may take some time depending on your hardware. For example it takes about 30 minutes on a 3.5GHz four core iMac with 24GB of RAM.

# Resources

* https://github.com/v8/v8
* https://github.com/v8/v8/wiki
* https://github.com/v8/v8/wiki/Building-with-GN
* https://en.wikipedia.org/wiki/ECMAScript
* https://gist.github.com/kevincennis/0cd2138c78a07412ef21
* https://github.com/andreburgaud/docker-v8

# MIT License

See the license file

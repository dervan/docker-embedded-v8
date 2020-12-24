FROM dervan/embedded-v8:latest

COPY ./cxx /app

WORKDIR /app
RUN mkdir -p build && cd build && cmake .. && make

ENTRYPOINT /app/build/run_app

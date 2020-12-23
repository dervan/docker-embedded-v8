# ==============================================================================
# First stage build (compile V8)
# ==============================================================================

FROM debian:bullseye-slim as builder

ARG V8_VERSION=8.9.99

RUN apt-get update && apt-get upgrade -yqq

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get install bison \
                    cdbs \
                    curl \
                    flex \
                    g++ \
                    git \
                    python \
                    vim \
                    pkg-config -yqq

RUN git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git

ENV PATH="/depot_tools:${PATH}"

RUN fetch v8 && \
    cd v8 && \
    git checkout ${V8_VERSION} && \
    ./tools/dev/v8gen.py x64.release.sample && \
    ninja -C out.gn/x64.release.sample v8_monolith d8 && \
    strip out.gn/x64.release.sample/d8

# ==============================================================================
# Second stage build
# ==============================================================================

FROM debian:bullseye-slim

LABEL v8.version=$V8_VERSION \
      maintainer="michal@jagielski.net"

RUN apt-get update && apt-get upgrade -yqq && \
    DEBIAN_FRONTEND=noninteractive apt-get install curl rlwrap cmake g++ vim -yqq && \
    apt-get clean

WORKDIR /v8

COPY --from=builder /v8/out.gn/x64.release.sample/ ./out.gn/x64.release.sample
COPY --from=builder /v8/include ./include

COPY vimrc /root/.vimrc

COPY entrypoint.sh /

RUN chmod +x /entrypoint.sh && \
    ln -s /v8/out.gn/x64.release.sample/d8 /usr/local/bin/d8

ENTRYPOINT ["/entrypoint.sh"]

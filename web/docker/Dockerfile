FROM sptrakesh/wt AS base

RUN apk --update --no-cache add libpq-dev \
  && git clone --branch 7.10.1 https://github.com/jtv/libpqxx.git \
  && cd libpqxx \
  && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=23 \
    -DSKIP_BUILD_TEST=ON \
    -B build -S . \
  && cmake --build build --parallel \
  && cmake --install build

COPY CMakeLists.txt /opt/spt/apm-viewer/CMakeLists.txt
COPY src /opt/spt/apm-viewer/src
COPY cmake /opt/spt/apm-viewer/cmake

WORKDIR /opt/spt/apm-viewer/build
RUN rm -rf * \
  && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/opt/local .. \
  && make -j8 \
  && make install

FROM sptrakesh/cppruntime

RUN addgroup spt \
  && adduser -DS -g "SPT User" -G spt -s /bin/sh -h /home/spt spt \
  && mkdir -p /opt/spt/logs \
  && chown spt:spt /opt/spt/logs \
  && apk --update --no-cache add libpq

COPY --from=base /opt/local/share/Wt /opt/local/share/Wt
COPY --from=base /opt/spt/bin /opt/spt/bin
COPY resources /opt/spt/var/
COPY docker/scripts/entrypoint.sh /opt/spt/bin

WORKDIR /opt/spt
ENV LD_LIBRARY_PATH=/usr/lib:/opt/local/lib
USER spt
ENTRYPOINT [ "/opt/spt/bin/entrypoint.sh" ]

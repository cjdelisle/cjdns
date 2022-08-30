FROM alpine:3.6
MAINTAINER Lars Gierth <larsg@systemli.org>

RUN apk add --update nodejs bash python git build-base linux-headers

ADD . /src

WORKDIR /src

RUN adduser -D -h /etc/cjdns -u 1000 cjdns \
    && rm -rf build_* && ./do \
    && cp cjdroute /usr/bin \
    && cp -r tools/* /usr/bin \
    && cp makekeys \
          mkpasswd \
          privatetopublic \
          publictoip6 \
          randombytes \
          sybilsim /usr/bin \
    && cp contrib/docker/entrypoint.sh / \
    && rm -rf /src /var/cache/apk/* \
    && apk del --purge python build-base linux-headers

VOLUME [ "/etc/cjdns" ]

ENTRYPOINT [ "/entrypoint.sh" ]

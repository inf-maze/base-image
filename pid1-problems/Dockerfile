FROM debian:buster-slim

LABEL maintainer="inf-maze@outlook.com"

ENV DEBIAN_FRONTEND noninteractive
ENV TERM linux
ENV PYTHONUNBUFFERED=1

# Use aliyun source
RUN echo 'deb [trusted=yes] http://mirrors.aliyun.com/debian stable main' > /etc/apt/sources.list \
    && echo 'deb [trusted=yes] http://mirrors.aliyun.com/debian-security stable/updates main' >> /etc/apt/sources.list \
    && echo 'deb [trusted=yes] http://mirrors.aliyun.com/debian stable-updates main' >> /etc/apt/sources.list

# install common package
RUN set -ex \
    && apt-get update -yqq \
    && apt-get install -yqq --no-install-recommends \
        procps \
        gcc \
        libc6-dev \
    && apt-get autoremove -yqq --purge \
    && apt-get clean \
    && rm -rf \
        /var/log/* \
        /var/lib/apt/lists/* \
        /tmp/* \
        /var/tmp/* \
        /usr/share/man \
        /usr/share/doc \
        /usr/share/doc-base

ADD https://github.com/krallin/tini/releases/download/v0.18.0/tini /tini

COPY zombie-reaping-problem.c /tmp/zombie-reaping-problem.c
COPY signal-forwarding-problem.c /tmp/signal-forwarding-problem.c
COPY signal-forwarding-problem-entrypoint.sh /usr/local/bin/signal-forwarding-problem-entrypoint.sh

RUN gcc /tmp/zombie-reaping-problem.c -o /usr/local/bin/zombie-reaping-problem \
    && gcc /tmp/signal-forwarding-problem.c -o /usr/local/bin/signal-forwarding-problem \
    && chmod +x /tini \
    && chmod +x /usr/local/bin/zombie-reaping-problem \
    && chmod +x /usr/local/bin/signal-forwarding-problem \
    && chmod +x /usr/local/bin/signal-forwarding-problem-entrypoint.sh

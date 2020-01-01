FROM debian:buster-slim

LABEL maintainer="inf-maze@outlook.com"

# Never prompts the user for choices on installation/configuration of packages
ENV DEBIAN_FRONTEND noninteractive
ENV TERM linux
ENV PYTHONUNBUFFERED=1
ENV APT_SOURCE 163,aliyun

COPY bin/apt-get /usr/local/bin/apt-get
COPY bin/echo_red /usr/local/bin/echo_red
COPY bin/echo_green /usr/local/bin/echo_green
COPY bin/echo_yellow /usr/local/bin/echo_yellow

RUN ln -s /usr/local/bin/apt-get /usr/local/bin/apt

# Use aliyun source
#RUN echo 'deb [trusted=yes] http://mirrors.aliyun.com/debian stable main' > /etc/apt/sources.list \
#    && echo 'deb [trusted=yes] http://mirrors.aliyun.com/debian-security stable/updates main' >> /etc/apt/sources.list \
#    && echo 'deb [trusted=yes] http://mirrors.aliyun.com/debian stable-updates main' >> /etc/apt/sources.list
#use aws s3 

# install common package
RUN set -e \
    && apt-get update -yqq \
    && apt-get install -yqq --no-install-recommends \
        locales \
        ca-certificates \
        vim-tiny \
        tree \
        procps \
        curl \
    && sed -i 's/^# en_US.UTF-8 UTF-8$/en_US.UTF-8 UTF-8/g' /etc/locale.gen \
    && locale-gen \
    && update-locale LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 \
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
#RUN cd /usr/share/i18n/locales && ls | grep -v en_US | xargs rm
#RUN cd /usr/share/i18n/charmaps && ls | grep -v UTF-8.gz | xargs rm

# Define en_US.UTF-8
ENV LANGUAGE en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LC_ALL en_US.UTF-8

RUN curl -sSL -o /tmp/s6-overlay.tar.gz https://github.com/just-containers/s6-overlay/releases/download/v1.22.1.0/s6-overlay-amd64.tar.gz \
    && tar xvfz /tmp/s6-overlay.tar.gz -C / \
    && rm /tmp/s6-overlay.tar.gz

ENV S6_KILL_GRACETIME 30

# optimize /etc/bash.bashrc
# optimize /etc/inputrc
COPY bashrc /etc/bash.bashrc
COPY vimrc.tiny /etc/vim/vimrc.tiny
COPY inputrc /etc/inputrc


ENTRYPOINT ["/init"]

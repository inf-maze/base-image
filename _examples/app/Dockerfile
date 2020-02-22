FROM base-image

LABEL maintainer="inf-maze@outlook.com"

RUN apt-get update \
    && apt-get install -y --no-install-recommends nginx redis-server

COPY services/nginx /etc/services.d/nginx
COPY services/redis /etc/services.d/redis
COPY services/zombie /etc/services.d/zombie

RUN curl -o /usr/local/bin/zombie -sSL https://github.com/inf-maze/base-image/raw/master/pid1-problems/zombie-reaping-problem \
    && chmod +x /usr/local/bin/zombie

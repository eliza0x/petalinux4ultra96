FROM ubuntu:18.04

COPY petalinux-v2019.2-final-installer.run /
COPY ultra96v2_oob_2019_2.zip /

RUN dpkg --add-architecture i386
RUN apt-get update
RUN apt-get install -y --no-install-recommends \
        python gawk gcc git make net-tools libncurses5-dev zlib1g:i386 libssl-dev flex bison libselinux1 gnupg wget diffstat chrpath socat xterm autoconf libtool tar unzip texinfo zlib1g-dev gcc-multilib build-essential screen pax gzip \
        xvfb libtool-bin cpio language-pack-en rsync \
        sudo vim less
# tftpd 

RUN update-locale LANG=en_US.UTF-8


ARG DOCKER_UID=1000
ARG DOCKER_USER=docker
ARG DOCKER_PASSWORD=docker
　RUN useradd -m --uid ${DOCKER_UID} --groups sudo ${DOCKER_USER} \
  && echo ${DOCKER_USER}:${DOCKER_PASSWORD} | chpasswd

USER ${DOCKER_UID}


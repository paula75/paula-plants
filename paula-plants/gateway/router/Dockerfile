FROM resin/raspberrypi3-debian:jessie

MAINTAINER Felipe Lalanne <felipe.lalanne@inria.cl>

ENV DEBIAN_FRONTEND noninteractive
ENV IFACE lowpan0
ENV IP 2001:dead:beef::1/64
ENV BUILD_DEPS dpkg-dev debhelper bzip2 build-essential unzip wget dh-autoreconf pkg-config libnl-3-dev libnl-genl-3-dev

# Configura raspbian repositories
RUN echo "deb http://archive.raspbian.org/raspbian jessie main contrib non-free" > /etc/apt/sources.list

# Install with apt-get
RUN apt-get update && apt-get -q -y --force-yes install libbz2-1.0=1.0.6-7 iproute2 bison libnl-3-200 libnl-genl-3-200 $BUILD_DEPS

# Install wpan-tools
RUN mkdir -p /opt/src  \

WORKDIR /opt/src

RUN wget https://github.com/linux-wpan/wpan-tools/archive/wpan-tools-0.7.zip && unzip wpan-tools-0.7.zip && mv wpan-tools-wpan-tools-0.7 wpan-tools-0.7 \
    && cd wpan-tools-0.7 && ./autogen.sh \
    && ./configure CFLAGS='-g -O0' --prefix=/usr --sysconfdir=/etc --libdir=/usr/lib \
    && make && make install \
    && iwpan list

# Install flex
RUN wget https://sourceforge.net/projects/flex/files/flex-2.6.0.tar.bz2 && tar xvfj flex-2.6.0.tar.bz2 \
    && cd flex-2.6.0 && ./configure \
    && make && make install

# Install radvd
RUN wget https://github.com/linux-wpan/radvd/archive/6lowpan.zip && unzip 6lowpan.zip
    && cd radvd-6lowpan && ./autogen.sh
    && ./configure --prefix=/usr/local --sysconfdir=/etc --mandir=/usr/share/man
    && make && make install

# Uninstall build dependencies
RUN apt-get -q -y autoremove --purge $BUILD_DEPS

# Remove source files
RUN rm -rf /opt/src

# Reset workdir
WORKDIR /

# Copy configuration
ADD radvd.conf /etc

# Copy main script
ADD radvd.sh /
RUN chmod 755 /radvd.sh

# Run the service
CMD /radvd.sh

FROM resin/raspberrypi3-debian:jessie

MAINTAINER Felipe Lalanne <felipe.lalanne@inria.cl>

ENV DEBIAN_FRONTEND noninteractive
ENV LISTEN [::]:5683
ENV BACKEND http://127.0.0.1/
ENV ACCESSLOG /dev/stdout
ENV ERRORLOG /dev/stderr
ENV BUILD_DEPS git

# Configura raspbian repositories
RUN echo "deb http://archive.raspbian.org/raspbian jessie main contrib non-free" > /etc/apt/sources.list

# Install with apt-get
RUN apt-get update && apt-get -q -y install golang $BUILD_DEPS

# Create the go package directory
RUN mkdir -p /opt/go

ENV GOPATH=/opt/go
ENV PATH=$PATH:$GOPATH/bin

# Install crosscoap
RUN go get github.com/ibm-security-innovation/crosscoap/cmd/crosscoap

RUN apt-get autoremove --purge $BUILD_DEPS

CMD crosscoap -accesslog $ACCESSLOG -listen $LISTEN -errorlog $ERRORLOG -backend $BACKEND

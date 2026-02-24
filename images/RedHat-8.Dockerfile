#FROM redhat/ubi8:8.8
FROM redhat/ubi8:8.10

# Download & install GEHC root certificates
RUN curl -o /etc/pki/ca-trust/source/anchors/gehealthcarerootca1.crt http://aia.ge-healthcare.net/gehealthcarerootca1.pem \
    && curl -o /etc/pki/ca-trust/source/anchors/gehealthcarerootca2.crt http://aia.ge-healthcare.net/gehealthcarerootca2.pem \
    && update-ca-trust

# Install build tools
RUN dnf update -y && dnf install -y \
    gcc-c++ \
    glibc-static \
    libstdc++-static \
    openssl-devel

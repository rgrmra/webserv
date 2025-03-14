FROM alpine:3.19

# Install build tools, CMake, Ninja, and inotify-tools for file watching 
RUN apk add --no-cache \
    build-base \
    cmake \
    ninja \
    inotify-tools \
    bash \
	g++ \
	make \
	php \
	php-cgi \
	python3

WORKDIR /app

COPY . .

RUN make

CMD ["tail", "-f", "/dev/null"]

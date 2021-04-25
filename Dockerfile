FROM alpine
RUN apk add --no-cache build-base
RUN apk add ncurses-dev
RUN apk add bash
WORKDIR /app
COPY . .
# Compile the binaries
RUN gcc -o game *.c -lncursesw
WORKDIR /app
CMD ["./game"]

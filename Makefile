SRCS := $(wildcard *.c)

drmario: ${SRCS}
	gcc -o $@ -g -O2 -Wall -Wextra $^

all: drmario

run: drmario
	./drmaio

.PHONY: drmario

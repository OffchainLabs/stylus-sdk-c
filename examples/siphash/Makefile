
STACK_SIZE=1024
CC=clang
LD=wasm-ld
CFLAGS=-I../../include/ -Iinterface-gen/ --target=wasm32 -Os --no-standard-libraries -mbulk-memory -Wall -g
LDFLAGS=-O2 --no-entry --stack-first -z stack-size=$(STACK_SIZE) -Bstatic

OBJECTS=main.o siphash.o

all: ./siphash.wasm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

siphash_unstripped.wasm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

siphash.wasm: siphash_unstripped.wasm
	wasm-strip -o $@ $<

clean:
	rm $(OBJECTS) siphash_unstripped.wasm siphash.wasm

.phony: all cargo-generate clean
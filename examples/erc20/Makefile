
STACK_SIZE=1024
CC=clang
LD=wasm-ld
CFLAGS=-I../../include/ -Iinterface-gen/ --target=wasm32 -Os --no-standard-libraries -mbulk-memory -Wall
LDFLAGS=-O2 --no-entry --stack-first -z stack-size=$(STACK_SIZE) -Bstatic

OBJECTS=build/impl.o build/lib/bebi.o build/lib/storage.o build/lib/simplelib.o build/lib/utils.o build/gen/ERC20_main.o

all: ./erc20.wasm

build/%.o: %.c cargo-generate
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/lib/%.o: ../../src/%.c
	mkdir -p build/lib
	$(CC) $(CFLAGS) -c $< -o $@

build/gen/%.o: interface-gen/erc20/%.c
	mkdir -p build/gen/
	$(CC) $(CFLAGS) -c $< -o $@

interface-gen/erc20/ERC20_main.c: cargo-generate

cargo-generate: build/interface.json
	mkdir -p build
	cargo-stylus c-headers $< interface-gen

build/interface.json: interface_compile.json erc20.sol
	mkdir -p build
	cat $< | solc --standard-json --pretty-json > $@

build/erc20.wasm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ --Map=build/sample.map

erc20.wasm: build/erc20.wasm
	wasm-strip -o $@ $<

.phony: all cargo-generate
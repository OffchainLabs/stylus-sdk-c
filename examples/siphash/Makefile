
STACK_SIZE=1024
CC=clang
LD=wasm-ld
CFLAGS=-I../../include/ -Iinterface-gen/ --target=wasm32 -Os --no-standard-libraries -mbulk-memory -Wall -g
LDFLAGS=-O2 --no-entry --stack-first -z stack-size=$(STACK_SIZE) -Bstatic

OBJECTS=main.o siphash.o

all: ./siphash.wasm

# Steps 1: build c-files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Steps 2: link
siphash_unstripped.wasm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Steps 3: strip symbols from wasm
siphash.wasm: siphash_unstripped.wasm
	wasm-strip -o $@ $<

# Steps 4: check the wasm using cargo-stylus
# cargo stylus check --wasm-file-path ./siphash.wasm  --endpoint $ENDPOINT --private-key=$PRIVATE_KEY

# Steps 5: deploy the wasm using cargo-stylus
# cargo stylus check --wasm-file-path ./siphash.wasm  --endpoint $ENDPOINT --private-key=$PRIVATE_KEY

clean:
	rm $(OBJECTS) siphash_unstripped.wasm siphash.wasm

.phony: all cargo-generate clean
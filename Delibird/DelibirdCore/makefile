RM=rm -rf build
CC=gcc

C_SRCS=$(shell find delibirdcore -iname "*.c" | tr '\n' ' ')
H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')
OBJS=$(C_SRCS:src/%.c=build/delibirdcore/%.o)

# Clean and compile .so
all: build/libdelibirdcore.so

create-dirs:
	mkdir -p build/delibirdcore

build/libdelibirdcore.so: create-dirs $(OBJS)
	$(CC) -shared -o "build/libdelibirdcore.so" $(OBJS)

build/delibirdcore/%.o: delibirdcore/%.c
	$(CC) -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

# Add debug parameters and compile
debug: CC += -DDEBUG -g
debug: all

# Clean build files
clean:
	$(RM) build

install: uninstall clean all
	cp -u build/libdelibirdcore.so /usr/lib
	cp --parents -u $(H_SRCS) /usr/include

uninstall:
	rm -f /usr/lib/libdelibirdcore.so
	rm -rf /usr/include/delibirdcore

.PHONY: all create-dirs clean install uninstall

# point it to where the compiler can find the Lua header files (lua.h, etc.)
# LUADIR = ../lua
LUADIR = /usr/include/lua5.1/

# define your own "large" integer type; not defining a proper type
# will default to 'long', which may cause problems with 'size_t'
INTTYPE = -DSTRUCT_INT="long long"


CWARNS = -Wall -W -pedantic \
        -Waggregate-return \
        -Wcast-align \
        -Wmissing-prototypes \
        -Wnested-externs \
        -Wpointer-arith \
        -Wshadow \
        -Wwrite-strings

CFLAGS = -D_POSIX_SOURCE $(CWARNS) $(INTTYPE) -O2 -I$(LUADIR)
CC = gcc

struct.so: struct.c makefile
	$(CC) $(CFLAGS) -shared -fpic -o struct.so struct.c


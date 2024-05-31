# to compile on linux, get the following binaries from its source or from your own distribution:
#  glew - https://github.com/nigels-com/glew
#  SDL2 - https://github.com/libsdl-org/SDL
#  opengl drivers




default_rule: test




#(windows^linux)
TARGET_OS = linux
#(dynamic^static)
TARGET_BUILD = dynamic

BIN = isola_example




ifeq (${TARGET_OS},linux)
 CC = clang
else ifeq (${TARGET_OS},windows)
 CC = x86_64-w64-mingw32-gcc
else
 $(error wrong TARGET_OS value)
endif

ifeq (${TARGET_BUILD},dynamic)
else ifeq (${TARGET_BUILD},static)
else
 $(error wrong TARGET_BUILD value)
endif




HDR = render.h isola.h mutil.h
SRC = main.c render.c isola.c
OBJ = ${SRC:.c=.o}

DEPS = ${SRC} ${HDR} Makefile




ifeq (${TARGET_OS},linux)

 INCS =
 ifeq (${TARGET_BUILD},static)
 # LIBS = -Wl,-Bstatic -lSDL2 -lGLEW -Wl,-Bdynamic -lGLU -lGL -lm -lX11 -lXext -lXcursor -lXi -lXfixes -lXrandr -lpthread
  LIBS = -lSDL2 -lGLEW -lGLU -lGL -lm
 else ifeq (${TARGET_BUILD},dynamic)
  LIBS = -lSDL2 -lGLEW -lGLU -lGL -lm
 endif


 #CFLAGS = -g -DISOLA_DBG -D_REENTRANT -DGLEW_STATIC
 CFLAGS = ${INCS} -Wall -Wextra -pedantic -std=c89 -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-result -Wno-sign-compare -MJ $@.json \
		  -Ofast3 -pipe -march=native
 #LDFLAGS = -v
 LDFLAGS = ${LIBS} -flto=full


else ifeq (${TARGET_OS},windows)

 INCS = -I./bin/glew-2.2.0/include -I./bin/SDL2-2.30.3/x86_64-w64-mingw32/include
 LIBS = -L./bin/glew-2.2.0/lib/Release/x64 -L./bin/SDL2-2.30.3/x86_64-w64-mingw32/lib \
		-Wl,-Bstatic -lmingw32 -lSDL2main -lSDL2 -lglew32s -lglu32 -lopengl32 -lm -static-libgcc \
		-ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -lsetupapi -lcfgmgr32 -luuid


 #CFLAGS =
 CFLAGS = ${INCS} -mwindows -std=c89 -O3 -pipe -DGLEW_STATIC -D_REENTRANT
 #LDFLAGS = 
 LDFLAGS = ${LIBS} -mwindows


endif




${OBJ}:%.o : %.c
	${CC} -c $< ${CFLAGS}

${OBJ}: ${DEPS}




ifeq (${TARGET_OS},linux)

test: ${BIN} compdb
	./${BIN}
	make clean


else ifeq (${TARGET_OS},windows)

test: ${BIN}
	make clean


endif




all: ${BIN}

${BIN}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

compdb: ${OBJ}
	./compdb.sh

clean:
	rm *.o*




.PHONY: test all clean compdb

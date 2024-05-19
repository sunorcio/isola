BIN = triakontadis
CC = clang


<<<<<<< HEAD
#CFLAGS = 
=======
#CFLAGS =
>>>>>>> 78c117ba3555663f33ab0277793cd4d3b921a663
CFLAGS = -Wall -Wextra -pedantic -std=c89 -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-result -Wno-sign-compare -MJ $@.json
OPT = -Ofast3 -pipe -march=native -flto=full
#OPT = -DISOLA_DBG -g


LIBS = -lGLEW -lGL -lGLU -lSDL2 -lm
LDFLAGS = ${LIBS}

HDR = isola.h render.h logic.h
SRC = main.c isola.c render.c logic.c
OBJ = ${SRC:.c=.o}

DEPS = ${SRC} ${HDR} Makefile



default_rule: test

test: ${BIN}
	./${BIN}
	make clean
<<<<<<< HEAD
=======

>>>>>>> 78c117ba3555663f33ab0277793cd4d3b921a663


.c.o:
	${CC} -c ${CFLAGS} ${OPT} $<

${OBJ}: ${DEPS}



all: ${BIN} Compdb clean

${BIN}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS} ${OPT}

Compdb: ${OBJ}
	./Compdb

clean:
	rm *.o*
	rm ${BIN}



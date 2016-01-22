CC=gcc
CFLAGS=-g
#CFLAGS=-O3

all: testVersion testCommit

testVersion: testVersion.o version.o
	${CC} ${CFLAGS} -o testVersion testVersion.o version.o ${LIB}

testVersion.o: testVersion.c
	${CC} ${CFLAGS} -c testVersion.c ${LIB}

version.o: version.c version.h
	${CC} ${CFLAGS} -c version.c version.h ${LIB}


testCommit: testCommit.o version.o commit.o
	${CC} ${CFLAGS} -o testCommit testCommit.o version.o commit.o ${LIB}

testCommit.o: testCommit.c
	${CC} ${CFLAGS} -c testCommit.c ${LIB}

commit.o: commit.c commit.h
	${CC} ${CFLAGS} -c commit.c commit.h ${LIB}

list.o: list.h
	${CC} ${CFLAGS} -c list.h ${LIB}


clean: 
	rm -f testVersion testCommit *~ *.o 





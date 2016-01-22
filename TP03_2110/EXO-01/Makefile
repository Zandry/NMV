.PHONY : all clean check
.ONESHELL:
LIB=$(HOME)/NMV/TP-03/EXO-01
FLAGS=-g

CHECK_PATCH=../../../../biblio/eudyptula/torvalds/scripts/checkpatch.pl

all : libread.so libfunc.so libanything.so cron_func

my_func.o: nothing.c func.h
	gcc -o $@ -Wall -fPIC -c $< $(FLAGS)

libfunc.so: my_func.o
	gcc -o $@ -shared -Wl,-soname,$@ $<

anything.o: anything.c func.h
	gcc -o $@ -Wall -fPIC -c $< $(FLAGS)

libanything.so: anything.o
	gcc -o $@ -ldl -shared -Wl,-soname,$@ $< $(FLAGS)

my_read.o: my_read.c
	gcc -o $@ -Wall -fPIC -c $< $(FLAGS)

libread.so: my_read.o
	gcc -o $@ -ldl -shared -Wl,-soname,$@ $<

%.o : %.c
	gcc -o $@ -Wall -c $< $(FLAGS)

cron_func : cron_func.o libfunc.so
	gcc -o $@ -ldl -Wall $< -L$(LIB) -lfunc $(FLAGS)

check :
	for f in *.c *.h ; do
		$(CHECK_PATCH) -f $$f
	done

clean :
	rm -rf *.o *.so cron_func

runRead: all
	LD_PRELOAD=./libread.so LD_LIBRARY_PATH=. ./cron_func

runFunc: all
	LD_LIBRARY_PATH=. ./cron_func

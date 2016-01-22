.PHONY : all clean check
.ONESHELL:

CHECK_PATCH=../../../../biblio/eudyptula/torvalds/scripts/checkpatch.pl


all : libfunc.so cron_func cron_func_static

libfunc.so : nothing.c
	gcc -fPIC -shared -o $@ $< 

%.o : %.c
	gcc -Wall -c $<

cron_func_static : cron_func.o nothing.o
	gcc -Wall $^ -o $@

cron_func : cron_func.o
	gcc -Wall $^ -o $@ -L. -lfunc

check :
	for f in *.c *.h ; do
		$(CHECK_PATCH) -f $$f
	done

clean :
	rm -rf *.o cron_func

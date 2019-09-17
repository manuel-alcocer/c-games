kentucky.o: kentuckyderby.c core.c core.h draw.c draw.h banner.txt
	cc -o ./kentucky.o ./kentuckyderby.c ./core.c ./draw.c

kentuckync.o: kentuckyderby.c core.c core.h ncdraw.c ncdraw.h banner.txt
	cc -o ./kentuckync.o -DTUI -lncurses ./kentuckyderby.c ./core.c ./ncdraw.c

all:
	make && make tui

tui:
	make ./kentuckync.o

clean:
	rm ./kentucky.o ./kentuckync.o 2>/dev/null

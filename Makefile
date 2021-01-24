
albsender: albsender.o 
	cc -O3 -o albsender albsender.o

albsender.o: albsender.c alb.h
	cc -c albsender.c

init.o: init.c alb.h
	cc -c init.c

albreceiver: albreceiver.o init.o 
	cc -O3 -o albreceiver albreceiver.o init.o

albreceiverd: albreceiverd.o init.o
	cc -O3 -o albreceiverd albreceiverd.o init.o

albreceiverd.o: albreceiverd.c alb.h
	cc -c albreceiverd.c

albreceiver.o: albreceiver.c alb.h
	cc -c albreceiver.c

install.receiver:
	install albreceiver /usr/local/bin/albreceiver

install.sender:
	install albsender /usr/local/bin/albsender

clean.sender:
	-rm albsender albsender.o

clean.receiver:
	-rm albreceiver albreceiver.o

clean: clean.sender clean.receiver


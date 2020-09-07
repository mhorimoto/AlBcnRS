
albsender: albsender.o
	cc -O3 -o albsender albsender.o

albsender.o: albsender.c
	cc -c albsender.c

init.o: init.c
	cc -c init.c

albreceiver: albreceiver.o init.o
	cc -O3 -o albreceiver albreceiver.o init.o

albreceiver.o: albreceiver.c
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


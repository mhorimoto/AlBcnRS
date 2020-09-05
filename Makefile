PROGRAM = albsender
SOURCE  = albsender.c

albsender: albsender.o
	cc -O3 -o albsender albsender.o

albsender.o: albsender.c
	cc -c albsender.c

install.sender:
	install albsender /usr/local/bin/albsender

clean.sender:
	-rm albsender albsender.o

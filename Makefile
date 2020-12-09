CC=gcc
CFLAGS=-Wall -Wextra -g

.PHONY:run
.PHONY:clean

download: download.o parser.o utils.o
	$(CC) download.o parser.o utils.o -o download $(CFLAGS)

download.o: download.c
	$(CC) -c download.c $(CFLAGS)

parser.o: parser.h parser.c
	$(CC) -c parser.c $(CFLAGS)

utils.o: utils.h utils.c

clean:
	rm download parser.o utils.o download.o

#make instructions
defaultUrl=ftp://anonymous:password@ftp.fe.up.pt/welcome.msg
run: download #run the file
	clear
	./download $(defaultUrl)

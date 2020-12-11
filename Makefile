CC=gcc
CFLAGS=-Wall -Wextra -g

.PHONY:run
.PHONY:clean

download: download.o parser.o utils.o getip.o ftp.o
	$(CC) download.o parser.o utils.o getip.o ftp.o -o download $(CFLAGS)

download.o: download.c
	$(CC) -c download.c $(CFLAGS)

parser.o: parser.h parser.c
	$(CC) -c parser.c $(CFLAGS)

utils.o: utils.h utils.c
	$(CC) -c utils.c $(CFLAGS)

getip.o: getip.h getip.c
	$(CC) -c getip.c $(CFLAGS)

ftp.o: ftp.h ftp.c
	$(CC) -c ftp.c $(CFLAGS)

clean:
	rm download parser.o utils.o download.o

#make instructions
defaultUrl=ftp://anonymous:password@ftp.fe.up.pt/welcome.msg
run: download #run the file
	clear
	./download $(defaultUrl)

CC=g++

a.out: main.o prog1.o slp.o util.o
	${CC} -g main.o prog1.o slp.o util.o

main.o: main.cc slp.h util.h
	${CC} -g -c main.cc

prog1.o: prog1.cc slp.h util.h
	${CC} -g -c prog1.cc

slp.o: slp.cc slp.h util.h
	${CC} -g -c slp.cc

util.o: util.cc util.h
	${CC} -g -c util.cc

clean: 
	rm -f a.out util.o prog1.o slp.o main.o

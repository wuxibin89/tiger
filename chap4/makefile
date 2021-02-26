a.out: parse.o absyn.o prabsyn.o symbol.o table.o y.tab.o lex.yy.o errormsg.o util.o
	cc -g parse.o absyn.o prabsyn.o symbol.o table.o y.tab.o lex.yy.o errormsg.o util.o

absyn.o: absyn.c
	cc -g -c absyn.c

parse.o: parse.c
	cc -g -c parse.c

prabsyn.o: prabsyn.c
	cc -g -c prabsyn.c

symbol.o: symbol.c
	cc -g -c symbol.c

table.o: table.c
	cc -g -c table.c

y.tab.o: y.tab.c
	cc -g -c y.tab.c

y.tab.c: tiger.y
	yacc -b y -d -v tiger.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

errormsg.o: errormsg.c
	cc -g -c errormsg.c

lex.yy.o: lex.yy.c
	cc -g -c lex.yy.c -I../chap2

lex.yy.c: ../chap2/tiger.l
	flex ../chap2/tiger.l

util.o: util.c
	cc -g -c util.c

clean: 
	rm -f *.out y.tab.* lex.yy.c *.o *.log *.output

test:
	@for num in {1..48}; do \
		fname="../testcases/test$$num.tig"; \
		echo "syntax analyze $$fname ... \c"; \
		./a.out $$fname > test.log 2>&1; \
		if [[ $$(echo $$?) != 0 ]]; then \
			echo "fail"; \
			exit 1; \
		else \
			echo "success"; \
		fi \
	done
	@for file in merge.tig queens.tig; do \
		fname="../testcases/$$file"; \
		echo "syntax analyze $$fname ... \c"; \
		./a.out $$fname > test.log 2>&1; \
		if [[ $$(echo $$?) != 0 ]]; then \
			echo "fail"; \
			break; \
		else \
			echo "success"; \
		fi \
	done

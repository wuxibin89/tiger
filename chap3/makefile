a.out: parsetest.o y.tab.o lex.yy.o errormsg.o util.o
	cc -g parsetest.o y.tab.o lex.yy.o errormsg.o util.o

parsetest.o: parsetest.c errormsg.h util.h
	cc -g -c parsetest.c

y.tab.o: y.tab.c
	cc -g -c y.tab.c

y.tab.c: tiger.y
	yacc -b y -d -v tiger.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

errormsg.o: errormsg.c errormsg.h util.h
	cc -g -c errormsg.c

lex.yy.o: lex.yy.c y.tab.h errormsg.h util.h
	cc -g -c lex.yy.c

#lex.yy.c: tiger.lex
#	lex tiger.lex

util.o: util.c util.h
	cc -g -c util.c

clean: 
	rm -f a.out util.o parsetest.o lex.yy.o errormsg.o y.tab.c y.tab.h y.tab.o y.output

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


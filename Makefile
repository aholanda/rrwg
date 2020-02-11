CFLAGS := -Wall -I./include -g
LDFLAGS := -lm

VPATH = src:include

rrwg: alloc.o graph.o io.o parse.o plot.o walk.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

trash += rrwg

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

trash += *.o

rrwg.1: rrwg.md
	pandoc -s -t man $< -o $@

trash += rrwg.1

clean:
	$(RM) -r $(trash)

checkalloc: rrwg
	valgrind -v --leak-check=full ./$<

help:
	@echo "---------------------------------------------------------------------"
	@echo "* RRWG - possible targets:"
	@echo "make rrwg"
	@echo " \t=> Compile the program generating the executable 'rrwg'."
	@echo "make rrwg.1"
	@echo " \t=> Generate man page of the program. Depends on Pandoc."
	@echo "make clean"
	@echo " \t=> Clean up all generated files."
	@echo "make checkalloc"
	@echo "\t=> Check program memory management."
	@echo "---------------------------------------------------------------------"

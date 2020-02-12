CFLAGS := -Wall -I./include -g
LDFLAGS := -lm
PREFIX := /usr/local

VPATH = src:include

rrwg: alloc.o data.o graph.o io.o main.o parse.o R.o walk.o
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
	valgrind -v --leak-check=full --track-origins=yes ./$<

install: rrwg rrwg.1
	@install rrwg $(PREFIX)/bin
	@install rrwg.1 $(PREFIX)/man/man1

uninstall:
	@rm -vf rrwg $(PREFIX)/bin/rrwg
	@rm -vf rrwg.1 $(PREFIX)/man/man1/rrwg.1

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
	@echo "make install"
	@echo "\t=> Intall the program using as prefix $(PREFIX)."
	@echo "make uninstall"
	@echo "\t=> Remove the program from prefix $(PREFIX)."
	@echo "---------------------------------------------------------------------"

.PHONY: clean help install uninstall

CC := gcc
CFLAGS := -Wall -I./include -g
LDFLAGS := -lm
PREFIX := /usr/local

VPATH = src:include

default: help

rrwg: wrapper.o input.o graph.o rrwg.o output.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

trash += rrwg *.dat *.log *.pdf *.R

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

trash += *.o

man: rrwg.1

rrwg.1: rrwg.md
	pandoc -s -t man $< -o $@

trash += rrwg.1

clean:
	$(RM) -rv $(trash)

checkalloc: rrwg example0.net
	valgrind -v --leak-check=full --track-origins=yes ./$^

install: rrwg rrwg.1
	@install -v rrwg $(PREFIX)/bin
	@install -v rrwg.1 $(PREFIX)/man/man1
	@echo "Successfully installed!"

uninstall:
	@rm -vf rrwg $(PREFIX)/bin/rrwg
	@rm -vf rrwg.1 $(PREFIX)/man/man1/rrwg.1
	@echo "Successfully Uninstalled!"

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

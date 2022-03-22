.POSIX:
PREFIX = /usr/local

.SUFFIXES:
all:
	cc -O3 -ansi -Wall -Wextra -Wno-missing-field-initializers -pedantic src/pop.c -o "pop"
	cc -O3 -ansi -Wall -Wextra -Wno-missing-field-initializers -pedantic src/pop-shield.c -o "pop-shield"
install:
	mkdir -p $(PREFIX)/bin
	mv pop $(PREFIX)/bin
	mv pop-shield $(PREFIX)/bin
uninstall:
	rm $(PREFIX)/bin/pop
	rm $(PREFIX)/bin/pop-shield

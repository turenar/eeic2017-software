CFLAGS = -Wall -Wextra -pedantic -g -lm
CC = gcc $(CFLAGS) -std=c99
CXX = g++ $(CFLAGS)
SRC = kadai01.c kadai02.c kadai03.c kadai04.c kadai05.c kadai06.c kadai07.c kadai08.c kadai09.c kadai10.c kadai11.c
DIR = 161013
ZIP = ../学籍番号.zip
EXE = kadai01.bin kadai02.bin kadai03.bin kadai04.bin kadai05.bin kadai06.bin kadai07.bin kadai08.bin kadai09.bin kadai10.bin kadai11.bin
all: Makefile $(EXE) $(ZIP)
.SUFFIXES: .bin
.c.bin:
	$(CC) $< -o $@
	$(CXX) $< -o $@.g++ && rm $@.g++
$(ZIP): $(EXE) $(SRC)
	-rm -f $@
	zip $@ $(SRC)
clean:
	-rm -f $(EXE)
.PRECIOUS: Makefile
Makefile: ../genmake.sh
	../genmake.sh

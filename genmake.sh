#!/bin/sh
DIR="$(basename "$(pwd)")"
exec 3> Makefile
echo 'CFLAGS = -Wall -Wextra -pedantic -g -lm' >&3
echo 'CC = gcc $(CFLAGS) -std=c99' >&3
echo 'CXX = g++ $(CFLAGS)' >&3
echo 'SRC =' kadai*.c >&3
echo 'DIR =' "${DIR}" >&3
echo 'ZIP =' '../学籍番号.zip' >&3
printf 'EXE =' >&3
for i in kadai*.c; do
	printf ' %s' "${i%.c}.bin" >&3
done
echo >&3

echo 'all: Makefile $(EXE) $(ZIP)' >&3

cat <<'EOD' >&3
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
EOD

echo 'Makefile:' $0 >&3
echo "	$0" >&3

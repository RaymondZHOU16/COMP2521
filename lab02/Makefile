# COMP2521 - Week 2 Lab

CC = gcc
CFLAGS = -Wall -Werror -g
SOURCE_FILES = gcd.c rabbits.c listTail.c listMax.c listShift.c listSum.c listInsertOrdered.c listInsertNth.c

########################################################################

.PHONY: all
all: gcd rabbits listTail listMax listShift listSum listInsertOrdered listInsertNth

gcd: gcd.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

rabbits: rabbits.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

listTail: listTail.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

listMax: listMax.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

listShift: listShift.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

listSum: listSum.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

listInsertOrdered: listInsertOrdered.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

listInsertNth: listInsertNth.c
	@cp $< $<.bak
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f gcd rabbits listTail listMax listShift listSum listInsertOrdered listInsertNth $(addsuffix .bak, $(SOURCE_FILES))


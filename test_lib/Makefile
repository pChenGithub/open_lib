target1+=liblevel1.so test
target2+=liblevel2.so test2
#
CC=gcc

# target all
all: ${target1} ${target2} 

test: test.c
	${CC} -o $@ $< -L. -llevel1

test2: test2.c
	${CC} -o $@ $< -L. -llevel2

liblevel1.so: level1.c
	${CC} -shared -o $@ $<

liblevel2.so: level2.c
	${CC} -shared -o $@ $<

# clean
PHONY: clean
clean:
	rm ${target1} ${target2} 


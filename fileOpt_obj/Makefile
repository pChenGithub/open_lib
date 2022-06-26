target=test


all: ${target}
	${debuger} ./${target}

${target} : test.c fileXOpt.c
	${CC} -o $@ $^	

clean:
	rm ${target}
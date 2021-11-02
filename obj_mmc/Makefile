target+=test_mmc


all: ${target}
	${DEBUGER} ./${target}

${target} : test.c mmc_opt.c
	${CC} $^ -o $@
target+=testParser
#target+=testFill

all: ${target}
	${DEBUGER} ./${target}


${target}: ${target}.o jsonlibC/jsonlib.c jsonlibC/cJSON.c

clean:
	rm *.o ${target}


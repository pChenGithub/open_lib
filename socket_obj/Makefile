pro ?= test

all: ${pro}
	$(DEBUGGER) ./${pro} ${ARGS}

${pro}: test.c link/net_link.c

clean:
	rm ${pro}
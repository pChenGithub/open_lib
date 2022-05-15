pro ?= test
pro_gate ?= test_gate

all: ${pro} ${pro_gate}
	$(DEBUGGER) ./${pro_gate} ${ARGS}

${pro}: test.c link/net_link.c

${pro_gate} : test_gate.c link/net_opt.c 

clean:
	rm ${pro}
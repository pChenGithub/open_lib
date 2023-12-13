target += testTask

src += core/task_core.c ../dstruct/linkedlist/linkedlist.c
CFLAGS += -I../dstruct
#LDFLAGS += -lpthread
all: ${target} 

${target} :  ${src} -lpthread

clean:
	rm ${target}

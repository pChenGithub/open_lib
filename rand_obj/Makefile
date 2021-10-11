target += rand_test


all: $(target)
	$(debuger) ./$(target)

$(target): test.c rand_ops.c
	$(CC) -o $@ $^
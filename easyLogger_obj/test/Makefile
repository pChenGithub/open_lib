export =dev

target = testc
target2 = testcpp

logfile = $(wildcard ./log/*.c)
logfileO = $(patsubst %.c, %.o, $(logfile))

all: $(target) $(target2)
	$(DEBUGER) ./$(target2)


$(target): main.c $(logfile)
	$(CC) -o $@ $^ -Ilog -lpthread

$(target2): test.o $(logfileO)
	$(CXX) -o $@ $^ -lpthread

%.o: %.c
	$(CC) -o $@ -c $< -Ilog

%.o: %.cpp 
	$(CXX) -o $@ -c $< -Ilog

clean:
	rm *.o log/*.o testc testcpp
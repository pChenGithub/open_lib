pro ?= http_download_file

all: $(pro)
	$(DEBUGGER) ./$(pro) $(ARGS)

$(pro): main.c http_download_file.c
	$(CC) $^ -o $@ -I../out/x86/include -L../out/x86/lib -lcurl

clean:
	rm $(pro)

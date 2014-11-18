CFLAGS = -Wall -lncurses
FILES = main

all: $(FILES)

clean:
	rm -f $(FILES) *.zip

CFLAGS=-Wall -std=c11 -pedantic-errors
BUILD_DIR=build_res

all: clean renderer

mk_build_dir:
	mkdir -p $(BUILD_DIR)

renderer: mk_build_dir
	$(CC) $(CFLAGS) main.c tga.c model.c -o $(BUILD_DIR)/$@ -lm

clean:
	rm -rf build_res

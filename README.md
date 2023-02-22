## Instructions for development
* Before you start, run `make init` to generate the directories needed. `src` directory should only contain source files while `include` directory should only contain header files. You can make multiple directories with any structure you want.
* `clang` is used in the `Makefile`, along with `lldb` for debugging.
* To compile and run the program, run `./compile_and_run.sh` for debugging mode, and `./compile_and_run.sh -r` for release mode. See `./compile_and_run.sh -h` for more options.


C_FILES = $(wildcard *.c)
C_OBJECTS = $(patsubst %.c,%.o,$(C_FILES))

.PHONY: help test gdb
 
help:
	@echo "There are several ways of using this makefile:"
	@echo "1. 'make test' will run the tests as a shared library using the cgreen-runner"
	@echo "2. 'make gdb' will compile the tests and launch gdb for the test(s) in the main() funciton."
	@echo "3. 'make all_tests' will compile the tests and you can run the test(s) that are in the main() function."

gdb: all_tests
	gdb ./all_tests
all_tests: $(C_OBJECTS)
	gcc -o $@ $(C_OBJECTS) -lcgreen -g
all_tests.so: $(C_OBJECTS)
	gcc -shared -o all_tests.so -fPIC $(C_OBJECTS) -lcgreen
$(C_OBJECTS): %.o : %.c queue.h
	gcc -c $(@:.o=.c) -o $@ -Wall -g
test: all_tests.so
	cgreen-runner ./all_tests.so
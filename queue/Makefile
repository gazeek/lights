
C_FILES = $(wildcard *.c)
C_OBJECTS = $(patsubst %.c,%.o,$(C_FILES))

.PHONY: all test gdb
 
all: all_tests.so
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
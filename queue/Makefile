
C_FILES = $(wildcard *.c)
C_OBJECTS = $(patsubst %.c,%.o,$(C_FILES))

.PHONY: all test
 
all: all_tests.so
all_tests.so: $(C_OBJECTS)
	gcc -shared -o all_tests.so -fPIC $(C_OBJECTS) -lcgreen
$(C_OBJECTS): %.o : %.c queue.h
	gcc -c $(@:.o=.c) -o $@ -Wall
test: all_tests.so
	cgreen-runner ./all_tests.so
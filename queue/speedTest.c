#include <stdlib.h>
#include "queue.h"

int main()
{
	queue_t q;
	element_t e;
	e.time_ms = 20;
	size_t testSize = 1000000000;
	queue_init(&q, sizeof(e), testSize);
	for(int i = 0; i < testSize; i++)
	{
		queue_push_back(&q, &e);
		queue_pop_front(&q, &e);
	} 
}
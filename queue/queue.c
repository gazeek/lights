#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

error_state_t queue_element_check_null(queue_t *q, void *element);
error_state_t queue_copy_from_next(queue_t *q);

void queue_init(queue_t *q, size_t element_size, size_t capacity)
{
    if(q != NULL)
    {
        q->buffer = (unsigned char *) malloc(capacity * element_size);
        if(q->buffer == NULL)
            perror("malloc: failed to allocate memory for queue buffer");
        q->capacity = capacity;
        q->element_count = 0;
        q->element_size = element_size;
        q->front_idx = 0;
        q->back_idx = 0;
        q->next_queue = NULL;
    }
}

error_state_t queue_push_back(queue_t *q, void *element)
{
#if LIGTHS_QUEUE_UNSAFE
    error_state_t es = queue_element_check_null(q, element);
    if(es != ERROR_NONE)
        return es;
    if(q->element_size == q->element_count)
    {
        perror("Requested push for full queue");
        return ERROR_FULL_QUEUE;
    }
#endif //LIGHTS_QUEUE_UNSAFE

    memcpy((q->buffer) + (q->back_idx * q->element_size), element, q->element_size);
    q->back_idx++;
    if(q->back_idx == q->capacity)
        q->back_idx = 0;
    q->element_count++;

    return ERROR_NONE;
}

error_state_t queue_pop_front(queue_t *q, void *element)
{
#if LIGTHS_QUEUE_UNSAFE
    error_state_t es = queue_element_check_null(q, element);
    if(es != ERROR_NONE)
        return es;
    if(q->front_idx == q->back_idx)
    {
        perror("Requested queue pop for empty queue");
        return ERROR_EMPTY_QUEUE;
    }
#endif //LIGHTS_QUEUE_UNSAFE

    memcpy(element, (q->buffer) + (q->front_idx * q->element_size), q->element_size);
    q->front_idx++;
    // wrap queue around to zero
    if(q->front_idx == q->capacity)
        q->front_idx = 0;
    q->element_count--;

    // here we add mutex and copy from the next queue

    return ERROR_NONE;
}

// Convenience function
error_state_t queue_element_check_null(queue_t *q, void *element)
{
    if(element == NULL)
    {
        perror("Requested queue pop for null-pointer element");
        return ERROR_NULL_ELEMENT;
    }
    if(q == NULL)
    {
        perror("Requested queue pop for null-pointer queue");
        return ERROR_NULL_QUEUE;
    }
    if(q->buffer == NULL)
    {
        perror("Requested queue pop for null-pointer queue buffer");
        return ERROR_NULL_BUFFER;
    }
    return ERROR_NONE;
}
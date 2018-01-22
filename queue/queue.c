#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"

#include <pthread.h>

// NOTE: we are using one global lock to lock the next_queue variable for all queues
static pthread_mutex_t next_queue_lock;

static uint lib_initialized = 0;

static error_state_t init_queue_lib();
static error_state_t queue_element_check_null(queue_t *q, void *element);
static error_state_t queue_check_null(queue_t *q);
static void dumb_buffer_copy(queue_t *qto, queue_t *qfrom, size_t element_count);
static void * find_last_queue(queue_t *q);

error_state_t queue_init(queue_t *q, size_t element_size, size_t capacity)
{
    if(q != NULL)
    {
        q->buffer = (unsigned char *) malloc(capacity * element_size);
        if(q->buffer == NULL)
        {
            printf("malloc: failed to allocate memory for queue buffer\n");
            return ERROR_MALLOC_FAIL;
        }
        q->capacity = capacity;
        q->element_count = 0;
        q->element_size = element_size;
        q->front_idx = 0;
        q->back_idx = 0;
        q->next_queue = NULL;
    }
    return init_queue_lib();
}


error_state_t init_queue_lib()
{
    if(lib_initialized)
        return ERROR_NONE;

    if(pthread_mutex_init(&next_queue_lock, NULL) != 0)
    {
        printf("mutex: lock init failed");
        return ERROR_MUTEX_FAIL;
    }
    return ERROR_NONE;
}


error_state_t queue_push_back(queue_t *q, void *element)
{
#if !LIGTHS_QUEUE_UNSAFE
    error_state_t es = queue_element_check_null(q, element);
    if(es != ERROR_NONE)
        return es;
    if(q->element_size == q->element_count)
    {
        printf("Requested push for full queue\n");
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
#if !LIGTHS_QUEUE_UNSAFE
    error_state_t es = queue_element_check_null(q, element);
    if(es != ERROR_NONE)
        return es;
    if(q->element_count == 0)
    {
        printf("Requested queue pop for empty queue\n");
        return ERROR_EMPTY_QUEUE;
    }
#endif //LIGHTS_QUEUE_UNSAFE

    memcpy(element, (q->buffer) + (q->front_idx * q->element_size), q->element_size);
    q->front_idx++;
    // wrap queue around to zero
    if(q->front_idx == q->capacity)
        q->front_idx = 0;
    q->element_count--;

    return ERROR_NONE;
}


error_state_t queue_append_queue(queue_t *q, queue_t *append)
{
    // FIMXE: do some data validation here
    // TODO: especially check if element_size is compatible
    pthread_mutex_lock(&next_queue_lock);
    queue_t * last_q = find_last_queue(q);
    last_q->next_queue = append;
    pthread_mutex_unlock(&next_queue_lock);
    return ERROR_NONE;
}

static void * find_last_queue(queue_t *q)
{
    // FIMXE: do some data validation here
    queue_t * next_queue = q;
    while(next_queue->next_queue != NULL)
        next_queue = next_queue->next_queue;
    return next_queue;
}

error_state_t queue_copy_from_next(queue_t *q)
{
    queue_t *qnext;
#if !LIGHTS_QUEUE_UNSAFE
    error_state_t es = queue_check_null(q);
    if(es != ERROR_NONE)
        return es;
#endif //LIGHTS_QUEUE_UNSAFE
    qnext = q->next_queue;
    if(qnext != NULL)
    {
#if !LIGHTS_QUEUE_UNSAFE
        es = queue_check_null(qnext);
        if(es != ERROR_NONE)
            return es;
        // Check if element size is compatible
#endif // LIGHTS_QUEUE_UNSAFE
        size_t element_copy_count_back_to_end = q->capacity - q->back_idx;
        if(element_copy_count_back_to_end >= qnext->element_count)
        {
            dumb_buffer_copy(q, qnext, qnext->element_count);
        }
        else
        {
            dumb_buffer_copy(q, qnext, element_copy_count_back_to_end);
            dumb_buffer_copy(q, qnext, qnext->element_count); // we copy what's left
        }
        // size_t element_copy_count_begin_to_front = q->front_idx;
        // // FIXME: when copying wraparound is not supported
 
        // memcpy(q->buffer,
        //        qnext->buffer + qnext->front_idx + element_copy_count_back_to_end,
        //        element_copy_count_begin_to_front * q->element_size);
        // update all inde

        pthread_mutex_lock(&next_queue_lock);
        //TODO: if qnext becomes empty, remove it from list and update 
        // appropriate qnext
        pthread_mutex_unlock(&next_queue_lock);
    }
    return ERROR_NONE;
}

static void dumb_buffer_copy(queue_t *qto, queue_t *qfrom, size_t element_count)
{
    // This function is to avoid code duplication
    // It does not check for anything so watch for SEGFAULTS
    memcpy(qto->buffer + (qto->back_idx * qto->element_size),
           qfrom->buffer + (qfrom->front_idx * qto->element_size),
           element_count * qto->element_size);
    qto->back_idx += element_count;
    qto->element_count += element_count;
    if(qto->back_idx == qto->capacity)
        qto->back_idx = 0;

    // Update qfrom
    qfrom->element_count -= element_count;
    qfrom->front_idx += element_count;
}


// Convenience function
static error_state_t queue_element_check_null(queue_t *q, void *element)
{
    if(element == NULL)
    {
        printf("Requested queue pop for null-pointer element\n");
        return ERROR_NULL_ELEMENT;
    }
    return queue_check_null(q);
}

static error_state_t queue_check_null(queue_t *q)
{
    if(q == NULL)
    {
        printf("Requested queue pop for null-pointer queue\n");
        return ERROR_NULL_QUEUE;
    }
    if(q->buffer == NULL)
    {
        printf("Requested queue pop for null-pointer queue buffer\n");
        return ERROR_NULL_BUFFER;
    }
    return ERROR_NONE;
}
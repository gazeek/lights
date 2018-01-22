
#ifndef LIGHTS_QUEUE_H
#define LIGHTS_QUEUE_H

#define LIGHTS_QUEUE_UNSAFE 0
#define VERBOSE 0

enum error_state {
    ERROR_NONE,
    ERROR_NULL_QUEUE,
    ERROR_NULL_ELEMENT,
    ERROR_NULL_BUFFER,
    ERROR_EMPTY_QUEUE,
    ERROR_FULL_QUEUE,
    ERROR_MALLOC_FAIL,
    ERROR_MUTEX_FAIL
};
typedef enum error_state error_state_t;

typedef struct{
    unsigned int time_ms;
    unsigned int brightness;
} element_t;

typedef struct{
    size_t front_idx;
    size_t back_idx;
    size_t capacity;
    _Atomic size_t element_count;
    size_t element_size;
    unsigned char *buffer;
    void * next_queue; // TODO:this can allow for asynchronous queue addition
} queue_t;

error_state_t queue_init(queue_t *q, size_t element_size, size_t size);
error_state_t queue_push_back(queue_t *q, void *element);
error_state_t queue_pop_front(queue_t *q, void *element);
error_state_t queue_append_queue(queue_t *q, queue_t *append);
error_state_t queue_copy_from_next(queue_t *q);


#endif // LIGHTS_QUEUE_H
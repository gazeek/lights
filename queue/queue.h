
#ifndef LIGHTS_QUEUE_H
#define LIGHTS_QUEUE_H

#define LIGHTS_QUEUE_UNSAFE 0
#define VERBOSE 0

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


enum error_state {
    ERROR_NONE,
    ERROR_NULL_QUEUE,
    ERROR_NULL_ELEMENT,
    ERROR_NULL_BUFFER,
    ERROR_EMPTY_QUEUE,
    ERROR_FULL_QUEUE,
    ERROR_MALLOC_FAIL,
    ERROR_MUTEX_FAIL,
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
    unsigned char must_be_deallocated; // used to determine if queue needs to be freed
} queue_t;

// Useful function if the queue is not dynamic.
error_state_t queue_init(queue_t *q, size_t element_size, size_t size);

// Create queue is a full constructor type function and inits the queue for you.
void *create_queue(size_t element_size, size_t capacity);
void queue_delete(queue_t *q);

// This operation is not thread-safe
// It copies the memory under element
error_state_t queue_push_back(queue_t *q, void *element);

// This function is thread safe if accessed from only one thread
// That is, you can use all other thread safe functions and they will not interfere
// with this function
error_state_t queue_pop_front(queue_t *q, void *element);

// Flush and empty the queue and print the results to stdout (for debugging).
error_state_t queue_flush_and_print(queue_t * q);

// The following functions are completely thread-safe:
error_state_t queue_append_queue(queue_t *q, queue_t *append);
error_state_t queue_copy_from_next(queue_t *q);


#endif // LIGHTS_QUEUE_H
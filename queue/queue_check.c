#include <cgreen/cgreen.h>
#include "queue.h"

Describe(lights_queue);
BeforeEach(lights_queue) {}
AfterEach(lights_queue) {}

Ensure(lights_queue, test_init)
{
    // Simply check init works
    queue_t q;
    element_t e;
    queue_init(&q, sizeof(e), 10);
    assert_that(q.buffer, is_non_null);
}

Ensure(lights_queue, push_pop_works)
{
    queue_t q;
    element_t e;
    unsigned int time_ms = 30;
    unsigned int brightness = 60;
    queue_init(&q, sizeof(e), 10);
    e.time_ms = time_ms;
    e.brightness = brightness;
    queue_push_back(&q, &e);
    queue_pop_front(&q, &e);
    assert_that(e.time_ms, is_equal_to(time_ms));
    assert_that(e.brightness, is_equal_to(brightness));
}

Ensure(lights_queue, multi_push_pop_works)
{
    queue_t q;
    element_t e;
    unsigned int time_ms = 30;
    unsigned int brightness = 60;
    queue_init(&q, sizeof(e), 10);
    size_t i;
    for(i = 0; i < 3; i++)
    {
        e.time_ms = time_ms * (double)(i) / 3;
        e.brightness = brightness;
        queue_push_back(&q, &e);
    }
    for(i = 0; i < 3; i++)
    {
        queue_pop_front(&q, &e);
        time_ms = e.time_ms;
        brightness = e.brightness;
    }        
    assert_that(e.time_ms, is_equal_to(time_ms));
    assert_that(e.brightness, is_equal_to(brightness));
}

Ensure(lights_queue, push_pop_wraps_around)
{
    queue_t q;
    element_t e;
    size_t i;
    size_t cap = 6;
    queue_init(&q, sizeof(e), cap);
    unsigned int time_ms_in[cap * 2];
    unsigned int time_ms_out[cap * 2];
    unsigned int brightness_in[cap * 2];
    unsigned int brightness_out[cap * 2];

    for(i = 0; i < 2 * cap; i++)
    {
        time_ms_in[i] = i;
        e.time_ms = time_ms_in[i];
        brightness_in[i] = i * 10;
        e.brightness = brightness_in[i];
        queue_push_back(&q, &e);
        queue_pop_front(&q, &e);
        time_ms_out[i] = e.time_ms;
        brightness_out[i] = e.brightness;
    } 
    assert_that(time_ms_in, 
                is_equal_to_contents_of(time_ms_out, sizeof(time_ms_out)));
    assert_that(brightness_in,
                is_equal_to_contents_of(brightness_out, sizeof(brightness_out)));
}

Ensure(lights_queue, append_queue_copies_new_queue)
{
    queue_t q, qnext;
    element_t e;
    size_t i;
    size_t cap = 10;
    size_t four = 4;
    queue_init(&q, sizeof(e), cap);
    queue_init(&qnext, sizeof(e), four);
    unsigned int time_ms_in[four * 2];
    unsigned int time_ms_out[four * 2];
    unsigned int brightness_in[four * 2];
    unsigned int brightness_out[four * 2];

    for(i = 0; i < four; i++)
    {
        time_ms_in[i] = i;
        e.time_ms = time_ms_in[i];
        brightness_in[i] = i * 10;
        e.brightness = brightness_in[i];
        queue_push_back(&q, &e);
    }

    for(i = four; i < 2 * four; i++)
    {
        time_ms_in[i] = i;
        e.time_ms = time_ms_in[i];
        brightness_in[i] = i * 10;
        e.brightness = brightness_in[i];
        queue_push_back(&qnext, &e);
    }

    queue_append_queue(&q, &qnext);

    for (i = 0; i < four * 2; i++)
    {
        queue_pop_front(&q, &e);
        time_ms_out[i] = e.time_ms;
        brightness_out[i] = e.brightness;
    } 
    assert_that(time_ms_in, 
                is_equal_to_contents_of(time_ms_out, sizeof(time_ms_out)));
    assert_that(brightness_in,
                is_equal_to_contents_of(brightness_out, sizeof(brightness_out)));
}

Ensure(lights_queue, append_queue_copies_new_queue_wraps_around)
{
    queue_t q, qnext;
    element_t e;
    size_t i;
    size_t cap = 10;
    size_t four = 4;
    queue_init(&q, sizeof(e), cap);
    queue_init(&qnext, sizeof(e), four);
    unsigned int time_ms_in[four * 2];
    unsigned int time_ms_out[four * 2];
    unsigned int brightness_in[four * 2];
    unsigned int brightness_out[four * 2];

    for(i = 0; i < four; i++)
    {
        queue_push_back(&q, &e);
        queue_pop_front(&q, &e);
    }

    for(i = 0; i < four; i++)
    {
        time_ms_in[i] = i;
        e.time_ms = time_ms_in[i];
        brightness_in[i] = i * 10;
        e.brightness = brightness_in[i];
        queue_push_back(&q, &e);
    }

    for(i = four; i < 2 * four; i++)
    {
        time_ms_in[i] = i;
        e.time_ms = time_ms_in[i];
        brightness_in[i] = i * 10;
        e.brightness = brightness_in[i];
        queue_push_back(&qnext, &e);
    }

    queue_append_queue(&q, &qnext);

    for (i = 0; i < four * 2; i++)
    {
        queue_pop_front(&q, &e);
        time_ms_out[i] = e.time_ms;
        brightness_out[i] = e.brightness;
    } 
    assert_that(time_ms_in, 
                is_equal_to_contents_of(time_ms_out, sizeof(time_ms_out)));
    assert_that(brightness_in,
                is_equal_to_contents_of(brightness_out, sizeof(brightness_out)));
}




int main()
{
    TestSuite *suite = create_test_suite();
    add_test_with_context(suite, lights_queue, append_queue_copies_new_queue);
    return run_single_test(suite, "append_queue_copies_new_queue", create_text_reporter());
}
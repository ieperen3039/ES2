#ifndef TIMER_H
#define TIMER_H

//function to start a new timer
//note that timers can be nested!
void timer_start();

//get current time passed in us
long int timer_get();

//stop timer and get passed time in us
long int timer_stop();

//destroy the current timer object (if nested the previous one will become active)
void timer_destroy();

//time with empty name
#define GET_TIMER(func) TIMED_FUNCTION_NAMED("", func);
#ifdef TIMING

//timeit macro to be wrapped around what should be timed
#define GET_NAMED_TIMER(name, func) \
    printf("Starting timer %s\n----------\n", name);\
    timer_start();\
    func;\
    printf("----------\nEnding timer %s\nTime taken: %ld us (%lf seconds)\n",name, timer_stop(), (double)timer_stop()/1000000.0 );\
    timer_destroy();
#else

//Dummy defines - nothing will be timed
#define GET_NAMED_TIMER(name, func) func

#endif
#endif

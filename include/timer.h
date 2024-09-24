#ifndef SMINES_TIMER_H
#define SMINES_TIMER_H

struct Timer {
    int fd;
};

// if error, returns -1 and errno is set by related clock/timer methods called
int timer_start(struct Timer *timer);
// returns -1 if error, and errno will be set
int timer_get_time(struct Timer *timer, struct timespec *out);
void timer_cleanup(struct Timer *timer);

#endif

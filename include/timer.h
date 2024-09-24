#ifndef SMINES_TIMER_H
#define SMINES_TIMER_H

struct Timer {
    int fd;
};

// if error, returns -1 and errno is set by related clock/timer methods called
int timer_start(struct Timer *timer);
timespec timer_get_time(struct Timer *timer);
void timer_cleanup(struct Timer *timer);

#endif

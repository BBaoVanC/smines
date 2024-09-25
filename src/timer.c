#include "timer.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>

int timer_start(struct Timer *timer) {
    struct timespec start_time = {0};
    int gettime_res = clock_gettime(CLOCK_MONOTONIC, &start_time);
    if (gettime_res == -1) {
        return -1;
    }
    timer->start = start_time;

    int fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (fd == -1) {
        return -1;
    }
    timer->fd = fd;

    struct itimerspec spec = {
        .it_interval = {
            .tv_sec = 1,
            .tv_nsec = 0,
        },
        .it_value = {
            .tv_sec = 1,
            .tv_nsec = 0,
        },
    };
    int settime_res = timerfd_settime(timer->fd, 0, &spec, NULL);
    if (settime_res == -1) {
        return -1;
    }

    return 0;
}

static struct timespec subtract_timespecs(const struct timespec end, const struct timespec start) {
    if (end.tv_nsec > start.tv_nsec) {
        return (struct timespec) {
            .tv_sec = end.tv_sec - start.tv_sec,
            .tv_nsec = end.tv_nsec - start.tv_nsec,
        };
    } else {
        return (struct timespec) {
            .tv_sec = end.tv_sec - start.tv_sec - 1,
            .tv_nsec = 1000000000 - start.tv_nsec + end.tv_nsec,
        };
    }
}
int timer_get_time(struct Timer *timer, struct timespec *out) {
    struct timespec now = {0};
    int res = clock_gettime(CLOCK_MONOTONIC, &now);
    if (res == -1) {
        return -1;
    }
    *out = subtract_timespecs(now, timer->start);
}

void timer_cleanup(struct Timer *timer) {
    close(timer->fd);
}

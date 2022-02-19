/* See LICENSE file for copyright and license details. */
#include <stdint.h>

extern char buf[1024];

#define LEN(x) (sizeof (x) / sizeof *(x))

#if defined(CLOCK_BOOTTIME)
  #define UPTIME_FLAG CLOCK_BOOTTIME
#elif defined(CLOCK_UPTIME)
  #define UPTIME_FLAG CLOCK_UPTIME
#else
  #define UPTIME_FLAG CLOCK_MONOTONIC
#endif

const char *bprintf(const char *fmt, ...);
const char *human_readable(uintmax_t num, const char *unit, const int base);
const char *pstring(const char *s);
int pscanf(const char *path, const char *fmt, ...);

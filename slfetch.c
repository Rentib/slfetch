/* See LICENSE file for copyright and license details. */
#define _POSIX_C_SOURCE 200809L
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include "slfetch.h"
#include "util.h"

struct arg {
  const char *(*func)();
  const char *fmt;
  const char *arg;
};

char buf[1024];

#include "config.h"

const char *
cpu_cores(void)
{
  FILE *fp;
  size_t k, cores;
  char *line;

  if (!(fp = fopen("/proc/cpuinfo", "r")))
    return NULL;
  line = NULL;
  cores = 0;
  while (getline(&line, &k, fp) != -1)
    cores += (sscanf(line, "processor %[^\n]", buf) > 0);
  return bprintf("%d", cores);
}

const char *
cpu_freq(const char *unit)
{
  uintmax_t freq;
  /* in kHz */
  if (pscanf("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq",
             "%ju", &freq) != 1)
    return NULL;
  return human_readable(freq * 1000, unit, 1000);
}

const char *
cpu_name(void)
{
  FILE *fp;
  size_t k;
  int i;
  char *line;

  if (!(fp = fopen("/proc/cpuinfo", "r")))
    return NULL;
  line = NULL;
  while (getline(&line, &k, fp) != -1)
    if (sscanf(line, "model name : %[^\n@]", buf) > 0)
      break;
  free(line);
  fclose(fp);
  
  /* remove whitespaces at the end of buf */
  for (i = 0; buf[i] != '\0'; ++i);
  while (i > 0 && buf[--i] == ' ')
    buf[i] = '\0';

  return buf;
}

/* stolen from slstatus */
const char *
cpu_perc(void)
{
		static long double a[7];
		long double b[7], sum;

		memcpy(b, a, sizeof(b));
		/* cpu user nice system idle iowait irq softirq */
		if (pscanf("/proc/stat", "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf",
		           &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6])
		    != 7) {
			return NULL;
		}
		if (b[0] == 0) {
			return NULL;
		}

		sum = (b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6]) -
		      (a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]);

		if (sum == 0) {
			return NULL;
		}

		return bprintf("%d", (int)(100 *
		               ((b[0] + b[1] + b[2] + b[5] + b[6]) -
		                (a[0] + a[1] + a[2] + a[5] + a[6])) / sum));
}

const char *
distro(void)
{
  FILE *fp;
  char *line = NULL;
  size_t len;

  if (!(fp = fopen("/etc/os-release", "r")))
    return NULL;

  while (getline(&line, &len, fp) != -1)
    if (sscanf(line, "NAME=" "\"%[^\n\"]s", buf) > 0)
      break;

  free(line);
  fclose(fp);

  return buf;
}


const char *
hostname(void)
{
  if (gethostname(buf, sizeof(buf)) < 0)
    return NULL;
  return buf;
}

const char *
kernel(void)
{
  struct utsname udata;
  if (uname(&udata) < 0)
    return NULL;
  return bprintf("%s", udata.release);
}

const char *
model_name(void)
{
  if (pscanf("/sys/devices/virtual/dmi/id/product_name", "%[^\n]", &buf) != 1)
    return NULL;
  return buf;
}

const char *
model_version(void)
{
  if (pscanf("/sys/devices/virtual/dmi/id/product_version", "%[^\n]", &buf) != 1)
    return NULL;
  return buf;
}

const char *
packages(void)
{
  /* this only works for pacman for now */
  /* to make it work for other package managers replace string in popen */
  FILE *fp;
  int num;

  if (!(fp = popen("pacman -Q | wc -l", "r")))
    return NULL;
  fscanf(fp, "%d", &num);
  return bprintf("%d", num);
}

const char *
ram_perc(void)
{
  uintmax_t total, available, used, perc;
  if (pscanf("/proc/meminfo",
             "MemTotal: %ju kB\n"
             "MemFree: %ju kB\n"
             "MemAvailable: %ju kB\n",
             &total, &available, &available) != 3)
    return NULL;
  
  used = total - available;
  perc = used * 1000 / total;
  if (perc % 10 > 4) /* round up */
    perc += 10;
  return bprintf("%d", perc / 10);
}

const char *
ram_total(const char *unit)
{
  uintmax_t total;
  if (pscanf("/proc/meminfo",
             "MemTotal: %ju kB\n",
             &total) != 1)
    return NULL;
  return human_readable(total << 10, unit, 1024);
  /* return human_readable(total * 1024, unit, 1024); */
}

const char *
ram_used(const char *unit)
{
  uintmax_t total, available;
  if (pscanf("/proc/meminfo",
             "MemTotal: %ju kB\n"
             "MemFree: %ju kB\n"
             "MemAvailable: %ju kB\n",
             &total, &available, &available) != 3)
    return NULL;
  return human_readable((total - available) << 10, unit, 1024);
  /* return human_readable((total - available) * 1024, unit); */
}

const char *
resolution(void) /* another tricky one */
{
  return NULL;
}

const char *
shell(void)
{
  /* returns default shell, maybe i will change it to running shell some day */
  char *name;
  int i, j, k;

  name = getenv("SHELL");
  i = strlen(name);
  j = i;
  k = 0;
  while (--i >= 0 && name[i] != '/');
  while (++i < j)
    buf[k++] = name[i];
  buf[k] = '\0';
  return buf;
}

const char *
terminal(void)
{
  return getenv("TERM");
}

const char *
uptime(void)
{
  uintmax_t h, m;
  struct timespec uptime;

  if (clock_gettime(UPTIME_FLAG, &uptime) < 0)
    return NULL;
  h = uptime.tv_sec / 3600;
  m = uptime.tv_sec % 3600 / 60;
  const char *mword = (m == 1 ? "min" : "mins");
  if (h == 0)
    return bprintf("%ju %s", m, mword);
  const char *hword = (h == 1 ? "hour" : "hours");
  return bprintf("%ju %s, %ju %s", h, hword, m, mword);
}

const char *
username(void)
{
  struct passwd *pw;

  if (!(pw = getpwuid(geteuid()))) 
    return NULL;
  return (pw->pw_name);
}

int
main()
{
  size_t i, j;
  const char *res;

  for (i = 0; i < LEN(args); ++i) {
    if (i < LEN(LOGO))
      printf("%s", LOGO[i]);

    for (j = 0; args[i][j].func != NULL; ++j) {
      if (!(res = args[i][j].func(args[i][j].arg)))
        res = unknown_str;
      printf(args[i][j].fmt, res);
    }
    printf("\n");
  }

  while (i < LEN(LOGO))
    printf("%s\n", LOGO[i++]);
}


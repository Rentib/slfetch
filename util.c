/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

const char *
bprintf(const char *fmt, ...)
{
  va_list ap;
  int ret;

  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  return (ret < 0 ? NULL : buf);
}

const char *
human_readable(uintmax_t num, const char *unit)
{
  /*         unit        0    1    2    3    4    5  */
  const char pref[] = { 'B', 'K', 'M', 'G', 'T', 'P' };
  size_t i, preflen, precision;
  double scaled = num;

  for (i = 0, preflen = LEN(pref); i < preflen && unit[0] != pref[i]; ++i)
    scaled /= 1024;
  if (i == preflen)
    return "Unsupported unit";
  precision = (i < 2 ? 0 : i - 2);

  return bprintf("%.*lf %s", precision, scaled, unit);
}

const char *
pstring(const char *s)
{
  return s;
}

int
pscanf(const char *path, const char *fmt, ...)
{
  FILE *fp;
  va_list ap;
  int n;

  if (!(fp = fopen(path, "r")))
    return -1;
  va_start(ap, fmt);
  n = vfscanf(fp, fmt, ap);
  va_end(ap);
  fclose(fp);

  return (n == EOF ? -1 : n);
}

void 
remove_newline(char *s)
{
  while (*s != '\0' && *s != '\n')
    s++;
  *s = '\0';
}

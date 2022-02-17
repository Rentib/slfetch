/* See LICENSE file for copyright and license details. */
#include "colors.h"
#include "logos/neofetch-arch.h"

#define MAX_ITEMS 7
static const char unknown_str[] = "n/a";

static const struct arg args[][MAX_ITEMS] = {
  /*  function, format,  argument */
  {
    { username, BCOL11 "%s" BCOL10 "@", NULL },
    { hostname, BCOL12 "%s", NULL },
  },
  {
    { pstring, RESET "%s", "-----------" },
  },
  {
    { distro, "OS: " RESET "%s", NULL },
  },
  {
    { model_name, "Host: " RESET "%s", NULL },
    { model_version, " %s", NULL },
  },
  {
    { kernel, "Kernel: " RESET "%s", NULL},
  },
  {
    { uptime, "Uptime: " RESET "%s", NULL },
  },
  {
    { packages, "Packages: " RESET "%s (pacman)", NULL },
  },
  {
    { shell, "Shell: " RESET "%s", NULL },
  },
  {
    { ram_used, "Memory: " RESET "%s", "MiB" },
    { ram_total, " / %s", "MiB" },
    { ram_perc, " (%s%%)", NULL },
  },
};


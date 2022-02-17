#include "colors.h"
#include "logos/neofetch-arch.h"

static const char unknown_str[] = "n/a";

static const struct arg *args[] = {
  /*  function, format,  argument */
  (struct arg[]){
    { username, BCOL11 "%s" BCOL10 "@", NULL },
    { hostname, BCOL12 "%s", NULL },
  },
  (struct arg[]){
    { pstring, RESET "%s", "-----------" },
  },
  (struct arg[]){
    { distro, "OS: " RESET "%s", NULL },
  },
  (struct arg[]){
    { model_name, "Host: " RESET "%s", NULL },
    { model_version, " %s", NULL },
  },
  (struct arg[]){
    { kernel, "Kernel: " RESET "%s", NULL},
  },
  (struct arg[]){
    { uptime, "Uptime: " RESET "%s", NULL },
  },
  (struct arg[]){
    { packages, "Packages: " RESET "%s (pacman)", NULL },
  },
  (struct arg[]){
    { shell, "Shell: " RESET "%s", NULL },
  },
  (struct arg[]){
    { ram_used, "Memory: " RESET "%s", "MiB" },
    { ram_total, " / %s", "MiB" },
    { ram_perc, " (%s%%)", NULL },
  },
};

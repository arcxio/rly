/*
 * rly.c - program for gaining privilege
 *
 * Copyright (C) 2024 arĉi <arcxi@dismail.de>
 * Copyright (C) 1992-3 Ian Jackson <ian@davenant.greenend.org.uk>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3,
 * or (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this file; if not, consult the Free Software
 * Foundation's website at www.fsf.org, or the GNU Project website at
 * www.gnu.org.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>

void usagemessage(void) {
  if (fputs("usage: rly <command> [<argument/option> ...]\n",
            stderr) == EOF) { perror("write usage"); exit(-1); }
}

#ifdef WHEEL_GROUP
static const char *wheelgroup= WHEEL_GROUP;
#else
static const char *wheelgroup= "wheel";
#endif

static int checkroot(void) {
  struct stat sb;
  if (stat("/proc/self/exe",&sb)<0) { perror("stat failed"); exit(-1); }
  if (sb.st_mode & S_IWGRP || sb.st_mode & S_IRWXO) { return -1; }
  struct group *grp= getgrgid(sb.st_gid);
  if (!grp) { perror("getgrgid failed"); exit(-1); }
  return strcmp(grp->gr_name,wheelgroup);
}

int main(int argc, const char *const *argv) {
  int r;
  r= getuid(); if (r<0) { perror("getuid failed"); exit(-1); }
  if (r && checkroot()) { fputs("sorry\n",stderr); exit(-1); }
  ++argv; if (!*argv) { usagemessage(); exit(0); }
  r= setuid(0); if (r) { perror("setuid failed"); exit(-1); }
  r= getuid(); if (r<0) { perror("getuid failed"); exit(-1); }
  if (r) { fputs("getuid mismatch",stderr); exit(-1); }
  r= geteuid(); if (r<0) { perror("geteuid failed"); exit(-1); }
  if (r) { fputs("geteuid mismatch",stderr); exit(-1); }
  execvp(argv[0],(char**)argv);
  perror("exec failed");
  exit(-1);
}

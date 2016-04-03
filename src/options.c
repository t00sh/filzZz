/************************************************************************/
/* FilzZz - A tool for fuzzing files          			        */
/* 								        */
/* Copyright (c) 2014, -TOSH-					        */
/* File coded by -TOSH-						        */
/* 								        */
/* This file is part of FilzZz.					        */
/* 								        */
/* FilzZz is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.				        */
/* 								        */
/* RopC is distributed in the hope that it will be useful,	        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.			        */
/* 								        */
/* You should have received a copy of the GNU General Public License    */
/* along with FilzZz.  If not, see <http://www.gnu.org/licenses/>       */
/************************************************************************/


#include "filzzz.h"

/* Options */
const char **options_filenames = NULL;
const char *options_filename = NULL;
char **options_argv = NULL;
long options_length = 0;
long options_offset = 0;
int options_bytes = 1;
int options_noerr = 0;

static const char** alloc_filenames(const char *filenames) {
  const char **alloc;
  int n;
  const char *p, *start;

  n = 0;
  p = filenames;

  while((p = strchr(p, ','))) {
    n++;
    p++;
  }

  alloc = xmalloc((n+2)*sizeof(char*));

  n = 0;
  start = filenames;
  while((p = strchr(start, ','))) {
    alloc[n++] = strndup(start, p-start);
    start = p+1;
  }

  alloc[n++] = strdup(start);
  alloc[n] = NULL;

  return alloc;
}

/* Display program version & quit */
static void version(void) {
  printf("%s version %s\n", PACKAGE, VERSION);
  printf("Compiled the %s at %s\n", __DATE__, __TIME__);
  exit(EXIT_SUCCESS);
}

/* Display program usage & quit */
static void usage(const char *progname) {
  printf("Usage : %s [OPTIONS] FILE COMMAND\n", progname);
  printf("Tool for fuzzing files\n");
  printf("\n");
  printf("  -n        Don't display the stderr output\n");
  printf("  -b        How many bytes to fuzz\n");
  printf("  -l        Fuzz length byte of file\n");
  printf("  -o        Fuzz the file starting at offset\n");
  printf("  -h        Print help\n");
  printf("  -v        Print version\n");
  exit(EXIT_SUCCESS);
}

void options_parse(int argc, char **argv) {
  int i;
  int opt;
  char *progname = argv[0];

  while((opt = getopt(argc, argv, "+nb:o:l:vh")) != -1) {
    switch(opt) {

    case 'n':
      options_noerr = 1;
      break;

    case 'b':
      options_bytes = strtol(optarg, NULL, 0);
      break;

    case 'o':
      options_offset = strtol(optarg, NULL, 0);
      break;

    case 'l':
      options_length = strtol(optarg, NULL, 0);
      break;

    case 'h':
      usage(progname);
      break;

    case 'v':
      version();
      break;

    default:
      usage(progname);
    }
  }

  if(options_offset < 0)
    FATAL_ERROR("Bad offset");
  if(options_length < 0)
    FATAL_ERROR("Bad length");
  if(options_bytes <= 0)
    FATAL_ERROR("Bad bytes");

  if(optind + 1 >= argc) {
    usage(progname);
  }

  options_filename = argv[optind++];
  options_filenames = alloc_filenames(options_filename);

  options_filename = options_filenames[0];

  options_argv = xcalloc(argc-optind+1, sizeof(*options_argv));

  for(i = optind; i < argc; i++) {
    if(!strcmp(argv[i], "%f"))
      options_argv[i-optind] = xstrdup(options_filename);
    else
      options_argv[i-optind] = xstrdup(argv[i]);
  }
  options_argv[i-optind] = NULL;
}

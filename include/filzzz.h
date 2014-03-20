#ifndef DEF_FILZZZ_H
#define DEF_FILZZZ_H

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#include "xfunc.h"

#define FATAL_ERROR(...) do {					\
    fprintf(stderr, "[-] ");					\
    fprintf(stderr, __VA_ARGS__);				\
    fprintf(stderr, "\n");					\
    exit(EXIT_FAILURE);						\
  }while(0)


#ifndef NDEBUG
#define DEBUG(...) do {				\
  fprintf(stderr, "[DEBUG] ");			\
  fprintf(stderr, __VA_ARGS__);			\
  fprintf(stderr, "\n");			\
  }while(0)
#else
#define DEBUG(...)
#endif


extern const char *options_filename;;
extern char **options_argv;
extern int options_argc;
extern int options_bytes;
extern long options_length;
extern long options_offset;
extern int options_noerr;


/* options */
void options_parse(int argc, char **argv);

/* safe_int */
int safe_add32(uint32_t *r, uint32_t a, uint32_t b);
int safe_sub32(uint32_t *r, uint32_t a, uint32_t b);
int safe_mul32(uint32_t *r, uint32_t a, uint32_t b);

#endif

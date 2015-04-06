#include "filzzz.h"

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

uint8_t *main_saved = NULL;
int files_tested = 0;

extern char **environ;


void restaure_file(const char *filename, uint8_t *save);

int _rand(uint32_t n) {
  return rand() % (n + 1);
}

void handle_sigint(int sig) {
  (void)sig;

  printf("Files tested = %d\n", files_tested);

  restaure_file(options_filename, main_saved);
  exit(EXIT_SUCCESS);
}

void set_signals(void) {
  struct sigaction sa;

  sa.sa_handler = handle_sigint;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  sigaction(SIGINT, &sa, NULL);
}


void fuzz_file(FILE *fd) {
  int n_byte;
  int off;
  uint8_t byte;


  n_byte = options_bytes;

  while(n_byte) {
    off = _rand(options_length) + options_offset;
    byte = _rand(0xff);

    xfseek(fd, off, SEEK_SET);

    fwrite(&byte, sizeof(uint8_t), 1, fd);
    n_byte--;
  }
}

void execute_cmd(void) {
  int null;
  pid_t pid;
  int status;

  null = xopen("/dev/null", O_WRONLY);
  pid = xfork();

  if(pid == 0) {
    dup2(null,1);
    if(options_noerr)
      dup2(null, 2);

    execve(options_argv[0], options_argv, environ);
    kill(getppid(), SIGINT);
    FATAL_ERROR("Failed to execve %s", options_argv[0]);
  } else {
    xwait(&status);
    if(WIFSIGNALED(status)) {
      if(WTERMSIG(status) == SIGABRT ||
	 WTERMSIG(status) == SIGILL ||
	 WTERMSIG(status) == SIGSEGV ||
	 WTERMSIG(status) == SIGSYS ||
	 WTERMSIG(status) == SIGFPE) {
	DEBUG("Fuzz is a success, aborting");
	DEBUG("%d files tested", files_tested);
	exit(EXIT_SUCCESS);
      }
    }
  }
  close(null);
}

FILE* load_file(const char *filename, uint8_t **save) {
  FILE *fd;

  fd = xfopen(filename, "r+");
  *save = xmalloc(options_length);

  xfseek(fd, options_offset, SEEK_SET);

  if(fread(*save, 1, options_length, fd) != (size_t)options_length)
    FATAL_ERROR("Failed to read file");

  return fd;
}

void restaure_file(const char *filename, uint8_t *save) {
  FILE *fd;

  fd = xfopen(filename, "r+");
  xfseek(fd, options_offset, SEEK_SET);
  fwrite(save, 1, options_length, fd);

  fclose(fd);

}

off_t get_file_size(const char *path) {
  struct stat st;

  if(stat(path, &st) < 0)
    return 0;

  return st.st_size;
}

int main(int argc, char **argv) {
  FILE *fd;

  srand(time(NULL));

  options_parse(argc, argv);

  set_signals();

  if(options_length == 0)
    options_length = get_file_size(options_filename);

  DEBUG("Fuzzing range %ld-%ld", options_offset, options_offset+options_length);
  DEBUG("Poisonning %d bytes\n", options_bytes);

  while(1) {
    files_tested++;

    if(files_tested % 100 == 0)
      DEBUG("Tested: %d", files_tested);

    fd = load_file(options_filename, &main_saved);
    fuzz_file(fd);
    fclose(fd);

    execute_cmd();
    restaure_file(options_filename, main_saved);
  }

  return EXIT_SUCCESS;
}

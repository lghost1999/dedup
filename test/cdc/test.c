#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fastcdc.h"

int print_chunk(void *arg, size_t offset, size_t len)
{
    static int chunk_id = 0;
    chunk_id++;
    printf("%08zu|%08zu  %d\n", offset, len, chunk_id);
    return chunk_id;
    return 0;
}

int main(int argc, char *argv[]) {
  FILE *s = fopen(argv[1], "r");
  if (!s)
    exit(-1);

  struct timespec tp1, tp2;

  clock_gettime(CLOCK_MONOTONIC, &tp1);
  printf("%8s|%8s\n", "offset", "length");
  size_t bytes = fastcdc_stream(s, 64 * 1024,  256 * 1024, 512 * 1024, print_chunk, NULL);
  clock_gettime(CLOCK_MONOTONIC, &tp2);
  double elapsed = (tp2.tv_sec - tp1.tv_sec)*1000 + (tp2.tv_nsec - tp1.tv_nsec)/1000000.0;
  printf("\n======\n%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0*bytes/(1024*1024), elapsed, 1000.0*bytes/(elapsed*1024*1024));
  fclose(s);
  return 0;
}

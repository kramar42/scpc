#include <stdio.h>

#include "util.h"
#include "main.h"

char* slurp_file(const char* filename, size_t* filesize)
{
  FILE* f;

#ifdef _WIN32
  if (fopen_s(&f, filename, "r") != 0)
#else
  if ((f = fopen(filename, "r")) == NULL)
#endif
  {
    printf("%s\n", filename);
    error("can't open file");
  }

  fseek(f, 0, SEEK_END);
  size_t m  = ftell(f);
  fseek(f, 0, SEEK_SET);

  char* buffer = malloc(m + 1);
  *filesize = fread(buffer, 1, m, f);
  if (*filesize != m)
  {
    printf("%s\n", filename);
    error("can't read whole file");
  }

  buffer[*filesize] = 0;
  fclose(f);
  return buffer;
}

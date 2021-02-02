#ifndef _plat_h_
#define _plat_h_

void  success   ();
void  error     (const char* message);
char* slurp_file(const char* filename, size_t* filesize);

#ifdef _plat_impl_

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
  #include <windows.h> // getcwd
  #include <conio.h>   // getch
#else
  #include <unistd.h>  // getcwd
#endif

void success()
{
  exit(EXIT_SUCCESS);
}

void error(const char* message)
{
  printf("error: %s\n", message);
  char cwd[256];
#ifdef _WIN32
  GetCurrentDirectory(sizeof(cwd), cwd);
#else
  getcwd(cwd, sizeof(cwd));
#endif
#ifdef _WIN32
  printf("cwd: %s\nPress any key...", cwd);
  _getch();
#endif
  exit(EXIT_FAILURE);
}

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

#endif // _plat_impl_
#endif // _plat_h_
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
  #include <windows.h> // for getcwd
  #include <conio.h>   // for getch
#else
  #include <unistd.h>  // for getcwd
#endif

#include <GLFW/glfw3.h>

#include "util.h"

void success()
{
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void error(const char* message)
{
  printf("error: %s\n", message);
  glfwTerminate();
  char cwd[256];
#ifdef _WIN32
  GetCurrentDirectory(sizeof(cwd), cwd);
#else
  getcwd(cwd, sizeof(cwd));
#endif
  printf("cwd: %s\nPress any key...", cwd);
#ifdef _WIN32
  _getch();
#else
  getchar();
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

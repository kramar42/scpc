#ifndef _plat_h_
#define _plat_h_

#include <stdlib.h>

void  success   ();
void  error     (const char* message);
char* slurp_file(const char* filename, size_t* filesize);

#endif // _plat_h_
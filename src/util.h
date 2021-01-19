#ifndef _util_h_
#define _util_h_

void  success   ();
void  error     (const char* message);
char* slurp_file(const char* filename, size_t* filesize);

#endif // _util_h_
#ifndef MTT_FILE_H
#define MTT_FILE_H

#include <Windows.h>

char *mtt_load_file_into_new_buf(const char *name, size_t off, size_t c, size_t *size, int str);

#endif
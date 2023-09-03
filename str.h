#ifndef MTT_STR_H
#define MTT_STR_H

#include <stddef.h>

#define FMT_FS_FILL_MODE_LEFT 0x00
#define FMT_FS_FILL_MODE_INTERNAL 0x01
#define FMT_FS_FILL_MODE_RIGHT 0x02

#define FMT_FS_LTR_CASE_MIXED 0x00
#define FMT_FS_LTR_CASE_UPPER 0x04
#define FMT_FS_LTR_CASE_LOWER 0x0C

#define FMT_FS_NO_NULL_TERM 0x10

struct mtt_fmt_t
{
	char plus;
	char minus;
	char fill;
	char base;
	char fs;
	size_t width;
};

void *mtt_mem_rev(void *mem, size_t n);

size_t mtt_fstr_to_ival(const char *fstr, const char **end, struct mtt_fmt_t fmt);

size_t mtt_ival_to_fstr(char *fstr, size_t ival, struct mtt_fmt_t fmt);

#endif
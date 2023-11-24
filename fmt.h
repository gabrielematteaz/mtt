#ifndef MTT_FMT_H
#define MTT_FMT_H

#include <stddef.h>

#define ATOI_FILLING_MASK 0x3
#define ATOI_FILLING_LEFT 0x0
#define ATOI_FILLING_INTERNAL 0x1
#define ATOI_FILLING_RIGTH 0x3

#define ATOI_LTR_CASE_MASK 0xC
#define ATOI_LTR_CASE_UNK 0x0
#define ATOI_LTR_CASE_UPPER 0x4
#define ATOI_LTR_CASE_LOWER 0xC

#define ITOA_LTR_CASE_LOWER 0x1

#define ITOA_FILLING_MASK 0x6
#define ITOA_FILLING_LEFT 0x0
#define ITOA_FILLING_INTERNAL 0x2
#define ITOA_FILLING_RIGHT 0x6

#define ITOA_NULL_TERM_NONE 0x8

struct mtt_atoi_fmt_t
{
	char plus;
	char minus;
	char fill;
	char base;
	char flags;
};

struct mtt_itoa_fmt_t
{
	char plus;
	char minus;
	char fill;
	char base;
	char flags;
	size_t width;
};

unsigned long mtt_atoi(const char *fstr, char **end, struct mtt_atoi_fmt_t fmt);

size_t mtt_itoa(char *fstr, unsigned long ival, struct mtt_itoa_fmt_t fmt);

#endif
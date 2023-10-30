#ifndef MTT_CMDLINE_H
#define MTT_CMDLINE_H

#include <stddef.h>

#define OPT_ARG_NONE 0x0
#define OPT_ARG_OPTIONAL 0x1
#define OPT_ARG_REQUIRED 0x3
#define OPT_ARG_MASK 0x3

struct mtt_opt_t
{
	char alias;
	char fs;
	const char *arg;
};

int mtt_extr_optv(int argc, char *argv[], size_t optc, struct mtt_opt_t *optv);

#endif
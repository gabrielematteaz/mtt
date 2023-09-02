#ifndef MTT_CMD_LINE_H
#define MTT_CMD_LINE_H

#include <stddef.h>

#define OPT_NO_ARG 0x00
#define OPT_OPTIONAL_ARG 0x01
#define OPT_REQUIRED_ARG 0x02

struct mtt_opt_t
{
	int alias;
	int fs;
	const char *arg;
};

int mtt_extr_optv(int argc, char *argv[], int optc, struct mtt_opt_t *optv);

#endif
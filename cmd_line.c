#include "cmd_line.h"

#include <stdio.h>

int mtt_extr_optv(int argc, char *argv[], int optc, struct mtt_opt_t *optv)
{
	if (argv == NULL || optv == NULL) return 0;

	char **av = argv + 1, **avc = argv + argc;
	struct mtt_opt_t *ovc = optv + optc;

	while (av < avc)
	{
		char *arg = *av;

		if (*arg == '-')
		{
			char *a = arg + 1, ac = *a;

			if (ac)
			{
				if (ac == '-') av++;
				else
				{
					do
					{
						struct mtt_opt_t *ov = optv;

						a++;

						while (ov < ovc)
						{
							if (ac == ov->alias)
							{
								int fs = ov->fs;

								if (fs & OPT_OPTIONAL_ARG) ov->arg = *a ? a : 0;
								else if (fs & OPT_REQUIRED_ARG)
								{
									if (*a) ov->arg = a;
									else
									{
										av++;

										if (*av == NULL) goto error;

										ov->arg = *av;
									}
								}
								else
								{
									ov->arg = arg;

									break;
								}

								goto next_arg;
							}

							ov++;
						}

						ac = *a;
					} while (ac);

					goto next_arg;
				}
			}

			break;
		}
		else break;

	next_arg:
		av++;
	}

error:
	return av - argv;
}
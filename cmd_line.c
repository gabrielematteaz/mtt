#include "cmd_line.h"

int mtt_extr_optv(int argc, char *argv[], int optc, struct mtt_opt_t *optv)
{
	char **av, **argvc, *arg, *a, ac, argtype;
	struct mtt_opt_t *ov, *optvc;

	if (argv == NULL || optv == NULL) return 0;

	av = argv + 1;
	argvc = argv + argc;
	optvc = optv + optc;

next_arg:
	arg = *av;

	if (arg)
	{
		if (*arg == '-')
		{
			a = arg + 1;
			ac = *a;

			if (ac)
			{
				av++;

				if (ac != '-')
				{
					do
					{
						a++;
						ov = optv;

						while (ov < optvc)
						{
							if (ac == ov->alias)
							{
								argtype = ov->fs & OPT_ARG_TYPE_MASK;

								if (argtype == OPT_ARG_TYPE_NONE)
								{
									ov->arg = arg;

									break;
								}

								if (*a) ov->arg = a;
								else
								{
									if (argtype == OPT_ARG_TYPE_REQUIRED)
									{
										ov->arg = *av;

										if (av == argvc) goto exit;

										av++;
									}
									else ov->arg = NULL;

									goto next_arg;
								}
							}

							ov++;
						}

						ac = *a;
					} while (ac);

					goto next_arg;
				}
			}
		}
	}

exit:
	return av - argv;
}
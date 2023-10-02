#include "cmdline.h"

int mtt_extr_optv(int argc, char *argv[], int optc, struct mtt_opt_t *optv)
{
	char **av, **argvc, *arg, *a, ac, argtype;
	struct mtt_opt_t *optvc, *ov;

	if (argv == NULL || optv == NULL) return 0;

	av = argv + 1;
	argvc = argv + argc;
	optvc = optv + optc;

next_arg:
	if (av < argvc)
	{
		arg = *av;

		if (arg && *arg == '-')
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

								if (argtype)
								{
									if (*a) ov->arg = a;
									else
									{
										if (argtype == OPT_ARG_TYPE_REQUIRED)
										{
											arg = *av;

											if (av == argvc && arg) goto error;

											ov->arg = arg;
											av++;
										}
										else ov->arg = NULL;

										goto next_arg;
									}
								}

								ov->arg = arg;

								break;
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

error:
	return av - argv;
}
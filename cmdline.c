#include "cmdline.h"

int mtt_extr_optv(int argc, char *argv[], size_t optc, struct mtt_opt_t *optv)
{
	int i;

	if (argv == NULL || optv == NULL)
	{
		char inv = 0;

		i = 0;

		while (inv == 0 && i < argc)
		{
			if (argv[i] == NULL)
			{
				inv = 1;
			}
			else
			{
				size_t j = 0;

				if (argv[i][j] == '-')
				{
					j++;

					if (argv[i][j] == '\0')
					{
						inv = 1;
					}
					else if (argv[i][j] == '-')
					{
						i++;
						inv = 1;
					}
					else
					{
						char skip = 0;

						do
						{
							char found = 0;
							size_t k = 0;

							while (found == 0 && k < optc)
							{
								if (argv[i][j] == optv[k].alias)
								{
									char arg = optv[k].fs & OPT_ARG_MASK;

									if (arg == OPT_ARG_NONE)
									{
										optv[k].arg = argv[i];
									}
									else
									{
										j++;

										if (argv[i][j] == '\0')
										{
											if (arg == OPT_ARG_OPTIONAL)
											{
												optv[k].arg = NULL;
											}
											else
											{
												i++;

												if (i == argc)
												{
													inv = 1;
												}
												else
												{
													optv[k].arg = argv[i];
												}
											}
										}
										else
										{
											optv[k].arg = argv[i] + j;
										}

										skip = 1;
									}

									found = 1;
								}

								k++;
							}

							j++;
						} while (skip == 0 && argv[i][j]);

						if (inv == 0)
						{
							i++;
						}
					}
				}
				else
				{
					inv = 1;
				}
			}
		}
	}
	else
	{
		i = 1;
	}

	return i;
}
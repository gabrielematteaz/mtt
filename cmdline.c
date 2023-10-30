#include "cmdline.h"

int mtt_extr_optv(int argc, char *argv[], size_t optc, struct mtt_opt_t *optv)
{
	int i;

	if (argv == NULL || optv == NULL)
	{
		i = 0;
	}
	else
	{
		char invarg = 0;

		i = 1;

		while (invarg == 0 && i < argc)
		{
			if (argv[i] == NULL)
			{
				invarg = 1;
			}
			else
			{
				size_t j = 0;

				if (argv[i][j] == '-')
				{
					j++;

					if (argv[i][j] == '\0')
					{
						invarg = 1;
					}
					else if (argv[i][j] == '-')
					{
						i++;
						invarg = 1;
					}
					else
					{
						char newarg = 0;

						do
						{
							char optfound = 0;
							size_t k = 0;

							while (optfound == 0 && k < optc)
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
											if (arg == OPT_ARG_REQUIRED)
											{
												i++;

												if (i == argc)
												{
													invarg = 1;
												}
												else
												{
													optv[k].arg = argv[i];
												}
											}
											else
											{
												optv[k].arg = NULL;
											}
										}
										else
										{
											optv[k].arg = argv[i] + j;
										}

										newarg = 1;
									}

									optfound = 1;
								}

								k++;
							}

							j++;
						} while (newarg == 0 && argv[i][j]);

						if (invarg == 0)
						{
							i++;
						}
					}
				}
				else
				{
					invarg = 1;
				}
			}
		}
	}

	return i;
}
#include "fmt.h"

#ifndef MTT_STR_H

static void *mtt_mem_rev(void *mem, size_t n)
{
	if (mem)
	{
		size_t i = 0;
		char *m = mem;

		while (i < n)
		{
			char mc = m[i];

			n--;
			m[i] = m[n];
			i++;
			m[n] = mc;
		}
	}

	return mem;
}

#endif

unsigned long mtt_atoi(const char *fstr, char **end, struct mtt_atoi_fmt_t fmt)
{
	unsigned long ival = 0;
	char sign;

	if (fstr == NULL || fmt.base < 2 || fmt.base > 36)
	{
		fstr = NULL;
		sign = 0;
	}
	else
	{
		size_t i = 0;

		if (fmt.fill == '\0')
		{
			if (fmt.minus && fstr[i] == fmt.minus)
			{
				sign = -1;
				i++;
			}
			else
			{
				sign = 1;

				if (fmt.plus && fstr[i] == fmt.plus)
				{
					i++;
				}
			}
		}
		else
		{
			char filling = fmt.flags & ATOI_FILLING_MASK;

			if (filling == ATOI_FILLING_INTERNAL)
			{
				if (fmt.minus && fstr[i] == fmt.minus)
				{
					sign = -1;
					i++;
				}
				else
				{
					sign = 1;

					if (fmt.plus && fstr[i] == fmt.plus)
					{
						i++;
					}
				}

				while (fstr[i] == fmt.fill)
				{
					i++;
				}
			}
			else
			{
				if (filling == ATOI_FILLING_LEFT)
				{
					while (fstr[i] == fmt.fill)
					{
						i++;
					}
				}

				if (fmt.minus && fstr[i] == fmt.minus)
				{
					sign = -1;
					i++;
				}
				else
				{
					sign = 1;

					if (fmt.plus && fstr[i] == fmt.plus)
					{
						i++;
					}
				}
			}
		}

		if (fmt.base > 10)
		{
			char ltrcase = fmt.flags & ATOI_LTR_CASE_MASK, inv = 0;

			if (ltrcase == ATOI_LTR_CASE_UNK)
			{
				char umax = 55 + fmt.base, lmax = umax + 32;

				while (inv == 0)
				{
					if ('0' <= fstr[i] && fstr[i] <= '9')
					{
						ival = ival * fmt.base + fstr[i] - '0';
						i++;
					}
					else if ('A' <= fstr[i] && fstr[i] < umax)
					{
						ival = ival * fmt.base + fstr[i] - 55;
						i++;
					}
					else if ('a' <= fstr[i] && fstr[i] < lmax)
					{
						ival = ival * fmt.base + fstr[i] - 87;
						i++;
					}
					else
					{
						inv = 1;
					}
				}
			}
			else
			{
				char min = ltrcase == ATOI_LTR_CASE_UPPER ? 'A' : 'a', m = min - 10, max = m + fmt.base;

				while (inv == 0)
				{
					if ('0' <= fstr[i] && fstr[i] <= '9')
					{
						ival = ival * fmt.base + fstr[i] - '0';
						i++;
					}
					else if (min <= fstr[i] && fstr[i] < max)
					{
						ival = ival * fmt.base + fstr[i] - m;
						i++;
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
			char max = '0' + fmt.base;

			while ('0' <= fstr[i] && fstr[i] < max)
			{
				ival = ival * fmt.base + fstr[i] - '0';
				i++;
			}
		}

		fstr = fstr + i;
	}

	if (end)
	{
		*end = (char *)fstr;
	}

	return ival * sign;
}

size_t mtt_itoa(char *fstr, unsigned long ival, struct mtt_itoa_fmt_t fmt)
{
	size_t len;

	if (fmt.base < 2 || fmt.base > 36)
	{
		len = 0;
	}
	else
	{
		if (fstr == NULL)
		{
			if (fmt.minus && (long)ival < 0)
			{
				ival = -ival;
				len = 1;
			}
			else
			{
				len = fmt.plus == '\0' ? 0 : 1;
			}

			do
			{
				ival /= fmt.base;
				len++;
			} while (ival);

			if (len < fmt.width)
			{
				len = fmt.width;
			}
		}
		else
		{
			char neg, filling = fmt.flags & ITOA_FILLING_MASK;

			if (fmt.minus && (long)ival < 0)
			{
				ival = -ival;
				neg = 1;
			}
			else
			{
				neg = 0;
			}

			len = 0;

			if (fmt.base > 10)
			{
				char a = fmt.flags & ITOA_LTR_CASE_LOWER ? 87 : 55;

				do
				{
					char rem = ival % fmt.base;

					fstr[len] = (rem < 10 ? '0' : a) + rem;
					ival /= fmt.base;
					len++;
				} while (ival);
			}
			else
			{
				do
				{
					fstr[len] = '0' + ival % fmt.base;
					ival /= fmt.base;
					len++;
				} while (ival);
			}

			if (fmt.fill == '\0')
			{
				fmt.width = 1;
			}

			if (filling == ITOA_FILLING_LEFT)
			{
				if (neg)
				{
					fstr[len] = fmt.minus;
					len++;
				}
				else if (fmt.plus)
				{
					fstr[len] = fmt.plus;
					len++;
				}

				while (len < fmt.width)
				{
					fstr[len] = fmt.fill;
					len++;
				}

				mtt_mem_rev(fstr, len);
			}
			else if (filling == ITOA_FILLING_INTERNAL)
			{
				char *fstrlen = fstr + len, *fstrwidth = fstr + fmt.width - 1;

				while (fstrlen < fstrwidth)
				{
					*fstrlen = fmt.fill;
					fstrlen++;
				}

				if (neg)
				{
					*fstrlen = fmt.minus;
					fstrlen++;
				}
				else if (fmt.plus)
				{
					*fstrlen = fmt.plus;
					fstrlen++;
				}
				if (fstrlen == fstrwidth)
				{
					*fstrlen = fmt.fill;
					fstrlen++;
				}

				len = fstrlen - fstr;
				mtt_mem_rev(fstr, len);
			}
			else
			{
				if (neg)
				{
					fstr[len] = fmt.minus;
					len++;
				}
				else if (fmt.plus)
				{
					fstr[len] = fmt.plus;
					len++;
				}

				mtt_mem_rev(fstr, len);

				while (len < fmt.width)
				{
					fstr[len] = fmt.fill;
					len++;
				}
			}

			if ((fmt.flags & ITOA_NULL_TERM_NONE) == 0)
			{
				fstr[len] = '\0';
			}
		}
	}

	return len;
}
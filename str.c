#include "str.h"

void *mtt_mem_rev(void *mem, size_t n)
{
	if (mem)
	{
		char *m = mem, *mn = m + n;

		while (m < mn)
		{
			mn--;

			char mc = *m;

			*m = *mn;
			m++;
			*mn = mc;
		}
	}

	return mem;
}

size_t mtt_fstr_to_ival(const char *fstr, const char **end, struct mtt_fstr_to_ival_fmt_t fmt)
{
	if (fstr == NULL || fmt.base < 2 || fmt.base > 36)
	{
		if (end) *end = NULL;

		return 0;
	}

	char fc = *fstr;
	ptrdiff_t sign;

	if (fmt.fill)
	{
		if (fmt.fs & FMT_FILL_MODE_INTERNAL)
		{
			if (fmt.minus && fc == fmt.minus)
			{
				fstr++;
				fc = *fstr;
				sign = -1;
			}
			else
			{
				if (fmt.plus && fc == fmt.plus)
				{
					fstr++;
					fc = *fstr;
				}

				sign = 1;
			}
		}
		else
		{
			if ((fmt.fs & FMT_FILL_MODE_RIGHT) == 0)
			{
				while (fc == fmt.fill)
				{
					fstr++;
					fc = *fstr;
				}
			}

			if (fmt.minus && fc == fmt.minus)
			{
				fstr++;
				fc = *fstr;
				sign = -1;
			}
			else
			{
				if (fmt.plus && fc == fmt.plus)
				{
					fstr++;
					fc = *fstr;
				}

				sign = 1;
			}
		}
	}
	else
	{
		if (fmt.minus && fc == fmt.minus)
		{
			fstr++;
			fc = *fstr;
			sign = -1;
		}
		else
		{
			if (fmt.plus && fc == fmt.plus)
			{
				fstr++;
				fc = *fstr;
			}

			sign = 1;
		}
	}

	size_t ival = 0;

	if (fmt.base > 10)
	{
		char diff;

		if (fmt.fs & FMT_LTR_CASE_UPPER)
		{
			char min = (fmt.fs & FMT_LTR_CASE_LOWER) == FMT_LTR_CASE_LOWER ? 'a' : 'A', diffmin = min - 10, max = diffmin + fmt.base;

			while (1)
			{
				if (fc >= '0' && fc <= '9') diff = '0';
				else if (fc >= min && fc < max) diff = diffmin;
				else break;

				fstr++;
				ival = ival * fmt.base + fc - diff;
				fc = *fstr;
			}
		}
		else
		{
			char umax = 55 + fmt.base, lmax = umax + 32;

			while (1)
			{
				if (fc >= '0' && fc <= '9') diff = '0';
				else if (fc >= 'A' && fc < umax) diff = 55;
				else if (fc >= 'a' && fc < lmax) diff = 87;
				else break;

				fstr++;
				ival = ival * fmt.base + fc - diff;
				fc = *fstr;
			}
		}
	}
	else
	{
		char max = '0' + fmt.base;

		while (fc >= '0' && fc < max)
		{
			fstr++;
			ival = ival * fmt.base + fc - '0';
			fc = *fstr;
		}
	}

	if (end) *end = fstr;

	return sign * ival;
}

size_t mtt_ival_to_fstr(char *fstr, size_t ival, struct mtt_ival_to_fstr_fmt_t fmt)
{
	if (fmt.from.base < 2 || fmt.from.base > 36) return 0;

	size_t len;

	if (fstr)
	{
		char neg;

		if (fmt.from.minus && (ptrdiff_t)ival < 0)
		{
			ival = -ival;
			neg = 1;
		}
		else neg = 0;

		char *f = fstr;

		if (fmt.from.base > 10)
		{
			char a = (fmt.from.fs & FMT_LTR_CASE_LOWER) == FMT_LTR_CASE_LOWER ? 87 : 55;

			do
			{
				char rem = ival % fmt.from.base;

				ival /= fmt.from.base;
				*f = (rem < 10 ? '0' : a) + rem;
				f++;
			} while (ival);
		}
		else
		{
			do
			{
				*f = '0' + ival % fmt.from.base;
				ival /= fmt.from.base;
				f++;
			} while (ival);
		}

		if (fmt.from.fill)
		{
			char *fw;

			if (fmt.from.fs & FMT_FILL_MODE_INTERNAL)
			{
				fw = fstr + fmt.width - 1;

				while (f < fw)
				{
					*f = fmt.from.fill;
					f++;
				}

				if (neg)
				{
					*f = fmt.from.minus;
					f++;	
				}
				else if (fmt.from.plus)
				{
					*f = fmt.from.plus;
					f++;
				}
				else if (f == fw)
				{
					*f = fmt.from.fill;
					f++;
				}

				len = f - fstr;
				mtt_mem_rev(fstr, len);
			}
			else if (fmt.from.fs & FMT_FILL_MODE_RIGHT)
			{
				if (neg)
				{
					*f = fmt.from.minus;
					f++;	
				}
				else if (fmt.from.plus)
				{
					*f = fmt.from.plus;
					f++;
				}

				mtt_mem_rev(fstr, f - fstr);
				fw = fstr + fmt.width;

				while (f < fw)
				{
					*f = fmt.from.fill;
					f++;
				}

				len = f - fstr;
			}
			else
			{
				if (neg)
				{
					*f = fmt.from.minus;
					f++;	
				}
				else if (fmt.from.plus)
				{
					*f = fmt.from.plus;
					f++;
				}

				fw = fstr + fmt.width;

				while (f < fw)
				{
					*f = fmt.from.fill;
					f++;
				}

				len = f - fstr;
				mtt_mem_rev(fstr, len);
			}
		}
		else
		{
			if (neg)
			{
				*f = fmt.from.minus;
				f++;
			}
			else if (fmt.from.plus)
			{
				*f = fmt.from.plus;
				f++;
			}

			len = f - fstr;
			mtt_mem_rev(fstr, len);
		}

		if ((fmt.from.fs & IVAL_TO_FSTR_FMT_NO_NULL_TERM) == 0) *f = 0;
	}
	else
	{
		if (fmt.from.minus && (ptrdiff_t)ival < 0)
		{
			ival = -ival;
			len = 1;
		}
		else len = fmt.from.plus ? 1 : 0;

		do
		{
			ival /= fmt.from.base;
			len++;
		} while (ival);

		if (len < fmt.width) len = fmt.width;
	}

	return len;
}
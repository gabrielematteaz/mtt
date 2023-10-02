#include "fmt.h"

#ifndef MTT_MISC_H

static void *mtt_mem_rev(void *mem, size_t n)
{
	if (mem)
	{
		char *m = mem, *mn = m + n, mc;

		while (m < mn)
		{
			mn--;
			mc = *m;
			*m = *mn;
			m++;
			*mn = mc;
		}
	}

	return mem;
}

#endif

size_t mtt_fstr_to_ival(const char *fstr, const char **end, struct mtt_ivalfmt_t fmt)
{
	char fstrc, filling, sign;
	size_t ival;

	if (fstr == NULL || fmt.base < 2 || fmt.base > 36)
	{
		if (end) *end = NULL;

		return 0;
	}

	fstrc = *fstr;

	if (fmt.fill == '\0') goto skip_filling;

	filling = fmt.fs & FMT_FILLING_MASK;

	if (filling == FMT_FILLING_INTERNAL)
	{
		if (fmt.minus && fstrc == fmt.minus)
		{
			fstr++;
			fstrc = *fstr;
			sign = -1;
		}
		else
		{
			if (fmt.plus && fstrc == fmt.plus)
			{
				fstr++;
				fstrc = *fstr;
			}

			sign = 1;
		}

		while (fstrc == fmt.fill)
		{
			fstr++;
			fstrc = *fstr;
		}
	}
	else
	{
		if (filling == FMT_FILLING_LEFT)
		{
			while (fstrc == fmt.fill)
			{
				fstr++;
				fstrc = *fstr;
			}
		}

	skip_filling:
		if (fmt.minus && fstrc == fmt.minus)
		{
			fstr++;
			fstrc = *fstr;
			sign = -1;
		}
		else
		{
			if (fmt.plus && fstrc == fmt.plus)
			{
				fstr++;
				fstrc = *fstr;
			}

			sign = 1;
		}
	}

	ival = 0;

	if (fmt.base > 10)
	{
		char ltrcase = fmt.fs & VALFMT_LTR_CASE_MASK, diff;

		if (ltrcase == VALFMT_LTR_CASE_UNK)
		{
			char umax = 55 + fmt.base, lmax = umax + 32;

			while (1)
			{
				if ('0' <= fstrc && fstrc <= '9') diff = '0';
				else if ('A' <= fstrc && fstrc < umax) diff = 55;
				else if ('a' <= fstrc && fstrc < lmax) diff = 87;
				else break;

				fstr++;
				ival = ival * fmt.base + fstrc - diff;
				fstrc = *fstr;
			}
		}
		else
		{
			char min = ltrcase == VALFMT_LTR_CASE_LOWER ? 'a' : 'A', d = min - 10, max = d + fmt.base;

			while (1)
			{
				if ('0' <= fstrc && fstrc <= '9') diff = '0';
				else if (min <= fstrc && fstrc < max) diff = d;
				else break;

				fstr++;
				ival = ival * fmt.base + fstrc - diff;
				fstrc = *fstr;
			}
		}
	}
	else
	{
		char max = '0' + fmt.base;

		while ('0' <= fstrc && fstrc < max)
		{
			fstr++;
			ival = ival * fmt.base + fstrc - '0';
			fstrc = *fstr;
		}
	}

	if (end) *end = fstr;

	return ival * sign;
}

size_t mtt_ival_to_fstr(char *fstr, size_t ival, struct mtt_ivalfmt_t fmt)
{
	size_t len;

	if (fmt.base < 2 || fmt.base > 36) return 0;

	if (fstr)
	{
		char neg, *f = fstr, filling, *fstrwidth;

		if (fmt.minus && (ptrdiff_t)ival < 0)
		{
			ival = -ival;
			neg = 1;
		}
		else neg = 0;

		if (fmt.base > 10)
		{
			char a = (fmt.fs & VALFMT_LTR_CASE_MASK) == VALFMT_LTR_CASE_LOWER ? 87 :  55, rem;

			do
			{
				rem = ival % fmt.base;
				ival /= fmt.base;
				*f = (rem < 10 ? '0' : a) + rem;
				f++;
			} while (ival);
		}
		else
		{
			do
			{
				*f = '0' + ival % fmt.base;
				ival /= fmt.base;
				f++;
			} while (ival);
		}

		if (fmt.fill == '\0') fmt.width = 0;

		filling = fmt.fs & FMT_FILLING_MASK;

		if (filling == FMT_FILLING_INTERNAL)
		{
			fstrwidth = fstr + fmt.width - 1;

			while (f < fstrwidth)
			{
				*f = fmt.fill;
				f++;
			}

			if (neg)
			{
				*f = fmt.minus;
				f++;
			}
			else if (fmt.plus)
			{
				*f = fmt.plus;
				f++;
			}
			else if (f == fstrwidth)
			{
				*f = fmt.fill;
				f++;
			}

			len = f - fstr;
			mtt_mem_rev(fstr, len);
		}
		else if (filling == FMT_FILLING_RIGHT)
		{
			if (neg)
			{
				*f = fmt.minus;
				f++;
			}
			else if (fmt.plus)
			{
				*f = fmt.plus;
				f++;
			}

			mtt_mem_rev(fstr, f - fstr);
			fstrwidth = fstr + fmt.width;

			while (f < fstrwidth)
			{
				*f = fmt.fill;
				f++;
			}

			len = f - fstr;
		}
		else
		{
			if (neg)
			{
				*f = fmt.minus;
				f++;
			}
			else if (fmt.plus)
			{
				*f = fmt.plus;
				f++;
			}

			fstrwidth = fstr + fmt.width;

			while (f < fstrwidth)
			{
				*f = fmt.fill;
				f++;
			}

			len = f - fstr;
			mtt_mem_rev(fstr, len);
		}

		if ((fmt.fs & FMT_NULL_TERM_NONE) == 0) *f = 0;
	}
	else
	{
		if (fmt.minus && (ptrdiff_t)ival < 0)
		{
			ival = -ival;
			len = 1;
		}
		else len = fmt.plus ? 1 : 0;

		do
		{
			ival /= fmt.base;
			len++;
		} while (ival);

		if (len < fmt.width) len = fmt.width;
	}

	return len;
}
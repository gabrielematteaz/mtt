#include "str.h"

void *mtt_mem_rev(void *mem, size_t n)
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
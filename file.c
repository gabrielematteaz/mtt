#include "file.h"

char *mtt_load_file_into_new_buf(const char *name, size_t off, size_t c, size_t *size, int str)
{
	HANDLE file = CreateFileA(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (file == INVALID_HANDLE_VALUE) goto error;

	if (c == 0) GetFileSizeEx(file, (PLARGE_INTEGER)&c);

	size_t s = c;
	char *cont;

	if (str)
	{
		s++;
		cont = malloc(s);

		if (cont)
		{
			OVERLAPPED overl;

			ZeroMemory(&overl, sizeof(overl));
			overl.Offset = off;
			overl.OffsetHigh = off >> 32;

			if (ReadFileEx(file, cont, c, &overl, NULL) == TRUE)
			{
				if (size) *size = overl.OffsetHigh;

				CloseHandle(file);
				cont[overl.OffsetHigh] = 0;

				return cont;
			}
		}
	}
	else
	{
		cont = malloc(s);

		if (cont)
		{
			OVERLAPPED overl;

			ZeroMemory(&overl, sizeof(overl));
			overl.Offset = off;
			overl.OffsetHigh = off >> 32;

			if (ReadFileEx(file, cont, c, &overl, NULL) == TRUE)
			{
				if (size) *size = overl.OffsetHigh;

				CloseHandle(file);

				return cont;
			}			
		}
	}

	CloseHandle(file);
	free(cont);

error:
	if (size) *size = 0;

	return NULL;
}
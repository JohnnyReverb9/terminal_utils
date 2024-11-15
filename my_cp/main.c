#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cp(FILE *src, FILE *dest);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Valid usage: %s <file_from> <file_to>\n", argv[0]);
		return 1;
	}

	const char *src_file = argv[1];
	const char *dest_file = argv[2];

	FILE *src = fopen(src_file, "rb");

	if (!src)
	{
		perror("error opening source file.");
		return 1;
	}

	FILE *dest = fopen(dest_file, "wb");

	if (!dest)
	{
		perror("error opening destination file.");
		fclose(src);
		return 1;
	}

	cp(src, dest);

	fclose(src);
	fclose(dest);

	return 0;
}

void cp(FILE *src, FILE *dest)
{
	char buffer[1024];
	size_t bytes;

	while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
	{
		if (fwrite(buffer, 1, bytes, dest) != bytes)
		{
			perror("error writing file.\n");
			exit(1);
		}
	}
}
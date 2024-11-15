#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void grep(FILE *file, const char *pattern);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Valid usage: %s <regexp> <file>\n", argv[0]);
		return 1;
	}

	const char *pattern = argv[1];
	const char *filename = argv[2];
	FILE *file = fopen(filename, "r");

	if (!file)
	{
		perror("error opening file.");
		return 1;
	}

	grep(file, pattern);
	fclose(file);

	return 0;
}

void grep(FILE *file, const char *pattern)
{
	char line[1024];
	regex_t regex;

	int ret = regcomp(&regex, pattern, REG_EXTENDED);

	if (ret)
	{
		fprintf(stderr, "%s\n", "regex compile error.");
		exit(1);
	}

	unsigned i = 0;

	while (fgets(line, sizeof(line), file))
	{
		i++;
		ret = regexec(&regex, line, 0, NULL, 0);

		if (!ret)
		{
			printf("line %d: %s\n\n", i, line);
		}
	}

	regfree(&regex);
}
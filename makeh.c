
#include <stdio.h>

main()
{
	FILE *fp;
	int	numfish;
	char	prefix[BUFSIZ];
	char **fishlist;
	int i;

	fp = fopen("FishList", "r");
	if (fp == NULL)
	{
		printf(stderr, "ERROR: cannot open FishList for read!\n");
		exit(1);
	}
	fscanf(fp, "%d\n", &numfish);

	printf("/*\n * Warning, this header file is automatically generated\n */\n");

	printf("\n#define NUM_FISH\t%d\n\n", numfish);
	fishlist = (char **)malloc(numfish * sizeof(char *));

	for (i=0; i<numfish; i++)
	{
		fscanf(fp, "%s\n", prefix);
		fishlist[i] = (char *)malloc(strlen(prefix) + 1);
		strcpy(fishlist[i], prefix);
	}
	fclose(fp);

	for (i=0; i<numfish; i++)
	{
		printf("#include \"%s.h\"\n", fishlist[i]);
	}
	printf("\n");

	printf("int rwidth[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_width,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("int rheight[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_height,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("int rcolors[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_colors,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("int rback[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_back,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("int *rreds[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_reds,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("int *rgreens[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_greens,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("int *rblues[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_blues,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("unsigned char *xfishRasterA[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_rasterA,\n", fishlist[i]);
	}
	printf("};\n\n");

	printf("unsigned char *xfishRasterB[] = {\n");
	for (i=0; i<numfish; i++)
	{
		printf("\t%s_rasterB,\n", fishlist[i]);
	}
	printf("};\n\n");
	exit(0);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numfish;
char **fishlist;

void prfish(char *t, char *f)
{
    int i;

    printf("%s = {\n", t);
    for (i = 0; i < numfish; i++)
	printf("\t%s_%s,\n", fishlist[i], f);
    printf("};\n\n");
}

main()
{
    FILE *fp;
    char prefix[BUFSIZ];
    int i;

    fp = fopen("FishList", "r");
    if (fp == NULL) {
	fprintf(stderr, "ERROR: cannot open FishList for read!\n");
	exit(1);
    }
    fscanf(fp, "%d\n", &numfish);

    printf("/*\n * Warning, this header file is automatically generated\n */\n");

    printf("\n#define NUM_FISH\t%d\n\n", numfish);
    fishlist = (char **) malloc(numfish * sizeof(char *));

    for (i = 0; i < numfish; i++) {
	fscanf(fp, "%s\n", prefix);
	fishlist[i] = (char *) malloc(strlen(prefix) + 1);
	strcpy(fishlist[i], prefix);
    }
    fclose(fp);

    for (i = 0; i < numfish; i++)
	printf("#include \"%s.h\"\n", fishlist[i]);
    printf("\n");

    prfish("int rwidth[]", "width");
    prfish("int rheight[]", "height");
    prfish("int rcolors[]", "colors");
    prfish("int rback[]", "back");
    prfish("int *rreds[]", "reds");
    prfish("int *rgreens[]", "greens");
    prfish("int *rblues[]", "blues");
    prfish("unsigned char *xfishRasterA[]", "rasterA");
    prfish("unsigned char *xfishRasterB[]", "rasterB");

    exit(0);
}

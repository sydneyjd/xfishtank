
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numfish;
char **fishlist;

char *get_fishname(char *path);
void prfish(char *t, char *f);

int
main(int ac, char *av[])
{
    char prefix[BUFSIZ];
    int i;

    numfish = ac - 1;

    printf("/*\n * Warning, this header file is automatically generated\n */\n");

    printf("\n#define NUM_FISH\t%d\n\n", numfish);
    fishlist = (char **) malloc(numfish * sizeof(char *));

    for (i = 0; i < numfish; i++) {
	fishlist[i] = get_fishname(av[i + 1]);
    }

    for (i = 0; i < numfish; i++)
	printf("#include \"%s.h\"\n", fishlist[i]);

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

char *
get_fishname(char *path)
{
    char *s0;
    int slen;

    path = strdup(path);

    /* If it has a "/", chop off everything before that */
    s0 = strrchr(path, '/');
    s0 = s0 ? s0 + 1 : path;

    /* If it ends with ".h", chop that off */
    slen = strlen(s0);
    if (slen > 2 && !strcmp(s0 + slen - 2, ".h"))
	*(s0 + slen - 2) = '\0';

    return s0;
}

void
prfish(char *t, char *f)
{
    int i;

    printf("\n%s = {\n", t);
    for (i = 0; i < numfish; i++)
	printf("\t%s_%s,\n", fishlist[i], f);
    printf("};\n");
}

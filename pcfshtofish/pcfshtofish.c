#include <stdio.h>

#define MAXCOLORS 256
#define	DEFAULT_NAME	"fish"

struct rgb_color {
    unsigned int red, green, blue;
};


void
ReadFshColors(ifp, colrs)
FILE *ifp;
struct rgb_color *colrs;
{
    int i;
    int val, val2;

    for (i = 0; i < 132; i++) {
	val = fgetc(ifp);
    }

    for (i = 0; i < 16; i++) {
	val = fgetc(ifp);
	colrs[i].blue = val;
	val = fgetc(ifp);
	colrs[i].green = val;
	val = fgetc(ifp);
	colrs[i].red = val;
	val = fgetc(ifp);
    }
}


unsigned char *
ReadFshData1(ifp)
FILE *ifp;
{
    unsigned char *data;
    int i, j, indx;
    int val, val2;

    data = (unsigned char *) malloc(64 * 64);
    for (j = 63; j >= 0; j--) {
	indx = 64 * j;
	for (i = 0; i < 32; i++) {
	    val = fgetc(ifp);
	    val2 = (val >> 4) & 0x0f;
	    data[indx] = (unsigned char) val2;
	    indx++;
	    val2 = val & 0x0f;
	    data[indx] = (unsigned char) val2;
	    indx++;
	}
    }
    return (data);
}


unsigned char *
ReadFshData2(ifp)
FILE *ifp;
{
    unsigned char *data;
    int i, j, indx;
    int val, val2;

    for (i = 0; i < (16 * 32); i++) {
	val = fgetc(ifp);
    }

    data = (unsigned char *) malloc(64 * 64);
    for (j = 63; j >= 0; j--) {
	indx = 64 * j;
	for (i = 0; i < 32; i++) {
	    val = fgetc(ifp);
	    val2 = (val >> 4) & 0x0f;
	    data[indx] = (unsigned char) val2;
	    indx++;
	    val2 = val & 0x0f;
	    data[indx] = (unsigned char) val2;
	    indx++;
	}
    }
    return (data);
}


struct rgb_color
 *
MapColors(data1, colrs1, data2, colrs2, width, height, Cnt)
unsigned char *data1;
struct rgb_color *colrs1;
unsigned char *data2;
struct rgb_color *colrs2;
int width, height;
int *Cnt;
{
    int i, j;
    int fcnt, indx;
    int Mapping[MAXCOLORS];
    unsigned char *ptr;
    struct rgb_color *fcolrs;

    fcolrs = (struct rgb_color *) malloc((MAXCOLORS + 1) * sizeof(struct rgb_color));

    fcnt = 0;
    ptr = data1;
    for (i = 0; i < (width * height); i++) {
	indx = (int) *ptr;
	for (j = 0; j < fcnt; j++) {
	    if ((fcolrs[j].red == colrs1[indx].red) &&
		(fcolrs[j].green == colrs1[indx].green) &&
		(fcolrs[j].blue == colrs1[indx].blue)) {
		break;
	    }
	}
	if (j == fcnt) {
	    fcolrs[j].red = colrs1[indx].red;
	    fcolrs[j].green = colrs1[indx].green;
	    fcolrs[j].blue = colrs1[indx].blue;
	    Mapping[indx] = j;
	    fcnt++;
	    if (fcnt > MAXCOLORS) {
		fprintf(stderr, "Error: cannot use more than %d colors in your fish\n",
			MAXCOLORS);
		exit(1);
	    }
	}
	ptr++;
    }
    ptr = data1;
    for (i = 0; i < (width * height); i++) {
	indx = (int) *ptr;
	*ptr = (unsigned char) (Mapping[indx]);
	ptr++;
    }

    ptr = data2;
    for (i = 0; i < (width * height); i++) {
	indx = (int) *ptr;
	for (j = 0; j < fcnt; j++) {
	    if ((fcolrs[j].red == colrs2[indx].red) &&
		(fcolrs[j].green == colrs2[indx].green) &&
		(fcolrs[j].blue == colrs2[indx].blue)) {
		break;
	    }
	}
	if (j == fcnt) {
	    fcolrs[j].red = colrs2[indx].red;
	    fcolrs[j].green = colrs2[indx].green;
	    fcolrs[j].blue = colrs2[indx].blue;
	    Mapping[indx] = j;
	    fcnt++;
	    if (fcnt > MAXCOLORS) {
		fprintf(stderr, "Error: cannot use more than %d colors in your fish\n",
			MAXCOLORS);
		exit(1);
	    }
	}
	ptr++;
    }
    ptr = data2;
    for (i = 0; i < (width * height); i++) {
	indx = (int) *ptr;
	*ptr = (unsigned char) (Mapping[indx]);
	ptr++;
    }

    for (i = 0; i < fcnt; i++) {
	fcolrs[i].red = fcolrs[i].red * 256;
	fcolrs[i].green = fcolrs[i].green * 256;
	fcolrs[i].blue = fcolrs[i].blue * 256;
    }
    for (i = fcnt; i < 16; i++) {
	fcolrs[i].red = 0;
	fcolrs[i].green = 0;
	fcolrs[i].blue = 0;
    }

    if (fcnt < 16) {
	fcnt = 16;
    }
    *Cnt = fcnt;
    return (fcolrs);
}


main(argc, argv)
int argc;
char *argv[];
{
    FILE *ifp;
    FILE *ofd;
    int i, j, cnt, fcnt;
    struct rgb_color colrs1[MAXCOLORS];
    struct rgb_color *fcolrs;
    unsigned char *data1, *data2;
    int width, height;
    char outfile[256];
    char *outname;

    ifp = NULL;
    if (argc > 1) {
	ifp = fopen(argv[1], "r");
    }
    if (ifp == NULL) {
	ifp = stdin;
    }

    width = 64;
    height = 64;
    ReadFshColors(ifp, colrs1);
    data1 = ReadFshData1(ifp);
    data2 = ReadFshData2(ifp);

    fcolrs = MapColors(data1, colrs1, data2, colrs1, width, height, &fcnt);

    if (argc > 2) {
	outname = argv[2];
    } else {
	cnt = 0;
	if (argc > 1) {
	    while ((argv[1][cnt] != '.') && (argv[1][cnt] != '\0')) {
		cnt++;
	    }
	}
	if (cnt == 0) {
	    outname = (char *) malloc(strlen(DEFAULT_NAME) + 1);
	    strcpy(outname, DEFAULT_NAME);
	} else {
	    outname = (char *) malloc(cnt + 1);
	    strncpy(outname, argv[1], cnt);
	    outname[cnt] = '\0';
	}
    }
    sprintf(outfile, "%s.h", outname);

    ofd = fopen(outfile, "w");
    if (ofd == NULL) {
	fprintf(stderr, "Error:  cannot open (%s) for writing\n", outfile);
	exit(1);
    }

    fprintf(ofd, "#define %s_width\t\t%d\n", outname, width);
    fprintf(ofd, "#define %s_height\t\t%d\n", outname, height);
    fprintf(ofd, "#define %s_colors\t\t%d\n", outname, fcnt);
    fprintf(ofd, "#define %s_back\t\t%d\n", outname, (int) data1[0]);
    fprintf(ofd, "int\t%s_reds[] = {", outname);
    for (i = 0; i < fcnt; i++) {
	if (i == (fcnt - 1)) {
	    fprintf(ofd, "%d};\n", fcolrs[i].red);
	} else {
	    fprintf(ofd, "%d, ", fcolrs[i].red);
	}
    }
    fprintf(ofd, "int\t%s_greens[] = {", outname);
    for (i = 0; i < fcnt; i++) {
	if (i == (fcnt - 1)) {
	    fprintf(ofd, "%d};\n", fcolrs[i].green);
	} else {
	    fprintf(ofd, "%d, ", fcolrs[i].green);
	}
    }
    fprintf(ofd, "int\t%s_blues[] = {", outname);
    for (i = 0; i < fcnt; i++) {
	if (i == (fcnt - 1)) {
	    fprintf(ofd, "%d};\n", fcolrs[i].blue);
	} else {
	    fprintf(ofd, "%d, ", fcolrs[i].blue);
	}
    }
    fprintf(ofd, "unsigned char\t%s_rasterA[] = {\n", outname);
    for (i = 0; i < (width * height); i++) {
	fprintf(ofd, "0x%02x,", data1[i]);
    }
    fprintf(ofd, "};\n");
    fprintf(ofd, "unsigned char\t%s_rasterB[] = {\n", outname);
    for (i = 0; i < (width * height); i++) {
	fprintf(ofd, "0x%02x,", data2[i]);
    }
    fprintf(ofd, "};\n");

    fclose(ofd);

    exit(0);
}

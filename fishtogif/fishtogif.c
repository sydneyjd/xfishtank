#include <stdio.h>
#include <strings.h>
#include <string.h>

#define MAXCOLORS 256


extern void WriteGif();


struct rgb_color {
	unsigned int red, green, blue;
};



unsigned char *ReadFish(fp, w, h, colrs, filename)
FILE *fp;
int *w, *h;
struct rgb_color *colrs;
char *filename;
{
	int i;
	char junk[80];
	char name[80];
	char *cptr;
	unsigned char *data;
	unsigned char *ptr;
	int scale, val;

	for (i=0; i<4; i++)
	{
		fscanf(fp, "%s %s %d\n", junk, name, &val);
		cptr = strrchr(name, '_');
		if (cptr == NULL)
		{
			cptr = name;
		}
		else
		{
			cptr++;
		}

		if (strcmp(cptr, "width") == 0)
		{
			*w = val;
		}
		else if (strcmp(cptr, "height") == 0)
		{
			*h = val;
		}
		else if (strcmp(cptr, "colors") == 0)
		{
			scale = val;
		}
	}

	cptr = strrchr(name, '_');
	if (cptr == NULL)
	{
		strcpy(filename, "");
	}
	else
	{
		*cptr = '\0';
		strcpy(filename, name);
	}

	val = fgetc(fp);
	while (val != '{')
	{
		val = fgetc(fp);
	}
	for (i=0; i<scale; i++)
	{
		fscanf(fp, " %d", &val);
		colrs[i].red = val / 256;
		val = fgetc(fp);
	}

	val = fgetc(fp);
	while (val != '{')
	{
		val = fgetc(fp);
	}
	for (i=0; i<scale; i++)
	{
		fscanf(fp, " %d", &val);
		colrs[i].green = val / 256;
		val = fgetc(fp);
	}

	val = fgetc(fp);
	while (val != '{')
	{
		val = fgetc(fp);
	}
	for (i=0; i<scale; i++)
	{
		fscanf(fp, " %d", &val);
		colrs[i].blue = val / 256;
		val = fgetc(fp);
	}

	for (i=scale; i<MAXCOLORS; i++)
	{
		colrs[i].red = 0;
		colrs[i].green = 0;
		colrs[i].blue = 0;
	}

	val = fgetc(fp);
	while (val != '{')
	{
		val = fgetc(fp);
	}
	data = (unsigned char *)malloc(2 * (*w) * (*h));
	ptr = data;
	for (i=0; i<((*w) * (*h)); i++)
	{
		fscanf(fp, " 0x%x", &val);
		*ptr++ = (unsigned char)val;
		val = fgetc(fp);
	}

	val = fgetc(fp);
	while (val != '{')
	{
		val = fgetc(fp);
	}
	for (i=0; i<((*w) * (*h)); i++)
	{
		fscanf(fp, " 0x%x", &val);
		*ptr++ = (unsigned char)val;
		val = fgetc(fp);
	}

	return(data);
}



main(argc, argv)
int argc;
char *argv[];
{
	FILE *ifd;
	int i;
	struct rgb_color colrs[MAXCOLORS];
	unsigned char *data;
	unsigned char *data1;
	unsigned char *data2;
	unsigned char *ptr;
	unsigned char *ptr2;
	int width, height;
	char outfile[256];
	char outname[80];

	if (argc < 2)
	{
		fprintf(stderr, "Usage:  %s <fish.h> [<outprefix>]\n", argv[0]);
		exit(1);
	}

	ifd = fopen(argv[1], "r");
	if (ifd == NULL)
	{
		fprintf(stderr, "Error:  cannot open (%s) for read\n", argv[1]);
		exit(1);
	}

	data = ReadFish(ifd, &width, &height, colrs, outname);
	fclose(ifd);
	ptr = data;

	data1 = (unsigned char *)malloc(width * height);
	ptr2 = data1;
	for (i=0; i<(width * height); i++)
	{
		*ptr2++ = *ptr++;
	}

	data2 = (unsigned char *)malloc(width * height);
	ptr2 = data2;
	for (i=0; i<(width * height); i++)
	{
		*ptr2++ = *ptr++;
	}

	if (argc >= 3)
	{
		strcpy(outname, argv[2]);
	}

	sprintf(outfile, "%s_1.gif", outname);

	WriteGif(outfile, data1, width, height, colrs);

	sprintf(outfile, "%s_2.gif", outname);

	WriteGif(outfile, data2, width, height, colrs);

	exit(0);
}


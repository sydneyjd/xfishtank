#include <stdio.h>

#define MAXCOLORS 256
#define COLS_PER_LINE	7
#define NUMS_PER_LINE	16

extern unsigned char *ReadGifBitmap();

struct ebmhdr {
	char magic[12];
	unsigned int width, height;
	int num_colors;
};
struct rgb_color {
	unsigned int red, green, blue;
	int pix_len;
};



/*
 * dscan is a quick replacement for:
 *
 *    fscanf (fp, "%d", &ip);
 */
dscan(fp, ip)
    FILE *fp;
    int *ip;
{

    register int v = 0;
    register int n = 0;
    int done       = 0;
    static int nerrors;
    
    while ( !done )
    {
	register int i;
	i = getc(fp);
	if (i < 0)
	{
	    *ip = v;
	    fprintf (stderr, "dscanf: EOF");
	    if (nerrors++ > 2)  
	       return (-1);
	    else if (nerrors++ > 4)  
	       exit (-1);
	    return (n);
	}
	i &=  0xFF;
	switch (i)
	{
	  case '0' :	  case '1' :
	  case '2' :	  case '3' :
	  case '4' :	  case '5' :
	  case '6' :	  case '7' :
	  case '8' :	  case '9' :
	      v =  i  -  '0'  +  (v * 10);
	      n++;
	      break;
	  case '\n' :
	  case '\r' :
	  case ' ' :
	  case '\t' :
	      if ( n )
	      {
		  *ip = v;
		  done = 1;
	      }
	      break;
	  default:
	    fprintf (stderr,
		     "dscanf: non numeric character (0x%x) '%c'\n",
		     i, i);
	    if (nerrors++ > 4)  
	       exit (-1);
	    return (-1);
	      
	}
    }
    return (n);
}



unsigned char *ReadEbmBitmap(fp, w, h, colrs)
FILE *fp;
int *w, *h;
struct rgb_color *colrs;
{
	int i, temp, scale;
	unsigned int bmap_size;
	int *ptr[3];
	int red, green, blue;
	char line[1024];
	unsigned char *bit_data;
	unsigned char *bitp;

	bit_data = NULL;
	*w = 0;
	*h = 0;

	if ((fscanf(fp, "%12s %d %d %d\n", line, w, h, &scale)==4)&&
		(strcmp(line, "xstitch_data")==0))
	{
		for (i=0; i<3; i++)
		{
			fscanf(fp, "%s", line);
			if (strcmp(line, "red")==0)
			{
				ptr[i] = &red;
			}
			else if (strcmp(line, "green")==0)
			{
				ptr[i] = &green;
			}
			else if (strcmp(line, "blue")==0)
			{
				ptr[i] = &blue;
			}
			else
			{
				fprintf(stderr, "File has improper format\n");
				return((unsigned char *)NULL);
			}
		}
		fscanf(fp, "[^\n]\n");
		for (i=0; i<scale; i++)
		{
			line[0] = '\0';
			if (fscanf(fp, "%d %d %d%[^\n]\n",
				ptr[0], ptr[1], ptr[2], line)<3)
			{
				fprintf(stderr, "Bad color data #%d\n", i);
			}
			colrs[i].red = red;
			colrs[i].green = green;
			colrs[i].blue = blue;
		}

		bmap_size = (*w) * (*h);
		bit_data = (unsigned char *)malloc(bmap_size);
		if (bit_data == NULL)
		{
			fprintf(stderr, "Cannot allocate space for bitmap\n");
		}
		bitp = bit_data;
		for (i=0; i<bmap_size; i++)
		{
			if (dscan(fp, &temp) == -1)
				break;
			if (temp > (scale - 1))
				temp = 0;
			*bitp = (unsigned char)temp;
			bitp++;
		}
	}
	else
	{
		return((unsigned char *)NULL);
	}
	return(bit_data);
}



int
swapint(in)
	int in;
{
	int out;

	out = 0;
	out = out | ((in >> 24) & 0xff);
	out = out | ((in >> 8) & 0xff00);
	out = out | ((in << 8) & 0xff0000);
	out = out | ((in << 24) & 0xff000000);
	return(out);
}



unsigned char *ReadCompactEbmBitmap(fp, w, h, colrs)
FILE *fp;
int *w, *h;
struct rgb_color *colrs;
{
	int i, scale;
	unsigned int bmap_size;
	char pix_file[1024];
	unsigned char *bit_data;
	unsigned char *bitp;
	struct ebmhdr Hdr;
	struct rgb_color color_data;
	int doswap;

	doswap = 0;
	bit_data = NULL;
	*w = 0;
	*h = 0;

	fread(&Hdr, sizeof(struct ebmhdr), 1, fp);
	if (strcmp(Hdr.magic, "xstitch") == 0)
	{
		/*
		 * a horrible hack to deal with different byte ordering on
		 * different machines.  Is based on assumption of only 2
		 * possible byte orderings, and never more than 256 colors.
		 */
		if (Hdr.num_colors > 256)
		{
			doswap = 1;
			Hdr.width = swapint(Hdr.width);
			Hdr.height = swapint(Hdr.height);
			Hdr.num_colors = swapint(Hdr.num_colors);
		}
		*w = Hdr.width;
		*h = Hdr.height;
		scale=Hdr.num_colors;
		for (i=0; i<Hdr.num_colors; i++)
		{
			fread(&colrs[i], sizeof(struct rgb_color), 1, fp);
			if (doswap)
			{
				colrs[i].red = swapint(colrs[i].red);
				colrs[i].green = swapint(colrs[i].green);
				colrs[i].blue = swapint(colrs[i].blue);
				colrs[i].pix_len = swapint(colrs[i].pix_len);
			}
			if (colrs[i].pix_len != 0)
			{
				if (colrs[i].pix_len > 1023)
				{
					fprintf(stderr, "bad pattern\n");
					return((unsigned char *)NULL);
				}
				fread(pix_file, sizeof(char),
					colrs[i].pix_len, fp);
			}
		}

		bmap_size = (*w) * (*h);
		bit_data = (unsigned char *)malloc(bmap_size);
		if (bit_data == NULL)
		{
			fprintf(stderr, "Cannot allocate space for bitmap\n");
		}
		fread(bit_data, sizeof(unsigned char), ((*w) * (*h)), fp);

		bitp = bit_data;
		for (i=0; i<bmap_size; i++)
		{
			if ((int)*bitp > (scale - 1))
				*bitp = (unsigned char)0;
			bitp++;
		}
	}
	else
	{
		return((unsigned char *)NULL);
	}
	return(bit_data);
}


struct rgb_color
*MapColors(data1, colrs1, data2, colrs2, width, height, Cnt)
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

	fcolrs = (struct rgb_color *)malloc((MAXCOLORS + 1) *
					sizeof(struct rgb_color));

	fcnt = 0;
	ptr = data1;
	for (i=0; i < (width * height); i++)
	{
		indx = (int)*ptr;
		for (j=0; j<fcnt; j++)
		{
			if ((fcolrs[j].red == colrs1[indx].red)&&
			    (fcolrs[j].green == colrs1[indx].green)&&
			    (fcolrs[j].blue == colrs1[indx].blue))
			{
				break;
			}
		}
		if (j == fcnt)
		{
			fcolrs[j].red = colrs1[indx].red;
			fcolrs[j].green = colrs1[indx].green;
			fcolrs[j].blue = colrs1[indx].blue;
			Mapping[indx] = j;
			fcnt++;
			if (fcnt > MAXCOLORS)
			{
				fprintf(stderr, "Error: cannot use more than %d colors in your fish\n", MAXCOLORS);
				exit(1);
			}
		}
		ptr++;
	}
	ptr = data1;
	for (i=0; i < (width * height); i++)
	{
		indx = (int)*ptr;
		*ptr = (unsigned char)(Mapping[indx]);
		ptr++;
	}

	ptr = data2;
	for (i=0; i < (width * height); i++)
	{
		indx = (int)*ptr;
		for (j=0; j<fcnt; j++)
		{
			if ((fcolrs[j].red == colrs2[indx].red)&&
			    (fcolrs[j].green == colrs2[indx].green)&&
			    (fcolrs[j].blue == colrs2[indx].blue))
			{
				break;
			}
		}
		if (j == fcnt)
		{
			fcolrs[j].red = colrs2[indx].red;
			fcolrs[j].green = colrs2[indx].green;
			fcolrs[j].blue = colrs2[indx].blue;
			Mapping[indx] = j;
			fcnt++;
			if (fcnt > MAXCOLORS)
			{
				fprintf(stderr, "Error: cannot use more than %d colors in your fish\n", MAXCOLORS);
				exit(1);
			}
		}
		ptr++;
	}
	ptr = data2;
	for (i=0; i < (width * height); i++)
	{
		indx = (int)*ptr;
		*ptr = (unsigned char)(Mapping[indx]);
		ptr++;
	}

	for (i=0; i<fcnt; i++)
	{
		fcolrs[i].red = fcolrs[i].red * 256;
		fcolrs[i].green = fcolrs[i].green * 256;
		fcolrs[i].blue = fcolrs[i].blue * 256;
	}
	for (i=fcnt; i<16; i++)
	{
		fcolrs[i].red = 0;
		fcolrs[i].green = 0;
		fcolrs[i].blue = 0;
	}

	if (fcnt < 16)
	{
		fcnt = 16;
	}
	*Cnt = fcnt;
	return(fcolrs);
}



main(argc, argv)
int argc;
char *argv[];
{
	FILE *ifd;
	FILE *ofd;
	int i, cnt, fcnt;
	struct rgb_color colrs1[MAXCOLORS];
	struct rgb_color colrs2[MAXCOLORS];
	struct rgb_color *fcolrs;
	unsigned char *data1, *data2;
	int width1, height1, width2, height2;
	char outfile[256];
	char *outname;
	int num_cnt;

	if (argc < 3)
	{
		fprintf(stderr, "Usage:  %s <fish1> <fish2> [<outprefix>]\n", argv[0]);
		exit(1);
	}

	ifd = fopen(argv[1], "r");
	if (ifd == NULL)
	{
		fprintf(stderr, "Error:  cannot open (%s) for read\n", argv[1]);
		exit(1);
	}

	data1 = ReadCompactEbmBitmap(ifd, &width1, &height1, colrs1);
	if (data1 == NULL)
	{
		rewind(ifd);
		data1 = ReadEbmBitmap(ifd, &width1, &height1, colrs1);
	}
	if (data1 == NULL)
	{
		rewind(ifd);
		data1 = ReadGifBitmap(ifd, &width1, &height1, colrs1);
	}
	fclose(ifd);

	if (data1 == NULL)
	{
		fprintf(stderr, "Error:  (%s) has unknown format\n", argv[1]);
		exit(1);
	}

	ifd = fopen(argv[2], "r");
	if (ifd == NULL)
	{
		fprintf(stderr, "Error:  cannot open (%s) for read\n", argv[2]);
		exit(1);
	}

	data2 = ReadCompactEbmBitmap(ifd, &width2, &height2, colrs2);
	if (data2 == NULL)
	{
		rewind(ifd);
		data2 = ReadEbmBitmap(ifd, &width2, &height2, colrs2);
	}
	if (data2 == NULL)
	{
		rewind(ifd);
		data2 = ReadGifBitmap(ifd, &width2, &height2, colrs2);
	}
	fclose(ifd);

	if (data2 == NULL)
	{
		fprintf(stderr, "Error:  (%s) has unknown format\n", argv[2]);
		exit(1);
	}


	if ((width1 != width2)||(height1 != height2))
	{
		fprintf(stderr, "Error:  fish must have same dimensions\n");
		exit(1);
	}

	fcolrs = MapColors(data1, colrs1, data2, colrs2, width1, height1,&fcnt);

	if (data1[0] != data2[0])
	{
		fprintf(stderr, "Error:  fish must have same backgrounds\n");
		exit(1);
	}

	if (argc >= 4)
	{
		outname = argv[3];
	}
	else
	{
		cnt = 0;
		while ((argv[1][cnt] == argv[2][cnt])&&(argv[1][cnt] != '.')&&
			(argv[2][cnt] != '.'))
		{
			cnt++;
		}
		if (cnt == 0)
		{
			fprintf(stderr, "Error:  files need common prefix\n");
			exit(1);
		}
		outname = (char *)malloc(cnt + 1);
		strncpy(outname, argv[1], cnt);
		outname[cnt] = '\0';
	}
	sprintf(outfile, "%s.h", outname);

	ofd = fopen(outfile, "w");
	if (ofd == NULL)
	{
		fprintf(stderr, "Error:  cannot open (%s) for writing\n", outfile);
		exit(1);
	}

	fprintf(ofd, "#define %s_width\t\t%d\n", outname, width1);
	fprintf(ofd, "#define %s_height\t\t%d\n", outname, height1);
	fprintf(ofd, "#define %s_colors\t\t%d\n", outname, fcnt);
	fprintf(ofd, "#define %s_back\t\t%d\n", outname, (int)data1[0]);
	fprintf(ofd, "int\t%s_reds[] = {", outname);
	num_cnt = 0;
	for (i=0; i<fcnt; i++)
	{
		if (i == (fcnt - 1))
		{
			fprintf(ofd, "%d};\n", fcolrs[i].red);
		}
		else
		{
			fprintf(ofd, "%d, ", fcolrs[i].red);
			if (++num_cnt >= COLS_PER_LINE)
			{
				fprintf(ofd, "\n");
				num_cnt = 0;
			}
		}
	}
	fprintf(ofd, "int\t%s_greens[] = {", outname);
	num_cnt = 0;
	for (i=0; i<fcnt; i++)
	{
		if (i == (fcnt - 1))
		{
			fprintf(ofd, "%d};\n", fcolrs[i].green);
		}
		else
		{
			fprintf(ofd, "%d, ", fcolrs[i].green);
			if (++num_cnt >= COLS_PER_LINE)
			{
				fprintf(ofd, "\n");
				num_cnt = 0;
			}
		}
	}
	fprintf(ofd, "int\t%s_blues[] = {", outname);
	num_cnt = 0;
	for (i=0; i<fcnt; i++)
	{
		if (i == (fcnt - 1))
		{
			fprintf(ofd, "%d};\n", fcolrs[i].blue);
		}
		else
		{
			fprintf(ofd, "%d, ", fcolrs[i].blue);
			if (++num_cnt >= COLS_PER_LINE)
			{
				fprintf(ofd, "\n");
				num_cnt = 0;
			}
		}
	}
	fprintf(ofd, "unsigned char\t%s_rasterA[] = {\n", outname);
	num_cnt = 0;
	for (i=0; i<(width1 * height1); i++)
	{
		fprintf(ofd, "0x%02x,", data1[i]);
		if (++num_cnt >= NUMS_PER_LINE)
		{
			fprintf(ofd, "\n");
			num_cnt = 0;
		}
	}
	fprintf(ofd, "};\n");
	fprintf(ofd, "unsigned char\t%s_rasterB[] = {\n", outname);
	num_cnt = 0;
	for (i=0; i<(width1 * height1); i++)
	{
		fprintf(ofd, "0x%02x,", data2[i]);
		if (++num_cnt >= NUMS_PER_LINE)
		{
			fprintf(ofd, "\n");
			num_cnt = 0;
		}
	}
	fprintf(ofd, "};\n");

	fclose(ofd);

	exit(0);
}


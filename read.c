#include <X11/Xlib.h>
#include <stdio.h>
#include "compact.h"
#include "medcut.h"

#define DEF_BLACK       BlackPixel(Dpy, DefaultScreen(Dpy))
#define DEF_WHITE       WhitePixel(Dpy, DefaultScreen(Dpy))
#define	MAX_LINE	81


extern unsigned char *ReadGifBitmap();


extern Display *Dpy;

int NumColors;

char nibMask[8] = {
	1, 2, 4, 8, 16, 32, 64, 128
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


unsigned char *ReadXpmPixmap(fp, w, h, colrs, Colors, CharsPP)
FILE *fp;
int *w, *h;
struct colr_data *colrs;
int Colors, CharsPP;
{
	unsigned char *pixels;
	char **Color_Vals;
	XColor tmpcolr;
	int i, j, k;
	int value, found;
	char line[BUFSIZ], name_and_type[MAX_LINE];
	unsigned char *dataP;
	unsigned char *bitp;
	int tchar;
	char *t;
	char *t2;

	NumColors = 256;
	if (Colors == 0)
	{
		fprintf(stderr, "Can't find Colors.\n");
		return((unsigned char *)NULL);
	}
	if (*w == 0)
	{
		fprintf(stderr, "Invalid width.\n");
		return((unsigned char *)NULL);
	}
	if (*h == 0)
	{
		fprintf(stderr, "Invalid height.\n");
		return((unsigned char *)NULL);
	}

	Color_Vals = (char **)malloc(sizeof(char *) * Colors);
	for (i=0; i<Colors; i++)
	{
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF))
		{
			tchar = getc(fp);
		}
		Color_Vals[i] = (char *)malloc(sizeof(char) * (CharsPP + 1));
		j = 0;
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF)&&(j < CharsPP))
		{
			Color_Vals[i][j] = (char)tchar;
			tchar = getc(fp);
			j++;
		}
		Color_Vals[i][j] = '\0';
		if (tchar != '"')
		{
			tchar = getc(fp);
			while ((tchar != '"')&&(tchar != EOF))
			{
				tchar = getc(fp);
			}
		}
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF))
		{
			tchar = getc(fp);
		}
		j = 0;
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF))
		{
			line[j] = (char)tchar;
			tchar = getc(fp);
			j++;
		}
		line[j] = '\0';
		XParseColor(Dpy, DefaultColormap(Dpy, DefaultScreen(Dpy)),
			line, &tmpcolr);
		colrs[i].red = tmpcolr.red;
		colrs[i].green = tmpcolr.green;
		colrs[i].blue = tmpcolr.blue;
	}
	for (i=Colors; i<256; i++)
	{
		colrs[i].red = 0;
		colrs[i].green = 0;
		colrs[i].blue = 0;
	}
	tchar = getc(fp);
	while ((tchar != ';')&&(tchar != EOF))
	{
		tchar = getc(fp);
	}

	for ( ; ; )
	{
		if (!(fgets(line, MAX_LINE, fp)))
		{
			fprintf(stderr, "Can't find Pixels\n");
			return((unsigned char *)NULL);
		}
		else if (sscanf(line,"static char * %s = {",name_and_type) == 1)
		{
			if ((t = (char *)rindex(name_and_type, '_')) == NULL)
			{
				t = name_and_type;
			}
			else
			{
				t++;
			}
			if ((t2 = (char *)index(name_and_type, '[')) != NULL)
			{
				*t2 = '\0';
			}
			if (!strcmp("pixels", t))
			{
				break;
			}
		}
	}
	pixels = (unsigned char *)malloc((*w) * (*h));
	if (pixels == NULL)
	{
		fprintf(stderr, "Not enough memory for data.\n");
		return((unsigned char *)NULL);
	}

	line[0] = '\0';
	t = line;
	dataP = pixels;
	tchar = getc(fp);
	while ((tchar != '"')&&(tchar != EOF))
	{
		tchar = getc(fp);
	}
	tchar = getc(fp);
	for (j=0; j<(*h); j++)
	{
		for (i=0; i<(*w); i++)
		{
			k = 0;
			while ((tchar != '"')&&(tchar != EOF)&&(k < CharsPP))
			{
				line[k] = (char)tchar;
				tchar = getc(fp);
				k++;
			}
			if ((k == 0)&&(tchar == '"'))
			{
				tchar = getc(fp);
				while ((tchar != '"')&&(tchar != EOF))
				{
					tchar = getc(fp);
				}
				k = 0;
				tchar = getc(fp);
				while ((tchar != '"')&&(tchar != EOF)&&
					(k < CharsPP))
				{
					line[k] = (char)tchar;
					tchar = getc(fp);
					k++;
				}
			}
			line[k] = '\0';
			found = 0;
			for (k=0; k<Colors; k++)
			{
				if (strncmp(Color_Vals[k], line, CharsPP) == 0)
				{
					*dataP++ = (unsigned char)k;
					found = 1;
					break;
				}
			}
			if (found == 0)
			{
				fprintf(stderr, "Invalid Pixel (%2s) in file\n", line);
				*dataP++ = (unsigned char)0;
			}
		}
	}
	bitp = pixels;
	for (i=0; i<((*w) * (*h)); i++)
	{
		if ((int)*bitp > (256 - 1))
			*bitp = (unsigned char)0;
		bitp++;
	}
	for (i=0; i<Colors; i++)
	{
		free((char *)Color_Vals[i]);
	}
	free((char *)Color_Vals);
	return(pixels);
}


unsigned char *ReadXbmBitmap(fp, w, h, colrs)
FILE *fp;
int *w, *h;
struct colr_data *colrs;
{
	int blackbit = BlackPixel(Dpy, DefaultScreen(Dpy));
	int whitebit = WhitePixel(Dpy, DefaultScreen(Dpy));
	char line[MAX_LINE], name_and_type[MAX_LINE];
	char *t;
	char *t2;
	unsigned char *ptr, *dataP;
	int bytes_per_line, version10p, raster_length, padding;
	int i, bytes, temp, value;
	int Ncolors, charspp, xpmformat;

	*w = 0;
	*h = 0;
	Ncolors = 0;
	charspp = 0;
	xpmformat = 0;
	for ( ; ; )
	{
		if (!(fgets(line, MAX_LINE, fp)))
			break;
		if (strlen(line) == (MAX_LINE - 1))
		{
			fprintf(stderr, "Line too long.\n");
			return((unsigned char *)NULL);
		}
		if (sscanf(line, "#define %s %d", name_and_type, &value) == 2)
		{
			if (!(t = (char *)rindex(name_and_type, '_')))
				t = name_and_type;
			else
				t++;
			if (!strcmp("width", t))
				*w= value;
			if (!strcmp("height", t))
				*h= value;
			if (!strcmp("ncolors", t))
				Ncolors = value;
			if (!strcmp("pixel", t))
				charspp = value;
			continue;
		}
		if (sscanf(line, "static short %s = {", name_and_type) == 1)
		{
			version10p = 1;
			break;
		}
		else if (sscanf(line,"static char * %s = {",name_and_type) == 1)
		{
			xpmformat = 1;
			if (!(t = (char *)rindex(name_and_type, '_')))
				t = name_and_type;
			else
				t++;
			if ((t2 = (char *)index(name_and_type, '[')) != NULL)
				*t2 = '\0';
			if (!strcmp("mono", t))
				continue;
			else
				break;
		}
		else if (sscanf(line, "static char %s = {", name_and_type) == 1)
		{
			version10p = 0;
			break;
		}
		else
			continue;
	}
	if (xpmformat)
	{
		dataP = ReadXpmPixmap(fp, w, h, colrs, Ncolors, charspp);
		return(dataP);
	}
	if (*w == 0)
	{
		fprintf(stderr, "Invalid width.\n");
		return((unsigned char *)NULL);
	}
	if (*h == 0)
	{
		fprintf(stderr, "Invalid height.\n");
		return((unsigned char *)NULL);
	}
	padding = 0;
	if (((*w % 16) >= 1)&&((*w % 16) <= 8)&&version10p)
	{
		padding = 1;
	}
	bytes_per_line = ((*w + 7) / 8) + padding;
	raster_length =  bytes_per_line * *h;
	dataP = (unsigned char *)malloc((*w) * (*h));
	if (dataP == NULL)
	{
		fprintf(stderr, "Not enough memory.\n");
		return((unsigned char *)NULL);
	}
	ptr = dataP;
	if (version10p)
	{
		int cnt = 0;
		int lim = (bytes_per_line - padding) * 8;
		for (bytes = 0; bytes < raster_length; bytes += 2)
		{
			if (fscanf(fp, " 0x%x%*[,}]%*[ \n]", &value) != 1)
			{
				fprintf(stderr, "Error scanning bits item.\n");
				return((unsigned char *)NULL);
			}
			temp = value;
			value = temp & 0xff;
			for (i = 0; i < 8; i++)
			{
				if (cnt < (*w))
				{
					if (value & nibMask[i])
						*ptr++ = blackbit;
					else
						*ptr++ = whitebit;
				}
				if (++cnt >= lim)
					cnt = 0;
			}
			if ((!padding)||((bytes+2) % bytes_per_line))
			{
				value = temp >> 8;
				for (i = 0; i < 8; i++)
				{
					if (cnt < (*w))
					{
						if (value & nibMask[i])
							*ptr++ = blackbit;
						else
							*ptr++ = whitebit;
					}
					if (++cnt >= lim)
						cnt = 0;
				}
			}
		}
	}
	else
	{
		int cnt = 0;
		int lim = bytes_per_line * 8;
		for (bytes = 0; bytes < raster_length; bytes++)
		{
			if (fscanf(fp, " 0x%x%*[,}]%*[ \n]", &value) != 1)
			{
				fprintf(stderr, "Error scanning bits item.\n");
				return((unsigned char *)NULL);
			}
			for (i = 0; i < 8; i++)
			{
				if (cnt < (*w))
				{
					if (value & nibMask[i])
						*ptr++ = blackbit;
					else
						*ptr++ = whitebit;
				}
				if (++cnt >= lim)
					cnt = 0;
			}
		}
	}
	NumColors = 2;
	return(dataP);
}


unsigned char *ReadEbmBitmap(fp, w, h, colrs)
FILE *fp;
int *w, *h;
struct colr_data *colrs;
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
			colrs[i].red = red*65536/scale;
			colrs[i].green = green*65536/scale;
			colrs[i].blue = blue*65536/scale;
		}
		for (i=scale; i<256; i++)
		{
			colrs[i].red = 0;
			colrs[i].green = 0;
			colrs[i].blue = 0;
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
			if (temp > (256 - 1))
				temp = 0;
			*bitp = (unsigned char)temp;
			bitp++;
		}
	}
	else
	{
		return((unsigned char *)NULL);
	}
	NumColors = scale;
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
struct colr_data *colrs;
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
			fread(&color_data, sizeof(struct rgb_color), 1, fp);
			if (doswap)
			{
				color_data.red = swapint(color_data.red);
				color_data.green = swapint(color_data.green);
				color_data.blue = swapint(color_data.blue);
				color_data.pix_len = swapint(color_data.pix_len);
			}
			colrs[i].red = color_data.red*65536/scale;
			colrs[i].green = color_data.green*65536/scale;
			colrs[i].blue = color_data.blue*65536/scale;
			if (color_data.pix_len != 0)
			{
				if (color_data.pix_len > 1023)
				{
					fprintf(stderr, "bad pattern\n");
					return((unsigned char *)NULL);
				}
				fread(pix_file, sizeof(char),
					color_data.pix_len, fp);
			}
		}
		for (i=scale; i<256; i++)
		{
			colrs[i].red = 0;
			colrs[i].green = 0;
			colrs[i].blue = 0;
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
			if ((int)*bitp > (256 - 1))
				*bitp = (unsigned char)0;
			bitp++;
		}
	}
	else
	{
		return((unsigned char *)NULL);
	}
	NumColors = scale;
	return(bit_data);
}


unsigned char *ReadBitmap(fp, w, h, colrs)
FILE *fp;
int *w, *h;
struct colr_data *colrs;
{
	unsigned char *bit_data;

	NumColors = 0;

	if (fp != NULL)
	{
		bit_data = ReadEbmBitmap(fp, w, h, colrs);
		if (bit_data != NULL)
		{
			return(bit_data);
		}
		rewind(fp);
		bit_data = ReadCompactEbmBitmap(fp, w, h, colrs);
		if (bit_data != NULL)
		{
			return(bit_data);
		}
		rewind(fp);
		bit_data = ReadGifBitmap(fp, w, h, colrs);
		if (bit_data != NULL)
		{
			return(bit_data);
		}
		rewind(fp);
		bit_data = ReadXbmBitmap(fp, w, h, colrs);
		if (bit_data != NULL)
		{
			return(bit_data);
		}
	}
	return((unsigned char *)NULL);
}


/* rasttofish.c - read a Sun rasterfile and produce an xfish header file
**
** All code to read sun rasters came from rasttopnm, his copyright follows.
**
** Copyright (C) 1989, 1991 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#ifdef __STDC__
#define ARGS(alist) alist
#else /*__STDC__*/
#define ARGS(alist) ()
#define const
#endif /*__STDC__*/


#include <stdio.h>
#include "rast.h"

#define MAXCOLORS 256
#define	DEFAULT_NAME	"fish"

struct rgb_color {
        unsigned int red, green, blue;
};


int
pm_readbiglong( in, lP )
    FILE* in;
    long* lP;
{
    int c;

    if ( (c = getc( in )) == EOF )
        return -1;
    *lP = ( c & 0xff ) << 24;
    if ( (c = getc( in )) == EOF )
        return -1;
    *lP |= ( c & 0xff ) << 16;
    if ( (c = getc( in )) == EOF )
        return -1;
    *lP |= ( c & 0xff ) << 8;
    if ( (c = getc( in )) == EOF )
        return -1;
    *lP |= c & 0xff;
    return 0;
}

int
pr_load_header( in, hP )
    FILE* in;
    struct rasterfile* hP;
{
    if ( pm_readbiglong( in, &(hP->ras_magic) ) == -1 )
        return PIX_ERR;
    if ( hP->ras_magic != RAS_MAGIC )
        return PIX_ERR;
    if ( pm_readbiglong( in, &(hP->ras_width) ) == -1 )
        return PIX_ERR;
    if ( pm_readbiglong( in, &(hP->ras_height) ) == -1 )
        return PIX_ERR;
    if ( pm_readbiglong( in, &(hP->ras_depth) ) == -1 )
        return PIX_ERR;
    if ( pm_readbiglong( in, &(hP->ras_length) ) == -1 )
        return PIX_ERR;
    if ( pm_readbiglong( in, &(hP->ras_type) ) == -1 )
        return PIX_ERR;
    if ( pm_readbiglong( in, &(hP->ras_maptype) ) == -1 )
        return PIX_ERR;
    if ( pm_readbiglong( in, &(hP->ras_maplength) ) == -1 )
        return PIX_ERR;
    return 0;
}

int
pr_load_colormap( in, hP, colormap )
    FILE* in;
    struct rasterfile* hP;
    colormap_t* colormap;
{
    if ( colormap == NULL || hP->ras_maptype == RMT_NONE )
	{
	char junk[30000];

	if ( fread( junk, 1, hP->ras_maplength, in ) != hP->ras_maplength )
	    return PIX_ERR;
	}
    else
	{
	colormap->type = hP->ras_maptype;
	switch ( hP->ras_maptype )
	    {
	    case RMT_EQUAL_RGB:
	    colormap->length = hP->ras_maplength / 3;
	    colormap->map[0] = (unsigned char*) malloc( colormap->length );
	    if ( colormap->map[0] == NULL )
		return PIX_ERR;
	    colormap->map[1] = (unsigned char*) malloc( colormap->length );
	    if ( colormap->map[1] == NULL )
		{
		free( colormap->map[0] );
		return PIX_ERR;
		}
	    colormap->map[2] = (unsigned char*) malloc( colormap->length );
	    if ( colormap->map[2] == NULL )
		{
		free( colormap->map[0] );
		free( colormap->map[1] );
		return PIX_ERR;
		}
	    if ( fread( colormap->map[0], 1, colormap->length, in ) != colormap->length ||
	         fread( colormap->map[1], 1, colormap->length, in ) != colormap->length ||
	         fread( colormap->map[2], 1, colormap->length, in ) != colormap->length )
		{
		free( colormap->map[0] );
		free( colormap->map[1] );
		free( colormap->map[2] );
		return PIX_ERR;
		}
	    break;

	    case RMT_RAW:
	    colormap->length = hP->ras_maplength;
	    colormap->map[0] = (unsigned char*) malloc( colormap->length );
	    if ( colormap->map[0] == NULL )
		return PIX_ERR;
	    colormap->map[2] = colormap->map[1] = colormap->map[0];
	    if ( fread( colormap->map[0], 1, hP->ras_maplength, in ) != hP->ras_maplength )
		{
		free( colormap->map[0] );
		return PIX_ERR;
		}
	    break;

	    default:
		fprintf(stderr, "unknown colormap type\n");
		exit(1);
	    }
	}
    return 0;
}


struct pixrect*
mem_create( w, h, depth )
    int w, h, depth;
{
    struct pixrect* p;
    struct mpr_data* m;

    p = (struct pixrect*) malloc( sizeof(struct pixrect) );
    if ( p == NULL )
        return NULL;
    p->pr_ops = NULL;
    p->pr_size.x = w;
    p->pr_size.y = h;
    p->pr_depth = depth;
    m = p->pr_data = (struct mpr_data*) malloc( sizeof(struct mpr_data) );
    if ( m == NULL )
        {
        free( p );
        return NULL;
        }
    /* According to the documentation, linebytes is supposed to be rounded
    ** up to a longword (except on 386 boxes).  However, this turns out
    ** not to be the case.  In reality, all of Sun's code rounds up to
    ** a short, not a long.
    */
    m->md_linebytes = ( w * depth + 15 ) / 16 * 2;
    m->md_offset.x = 0;
    m->md_offset.y = 0;
    m->md_flags = 0;
    m->md_image = (unsigned char*) malloc( m->md_linebytes * h );
    if ( m->md_image == NULL )
        {
        free( m );
        free( p );
        return NULL;
        }

    return p;
}


void
mem_free( p )
    struct pixrect* p;
{
    free( p->pr_data->md_image );
    free( p->pr_data );
    free( p );
}




struct pixrect*
pr_load_image( in, hP, colormap )
    FILE* in;
    struct rasterfile* hP;
    colormap_t* colormap;
{
    struct pixrect* p;
    unsigned char* beimage;
    register unsigned char* bep;
    register unsigned char* bp;
    register unsigned char c;
    int i;
    register int j, count;

    p = mem_create( hP->ras_width, hP->ras_height, hP->ras_depth );
    if ( p == NULL )
	return NULL;

    switch ( hP->ras_type )
	{
	case RT_OLD:
		fprintf(stderr, "old rasterfile type is not supported\n");
		exit(1);

	case RT_FORMAT_TIFF:
		fprintf(stderr, "tiff rasterfile type is not supported\n");
		exit(1);

	case RT_FORMAT_IFF:
		fprintf(stderr, "iff rasterfile type is not supported\n");
		exit(1);

	case RT_EXPERIMENTAL:
		fprintf(stderr, "experimental rasterfile type is not supported\n");
		exit(1);

	case RT_STANDARD:
	case RT_FORMAT_RGB:
	/* Ignore hP->ras_length. */
	i = p->pr_size.y * p->pr_data->md_linebytes;
	if ( fread( p->pr_data->md_image, 1, i, in ) != i )
	    {
	    mem_free( p );
	    return NULL;
	    }
	break;

	case RT_BYTE_ENCODED:
	beimage = (unsigned char*) malloc( hP->ras_length );
	if ( beimage == NULL )
	    {
	    mem_free( p );
	    return NULL;
	    }
	if ( fread( beimage, 1, hP->ras_length, in ) != hP->ras_length )
	    {
	    mem_free( p );
	    free( beimage );
	    return NULL;
	    }
	bep = beimage;
	bp = p->pr_data->md_image;
	for ( i = 0; i < hP->ras_length; )
	    {
	    c = *bep++;
	    if ( c == 128 )
		{
		count = ( *bep++ ) + 1;
		if ( count == 1 )
		    {
		    *bp++ = 128;
		    i += 2;
		    }
		else
		    {
		    c = *bep++;
		    for ( j = 0; j < count; ++j )
			*bp++ = c;
		    i += 3;
		    }
		}
	    else
		{
		*bp++ = c;
		++i;
		}
	    }
	free( beimage );
	break;

	default:
		fprintf(stderr, "unknown rasterfile type\n");
		exit(1);
	}

    return p;
}


unsigned char *
ReadRaster(ifp, w, h, colrs)
FILE *ifp;
int *w, *h;
struct rgb_color *colrs;
{
	struct rasterfile header;
	colormap_t pr_colormap;
	struct pixrect* pr;
	unsigned char *data;
	unsigned char *dptr;
	unsigned char *rdata;
	unsigned char *ptr;
	int linesize;
	int depth;
	int maxval;
	int i, j;

	/* Read in the rasterfile.  First the header. */
	if (pr_load_header(ifp, &header) != 0)
	{
		fprintf(stderr, "unable to read in rasterfile header\n");
		exit(1);
	}

	*w = header.ras_width;
	*h = header.ras_height;

	depth = header.ras_depth;

	if (*w <= 0)
	{
		fprintf(stderr, "invalid width\n");
		exit(1);
	}
	if (*h <= 0)
	{
		fprintf(stderr, "invalid height\n");
		exit(1);
	}

	/* If there is a color map, read it. */
	if (header.ras_maplength != 0)
	{
		if (pr_load_colormap(ifp, &header, &pr_colormap) != 0)
		{
			fprintf(stderr, "no colormap\n");
			exit(1);
		}
	}

	/* Check the depth and color map. */
	switch (depth)
	{
		case 8:
			if (header.ras_maptype != RMT_EQUAL_RGB)
			{
				fprintf(stderr, "not an RGB colormap\n");
				exit(1);
			}
			maxval = 255;
			break;
		default:
			fprintf(stderr, "Can only handle depth 8 images\n");
			exit(1);
	}

	for (i=0; i<maxval; i++)
	{
		colrs[i].red = pr_colormap.map[0][i];
		colrs[i].green = pr_colormap.map[1][i];
		colrs[i].blue = pr_colormap.map[2][i];
	}

	/* Now load the data.  The pixrect returned is a memory pixrect. */
	if ((pr = pr_load_image(ifp, &header, NULL)) == NULL)
	{
		fprintf(stderr, "unable to read image from the rasterfile\n");
		exit(1);
	}

	linesize = ((struct mpr_data*)pr->pr_data)->md_linebytes;
	data = ((struct mpr_data*)pr->pr_data)->md_image;

	rdata = (unsigned char *)malloc((*w) * (*h));
	ptr = rdata;
	for (j=0; j<(*h); j++)
	{
		dptr = (unsigned char *)data;
		for (i=0; i<(*w); i++)
		{
			*ptr++ = *dptr++;
		}
		data += linesize;
	}
	return(rdata);
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
	FILE* ifp;
	FILE *ofd;
	int i, j, cnt, fcnt;
	struct rgb_color colrs1[MAXCOLORS];
	struct rgb_color colrs2[MAXCOLORS];
	struct rgb_color *fcolrs;
	unsigned char *data;
	unsigned char *ptr, *ptr1, *ptr2;
	unsigned char *data1, *data2;
	int width1, height1, width2, height2;
	char outfile[256];
	char *outname;

	ifp = NULL;
	if (argc > 1)
	{
		ifp = fopen(argv[1], "r");
	}
	if (ifp == NULL)
	{
		ifp = stdin;
	}

	data = ReadRaster(ifp, &width1, &height1, colrs1);

	/* Mirror around Y axis, and copy */
	for (i=0; i<MAXCOLORS; i++)
	{
		colrs2[i].red = colrs1[i].red;
		colrs2[i].green = colrs1[i].green;
		colrs2[i].blue = colrs1[i].blue;
	}
	width2 = width1;
	height2 = height1;
	data1 = (unsigned char *)malloc(width1 * height1);
	data2 = (unsigned char *)malloc(width1 * height1);
	ptr1 = data1;
	ptr2 = data2;
	for (j=0; j<height1; j++)
	{
		ptr = (unsigned char *)(data + ((j + 1) * width1) - 1);
		for (i=0; i<width1; i++)
		{
			*ptr1++ = *ptr;
			*ptr2++ = *ptr;
			ptr--;
		}
	}

	fcolrs = MapColors(data1, colrs1, data2, colrs2, width1, height1,&fcnt);

	if (argc > 2)
	{
		outname = argv[2];
	}
	else
	{
		cnt = 0;
		if (argc > 1)
		{
			while ((argv[1][cnt] != '.')&&(argv[1][cnt] != '\0'))
			{
				cnt++;
			}
		}
		if (cnt == 0)
		{
			outname = (char *)malloc(strlen(DEFAULT_NAME) + 1);
			strcpy(outname, DEFAULT_NAME);
		}
		else
		{
			outname = (char *)malloc(cnt + 1);
			strncpy(outname, argv[1], cnt);
			outname[cnt] = '\0';
		}
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
	for (i=0; i<fcnt; i++)
	{
		if (i == (fcnt - 1))
		{
			fprintf(ofd, "%d};\n", fcolrs[i].red);
		}
		else
		{
			fprintf(ofd, "%d, ", fcolrs[i].red);
		}
	}
	fprintf(ofd, "int\t%s_greens[] = {", outname);
	for (i=0; i<fcnt; i++)
	{
		if (i == (fcnt - 1))
		{
			fprintf(ofd, "%d};\n", fcolrs[i].green);
		}
		else
		{
			fprintf(ofd, "%d, ", fcolrs[i].green);
		}
	}
	fprintf(ofd, "int\t%s_blues[] = {", outname);
	for (i=0; i<fcnt; i++)
	{
		if (i == (fcnt - 1))
		{
			fprintf(ofd, "%d};\n", fcolrs[i].blue);
		}
		else
		{
			fprintf(ofd, "%d, ", fcolrs[i].blue);
		}
	}
	fprintf(ofd, "unsigned char\t%s_rasterA[] = {\n", outname);
	for (i=0; i<(width1 * height1); i++)
	{
		fprintf(ofd, "0x%02x,", data1[i]);
	}
	fprintf(ofd, "};\n");
	fprintf(ofd, "unsigned char\t%s_rasterB[] = {\n", outname);
	for (i=0; i<(width1 * height1); i++)
	{
		fprintf(ofd, "0x%02x,", data2[i]);
	}
	fprintf(ofd, "};\n");

	fclose(ofd);

	exit(0);
}


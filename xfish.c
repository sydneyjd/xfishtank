
/*

  *	Original Author Unknow.

  *	8/10/88 - Ported from X10 to X11R3 by:

		Jonathan Greenblatt (jonnyg@rover.umd.edu)

  *	Cleaned up by Dave Lemke (lemke@sun.com)

  *	Ported to monocrome by Jonathan Greenblatt (jonnyg@rover.umd.edu)

  TODO:

	Parameter parsing needs to be redone.

  *	Throughout 1991 improved for animation and color and multiple
  	fish types.  Broke monocrome in the process.
  	Eric Bina (ebina@ncsa.uiuc.edu)

*/

#ifndef HPUX
#include <sys/time.h>
#else
#include <time.h>
#endif

#include <stdio.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "xfishy.h"
#include "bubbles.h"

/* constants are based on rand(3C) returning an integer between 0 and 32767 */

#if defined(ultrix) || defined(sun)
#define  RAND_I_1_16   134217728
#define  RAND_F_1_8    268435455.875
#define  RAND_I_1_4    536870911
#define  RAND_I_1_2   1073741823
#define  RAND_I_3_4   1610612735
#define  RAND_F_MAX   2147483647.0
#else
#define  RAND_I_1_16   2048
#define  RAND_F_1_8    4096.0
#define  RAND_I_1_4    8096
#define  RAND_I_1_2   16384
#define  RAND_I_3_4   24575
#define  RAND_F_MAX   32767.0
#endif

/* externals for pixmap and bimaps from xfishy.h */


/* typedefs for bubble and fish structures, also caddr_t (not used in X.h) */
typedef struct {
    int         x,
                y,
                s,
		erased,
                i;
}           bubble;
typedef struct {
    int         x,
                y,
                d,
                frame,
                type,
                i;
}           fish;
typedef unsigned char *caddrt;


/* bubble increment and yes check tables */
int         binc[] = {0, 64, 56, 48, 40, 32, 24, 16, 8};
char       *yess[] = {"yes", "Yes", "YES", "on", "On", "ON"};


char       *pname,		/* program name from argv[0] */
            sname[64],		/* host:display specification */
            cname[64];		/* colorname specification */
int         blimit = 32,	/* bubble limit */
            flimit = 10,	/* fish limit */
            pmode = 1,		/* pop mode, (1 for lower, 0 for raise) */
            width,		/* width of initial window in pixels */
            height,		/* height of initial window in pixels */
            screen,		/* Default screen of this display */
	    Init_B,
	    *cmap;		/* Initialize bubbles with random y value */
double      rate = 0.2,		/* update interval in seconds */
            smooth = 0.2;	/* smoothness increment multiplier */
bubble     *binfo;		/* bubble info structures, allocated 
				 * dynamically  */
fish       *finfo;		/* fish info structures, allocated dynamically */
Display    *Dpy;
XImage     *xfishA[NUM_FISH][3]; /* fish pixmaps (1 is left-fish, 2 is
				  * right-fish) */
XImage     *xfishB[NUM_FISH][3]; /* fish pixmaps (1 is left-fish, 2 is
				  * right-fish) */
Pixmap      pfishA[NUM_FISH][3];
Pixmap      pfishB[NUM_FISH][3];
Pixmap      xbubbles[9];	/* bubbles bitmaps (1 to 8, by size in pixels)*/
Window      wid;		/* aqaurium window */
unsigned long white, black,bcolor;
Colormap    colormap;
GC          gc,
            bgc;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Output desired error message and exit.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
msgdie(message)
    char       *message;
{
    fprintf(stderr, "%s: %s\n", pname, message);
    exit(1);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Set up program defaults, get X defaults, parse command line using getopts.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
parse(argc, argv)
    int         argc;
    char      **argv;
{
    int         c,
                i;
    char       *p;
    extern int  optind;
    extern char *optarg;
    extern double atof();

    pname = argv[0];
    strcpy(sname, getenv("DISPLAY"));
    strcpy(cname, "MediumAquamarine");

    if ((p = XGetDefault(Dpy, pname, "BubbleLimit")) != NULL)
	blimit = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "Color")) != NULL)
	strcpy(cname, p);
    if ((p = XGetDefault(Dpy, pname, "FishLimit")) != NULL)
	flimit = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "IncMult")) != NULL)
	smooth = atof(p);
    if ((p = XGetDefault(Dpy, pname, "Rate")) != NULL)
	rate = atof(p);
    if ((p = XGetDefault(Dpy, pname, "Secure")) != NULL)
	for (i = 0; i < 6; i++)
	    if (strcmp(p, yess[i]) == 0)
		pmode = 0;

    while ((c = getopt(argc, argv, "b:c:f:i:r:s")) != EOF) {
	switch (c) {
	case 'b':
	    blimit = atoi(optarg);
	    break;
	case 'c':
	    strcpy(cname, optarg);
	    break;
	case 'f':
	    flimit = atoi(optarg);
	    break;
	case 'i':
	    smooth = atof(optarg);
	    break;
	case 'r':
	    rate = atof(optarg);
	    break;
	case 's':
	    pmode = 0;
	    break;
	case '?':
	    fprintf(stderr, "usage: %s [-b limit][-c color][-f limit][-i mult][-r rate][-s][host:display]\n", pname);
	    exit(1);
	}
    }

    if (optind < argc)
	strcpy(sname, argv[optind]);
}

void
putfish(f)
	fish *f;
{
	if (f->frame)
	{
		if (pfishA[f->type][f->d])
		{
			XCopyArea(Dpy, pfishA[f->type][f->d], wid, gc, 0, 0,
				rwidth[f->type], rheight[f->type], f->x, f->y);
		}
		else
		{
			XPutImage(Dpy, wid, gc, xfishA[f->type][f->d], 0, 0,
				f->x, f->y, rwidth[f->type], rheight[f->type]);
		}
		f->frame = 0;
	}
	else
	{
		if (pfishB[f->type][f->d])
		{
			XCopyArea(Dpy, pfishB[f->type][f->d], wid, gc, 0, 0,
				rwidth[f->type], rheight[f->type], f->x, f->y);
		}
		else
		{
			XPutImage(Dpy, wid, gc, xfishB[f->type][f->d], 0, 0,
				f->x, f->y, rwidth[f->type], rheight[f->type]);
		}
		f->frame = 1;
	}
}


putbubble(b, s, c)
    bubble     *b;
    int         s;
    unsigned long c;
{
    XGCValues   gcv;

    gcv.foreground = c;
    gcv.clip_mask = xbubbles[s];
    gcv.clip_x_origin = b->x;
    gcv.clip_y_origin = b->y;
    XChangeGC(Dpy, bgc, GCForeground | GCClipMask | GCClipXOrigin |
	      GCClipYOrigin, &gcv);
    XFillRectangle(Dpy, wid, bgc, b->x, b->y, s, s);
}


/*
 * Find the closest color by allocating it, or picking an already allocated
 * color
 */
void
FindColor(Dpy, colormap, colr)
	Display *Dpy;
	Colormap colormap;
	XColor *colr;
{
	int i, match;
	double rd, gd, bd, dist, mindist;
	int cindx;
	XColor def_colrs[256];
	int NumCells;

	match = XAllocColor(Dpy, colormap, colr);
	if (match == 0)
	{
		NumCells = DisplayCells(Dpy, DefaultScreen(Dpy));
		for (i=0; i<NumCells; i++)
		{
			def_colrs[i].pixel = i;
		}
		XQueryColors(Dpy, colormap, def_colrs, NumCells);
		mindist = 65536.0 * 65536.0;
		cindx = colr->pixel;
		for (i=0; i<NumCells; i++)
		{
			rd = (def_colrs[i].red - colr->red) / 256.0;
			gd = (def_colrs[i].green - colr->green) / 256.0;
			bd = (def_colrs[i].blue - colr->blue) / 256.0;
			dist = (rd * rd * rd * rd) +
				(gd * gd * gd * gd) +
				(bd * bd * bd * bd);
			if (dist < mindist)
			{
				mindist = dist;
				cindx = def_colrs[i].pixel;
			}
		}
		colr->pixel = cindx;
		colr->red = def_colrs[cindx].red;
		colr->green = def_colrs[cindx].green;
		colr->blue = def_colrs[cindx].blue;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Initialize colormap for background color and required fish colors.
The fish colors are coded in xfishy.h as a trio of tables.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
init_colormap()
{
	int i, j, cnt;
	XColor hdef, edef;
	extern char *malloc();

	colormap = XDefaultColormap(Dpy, screen);

	if (colormap == NULL)
	{
		return;
	}

	cnt = 0;
	for (i=0; i<NUM_FISH; i++)
	{
		cnt += rcolors[i];
	}
	cmap = (int *) malloc((cnt + 1) * sizeof(int));

	XLookupColor(Dpy, colormap, cname, &hdef, &edef);
	hdef.flags = DoRed|DoGreen|DoBlue;
	FindColor(Dpy, colormap, &hdef);
	cmap[0] = hdef.pixel;

	cnt = 1;
	for (j=0; j<NUM_FISH; j++)
	{
		int *rp, *gp, *bp;

		rp = rreds[j];
		gp = rgreens[j];
		bp = rblues[j];
		for (i = 0; i < rcolors[j]; i++)
		{
			hdef.red = *rp++;
			hdef.green = *gp++;
			hdef.blue = *bp++;
			hdef.flags = DoRed|DoGreen|DoBlue;
			FindColor(Dpy, colormap, &hdef);
			cmap[cnt] = hdef.pixel;
			if (i == rback[j])
			{
				cmap[cnt] = cmap[0];
			}
			cnt++;
		}
	}

	bcolor = white;
}


/*
 * Make am image of appropriate depth for display from image data.
 */
XImage *
MakeImage(data, width, height)
	unsigned char *data;
	int width, height;
{
	int linepad, shiftnum;
	int shiftstart, shiftstop, shiftinc;
	int bytesperline;
	int depth, temp;
	int w, h;
	XImage *newimage;
	unsigned char *bit_data, *bitp, *datap;

	depth = DefaultDepth(Dpy, DefaultScreen(Dpy));
	if ((depth != 1)&&(depth != 2)&&(depth != 4)&&(depth != 8))
	{
		fprintf(stderr, "Don't know how to format image for display of depth %d\n", depth);
		exit(1);
	}
	if (BitmapBitOrder(Dpy) == LSBFirst)
	{
		shiftstart = 0;
		shiftstop = 8;
		shiftinc = depth;
	}
	else
	{
		shiftstart = 8 - depth;
		shiftstop = -depth;
		shiftinc = -depth;
	}
	linepad = 8 - (width % 8);
	bit_data = (unsigned char *)malloc(((width + linepad) * height) + 1);
	bitp = bit_data;
	datap = data;
	*bitp = 0;
	shiftnum = shiftstart;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = *datap++ << shiftnum;
			*bitp = *bitp | temp;
			shiftnum = shiftnum + shiftinc;
			if (shiftnum == shiftstop)
			{
				shiftnum = shiftstart;
				bitp++;
				*bitp = 0;
			}
		}
		for (w=0; w<linepad; w++)
		{
			shiftnum = shiftnum + shiftinc;
			if (shiftnum == shiftstop)
			{
				shiftnum = shiftstart;
				bitp++;
				*bitp = 0;
			}
		}
	}
	bytesperline = (width + linepad) * depth / 8;
	newimage = XCreateImage(Dpy, DefaultVisual(Dpy, screen), depth,
		ZPixmap, 0, (char *)bit_data,
		(width + linepad), height, 8, bytesperline);

	return(newimage);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Calibrate the pixmaps and bimaps.  The right-fish data is coded in xfishy.h,
this is transformed to create the left-fish.  The eight bubbles are coded
in bubbles.h as a two dimensional array.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
init_pixmap()
{
	register caddrt p, q, x1A, x1B, x2A, x2B;
	register int i, j, k;
	int cnt;
	extern char *malloc();

	cnt = 1;
	for (k=0; k<NUM_FISH; k++)
	{
		j = rwidth[k] * rheight[k];
		x1A = (caddrt) malloc(rwidth[k] * rheight[k]);
		p = (caddrt) xfishRasterA[k];
		q = x1A;
		for (i = 0; i < j; i++)
		{
			*q = cmap[cnt + (int)(*p)];
			p++;
			q++;
		}

		x1B = (caddrt) malloc(rwidth[k] * rheight[k]);
		p = (caddrt) xfishRasterB[k];
		q = x1B;
		for (i = 0; i < j; i++)
		{
			*q = cmap[cnt + (int)(*p)];
			p++;
			q++;
		}

		x2A = (caddrt) malloc(rwidth[k] * rheight[k]);
		for (i = 0; i < rheight[k]; i++)
		{
			p = x1A + i * rwidth[k];
			q = x2A + (i + 1) * rwidth[k] - 1;
			for (j = 0; j < rwidth[k]; j++)
			{
				*q-- = *p++;
			}
		}

		x2B = (caddrt) malloc(rwidth[k] * rheight[k]);
		for (i = 0; i < rheight[k]; i++)
		{
			p = x1B + i * rwidth[k];
			q = x2B + (i + 1) * rwidth[k] - 1;
			for (j = 0; j < rwidth[k]; j++)
			{
				*q-- = *p++;
			}
		}

		xfishA[k][2] = MakeImage(x1A, rwidth[k], rheight[k]);
		free((char *)x1A);
		xfishA[k][1] = MakeImage(x2A, rwidth[k], rheight[k]);
		free((char *)x2A);
		xfishB[k][2] = MakeImage(x1B, rwidth[k], rheight[k]);
		free((char *)x1B);
		xfishB[k][1] = MakeImage(x2B, rwidth[k], rheight[k]);
		free((char *)x2B);

		i = DisplayPlanes(Dpy, screen);

		pfishA[k][1] = XCreatePixmap(Dpy, wid,
			rwidth[k], rheight[k], i);
		pfishA[k][2] = XCreatePixmap(Dpy, wid,
			rwidth[k], rheight[k], i);
		pfishB[k][1] = XCreatePixmap(Dpy, wid,
			rwidth[k], rheight[k], i);
		pfishB[k][2] = XCreatePixmap(Dpy, wid,
			rwidth[k], rheight[k], i);

		if (pfishA[k][1])
		{
			XPutImage(Dpy, pfishA[k][1], gc, xfishA[k][1], 0, 0,
				0, 0, rwidth[k], rheight[k]);
		}
		if (pfishA[k][2])
		{
			XPutImage(Dpy, pfishA[k][2], gc, xfishA[k][2], 0, 0,
				0, 0, rwidth[k], rheight[k]);
		}
		if (pfishB[k][1])
		{
			XPutImage(Dpy, pfishB[k][1], gc, xfishB[k][1], 0, 0,
				0, 0, rwidth[k], rheight[k]);
		}
		if (pfishB[k][2])
		{
			XPutImage(Dpy, pfishB[k][2], gc, xfishB[k][2], 0, 0,
				0, 0, rwidth[k], rheight[k]);
		}

		cnt += rcolors[k];
	}

	for (i = 1; i <= 8; i++)
	{
		xbubbles[i] = XCreateBitmapFromData(Dpy, wid,
			(char *)xbBits[i], i, i);
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Toggle secure mode on receipt of signal
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
toggle_secure()
{
    pmode = !pmode;
    if (pmode)
	XLowerWindow(Dpy, wid);
    else
	XRaiseWindow(Dpy, wid);
    XFlush(Dpy);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Initialize signal so that SIGUSR1 causes secure mode to toggle.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
init_signals()
{
#ifdef MOTOROLA
    sigset(SIGUSR1, toggle_secure);
#else
    struct sigvec vec;

    vec.sv_handler = toggle_secure;
    vec.sv_mask = 0;
    vec.sv_onstack = 0;

#ifndef HPUX
    sigvec(SIGUSR1, &vec, &vec);
#else
    sigvector(SIGUSR1, &vec, &vec);
#endif
#endif /* MOTOROLA */
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Variety of initialization calls, including getting the window up and running.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
initialize()
{
    XWindowAttributes winfo;
    XSetWindowAttributes attr;
    XGCValues   vals;
    extern char *malloc();
    XSizeHints  xsh;
    int i;

    XGetWindowAttributes(Dpy, DefaultRootWindow(Dpy), &winfo);
    width = winfo.width;
    height = winfo.height;

    init_colormap();
    attr.override_redirect = True;
    attr.background_pixel = cmap[0];

    wid = XCreateWindow(Dpy, DefaultRootWindow(Dpy),
	1, 1, width - 2, height - 2, 0,
	CopyFromParent, CopyFromParent, CopyFromParent, 
	CWBackPixel | CWOverrideRedirect, &attr);

    if (!wid)
	msgdie("XCreateWindow failed");

    vals.foreground = vals.background = cmap[0];
    vals.graphics_exposures = False;
    gc = XCreateGC(Dpy, wid, GCForeground | GCBackground | GCGraphicsExposures,
	&vals);
    bgc = XCreateGC(Dpy, wid, GCForeground | GCBackground | GCGraphicsExposures,
	&vals);

    for (i=0; i<NUM_FISH; i++)
    {
	pfishA[i][0] = 0;
	pfishA[i][1] = 0;
	pfishA[i][2] = 0;
	pfishB[i][0] = 0;
	pfishB[i][1] = 0;
	pfishB[i][2] = 0;
    }

    init_pixmap();
    init_signals();

    XStoreName(Dpy, wid, pname);

    xsh.flags = USSize | USPosition | PPosition | PSize;
    xsh.x = xsh.y = 0;
    xsh.width = width;
    xsh.height = height;
    XSetNormalHints(Dpy, wid, &xsh);

    XMapWindow(Dpy, wid);

    binfo = (bubble *) malloc(blimit * sizeof(bubble));
    finfo = (fish *) malloc(flimit * sizeof(fish));
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Create a new bubble.  Placement along the x axis is random, as is the size of
the bubble.  Increment value is determined by speed.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
new_bubble(b0)
    bubble     *b0;
{
    register int s;
    register bubble *b = b0;

    b->x = width * (rand() / RAND_F_MAX);
    if (Init_B)
	b->y = (height / 16) * (rand() / RAND_I_1_16 + 1) - 1;
    else
	b->y = height - 1;
    b->s = s = 1.0 + rand() / RAND_F_1_8;
    if ((b->i = smooth * height / (float) binc[s]) == 0)
	b->i = 1;
    b->erased = 0;
    putbubble(b, s, bcolor);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Erase old bubbles, move and draw new bubbles.  Random left-right factor
can move bubble one size-unit in either direction.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
step_bubbles()
{
	register int i, j, s;
	register bubble *b;

	for (i = 0; i < blimit; i++)
	{
		b = &binfo[i];
		s = b->s;
		/* clear */
		if ((b->y > 0)&&(b->erased == 0))
		{
			putbubble(b, s, cmap[0]);
		}
		if ((b->y -= b->i) > 0)
		{
			j = rand();
			if (j < RAND_I_1_4)
			{
				b->x -= s;
			}
			else if (j > RAND_I_3_4)
			{
				b->x += s;
			}
			putbubble(b, s, bcolor);
		}
		else
		{
			if (rand() < RAND_I_1_4)
			{
				new_bubble(b);
			}
		}
		b->erased = 0;
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Fish over bubble collision detection.  The specified fish is checked against
all bubbles for overlap.  This way we don't try and erase bubbles that are
already gone.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
collide_bubbles(f0, ofx, ofy)
	fish *f0;
	int ofx, ofy;
{
	int i, delta;
	register fish *f = f0;
	register bubble *b;

	for (i = 0; i < blimit; i++)
	{
		b = &binfo[i];
		delta = b->x - ofx;
		if ((delta >= 0)&&(delta <= (rwidth[f->type] - b->s)))
		{
			delta = b->y - ofy;
			if ((delta >= 0)&&(delta <= (rheight[f->type] - b->s)))
			{
				b->erased = 1;
				continue;
			}
		}
		delta = b->x - f->x;
		if ((delta >= 0)&&(delta <= (rwidth[f->type] - b->s)))
		{
			delta = b->y - f->y;
			if ((delta >= 0)&&(delta <= (rheight[f->type] - b->s)))
			{
				b->erased = 1;
				continue;
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Fish collision detection.  The specified fish is checked against all other
fish for overlap.  The xt parameter specifies a x axis multiplier for overlap.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int 
collide_fish(f0, xt)
	fish *f0;
	int xt;
{
	int i, j;
	register fish *f = f0;

	for (i = 0; i < flimit; i++)
	{
		if (&finfo[i] != f)
		{
			j = finfo[i].y - f->y;
			if ((j > -rheight[finfo[i].type]) &&
			    (j < rheight[f->type]))
			{
				j = finfo[i].x - f->x;
				if ((j > -xt * rwidth[finfo[i].type]) &&
				    (j < xt * rwidth[f->type]))
				{
					return (1);
				}
			}
		}
	}
	return (0);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Create a new fish.   Placement along the y axis is random, as is the side
>from which the fish appears.  Direction is determined from side.  Increment
is also random.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
new_fish(f0)
	fish *f0;
{
	int i, collide;
	fish *f = f0;

	f->type = rand() % NUM_FISH;
	for (i = 0, collide = 1; (i < 16) && (collide); i++)
	{
		f->y = (height - rheight[f->type]) * (rand() / RAND_F_MAX);
		if ((f->i = smooth * width /
			(8.0 * (1.0 + rand() / RAND_F_1_8))) == 0)
		{
			f->i = 1;
		}
		if (rand() < RAND_I_1_2)
		{
			f->d = 1;
			f->x = width;
		}
		else
		{
			f->d = 2;
			f->x = -rwidth[f->type];
		}
		collide = collide_fish(f, 2);
	}

	if (!collide)
	{
		putfish(f);
	}
	else
	{
		f->d = 0;
	}

	f->frame = 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Move all the fish.  Clearing old fish is accomplished by masking only the
exposed areas of the old fish.  Random up-down factor can move fish 1/4 a
fish height in either direction, if no collisions are caused.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
move_fish()
{
	register int i, j, x, y, ofx, ofy, done;
	register fish *f;

	for (i = 0; i < flimit; i++)
	{
		f = &finfo[i];
		if (f->d)
		{
			ofx = f->x;
			ofy = f->y;

			if (f->d == 1)
			{
				done = ((f->x -= f->i) < -rwidth[f->type]);
				x = f->x + rwidth[f->type];
			}
			else if (f->d == 2)
			{
				done = ((f->x += f->i) > width);
				x = f->x - f->i;
			}

			if (!collide_fish(f, 1))
			{
				if (!done)
				{
					j = rand();
					if (j < RAND_I_1_4)
					{
						y = f->i / 4;
					}
					else if (j > RAND_I_3_4)
					{
						y = f->i / -4;
					}
					else
					{
						y = 0;
					}

					if (y)
					{
						f->y += y;
						if (collide_fish(f, 1))
						{
							f->y -= y;
							y = 0;
						}
						else
						{
							if (y > 0)
							{
								j = f->y - y;
							}
							else
							{
								j = f->y +
								   rheight
								      [f->type];
								y *= -1;
							}
						}
					}
					putfish(f);
					XClearArea(Dpy, wid, x, ofy,
						f->i, rheight[f->type], 0);
					if (y)
					{
						XClearArea(Dpy, wid, ofx, j,
							rwidth[f->type], y, 0);
					}
				}
				else
				{
					XClearArea(Dpy, wid, x, f->y,
						f->i, rheight[f->type], 0);
					new_fish(f);
				}
			}
			else
			{
				if ((f->d = 3 - f->d) == 1)
				{
					f->x = f->x - 2 * f->i;
					x = f->x + rwidth[f->type];
				}
				else
				{
					f->x = f->x + 2 * f->i;
					x = f->x - f->i;
				}
				putfish(f);
				XClearArea(Dpy, wid, x, f->y,
					f->i, rheight[f->type], 0);
			}
			collide_bubbles(f, ofx, ofy);
		}
		else
		{
			new_fish(f);
		}
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Higher-resolution sleep
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
high_res_sleep(seconds)
    double      seconds;
{
    int         fds = 0;
    struct timeval timeout;

    timeout.tv_sec = seconds;
    timeout.tv_usec = (seconds - timeout.tv_sec) * 1000000.0;
    select(0, &fds, &fds, &fds, &timeout);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
main(argc, argv)
    int         argc;
    char      **argv;
{
    int         i;
    XEvent      ev;

    if ((Dpy = XOpenDisplay("")) == 0)
	msgdie("XOpenDisplay failed");
    screen = DefaultScreen(Dpy);

    white = WhitePixel(Dpy, screen);
    black = BlackPixel(Dpy, screen);
    parse(argc, argv);
    initialize();

    srand((unsigned) getpid());

    Init_B = 1;
    for (i = 0; i < blimit; i++)
	new_bubble(&binfo[i]);
    for (i = 0; i < flimit; i++)
    {
	finfo[i].x = 0;
	finfo[i].y = 0;
	finfo[i].type = 0;
    }
    for (i = 0; i < flimit; i++)
	new_fish(&finfo[i]);
    if (pmode)
	XLowerWindow(Dpy, wid);
    else
	XRaiseWindow(Dpy, wid);
    XFlush(Dpy);

    Init_B = 0;

    for (;;) {
	if (XPending(Dpy))
	    XNextEvent(Dpy, &ev);

	high_res_sleep(rate);

	move_fish();

	step_bubbles();

	if (pmode)
	    XLowerWindow(Dpy, wid);
	else
	    XRaiseWindow(Dpy, wid);
    }
}



/*

  *	Original Author Unknow.

  *	8/10/88 - Ported from X10 to X11R3 by:

		Jonathan Greenblatt (jonnyg@rover.umd.edu)

  *	Cleaned up by Dave Lemke (lemke@sun.com)

  *	Ported to monocrome by Jonathan Greenblatt (jonnyg@rover.umd.edu)

  *     05/02/1996 Added TrueColor support by TJ Phan (phan@aur.alcatel.com)

  TODO:

	Parameter parsing needs to be redone.

  *	Throughout 1991 improved for animation and color and multiple
  	fish types.  Broke monocrome in the process.
  	Eric Bina (ebina@ncsa.uiuc.edu)

  *	1992 added extra color remapping control options, as well as ways
	to let the fish swim on the root window, or an image of the users
	choice.  Eric Bina (ebina@ncsa.uiuc.edu)

*/

#include <sys/types.h>
#ifndef hpux
#include <sys/time.h>
#else
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#ifdef sgi
#define _BSD_SIGNALS
#endif
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "vroot.h"
#include "xfishy.h"
#include "bubbles.h"
#include "medcut.h"

/* constants are based on rand(3C) returning an integer between 0 and 32767 */

#if defined(ultrix) || defined(sun) || defined(linux)
#define  RAND_I_1_16   134217728
#define  RAND_F_1_8    268435455.875
#define  RAND_I_1_4    536870911
#define  RAND_I_1_2   1073741823
#define  RAND_I_3_4   1610612735
#define  RAND_F_MAX   2147483647.0
#elif defined(__FreeBSD__) || defined(__OpenBSD__)
#define  RAND_I_1_16   (RAND_MAX>>4)
#define  RAND_F_1_8    ((float)(RAND_MAX>>3))
#define  RAND_I_1_4    (RAND_MAX>>2)
#define  RAND_I_1_2    (RAND_MAX>>1)
#define  RAND_I_3_4    ((RAND_MAX>>2)*3)
#define  RAND_F_MAX    ((float)RAND_MAX)
#else
#define  RAND_I_1_16   2048
#define  RAND_F_1_8    4096.0
#define  RAND_I_1_4    8096
#define  RAND_I_1_2   16384
#define  RAND_I_3_4   24575
#define  RAND_F_MAX   32767.0
#endif


extern unsigned char *ReadBitmap();


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
char	    picname[256];	/* name of the background picture file */
int         *Allocated;		/* mark the used colors */
int         AllocCnt;		/* count number of colors used */
int         mlimit = 0;		/* num colors to median cut to. 0 = no limit */
int         climit = 0;		/* limit on color use. 0 = no limit */
int         DoubleBuf = 0;	/* Should we use double buffering */
int         Overlap = 0;	/* Should fish swim over each other */
int         DoClipping = 0;	/* Should clip masks be used. */
int         blimit = 32,	/* bubble limit */
            flimit = 10,	/* fish limit */
            pmode = 1,		/* pop mode, (1 for lower, 0 for raise) */
            width,		/* width of initial window in pixels */
            height,		/* height of initial window in pixels */
            screen,		/* Default screen of this display */
	    Init_B,
	    *cmap;		/* Initialize bubbles with random y value */
int	    Pwidth;		/* width of background picture */
int	    Pheight;		/* height of background picture */
int	    Pcnt;		/* number of colors in background picture */
unsigned char *Pdata;		/* data from background picture */
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

Pixmap      mfishA[NUM_FISH][3]; /* masking pixmaps for fish to use as */
Pixmap      mfishB[NUM_FISH][3]; /*  clipmasks */

Pixmap      PicMap;		/* pixmap for background picture */

Pixmap      PixBuf;		/* Pixmap buffer for double buffering */
Pixmap      ClipBuf;		/* Clipmask buffer for double buffering */

Pixmap      xbubbles[9];	/* bubbles bitmaps (1 to 8, by size in pixels)*/
Window      wid;		/* aqaurium window */
unsigned long white, black,bcolor;
Colormap    colormap;
GC          c0gc, cpgc;		/* GCs to operateon the Clipmask buffer */
GC          pgc;
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
    extern int  optind;
    extern char *optarg;
    extern double atof();

    pname = argv[0];
    strncpy(sname, getenv("DISPLAY"),sizeof(sname)-1);
    strcpy(cname, "MediumAquamarine");

    while ((c = getopt(argc, argv, "dDob:C:c:p:m:f:i:r:s")) != EOF) {
	switch (c) {
	case 'd':
	    DoClipping = 1;
	    break;
	case 'D':
	    DoubleBuf = 1;
	    break;
	case 'o':
	    Overlap = 1;
	    break;
	case 'b':
	    blimit = atoi(optarg);
	    break;
	case 'C':
	    climit = atoi(optarg);
	    break;
	case 'm':
	    mlimit = atoi(optarg);
	    break;
	case 'c':
	    strncpy(cname, optarg,sizeof(cname)-1);
	    break;
	case 'p':
	    strncpy(picname, optarg,sizeof(picname)-1);
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
	    fprintf(stderr, "usage: %s\n", pname);
	    fprintf(stderr, "\t\t[-c color]  background color\n");
	    fprintf(stderr, "\t\t[-b limit]  number of bubbles (default 32)\n");
	    fprintf(stderr, "\t\t[-f limit]  number of fish (default 10)\n");
	    fprintf(stderr, "\t\t[-i mult]   move interval (default 0.2)\n");
	    fprintf(stderr, "\t\t[-r rate]   move frequency (default 0.2)\n");
	    fprintf(stderr, "\t\t[-m num]    median cut to this many colors\n");
	    fprintf(stderr, "\t\t[-C num]    use only this many color cells\n");
	    fprintf(stderr, "\t\t[-d]        clip fish, swim on root window\n");
	    fprintf(stderr, "\t\t[-p file]   fish swim on picture in file\n");
	    fprintf(stderr, "\t\t[host:display]\n");
	    exit(1);
	}
    }

    if (optind < argc) {
        char *display;
	strncpy(sname, argv[optind], sizeof(sname)-1);
	display = (char *)malloc(strlen(sname) + 9);
	snprintf(display,sizeof(display)-1, "DISPLAY=%s", sname);
	putenv(display);
    }

}


void
erasefish(f, x, y, d)
	fish *f;
	int x, y, d;
{
	/*
	 * for something as small as a bubble, it was never worth the
	 * effort of using clipmasks to only turn of the bubble itself, so
	 * we just clear the whole rectangle.
	 */
	XClearArea(Dpy, wid, x, y, rwidth[f->type], rheight[f->type], False);
/*
	XGCValues   gcv;

	if (f->frame)
	{
		gcv.foreground = cmap[0];
		gcv.fill_style = FillTiled;
		gcv.fill_style = FillSolid;
		gcv.tile = pfishB[f->type][d];
		gcv.ts_x_origin = f->x;
		gcv.ts_y_origin = f->y;
		gcv.clip_mask = mfishB[f->type][d];
		gcv.clip_x_origin = x;
		gcv.clip_y_origin = y;
		XChangeGC(Dpy, gc, GCForeground | GCClipMask |
			GCTile | GCTileStipXOrigin | GCTileStipYOrigin |
			GCFillStyle | GCClipXOrigin | GCClipYOrigin,
			&gcv);
		XCopyPlane(Dpy, mfishB[f->type][d], wid, gc, 0, 0,
			rwidth[f->type], rheight[f->type],
			x, y, (unsigned long)1);
	}
	else
	{
		gcv.foreground = cmap[0];
		gcv.fill_style = FillTiled;
		gcv.fill_style = FillSolid;
		gcv.tile = pfishA[f->type][d];
		gcv.ts_x_origin = f->x;
		gcv.ts_y_origin = f->y;
		gcv.clip_mask = mfishA[f->type][d];
		gcv.clip_x_origin = x;
		gcv.clip_y_origin = y;
		XChangeGC(Dpy, gc, GCForeground | GCClipMask |
			GCTile | GCTileStipXOrigin | GCTileStipYOrigin |
			GCFillStyle | GCClipXOrigin | GCClipYOrigin,
			&gcv);
		XCopyPlane(Dpy, mfishA[f->type][d], wid, gc, 0, 0,
			rwidth[f->type], rheight[f->type],
			x, y, (unsigned long)1);
	}
*/
}


/*
 * Just places a fish.  Normally this is all you need for animation, since
 * placeing the fish places an entire rectangle which erases most of the old
 * fish (the rest being cleaned up by the function that called putfish.
 * If DoClipping is set, this function is only called when placing a new
 * fish, otherwise newfish is called.
 */
void
putfish(f)
	fish *f;
{
	XGCValues   gcv;

	if (f->frame)
	{
		/*
		 * If we have a pixmap of the fish use it, otherwise use
		 * the XImage of the fish.  In reality we will never use
		 * the XImage since X dies if the pixmap create failed
		 */
		if (pfishA[f->type][f->d])
		{
			/*
			 * Clipping overrides background picture because
			 * the clipping prevents the drawing of any background
			 * anyway.
			 * DoClipping says just print a fish leaving the
			 * background unchanged.
			 * If there is a background picture, we use a buffer
			 * to prevent flashing, we combine the background
			 * picture and the fish, and then copy the
			 * whole rectangle in.
			 * Default is just copy in fish in with a background
			 * color.
			 */
			if (DoClipping)
			{
				gcv.clip_mask = mfishA[f->type][f->d];
				gcv.clip_x_origin = f->x;
				gcv.clip_y_origin = f->y;
				XChangeGC(Dpy, gc,
				    GCClipMask | GCClipXOrigin | GCClipYOrigin,
				    &gcv);
				XCopyArea(Dpy, pfishA[f->type][f->d], wid, gc,
					0, 0, rwidth[f->type], rheight[f->type],
					f->x, f->y);
			}
			else if (picname[0] != '\0')
			{
				gcv.fill_style = FillTiled;
				gcv.tile = PicMap;
				gcv.ts_x_origin = -(f->x);
				gcv.ts_y_origin = -(f->y);
				gcv.clip_mask = None;
				XChangeGC(Dpy, pgc, (GCFillStyle |
					GCTile | GCTileStipXOrigin |
					GCTileStipYOrigin | GCClipMask),
					&gcv);
				XFillRectangle(Dpy, PixBuf, pgc, 0, 0,
					rwidth[f->type], rheight[f->type]);

				gcv.clip_mask = mfishA[f->type][f->d];
				gcv.clip_x_origin = 0;
				gcv.clip_y_origin = 0;
				XChangeGC(Dpy, pgc,
				    GCClipMask | GCClipXOrigin | GCClipYOrigin,
				    &gcv);
				XCopyArea(Dpy, pfishA[f->type][f->d],PixBuf,pgc,
					0, 0, rwidth[f->type], rheight[f->type],
					0, 0);

				XCopyArea(Dpy, PixBuf, wid, gc,
					0, 0, rwidth[f->type], rheight[f->type],
					f->x, f->y);
			}
			else
			{
				XCopyArea(Dpy, pfishA[f->type][f->d], wid, gc,
					0, 0, rwidth[f->type], rheight[f->type],
					f->x, f->y);
			}
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
		/*
		 * same as the above, only for the second frame of animation
		 */
		if (pfishB[f->type][f->d])
		{
			if (DoClipping)
			{
				gcv.clip_mask = mfishB[f->type][f->d];
				gcv.clip_x_origin = f->x;
				gcv.clip_y_origin = f->y;
				XChangeGC(Dpy, gc,
				    GCClipMask | GCClipXOrigin | GCClipYOrigin,
				    &gcv);
				XCopyArea(Dpy, pfishB[f->type][f->d], wid, gc,
					0, 0, rwidth[f->type], rheight[f->type],
					f->x, f->y);
			}
			else if (picname[0] != '\0')
			{
				gcv.fill_style = FillTiled;
				gcv.tile = PicMap;
				gcv.ts_x_origin = -(f->x);
				gcv.ts_y_origin = -(f->y);
				gcv.clip_mask = None;
				XChangeGC(Dpy, pgc, (GCFillStyle |
					GCTile | GCTileStipXOrigin |
					GCTileStipYOrigin | GCClipMask),
					&gcv);
				XFillRectangle(Dpy, PixBuf, pgc, 0, 0,
					rwidth[f->type], rheight[f->type]);

				gcv.clip_mask = mfishB[f->type][f->d];
				gcv.clip_x_origin = 0;
				gcv.clip_y_origin = 0;
				XChangeGC(Dpy, pgc,
				    GCClipMask | GCClipXOrigin | GCClipYOrigin,
				    &gcv);
				XCopyArea(Dpy, pfishB[f->type][f->d],PixBuf,pgc,
					0, 0, rwidth[f->type], rheight[f->type],
					0, 0);

				XCopyArea(Dpy, PixBuf, wid, gc,
					0, 0, rwidth[f->type], rheight[f->type],
					f->x, f->y);
			}
			else
			{
				XCopyArea(Dpy, pfishB[f->type][f->d], wid, gc,
					0, 0, rwidth[f->type], rheight[f->type],
					f->x, f->y);
			}
		}
		else
		{
			XPutImage(Dpy, wid, gc, xfishB[f->type][f->d], 0, 0,
				f->x, f->y, rwidth[f->type], rheight[f->type]);
		}
		f->frame = 1;
	}
}


/*
 * This function can only be called if DoClipping is True.  It is used to
 * move a clipmasked fish.  First the area under the fish is cleared,
 * and then the new fish is masked in.
 * The parameters x, y, amd d are from the old fish that is being
 * erased before the new fish is drawn.
 */
void
movefish(f, x, y, d)
	fish *f;
	int x, y, d;
{
	XGCValues   gcv;
	int bx, by, bw, bh;

	/*
	 * If we are going to double buffer, we need to find the bounding
	 * rectangle of the overlap of the bounding rectangles of the old
	 * and the new fish.
	 */
	if (DoubleBuf)
	{
		if (x < f->x)
		{
			bx = x;
			bw = f->x - x + rwidth[f->type];
		}
		else
		{
			bx = f->x;
			bw = x - f->x + rwidth[f->type];
		}
		if (y < f->y)
		{
			by = y;
			bh = f->y - y +rheight[f->type];
		}
		else
		{
			by = f->y;
			bh = y - f->y +rheight[f->type];
		}
	}

	if (f->frame)
	{
		/*
		 * If there is a pixmap use it.
		 * This branchis always taken since right now, if the pixmap
		 * allocation failed, the program dies.
		 */
		if (pfishA[f->type][f->d])
		{
			/*
			 * A pointless if, you now only come here if
			 * DoClipping is set, I've just been too lazy to
			 * clean up my code.
			 */
			if (DoClipping)
			{
				/*
				 * Set up the masked gc for when we eventually
				 * draw the fish.  Origin is different for
				 * whether we are drawing into the buffer
				 * or into the window
				 */
				gcv.clip_mask = mfishA[f->type][f->d];
				if (DoubleBuf)
				{
					gcv.clip_x_origin = f->x - bx;
					gcv.clip_y_origin = f->y - by;
				}
				else
				{
					gcv.clip_x_origin = f->x;
					gcv.clip_y_origin = f->y;
				}
				XChangeGC(Dpy, gc,
				    GCClipMask | GCClipXOrigin | GCClipYOrigin,
				    &gcv);

				/*
				 * If we have a background picture we want to
				 * clear to that background, otherwise we just
				 * do an XCleararea, and let the root restore
				 * the background.
				 */
				if (picname[0] != '\0')
				{
					gcv.fill_style = FillTiled;
					gcv.tile = PicMap;
					gcv.clip_mask = mfishB[f->type][d];
				    if (DoubleBuf)
				    {
					gcv.ts_x_origin = 0 - bx;
					gcv.ts_y_origin = 0 - by;
					gcv.clip_x_origin = x - bx;
					gcv.clip_y_origin = y - by;
				    }
				    else
				    {
					gcv.ts_x_origin = 0;
					gcv.ts_y_origin = 0;
					gcv.clip_x_origin = x;
					gcv.clip_y_origin = y;
				    }
					XChangeGC(Dpy, pgc, (GCFillStyle |
						GCTile | GCTileStipXOrigin |
						GCTileStipYOrigin | GCClipMask |
						GCClipXOrigin | GCClipYOrigin),
						&gcv);

				    /*
				     * if bouble buffering we clear the buffer
				     * to the backgound picture, and then
				     * shape the clip buffer to the shape of
				     * the fish being erased.
				     */
				    if (DoubleBuf)
				    {
					XFillRectangle(Dpy, PixBuf, pgc,
						x - bx, y - by,
						rwidth[f->type],
						rheight[f->type]);
					XFillRectangle(Dpy, ClipBuf, c0gc, 0, 0,
						500, 500);
					XCopyArea(Dpy, mfishB[f->type][d],
						ClipBuf, cpgc, 0, 0,
						rwidth[f->type],
						rheight[f->type],
						x - bx, y - by);
				    }
				    else
				    {
					XFillRectangle(Dpy, wid, pgc, x, y,
						rwidth[f->type],
						rheight[f->type]);
				    }
				}
				else
				{
				    XClearArea(Dpy, wid, x, y,
					rwidth[f->type], rheight[f->type], 0);
				}
			}
		    /*
		     * Now we just copy in the new fish with a clipmasked gc.
		     * But if we doublebuffered, we copy the new fish into
		     * the buffer, combine the new fishes clipmask in, and
		     * then mask the whole lot from the buffer to the window.
		     */
		    if (DoubleBuf)
		    {
			XCopyArea(Dpy, pfishA[f->type][f->d], PixBuf, gc, 0, 0,
				rwidth[f->type], rheight[f->type],
				f->x - bx, f->y - by);
			XCopyArea(Dpy, mfishA[f->type][f->d], ClipBuf, cpgc,
				0, 0, rwidth[f->type], rheight[f->type],
				f->x - bx, f->y - by);
			gcv.clip_mask = ClipBuf;
			gcv.clip_x_origin = bx;
			gcv.clip_y_origin = by;
			XChangeGC(Dpy, gc,
			    GCClipMask | GCClipXOrigin | GCClipYOrigin,
			    &gcv);
			XCopyArea(Dpy, PixBuf, wid, gc, 0, 0, bw, bh, bx, by);
		    }
		    else
		    {
			XCopyArea(Dpy, pfishA[f->type][f->d], wid, gc, 0, 0,
				rwidth[f->type], rheight[f->type], f->x, f->y);
		    }
		}
		else
		{
			if (DoClipping)
			{
				if (picname[0] != '\0')
				{
					gcv.fill_style = FillTiled;
					gcv.tile = PicMap;
					gcv.ts_x_origin = 0;
					gcv.ts_y_origin = 0;
					gcv.clip_mask = mfishB[f->type][d];
					gcv.clip_x_origin = x;
					gcv.clip_y_origin = y;
					XChangeGC(Dpy, pgc, (GCFillStyle |
						GCTile | GCTileStipXOrigin |
						GCTileStipYOrigin | GCClipMask |
						GCClipXOrigin | GCClipYOrigin),
						&gcv);
					XFillRectangle(Dpy, wid, pgc, x, y,
						rwidth[f->type],
						rheight[f->type]);
				}
				else
				{
				    XClearArea(Dpy, wid, x, y,
					rwidth[f->type], rheight[f->type], 0);
				}
			}
			XPutImage(Dpy, wid, gc, xfishA[f->type][f->d], 0, 0,
				f->x, f->y, rwidth[f->type], rheight[f->type]);
		}
		f->frame = 0;
	}
	else
	{
		/*
		 * Same as above, only for the second frame of animation.
		 */
		if (pfishB[f->type][f->d])
		{
			if (DoClipping)
			{
				gcv.clip_mask = mfishB[f->type][f->d];
				if (DoubleBuf)
				{
					gcv.clip_x_origin = f->x - bx;
					gcv.clip_y_origin = f->y - by;
				}
				else
				{
					gcv.clip_x_origin = f->x;
					gcv.clip_y_origin = f->y;
				}
				XChangeGC(Dpy, gc,
				    GCClipMask | GCClipXOrigin | GCClipYOrigin,
				    &gcv);
				if (picname[0] != '\0')
				{
					gcv.fill_style = FillTiled;
					gcv.tile = PicMap;
					gcv.clip_mask = mfishA[f->type][d];
				    if (DoubleBuf)
				    {
					gcv.ts_x_origin = 0 - bx;
					gcv.ts_y_origin = 0 - by;
					gcv.clip_x_origin = x - bx;
					gcv.clip_y_origin = y - by;
				    }
				    else
				    {
					gcv.ts_x_origin = 0;
					gcv.ts_y_origin = 0;
					gcv.clip_x_origin = x;
					gcv.clip_y_origin = y;
				    }
					XChangeGC(Dpy, pgc, (GCFillStyle |
						GCTile | GCTileStipXOrigin |
						GCTileStipYOrigin | GCClipMask |
						GCClipXOrigin | GCClipYOrigin),
						&gcv);
				    if (DoubleBuf)
				    {
					XFillRectangle(Dpy, PixBuf, pgc,
						x - bx, y - by,
						rwidth[f->type],
						rheight[f->type]);
					XFillRectangle(Dpy, ClipBuf, c0gc, 0, 0,
						500, 500);
					XCopyArea(Dpy, mfishA[f->type][d],
						ClipBuf, cpgc, 0, 0,
						rwidth[f->type],
						rheight[f->type],
						x - bx, y - by);
				    }
				    else
				    {
					XFillRectangle(Dpy, wid, pgc, x, y,
						rwidth[f->type],
						rheight[f->type]);
				    }
				}
				else
				{
				    XClearArea(Dpy, wid, x, y,
					rwidth[f->type], rheight[f->type], 0);
				}
			}
		    if (DoubleBuf)
		    {
			XCopyArea(Dpy, pfishB[f->type][f->d], PixBuf, gc, 0, 0,
				rwidth[f->type], rheight[f->type],
				f->x - bx, f->y - by);
			XCopyArea(Dpy, mfishB[f->type][f->d], ClipBuf, cpgc,
				0, 0, rwidth[f->type], rheight[f->type],
				f->x - bx, f->y - by);
			gcv.clip_mask = ClipBuf;
			gcv.clip_x_origin = bx;
			gcv.clip_y_origin = by;
			XChangeGC(Dpy, gc,
			    GCClipMask | GCClipXOrigin | GCClipYOrigin,
			    &gcv);
			XCopyArea(Dpy, PixBuf, wid, gc, 0, 0, bw, bh, bx, by);
		    }
		    else
		    {
			XCopyArea(Dpy, pfishB[f->type][f->d], wid, gc, 0, 0,
				rwidth[f->type], rheight[f->type], f->x, f->y);
		    }
		}
		else
		{
			if (DoClipping)
			{
				if (picname[0] != '\0')
				{
					gcv.fill_style = FillTiled;
					gcv.tile = PicMap;
					gcv.ts_x_origin = 0;
					gcv.ts_y_origin = 0;
					gcv.clip_mask = mfishA[f->type][d];
					gcv.clip_x_origin = x;
					gcv.clip_y_origin = y;
					XChangeGC(Dpy, pgc, (GCFillStyle |
						GCTile | GCTileStipXOrigin |
						GCTileStipYOrigin | GCClipMask |
						GCClipXOrigin | GCClipYOrigin),
						&gcv);
					XFillRectangle(Dpy, wid, pgc, x, y,
						rwidth[f->type],
						rheight[f->type]);
				}
				else
				{
				    XClearArea(Dpy, wid, x, y,
					rwidth[f->type], rheight[f->type], 0);
				}
			}
			XPutImage(Dpy, wid, gc, xfishB[f->type][f->d], 0, 0,
				f->x, f->y, rwidth[f->type], rheight[f->type]);
		}
		f->frame = 1;
	}
}


erasebubble(b, s)
    bubble     *b;
    int         s;
{
	XClearArea(Dpy, wid, b->x, b->y, s, s, 0);
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
Visual (*visual_info) = NULL;
int r_mask, g_mask, b_mask;
int r_shift=0, g_shift=0, b_shift=0;
int r_bits=0, g_bits=0, b_bits=0;
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

	if( visual_info == NULL &&  DefaultDepth(Dpy, DefaultScreen(Dpy)) > 8 )
	{
	   visual_info = DefaultVisual(Dpy, DefaultScreen(Dpy));
	   r_mask = visual_info->red_mask;
	   while( !(r_mask & 1) )
	   {
	      r_mask >>= 1;
	      r_shift++;
	   }
	   while( r_mask & 1 )
	   {
	      r_mask >>= 1;
	      r_bits++;
	   }

	   g_mask = visual_info->green_mask;
	   while( !(g_mask & 1) )
	   {
	      g_mask >>= 1;
	      g_shift++;
	   }
	   while( g_mask & 1 )
	   {
	      g_mask >>= 1;
	      g_bits++;
	   }

	   b_mask = visual_info->blue_mask;
	   while( !(b_mask &1) )
	   {
	      b_mask >>= 1;
	      b_shift++;
	   }
	   while( b_mask & 1 )
	   {
	      b_mask >>= 1;
	      b_bits++;
	   }
	}

	if( DefaultDepth(Dpy, DefaultScreen(Dpy)) > 8 )
	{
	   colr->red >>= 16 - r_bits;
	   colr->green >>= 16 - g_bits;
	   colr->blue >>= 16 - b_bits;

	   colr->pixel = ((colr->red << r_shift) & visual_info->red_mask) |
	      ((colr->green << g_shift) & visual_info->green_mask) |
	      ((colr->blue << b_shift) & visual_info->blue_mask);
	   return;
	}

	if (AllocCnt < climit)
	{
		match = XAllocColor(Dpy, colormap, colr);
	}
	else
	{
		match = 0;
	}
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
	else
	{
		if (Allocated[colr->pixel] == 0)
		{
			Allocated[colr->pixel] = 1;
			AllocCnt++;
		}
	}
}


int
ColorUsage(data, width, height, colrs)
	unsigned char *data;
	int width, height;
	struct colr_data *colrs;
{
	int mapping[256];
	int i, size;
	int cnt, indx;
	unsigned char *ptr;
	struct colr_data newcol[256];

	for (i=0; i<256; i++)
	{
		mapping[i] = -1;
	}

	size = width * height;
	cnt = 0;
	ptr = data;
	for (i=0; i<size; i++)
	{
		indx = (int)*ptr;
		if (mapping[indx] == -1)
		{
			mapping[indx] = cnt;
			newcol[cnt].red = colrs[indx].red;
			newcol[cnt].green = colrs[indx].green;
			newcol[cnt].blue = colrs[indx].blue;
			cnt++;
		}
		ptr++;
	}

	ptr = data;
	for (i=0; i<size; i++)
	{
		indx = (int)*ptr;
		*ptr = (unsigned char)mapping[indx];
		ptr++;
	}

	for (i=0; i<cnt; i++)
	{
		colrs[i].red = newcol[i].red;
		colrs[i].green = newcol[i].green;
		colrs[i].blue = newcol[i].blue;
	}
	for (i=cnt; i<256; i++)
	{
		colrs[i].red = 0;
		colrs[i].green = 0;
		colrs[i].blue = 0;
	}

	return(cnt);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Initialize colormap for background color and required fish colors.
The fish colors are coded in xfishy.h as a trio of tables.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
init_colormap()
{
	FILE *fp;
	int i, j, cnt;
	int NumCells;
	XColor hdef, edef;
	struct colr_data *cdp;
	struct colr_data colrs[256];

	colormap = XDefaultColormap(Dpy, screen);

	NumCells = DisplayCells(Dpy, DefaultScreen(Dpy));
	Allocated = (int *)malloc(NumCells * sizeof(int));
	for (i=0; i<NumCells; i++)
	{
		Allocated[i] = 0;
	}
	AllocCnt = 0;
	if ((climit <= 0)||(climit > NumCells))
	{
		climit = NumCells;
	}

	Pcnt = 0;
	if (picname[0] != '\0')
	{
		fp = fopen(picname, "r");
		if (fp == NULL)
		{
			fprintf(stderr, "Cannot open picture %s for reading\n",
				picname);
		}
		else
		{
			Pdata = ReadBitmap(fp, &Pwidth, &Pheight, colrs);
			fclose(fp);
			Pcnt = ColorUsage(Pdata, Pwidth, Pheight, colrs);
		}
	}

	cnt = 0;
	cnt += Pcnt;
	for (i=0; i<NUM_FISH; i++)
	{
		cnt += rcolors[i];
	}
	cmap = (int *) malloc((cnt + 1) * sizeof(int));

	XLookupColor(Dpy, colormap, cname, &hdef, &edef);
	hdef.flags = DoRed|DoGreen|DoBlue;
	FindColor(Dpy, colormap, &hdef);
	cmap[0] = hdef.pixel;

	if (mlimit > 0)
	{
		MedianInit();
	}

	if (mlimit > 0)
	{
		if (picname[0] != '\0')
		{
			MedianCount(Pdata, Pwidth, Pheight, colrs);
		}
		for (j=0; j<NUM_FISH; j++)
		{
			int *rp, *gp, *bp;

			cdp = (struct colr_data *)malloc(rcolors[j] *
				sizeof(struct colr_data));
			rp = rreds[j];
			gp = rgreens[j];
			bp = rblues[j];
			for (i = 0; i < rcolors[j]; i++)
			{
				cdp[i].red = *rp++;
				cdp[i].green = *gp++;
				cdp[i].blue = *bp++;
			}
			MedianCount((unsigned char *)xfishRasterA[j],
				(int)rwidth[j], (int)rheight[j], cdp);
			free((char *)cdp);
		}
		MedianSplit(mlimit);
	}

	cnt = 1;
	if (picname[0] != '\0')
	{
		for (i = 0; i < Pcnt; i++)
		{
			int rv, gv, bv;

			rv = colrs[i].red;
			gv = colrs[i].green;
			bv = colrs[i].blue;

			if (mlimit > 0)
			{
				ConvertColor(&rv, &gv, &bv);
			}

			hdef.red = rv;
			hdef.green = gv;
			hdef.blue = bv;
			hdef.flags = DoRed|DoGreen|DoBlue;
			FindColor(Dpy, colormap, &hdef);
			cmap[cnt] = hdef.pixel;
			cnt++;
		}
	}
	for (j=0; j<NUM_FISH; j++)
	{
		int *rp, *gp, *bp;

		rp = rreds[j];
		gp = rgreens[j];
		bp = rblues[j];
		for (i = 0; i < rcolors[j]; i++)
		{
			int rv, gv, bv;

			rv = *rp++;
			gv = *gp++;
			bv = *bp++;

			if (mlimit > 0)
			{
				ConvertColor(&rv, &gv, &bv);
			}

			hdef.red = rv;
			hdef.green = gv;
			hdef.blue = bv;
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

	bytesperline = (width  * depth / 8 + linepad);
	newimage = XCreateImage(Dpy, DefaultVisual(Dpy, screen), depth,
		ZPixmap, 0, (char *)bit_data,
		(width + linepad), height, 8, bytesperline);

	return(newimage);
}



static unsigned char bits[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Calibrate the pixmaps and bimaps.  The right-fish data is coded in xfishy.h,
this is transformed to create the left-fish.  The eight bubbles are coded
in bubbles.h as a two dimensional array.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
init_pixmap()
{
	register caddrt p, q, x1A, x1B, x2A, x2B;
	unsigned char *data;
	register int i, j, k;
	int cnt, wcnt;

	cnt = 1;
	cnt += Pcnt;
	for (k=0; k<NUM_FISH; k++)
	{

	/*
	 * The clipmasks must be created before we remap colors.
	 * otherwise an opaque color might get remapped to a 
	 * transparent color.
	 */
	if ((DoClipping)||(picname[0] != '\0'))
	{
		data = (unsigned char *) malloc((rwidth[k]+7) / 8 * rheight[k]);

		p = (caddrt) xfishRasterA[k];
		q = data;
		wcnt = 0;
		for (i = 0; i < ((rwidth[k]+7) / 8 * rheight[k]); i++)
		{
			unsigned char bt = 0x00;
			for (j = 0; j < 8; j++)
			{
				if (*p != rback[k])
				{
					bt = bt | bits[j];
				}
				wcnt++;
				p++;
				if (wcnt == rwidth[k])
				{
					wcnt = 0;
					break;
				}
			}
			*q++ = bt;
		}
		mfishA[k][2] = XCreateBitmapFromData(Dpy, wid,
			(char *)data, rwidth[k], rheight[k]);

		p = (caddrt) xfishRasterA[k];
		p = p + rwidth[k] - 1;
		q = data;
		wcnt = 0;
		for (i = 0; i < ((rwidth[k]+7) / 8 * rheight[k]); i++)
		{
			unsigned char bt = 0x00;
			for (j = 0; j < 8; j++)
			{
				if (*p != rback[k])
				{
					bt = bt | bits[j];
				}
				wcnt++;
				p--;
				if (wcnt == rwidth[k])
				{
					wcnt = 0;
					p = p + (2 * rwidth[k]);
					break;
				}
			}
			*q++ = bt;
		}
		mfishA[k][1] = XCreateBitmapFromData(Dpy, wid,
			(char *)data, rwidth[k], rheight[k]);

		p = (caddrt) xfishRasterB[k];
		q = data;
		wcnt = 0;
		for (i = 0; i < ((rwidth[k]+7) / 8 * rheight[k]); i++)
		{
			unsigned char bt = 0x00;
			for (j = 0; j < 8; j++)
			{
				if (*p != rback[k])
				{
					bt = bt | bits[j];
				}
				wcnt++;
				p++;
				if (wcnt == rwidth[k])
				{
					wcnt = 0;
					break;
				}
			}
			*q++ = bt;
		}
		mfishB[k][2] = XCreateBitmapFromData(Dpy, wid,
			(char *)data, rwidth[k], rheight[k]);

		p = (caddrt) xfishRasterB[k];
		p = p + rwidth[k] - 1;
		q = data;
		wcnt = 0;
		for (i = 0; i < ((rwidth[k]+7) / 8 * rheight[k]); i++)
		{
			unsigned char bt = 0x00;
			for (j = 0; j < 8; j++)
			{
				if (*p != rback[k])
				{
					bt = bt | bits[j];
				}
				wcnt++;
				p--;
				if (wcnt == rwidth[k])
				{
					wcnt = 0;
					p = p + (2 * rwidth[k]);
					break;
				}
			}
			*q++ = bt;
		}
		mfishB[k][1] = XCreateBitmapFromData(Dpy, wid,
			(char *)data, rwidth[k], rheight[k]);

		free((char *)data);
	}

	if( DisplayPlanes(Dpy, screen) < 8 )
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
		xfishA[k][1] = MakeImage(x2A, rwidth[k], rheight[k]);
		xfishB[k][2] = MakeImage(x1B, rwidth[k], rheight[k]);
		xfishB[k][1] = MakeImage(x2B, rwidth[k], rheight[k]);

		free((char *)x1A);
		free((char *)x2A);
		free((char *)x1B);
		free((char *)x2B);
	}
	else
	{
		i = DisplayPlanes(Dpy, screen);

		xfishA[k][2] = XGetImage(Dpy, DefaultRootWindow(Dpy), 0, 0, rwidth[k], rheight[k], 0, ZPixmap);

		p = (caddrt) xfishRasterA[k];

		for (j = 0; j < rheight[k]; j++)
		{
		   for( i = 0; i < rwidth[k]; i++ )
		   {
		      XPutPixel(xfishA[k][2], i, j, cmap[cnt + (int)(*p)]);
		      p++;
		   }
		}

		xfishB[k][2] = XGetImage(Dpy, DefaultRootWindow(Dpy), 0, 0, rwidth[k], rheight[k], 0, ZPixmap);

		p = (caddrt) xfishRasterB[k];

		for (j = 0; j < rheight[k]; j++)
		{
		   for( i = 0; i < rwidth[k]; i++ )
		   {
		      XPutPixel(xfishB[k][2], i, j, cmap[cnt + (int)(*p)]);
		      p++;
		   }
		}

		xfishA[k][1] = XGetImage(Dpy, DefaultRootWindow(Dpy), 0, 0, rwidth[k], rheight[k], 0, ZPixmap);

		for (j = 0; j < rheight[k]; j++)
		{
		   for( i = 0; i < rwidth[k]; i++ )
		   {
		      XPutPixel(xfishA[k][1], i, j,
				XGetPixel(xfishA[k][2], rwidth[k] - i -1, j));
		   }
		}

		xfishB[k][1] = XGetImage(Dpy, DefaultRootWindow(Dpy), 0, 0, rwidth[k], rheight[k], 0, ZPixmap);

		for (j = 0; j < rheight[k]; j++)
		{
		   for( i = 0; i < rwidth[k]; i++ )
		   {
		      XPutPixel(xfishB[k][1], i, j,
				XGetPixel(xfishB[k][2], rwidth[k] - i - 1, j));
		   }
		}

	}


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
#ifdef sgi
int
#else
void
#endif
toggle_secure()
{
    pmode = !pmode;
    if (pmode)
	XLowerWindow(Dpy, wid);
    else
	XRaiseWindow(Dpy, wid);
    XFlush(Dpy);
#ifdef sgi
    return(1);
#endif
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Initialize signal so that SIGUSR1 causes secure mode to toggle.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void 
init_signals()
{
	int ret;
#ifdef linux
	signal(SIGUSR1, toggle_secure);
#else			
#if defined(MOTOROLA) || defined(SCO)
    sigset(SIGUSR1, toggle_secure);
#else
    struct sigvec vec;

    vec.sv_handler = toggle_secure;
    vec.sv_mask = 0;
    vec.sv_onstack = 0;

#ifndef hpux
    ret = sigvec(SIGUSR1, &vec, &vec);
#if 0
	if (ret != 0)
	{
		fprintf(stderr, "sigvec call failed\n");
	}
	else
	{
		fprintf(stderr, "sigvec call OK\n");
	}
#endif
#else
    sigvector(SIGUSR1, &vec, &vec);
#endif
#endif /* MOTOROLA */
#endif /* LINUX */
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
    XSizeHints  xsh;
    XImage *pimage;
    int i, size, cnt;
    char *p;
    unsigned char *ndata;
    unsigned char *ptr1, *ptr2;

    XGetWindowAttributes(Dpy, DefaultRootWindow(Dpy), &winfo);
    width = winfo.width;
    height = winfo.height;

    picname[0] = '\0';
    if ((p = XGetDefault(Dpy, pname, "BubbleLimit")) != NULL)
	blimit = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "ColorLimit")) != NULL)
	climit = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "MedianCutLimit")) != NULL)
	mlimit = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "DoClipping")) != NULL)
	DoClipping = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "DoubleBuffer")) != NULL)
	DoubleBuf = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "Overlap")) != NULL)
	Overlap = atoi(p);
    if ((p = XGetDefault(Dpy, pname, "Color")) != NULL)
	strcpy(cname, p);
    if ((p = XGetDefault(Dpy, pname, "Picture")) != NULL)
	strcpy(picname, p);
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

    /*
     * DoubleBuf is only useful if we are doing clipping on our
     * own background picture, otherwise turn it off.
     */
    if ((DoubleBuf)&&((!DoClipping)||(picname[0] == '\0')))
    {
      DoubleBuf = 0;
    }

    init_colormap();

    if (picname[0] != '\0')
    {
	size = Pwidth * Pheight;
	ndata = (unsigned char *)malloc(size);
	ptr1 = Pdata;
	ptr2 = ndata;
	cnt = 1;
	for (i = 0; i < size; i++)
	{
		*ptr2 = cmap[cnt + (int)(*ptr1)];
		ptr1++;
		ptr2++;
	}
	pimage = MakeImage(ndata, Pwidth, Pheight);
	free((char *)ndata);
	i = DisplayPlanes(Dpy, screen);
	PicMap = XCreatePixmap(Dpy, DefaultRootWindow(Dpy), Pwidth, Pheight, i);
    }

    if ((DoubleBuf)||(picname[0] != '\0'))
    {
	i = DisplayPlanes(Dpy, screen);
	PixBuf = XCreatePixmap(Dpy, DefaultRootWindow(Dpy), 500, 500, i);
	ClipBuf = XCreatePixmap(Dpy, DefaultRootWindow(Dpy), 500, 500, 1);
	c0gc = XCreateGC(Dpy, ClipBuf, 0, NULL);
	XSetForeground(Dpy, c0gc, (unsigned long)0);
	XSetFunction(Dpy, c0gc, GXcopy);
	cpgc = XCreateGC(Dpy, ClipBuf, 0, NULL);
	XSetFunction(Dpy, cpgc, GXor);
    }


    attr.override_redirect = True;
    attr.background_pixel = cmap[0];

    if ((!DoClipping)||(picname[0] != '\0'))
    {
       wid = XCreateWindow(Dpy, DefaultRootWindow(Dpy),
	1, 1, width - 2, height - 2, 0,
	CopyFromParent, CopyFromParent, CopyFromParent, 
	CWBackPixel | CWOverrideRedirect, &attr);

    if (!wid)
	msgdie("XCreateWindow failed");
    }
    else
    {
       wid = DefaultRootWindow(Dpy);
       XClearArea(Dpy, wid, 0, 0, 0, 0, False);
    }

    vals.foreground = vals.background = cmap[0];
    vals.graphics_exposures = False;
    gc = XCreateGC(Dpy, wid, GCForeground | GCBackground | GCGraphicsExposures,
	&vals);
    pgc = XCreateGC(Dpy, wid, GCForeground | GCBackground | GCGraphicsExposures,
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

	mfishA[i][0] = 0;
	mfishA[i][1] = 0;
	mfishA[i][2] = 0;
	mfishB[i][0] = 0;
	mfishB[i][1] = 0;
	mfishB[i][2] = 0;
    }

    init_pixmap();
    init_signals();

    if ((!DoClipping)||(picname[0] != '\0'))
    {
       XStoreName(Dpy, wid, pname);

       xsh.flags = USSize | USPosition | PPosition | PSize;
       xsh.x = xsh.y = 0;
       xsh.width = width;
       xsh.height = height;
       XSetNormalHints(Dpy, wid, &xsh);

       if (picname[0] != '\0')
       {
	  XPutImage(Dpy, PicMap, gc, pimage, 0, 0, 0, 0, Pwidth, Pheight);
	  XSetWindowBackgroundPixmap(Dpy, wid, PicMap);
       }

       XMapWindow(Dpy, wid);
    }

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
			if ((DoClipping)||(picname[0] != '\0'))
			{
				erasebubble(b, s);
			}
			else
			{
				putbubble(b, s, cmap[0]);
			}
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

	if (Overlap)
	{
		return(0);
	}

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
	register int i, j, x, y, ofx, ofy, ofd, done;
	register fish *f;

	for (i = 0; i < flimit; i++)
	{
		f = &finfo[i];
		if (f->d)
		{
			ofx = f->x;
			ofy = f->y;
			ofd = f->d;

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
				if (DoClipping)
				{
					movefish(f, ofx, ofy, ofd);
				}
				else
				{
					putfish(f);
					XClearArea(Dpy, wid, x, ofy,
						f->i, rheight[f->type], 0);
					if (y)
					{
						XClearArea(Dpy, wid, ofx, j,
							rwidth[f->type], y, 0);
					}
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
				if (DoClipping)
				{
					movefish(f, ofx, ofy, ofd);
				}
				else
				{
					putfish(f);
					XClearArea(Dpy, wid, x, f->y,
						f->i, rheight[f->type], 0);
				}
			}
			if ((!DoClipping)||(picname[0] == '\0'))
			{
				collide_bubbles(f, ofx, ofy);
			}
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
    struct timeval timeout;

    timeout.tv_sec = seconds;
    timeout.tv_usec = (seconds - timeout.tv_sec) * 1000000.0;
    select(0, NULL, NULL, NULL, &timeout);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
main(argc, argv)
    int         argc;
    char      **argv;
{
    int         i;
    XEvent      ev;

    parse(argc, argv);
    if ((Dpy = XOpenDisplay(sname)) == 0)
	msgdie("XOpenDisplay failed");
    screen = DefaultScreen(Dpy);

    white = WhitePixel(Dpy, screen);
    black = BlackPixel(Dpy, screen);
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

    return 0;
}


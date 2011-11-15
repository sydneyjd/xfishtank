#include <stdio.h>
#include <stdlib.h>
#include "medcut.h"

#define RED     0
#define GREEN   1
#define BLUE    2

#define FindHash(red, green, blue, h_ptr) \
	h_ptr = Hash[((red * 299) + (green * 587) + (blue * 114)) / 1000 * NCells / 256]; \
	while(h_ptr != NULL) \
	{ \
		if ((h_ptr->pixel[RED] == red)&& \
		    (h_ptr->pixel[GREEN] == green)&& \
		    (h_ptr->pixel[BLUE] == blue)) \
		{ \
			break; \
		} \
		h_ptr = h_ptr->hash_next; \
	}

struct color_rec {
    int pixel[3];
    int box_num;
    struct color_rec *hash_next;
    struct color_rec *next;
} *Hash[256];
struct c_box_rec {
    int min_pix[3];
    int max_pix[3];
    int count;
    struct color_rec *c_data;
} C_boxes[256];

int BoxCount;
struct color_rec *hash_ptr;
struct color_rec *tptr;
static int Width, Height;
int ColorCnt;
int NCells;
struct color_rec *Tptr;


void
InitMinMax(boxnum)
int boxnum;
{
    C_boxes[boxnum].min_pix[RED] = 256;
    C_boxes[boxnum].max_pix[RED] = 0;
    C_boxes[boxnum].min_pix[GREEN] = 256;
    C_boxes[boxnum].max_pix[GREEN] = 0;
    C_boxes[boxnum].min_pix[BLUE] = 256;
    C_boxes[boxnum].max_pix[BLUE] = 0;
}


struct color_rec *
AddHash(red, green, blue)
int red, green, blue;
{
    int lum;

    lum = ((red * 299) + (green * 587) + (blue * 114)) / 1000 * NCells / 256;;
    hash_ptr = (struct color_rec *) malloc(sizeof(struct color_rec));
    if (hash_ptr == NULL) {
	fprintf(stderr, "Cannot malloc %dth color\n", ColorCnt);
	exit(1);
    }
    hash_ptr->pixel[RED] = red;
    hash_ptr->pixel[GREEN] = green;
    hash_ptr->pixel[BLUE] = blue;
    hash_ptr->box_num = 0;
    hash_ptr->next = NULL;
    hash_ptr->hash_next = Hash[lum];
    Hash[lum] = hash_ptr;
    return (hash_ptr);
}


void
AddColor(cptr, boxnum)
struct color_rec *cptr;
int boxnum;
{
    struct color_rec *ptr;

    while (cptr != NULL) {
	ptr = cptr;
	cptr = cptr->next;
	ptr->box_num = boxnum;
	ptr->next = C_boxes[boxnum].c_data;
	C_boxes[boxnum].c_data = ptr;
	if (ptr->pixel[RED] < C_boxes[boxnum].min_pix[RED])
	    C_boxes[boxnum].min_pix[RED] = ptr->pixel[RED];
	if (ptr->pixel[RED] > C_boxes[boxnum].max_pix[RED])
	    C_boxes[boxnum].max_pix[RED] = ptr->pixel[RED];
	if (ptr->pixel[GREEN] < C_boxes[boxnum].min_pix[GREEN])
	    C_boxes[boxnum].min_pix[GREEN] = ptr->pixel[GREEN];
	if (ptr->pixel[GREEN] > C_boxes[boxnum].max_pix[GREEN])
	    C_boxes[boxnum].max_pix[GREEN] = ptr->pixel[GREEN];
	if (ptr->pixel[BLUE] < C_boxes[boxnum].min_pix[BLUE])
	    C_boxes[boxnum].min_pix[BLUE] = ptr->pixel[BLUE];
	if (ptr->pixel[BLUE] > C_boxes[boxnum].max_pix[BLUE])
	    C_boxes[boxnum].max_pix[BLUE] = ptr->pixel[BLUE];
    }
}


void
CountColors(data, colrs)
unsigned char *data;
struct colr_data *colrs;
{
    unsigned char *dptr;
    register int i, j;
    int red, green, blue;
    register struct color_rec *tptr;

    InitMinMax(0);
    C_boxes[0].c_data = NULL;
    tptr = C_boxes[0].c_data;
    ColorCnt = 0;

    dptr = data;

    for (j = 0; j < Height; j++) {
	for (i = 0; i < Width; i++) {
	    red = colrs[(int) *dptr].red;
	    green = colrs[(int) *dptr].green;
	    blue = colrs[(int) *dptr].blue;
	    FindHash(red, green, blue, tptr);
	    if (tptr == NULL) {
		tptr = AddHash(red, green, blue);
		tptr->next = NULL;
		AddColor(tptr, 0);
		ColorCnt++;
	    }
	    dptr++;
	}
    }
}


int
FindTarget(tptr)
int *tptr;
{
    int range, i, indx;

    range = 0;
    for (i = 0; i < BoxCount; i++) {
	int rr, gr, br;

	rr = C_boxes[i].max_pix[RED] - C_boxes[i].min_pix[RED];
	gr = C_boxes[i].max_pix[GREEN] - C_boxes[i].min_pix[GREEN];
	br = C_boxes[i].max_pix[BLUE] - C_boxes[i].min_pix[BLUE];
	if (rr > range) {
	    range = rr;
	    *tptr = i;
	    indx = RED;
	}
	if (gr > range) {
	    range = gr;
	    *tptr = i;
	    indx = GREEN;
	}
	if (br > range) {
	    range = br;
	    *tptr = i;
	    indx = BLUE;
	}
    }
    return (indx);
}


void
SplitBox(boxnum, color_indx)
int boxnum, color_indx;
{
    struct color_rec *low, *high;
    struct color_rec *data;
    int med_cnt, split_val;
    int low_cnt, high_cnt;
    int Low_cnt, High_cnt;
    int Greater, Lesser;

    Greater = BoxCount++;
    Lesser = boxnum;
    InitMinMax(Lesser);
    InitMinMax(Greater);
    data = C_boxes[boxnum].c_data;
    med_cnt = C_boxes[boxnum].count / 2;
    C_boxes[Lesser].c_data = NULL;
    C_boxes[Greater].c_data = NULL;
    Low_cnt = 0;
    High_cnt = 0;
    while (med_cnt > 0) {
	if (data->pixel[color_indx] < data->next->pixel[color_indx]) {
	    low = data;
	    high = data->next;
	    data = high->next;
	} else {
	    high = data;
	    low = data->next;
	    data = low->next;
	}
	low->next = NULL;
	high->next = NULL;
	low_cnt = 1;
	high_cnt = 1;
	split_val = low->pixel[color_indx];
	while (data != NULL) {
	    tptr = data;
	    data = data->next;
	    if (tptr->pixel[color_indx] > split_val) {
		tptr->next = high;
		high = tptr;
		high_cnt++;
	    } else {
		tptr->next = low;
		low = tptr;
		low_cnt++;
	    }
	}			/* end while data->next != NULL */
	if (low_cnt <= med_cnt) {
	    AddColor(low, Lesser);
	    Low_cnt += low_cnt;
	    med_cnt -= low_cnt;
	    if (med_cnt == 0) {
		AddColor(high, Greater);
		High_cnt += high_cnt;
	    }
	    data = high;
	} else {
	    AddColor(high, Greater);
	    High_cnt += high_cnt;
	    data = low;
	}
    }				/* end while med_cnt */
    C_boxes[Lesser].count = Low_cnt;
    C_boxes[Greater].count = High_cnt;

}


void
SplitColors(e_cnt)
int e_cnt;
{
    if (ColorCnt < e_cnt) {
	int i;

	tptr = C_boxes[0].c_data;
	for (i = 0; i < ColorCnt; i++) {
	    hash_ptr = tptr;
	    tptr = tptr->next;
	    C_boxes[i].c_data = hash_ptr;
	    C_boxes[i].count = 1;
	    hash_ptr->box_num = i;
	    hash_ptr->next = NULL;
	}
	BoxCount = ColorCnt;
    } else {
	BoxCount = 1;
	while (BoxCount < e_cnt) {
	    int target, color_indx;

	    target = 0;
	    color_indx = 0;
	    color_indx = FindTarget(&target);
	    SplitBox(target, color_indx);
	}
    }
}


/*
 * Colors passed in 0-65535, internally are 0-255
 */
void
ConvertColor(rp, gp, bp)
int *rp, *gp, *bp;
{
    struct color_rec *cptr;
    register struct color_rec *hash_ptr;
    int indx;
    int red, green, blue;
    int Tred, Tgreen, Tblue;
    int c_cnt;

    red = *rp / NCells;
    green = *gp / NCells;
    blue = *bp / NCells;
    FindHash(red, green, blue, hash_ptr);
    if (hash_ptr == NULL) {
	fprintf(stderr, "Unknown color (%d,%d,%d)\n", red, green, blue);
	hash_ptr = Hash[0];
    }
    indx = hash_ptr->box_num;
    cptr = C_boxes[indx].c_data;
    c_cnt = 0;
    Tred = Tgreen = Tblue = 0;
    while (cptr != NULL) {
	Tred += cptr->pixel[RED];
	Tgreen += cptr->pixel[GREEN];
	Tblue += cptr->pixel[BLUE];
	c_cnt++;
	cptr = cptr->next;
    }
    if (c_cnt) {
	red = Tred / c_cnt;
	green = Tgreen / c_cnt;
	blue = Tblue / c_cnt;
    }

    *rp = red * NCells;
    *gp = green * NCells;
    *bp = blue * NCells;
}


void
ConvertData(data, colrs)
unsigned char *data;
struct colr_data *colrs;
{
    unsigned char *dptr;
    register int i, j;
    int red, green, blue;
    register struct color_rec *hash_ptr;

    dptr = data;

    for (j = 0; j < Height; j++) {
	for (i = 0; i < Width; i++) {
	    red = colrs[(int) *dptr].red;
	    green = colrs[(int) *dptr].green;
	    blue = colrs[(int) *dptr].blue;
	    FindHash(red, green, blue, hash_ptr);
	    if (hash_ptr == NULL) {
		fprintf(stderr, "Unknown color (%d,%d,%d)\n", red, green, blue);
		hash_ptr = Hash[0];
	    }
	    *dptr++ = (unsigned char) hash_ptr->box_num;
	}
    }
}


void
PrintColormap(e_cnt, colrs)
int e_cnt;
struct colr_data *colrs;
{
    int i;

    for (i = 0; i < BoxCount; i++) {
	int Tred, Tgreen, Tblue;
	int c_cnt;

	c_cnt = 0;
	Tred = Tgreen = Tblue = 0;
	tptr = C_boxes[i].c_data;
	while (tptr != NULL) {
	    Tred += tptr->pixel[RED];
	    Tgreen += tptr->pixel[GREEN];
	    Tblue += tptr->pixel[BLUE];
	    c_cnt++;
	    tptr = tptr->next;
	}
	colrs[i].red = Tred / c_cnt;
	colrs[i].green = Tgreen / c_cnt;
	colrs[i].blue = Tblue / c_cnt;
    }
    for (i = BoxCount; i < e_cnt; i++) {
	colrs[i].red = 0;
	colrs[i].green = 0;
	colrs[i].blue = 0;
    }
}


void
MedianCut(data, w, h, colrs, start_cnt, end_cnt)
unsigned char *data;
int *w, *h;
struct colr_data *colrs;
int start_cnt, end_cnt;
{
    int i;

    Width = *w;
    Height = *h;
    NCells = start_cnt;
    BoxCount = 0;
    ColorCnt = 0;
    for (i = 0; i < 256; i++) {
	Hash[i] = NULL;
	C_boxes[i].c_data = NULL;
	C_boxes[i].count = 0;
    }
    CountColors(data, colrs);
    C_boxes[0].count = ColorCnt;
    SplitColors(end_cnt);
    ConvertData(data, colrs);
    PrintColormap(end_cnt, colrs);
    for (i = 0; i < 256; i++) {
	hash_ptr = Hash[i];
	while (hash_ptr != NULL) {
	    tptr = hash_ptr;
	    hash_ptr = hash_ptr->hash_next;
	    free((char *) tptr);
	}
    }
}


void
MedianInit()
{
    int i;

    NCells = 256;
    BoxCount = 0;
    ColorCnt = 0;
    for (i = 0; i < 256; i++) {
	Hash[i] = NULL;
	C_boxes[i].c_data = NULL;
	C_boxes[i].count = 0;
    }
    InitMinMax(0);
    C_boxes[0].c_data = NULL;
    Tptr = C_boxes[0].c_data;
}


/*
 * colrs are passed on 0-65535, but this median cut only deals with
 * 0-255.
 */
void
MedianCount(data, w, h, colrs)
unsigned char *data;
int w, h;
struct colr_data *colrs;
{
    unsigned char *dptr;
    register int i, j;
    int red, green, blue;

    Width = w;
    Height = h;
    dptr = data;

    for (j = 0; j < Height; j++) {
	for (i = 0; i < Width; i++) {
	    red = colrs[(int) *dptr].red / NCells;
	    green = colrs[(int) *dptr].green / NCells;
	    blue = colrs[(int) *dptr].blue / NCells;
	    FindHash(red, green, blue, tptr);
	    if (tptr == NULL) {
		tptr = AddHash(red, green, blue);
		tptr->next = NULL;
		AddColor(tptr, 0);
		ColorCnt++;
	    }
	    dptr++;
	}
    }
}


void
MedianSplit(end_cnt)
int end_cnt;
{
    C_boxes[0].count = ColorCnt;
    SplitColors(end_cnt);
}


void
MedianConvert(data, w, h, colrs)
unsigned char *data;
int w, h;
struct colr_data *colrs;
{
    Width = w;
    Height = h;
    ConvertData(data, colrs);
}


void
MedianMerge(colrs, end_cnt)
struct colr_data *colrs;
int end_cnt;
{
    int i;

    PrintColormap(end_cnt, colrs);
    for (i = 0; i < 256; i++) {
	hash_ptr = Hash[i];
	while (hash_ptr != NULL) {
	    tptr = hash_ptr;
	    hash_ptr = hash_ptr->hash_next;
	    free((char *) tptr);
	}
    }
}

#include <stdio.h>


#define BUFFSIZE	128


struct rgb_color {
        unsigned int red, green, blue;
};

struct rgb_color Colors[16];

char Head[256];

int landing = 0;
int FishCount = 0;

int buff[BUFFSIZE];
int buffindx = BUFFSIZE;


int buffget();


void
readandwrite(w, h)
	int w, h;
{
	FILE *fp;
	int i, j;
	int val1, val2;
	char name1[256];
	char name2[256];

	landing = 1;
	sprintf(name1, "%s%03d.1", Head, FishCount);
	sprintf(name2, "%s%03d.2", Head, FishCount);
	fprintf(stderr, "Landing into files (%s) and (%s)\n", name1, name2);

	fp = fopen(name1, "w");
	if (fp == NULL)
	{
		fprintf(stderr,"You should have seen the one that got away!\n");
		fprintf(stderr," (cannot open (%s) for write)\n", name1);
		exit(1);
	}
	fprintf(fp, "xstitch_data %d %d 256\nred green blue\n", w, h);
	for (i=0; i<16; i++)
	{
		fprintf(fp, "%d %d %d\n", Colors[i].red, Colors[i].green,
				Colors[i].blue);
	}
	for (i=16; i<256; i++)
	{
		fprintf(fp, "0 0 0\n");
	}
	for (j=0; j<h; j++)
	{
		for (i=0; i<(w/2); i++)
		{
			val1 = buffget();
			val2 = (val1 >> 4) & 0x0f;
			fprintf(fp, "%d ", val2);
			val2 = val1 & 0x0f;
			fprintf(fp, "%d ", val2);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	for (i=0; i< ((w/2) * (h/2)); i++)
	{
		val1 = buffget();
	}

	fp = fopen(name2, "w");
	if (fp == NULL)
	{
		fprintf(stderr,"You should have seen the one that got away!\n");
		fprintf(stderr," (cannot open (%s) for write)\n", name2);
		exit(1);
	}
	fprintf(fp, "xstitch_data %d %d 256\nred green blue\n", w, h);
	for (i=0; i<16; i++)
	{
		fprintf(fp, "%d %d %d\n", Colors[i].red, Colors[i].green,
				Colors[i].blue);
	}
	for (i=16; i<256; i++)
	{
		fprintf(fp, "0 0 0\n");
	}
	for (j=0; j<h; j++)
	{
		for (i=0; i<(w/2); i++)
		{
			val1 = buffget();
			val2 = (val1 >> 4) & 0x0f;
			fprintf(fp, "%d ", val2);
			val2 = val1 & 0x0f;
			fprintf(fp, "%d ", val2);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	landing = 0;
	FishCount++;
}


void
fillbuff()
{
	int i;

	if (buffindx < BUFFSIZE)
	{
		for (i=buffindx; i< BUFFSIZE; i++)
		{
			buff[(i - buffindx)] = buff[i];
		}
		buffindx = BUFFSIZE - buffindx;
	}
	else
	{
		buffindx = 0;
	}

	for (i=buffindx; i< BUFFSIZE; i++)
	{
		buff[i] = fgetc(stdin);
		if (buff[i] == EOF)
		{
			if (landing)
			{
		fprintf(stderr,"You should have seen the one that got away!\n");
			}
			fprintf(stderr, " (EOF reached)\n");
			exit(0);
		}
	}
	buffindx = 0;
}


int
buffget()
{
	int val;

	if (buffindx == BUFFSIZE)
	{
		fillbuff();
	}

	val = buff[buffindx];
	buffindx++;
	return(val);
}


int
lookstart()
{
	int i, val;

	if ((buff[14] == 0)&&(buff[15] == 0)&&(buff[16] == 0)&&
		(buff[17] == 15))
	{
		for (i=0; i<18; i++)
		{
			val = buffget();
		}
		return(1);
	}
	return(0);
}


int
lookhint(w, h)
	int *w;
	int *h;
{
	int vals[4];
	int i, indx, indx2, status;

	vals[0] = buffget();
	vals[1] = buffget();
	vals[2] = buffget();
	vals[3] = buffget();


	indx = 0;
	while(1)
	{
		indx2 = indx;
		if (vals[indx2] == 0)
		{
			indx2++;
			if (indx2 == 4)
			{
				indx2 = 0;
			}
			if (vals[indx2] != 0)
			{
				*w = vals[indx2];
				indx2++;
				if (indx2 == 4)
				{
					indx2 = 0;
				}
				if (vals[indx2] == 0)
				{
					indx2++;
					if (indx2 == 4)
					{
						indx2 = 0;
					}
					if (vals[indx2] != 0)
					{
						*h = vals[indx2];
						fillbuff();
						fprintf(stderr, "?");
						status = lookstart();
						if (status)
						{
							return;
						}
					}
				}
			}
		}
		vals[indx] = buffget();
		indx++;
		if (indx == 4)
		{
			indx = 0;
		}
	}
}


int
lookfish()
{
	int i, indx;

	indx = buffindx;
	for (i=0; i<16; i++)
	{
		if ((buff[indx] != 0)||(buff[indx + 1] != i))
		{
			return(0);
		}
		indx += 2;
/* check red
		if (buff[indx] != buff[indx + 1])
		{
			return(0);
		}
*/
		indx += 2;
/* check green
		if (buff[indx] != buff[indx + 1])
		{
			return(0);
		}
*/
		indx += 2;
/* check blue
		if (buff[indx] != buff[indx + 1])
		{
			return(0);
		}
*/
		indx += 2;
	}
	return(1);
}


void
getcolors()
{
	int i, val;

	for (i=0; i<16; i++)
	{
		val = buffget();
		val = buffget();

		val = buffget();
		Colors[i].red = val;
		val = buffget();

		val = buffget();
		Colors[i].green = val;
		val = buffget();

		val = buffget();
		Colors[i].blue = val;
		val = buffget();
	}
}


main(argc, argv)
	int argc;
	char **argv;
{
	int i, status;
	int w, h;

	if (argc > 1)
	{
		strcpy(Head, argv[1]);
	}
	else
	{
		strcpy(Head, "fish");
	}
	fprintf(stderr, "Fishing .....\n");
	while(1)
	{
		lookhint(&w, &h);
		fprintf(stderr, "I've got a nibble.  ");
		fillbuff();
		fprintf(stderr, "Setting the hook.  ");
		status = lookfish();
		if (status == 0)
		{
			fprintf(stderr, "Missed it!\n");
			continue;
		}
		getcolors();
		fprintf(stderr, "Got it!\n");
		readandwrite(w, h);
		fprintf(stderr, "Trying for another .....\n");
	}
}


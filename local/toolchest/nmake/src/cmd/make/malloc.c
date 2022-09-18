/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 */

/*
 * malloc() for for programs that rarely call free()
 */

#define BLOCK		((sizeof(int)==sizeof(long))?8192:1024)
#define ERROR		(char*)-1

#define align(x)	(char*)round(x,(sizeof(struct{char a;union{char *a;double b;}b;})-sizeof(union{char *a;double b;})))
#define block(x)	round(x,BLOCK)
#define extra(x)	((long)x&(BLOCK-1))
#define round(x,y)	(((long)x+(y-1))&~(y-1))

static char	*cur;			/* current pointer		*/
static char	*max;			/* max allocated pointer	*/
static char	*old;			/* previous current pointer	*/

extern char	*sbrk();

/*
 * return aligned pointer to siz sizeof's
 */

char*
malloc(siz)
register unsigned	siz;
{
	register char	*new;

	old = cur;
	if ((cur = align(cur + siz)) > max)
	{
		if (max == (new = sbrk(block(siz)))) max += block(siz);
		else
		{
			if (new == ERROR) error(2, "malloc out of space");
			old = new;
			cur = align(new + siz);
			new += block(siz);

			/*
			 * align to a block boundary
			 */

			if (extra(new))
			{
				new = sbrk((char*)block(new) - new);
				if (new == ERROR) error(2, "malloc out of space");
				new = (char*)block(new);
			}
			max = new;
		}
	}
#ifdef APOLLO
	memset(old, '\0', siz);
#endif
	return(old);
}

/*
 * free a malloc'd area
 * only the most recent malloc() space is freed
 */

void
free(ptr)
char	*ptr;
{
	if (ptr == old) cur = old;
}

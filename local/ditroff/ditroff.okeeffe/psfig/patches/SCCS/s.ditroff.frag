h52569
s 00041/00000/00000
d D 1.1 88/03/04 17:22:30 jaap 1 0
c date and time created 88/03/04 17:22:30 by jaap
e
u
U
t
T
I 1
ptout0(pi)
tchar	*pi;
{
	register short j, k, w;
	short	z, dx, dy, dx2, dy2, n;
	register tchar	i;
	int outsize;	/* size of object being printed */

	outsize = 1;	/* default */
	i = *pi;
	k = cbits(i);
	if (ismot(i)) {
		j = absmot(i);
		if (isnmot(i))
			j = -j;
		if (isvmot(i))
			lead += j;
		else 
			esc += j;
		return(outsize);
	}
	if (k == XON) {
		int c;
		if (xfont != mfont)
			ptfont();
		if (xpts != mpts)
			ptps();
		if (lead)
			ptlead();
/* ADD these two lines	vvv */
		if (esc)
			ptesc();
/* 		 	^^^ */
		fdprintf(ptid, "x X ");
		for (j = 1; (c=cbits(pi[j])) != XOFF; j++)
			outascii(pi[j]);
		oput('\n');
		return j+1;
	}
			;

E 1

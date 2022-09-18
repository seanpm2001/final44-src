h17224
s 00009/00002/00749
d D 4.5 91/04/18 16:49:15 bostic 5 4
c new copyright; att/bsd/shared
e
s 00003/00004/00748
d D 4.4 87/06/25 15:26:40 bostic 4 3
c fix for vax compiler
e
s 00001/00001/00751
d D 4.3 87/04/30 11:59:34 karels 3 2
c 
e
s 00112/00044/00640
d D 4.2 82/10/28 10:31:31 rrh 2 1
c Rationalize the reporting of "macro argument too long"; 
c paramaterize the maximum number of characaters per macro argument, and the
c maximum number of macro arguments.
e
s 00684/00000/00000
d D 4.1 82/06/07 15:45:12 rrh 1 0
c date and time created 82/06/07 15:45:12 by rrh
e
u
U
t
T
I 5
/*-
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.proprietary.c%
 */

E 5
I 1
#ifndef lint
D 5
static char sccsid[] = "%W% %G%";
#endif lint
E 5
I 5
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* not lint */
E 5

#include "tdef.h"
extern
#include "d.h"
extern
#include "v.h"
#ifdef NROFF
extern
#include "tw.h"
#endif
#include "sdef.h"

/*
troff3.c

macro and string routines, storage allocation
*/

unsigned blist[NBLIST];
extern struct s *frame, *stk, *nxf;
extern filep ip;
extern filep offset;
extern filep nextb;
extern char *enda;

extern int ch;
extern int ibf;
extern int lgf;
extern int copyf;
extern int ch0;
extern int app;
extern int ds;
extern int nlflg;
extern int *argtop;
extern int *ap;
extern int nchar;
extern int pendt;
extern int rchar;
extern int dilev;
extern int nonumb;
extern int lt;
extern int nrbits;
extern int nform;
extern int fmt[];
extern int oldmn;
extern int newmn;
extern int macerr;
extern filep apptr;
extern int diflg;
extern filep woff;
extern filep roff;
extern int wbfi;
extern int po;
extern int *cp;
extern int xxx;
int pagech = '%';
int strflg;
extern struct contab {
	int rq;
	union {
		int (*f)();
		unsigned mx;
	}x;
}contab[NM];
#ifndef VMUNIX
int wbuf[BLK];
int rbuf[BLK];
#else
int *wbuf;
int *rbuf;
int Buf[NBLIST*BLK + NEV*EVS];
#endif

caseig(){
	register i;

	offset = 0;
	if((i = copyb()) != '.')control(i,1);
}
casern(){
	register i,j;

	lgf++;
	skip();
	if(((i=getrq())==0) || ((oldmn=findmn(i)) < 0))return;
	skip();
	clrmn(findmn(j=getrq()));
	if(j)contab[oldmn].rq = (contab[oldmn].rq & MMASK) | j;
}
caserm(){
	lgf++;
	while(!skip()){
		clrmn(findmn(getrq()));
	}
}
caseas(){
	app++;
	caseds();
}
caseds(){
	ds++;
	casede();
}
caseam(){
	app++;
	casede();
}
casede(){
	register i, req;
	register filep savoff;
	extern filep finds();

	if(dip != d)wbfl();
	req = '.';
	lgf++;
	skip();
	if((i=getrq())==0)goto de1;
	if((offset=finds(i)) == 0)goto de1;
	if(ds)copys();
		else req = copyb();
	wbfl();
	clrmn(oldmn);
	if(newmn)contab[newmn].rq = i | MMASK;
	if(apptr){
		savoff = offset;
		offset = apptr;
		wbt(IMP);
		offset = savoff;
	}
	offset = dip->op;
	if(req != '.')control(req,1);
de1:
	ds = app = 0;
	return;
}
findmn(i)
int i;
{
	register j;

	for(j=0;j<NM;j++){
		if(i == (contab[j].rq & ~MMASK))break;
	}
	if(j==NM)j = -1;
	return(j);
}
clrmn(i)
int i;
{
	extern filep boff();
	if(i >= 0){
		if(contab[i].rq & MMASK)ffree(((filep)contab[i].x.mx)<<BLKBITS);
		contab[i].rq = 0;
		contab[i].x.mx = 0;
	}
}
filep finds(mn)
int mn;
{
	register i;
	extern filep boff();
	register filep savip;
	extern filep alloc();
	extern filep incoff();

	oldmn = findmn(mn);
	newmn = 0;
	apptr = (filep)0;
	if(app && (oldmn >= 0) && (contab[oldmn].rq & MMASK)){
			savip = ip;
			ip = (((filep)contab[oldmn].x.mx)<<BLKBITS);
			oldmn = -1;
			while((i=rbf()) != 0);
			apptr = ip;
			if(!diflg)ip = incoff(ip);
			nextb = ip;
			ip = savip;
	}else{
		for(i=0;i<NM;i++){
			if(contab[i].rq == 0)break;
		}
		if((i==NM) ||
		   (nextb = alloc()) == 0){
			app = 0;
			if(macerr++ > 1)done2(02);
			prstr("Too many string/macro names.\n");
			edone(04);
			return(offset = 0);
		}
			contab[i].x.mx = (unsigned)(nextb>>BLKBITS);
		if(!diflg){
			newmn = i;
			if(oldmn == -1)contab[i].rq = -1;
		}else{
			contab[i].rq = mn | MMASK;
		}
	}

	app = 0;
	return(offset = nextb);
}
skip(){
	register i;

	while(((i=getch()) & CMASK) == ' ');
	ch=i;
	return(nlflg);
}
copyb()
{
	register i, j, k;
	int ii, req, state;
	filep savoff;

	if(skip() || !(j=getrq()))j = '.';
	req = j;
	k = j>>BYTE;
	j &= BMASK;
	copyf++;
	flushi();
	nlflg = 0;
	state = 1;
	while(1){
		i = (ii = getch()) & CMASK;
		if(state == 3){
			if(i == k)break;
			if(!k){
				ch = ii;
				i = getach();
				ch = ii;
				if(!i)break;
			}
			state = 0;
			goto c0;
		}
		if(i == '\n'){
			state = 1;
			nlflg = 0;
			goto c0;
		}
		if((state == 1) && (i == '.')){
			state++;
			savoff = offset;
			goto c0;
		}
		if((state == 2) && (i == j)){
			state++;
			goto c0;
		}
		state = 0;
c0:
		if(offset)wbf(ii);
	}
	if(offset){
		wbfl();
		offset = savoff;
		wbt(0);
	}
	copyf--;
	return(req);
}
copys()
{
	register i;

	copyf++;
	if(skip())goto c0;
	if(((i=getch()) & CMASK) != '"')wbf(i);
	while(((i=getch()) & CMASK) != '\n')wbf(i);
c0:
	wbt(0);
	copyf--;
}
filep alloc()
{
	register i;
	extern filep boff();
	filep j;

	for(i=0;i<NBLIST;i++){
		if(blist[i] == 0)break;
	}
	if(i==NBLIST){
		j = 0;
	}else{
		blist[i] = -1;
		if((j = boff(i)) < NEV*EVS)j = 0;
	}
	return(nextb = j);
}
ffree(i)
filep i;
{
	register j;

	while((blist[j = blisti(i)]) != -1){
		i = ((filep)blist[j])<<BLKBITS;
		blist[j] = 0;
	}
	blist[j] = 0;
}
filep boff(i)
int i;
{
	return(((filep)i)*BLK + NEV*EVS);
}
wbt(i)
int i;
{
	wbf(i);
	wbfl();
}
wbf(i)
int i;
{
	register j;

	if(!offset)return;
	if(!woff){
		woff = offset;
#ifdef VMUNIX
		wbuf = &Buf[woff];
#endif
		wbfi = 0;
	}
	wbuf[wbfi++] = i;
	if(!((++offset) & (BLK-1))){
		wbfl();
		if(blist[j = blisti(--offset)] == -1){
			if(alloc() == 0){
				prstr("Out of temp file space.\n");
				done2(01);
			}
			blist[j] = (unsigned)(nextb>>BLKBITS);
		}
		offset = ((filep)blist[j])<<BLKBITS;
	}
	if(wbfi >= BLK)wbfl();
}
wbfl(){
	if(woff == 0)return;
#ifndef VMUNIX
	lseek(ibf, ((long)woff) * sizeof(int), 0);
	write(ibf, (char *)wbuf, wbfi * sizeof(int));
#endif
	if((woff & (~(BLK-1))) == (roff & (~(BLK-1))))roff = -1;
	woff = 0;
}
blisti(i)
filep i;
{
	return((i-NEV*EVS)/(BLK));
}
rbf(){
	register i;
	extern filep incoff();

	if((i=rbf0(ip)) == 0){
		if(!app)i = popi();
	}else{
		ip = incoff(ip);
	}
	return(i);
}
rbf0(p)
filep p;
{
	register filep i;

	if((i = (p & (~(BLK-1)))) != roff){
		roff = i;
#ifndef VMUNIX
		lseek(ibf, ((long)roff) * sizeof(int), 0);
		if(read(ibf, (char *)rbuf, BLK * sizeof(int)) == 0)return(0);
#else
		rbuf = &Buf[roff];
#endif
	}
	return(rbuf[p & (BLK-1)]);
}
filep incoff(p)
filep p;
{
	register i;
	register filep j;
	if(!((j = (++p)) & (BLK-1))){
		if((i = blist[blisti(--p)]) == -1){
			prstr("Bad storage allocation.\n");
			done2(-5);
		}
		j = ((filep)i)<<BLKBITS;
	}
	return(j);
}
popi(){
	register struct s *p;

	if(frame == stk)return(0);
	if(strflg)strflg--;
	p = nxf = frame;
	p->nargs = 0;
	frame = p->pframe;
	ip = p->pip;
	nchar = p->pnchar;
	rchar = p->prchar;
	pendt = p->ppendt;
	ap = p->pap;
	cp = p->pcp;
	ch0 = p->pch0;
	return(p->pch);
}
I 2

/*
 *	test that the end of the allocation is above a certain location
 *	in memory
 */
#define SPACETEST(base, size) while ((enda - (size)) <= (char *)(base)){setbrk(DELTA);}

E 2
pushi(newip)
filep newip;
{
	register struct s *p;
D 2
	extern char *setbrk();
E 2
I 2
	extern char	*setbrk();
E 2

D 2
	if((enda - sizeof(struct s)) < (char *)nxf)setbrk(DELTA);
E 2
I 2
	SPACETEST(nxf, sizeof(struct s));
E 2
	p = nxf;
	p->pframe = frame;
	p->pip = ip;
	p->pnchar = nchar;
	p->prchar = rchar;
	p->ppendt = pendt;
	p->pap = ap;
	p->pcp = cp;
	p->pch0 = ch0;
	p->pch = ch;
	cp = ap = 0;
	nchar = rchar = pendt = ch0 = ch = 0;
	frame = nxf;
D 2
	if(nxf->nargs == 0) nxf += 1;
		else nxf = (struct s *)argtop;
E 2
I 2
	if (nxf->nargs == 0) 
		nxf += 1;
	else 
		nxf = (struct s *)argtop;
E 2
	return(ip = newip);
}
D 2
char *setbrk(x)
int x;
E 2
I 2


char	*setbrk(x)
int	x;
E 2
{
D 2
	register char *i;
	char *sbrk();
E 2
I 2
	register char	*i;
	char	*sbrk();
E 2

D 2
	if((i = sbrk(x)) == MAXPTR){
E 2
I 2
D 4
/* ought to be rounded up by sizeof(int) */
	if (x % 2 == 1) 
		x++;
D 3
	if ( (i = sbrk(x)) >= (char *)MAXPTR) {
E 3
I 3
	if ( (i = sbrk(x)) >= (char *)-1) {
E 4
I 4
	x += sizeof(int) - 1;
	x &= ~(sizeof(int) - 1);
	if ((u_int)(i = sbrk(x)) == -1) {
E 4
E 3
E 2
		prstrfl("Core limit reached.\n");
		edone(0100);
D 2
	}else{
E 2
I 2
	} else {
E 2
		enda = i + x;
	}
	return(i);
}
D 2
getsn(){
E 2
I 2


getsn()
{
E 2
	register i;

D 2
	if((i=getach()) == 0)return(0);
	if(i == '(')return(getrq());
		else return(i);
E 2
I 2
	if ((i = getach()) == 0)
		return(0);
	if (i == '(')
		return(getrq());
	else 
		return(i);
E 2
}
D 2
setstr(){
E 2
I 2


setstr()
{
E 2
	register i;

	lgf++;
D 2
	if(((i=getsn()) == 0) ||
	   ((i=findmn(i)) == -1) ||
	   !(contab[i].rq & MMASK)){
E 2
I 2
	if (    ((i = getsn()) == 0)
	     || ((i = findmn(i)) == -1)
	     ||  !(contab[i].rq & MMASK)) {
E 2
		lgf--;
		return(0);
D 2
	}else{
		if((enda-2) < (char *)nxf)setbrk(DELTA);
E 2
I 2
	} else {
		SPACETEST(nxf, sizeof(struct s));
E 2
		nxf->nargs = 0;
		strflg++;
		lgf--;
		return(pushi(((filep)contab[i].x.mx)<<BLKBITS));
	}
}
I 2

typedef	int	tchar;
#define	cbits(x)	((x) & CMASK)

E 2
collect()
{
D 2
	register i;
	register int *strp;
	int *lim;
	int **argpp, **argppend;
	int quote;
E 2
I 2
	register j;
	tchar i;
	register tchar *strp;
	tchar * lim;
	tchar * *argpp, **argppend;
	int	quote;
E 2
	struct s *savnxf;

	copyf++;
	nxf->nargs = 0;
	savnxf = nxf;
D 2
	if(skip())goto rtn;
	lim = (int *)(nxf = savnxf + sizeof(struct s)/sizeof(savnxf));
E 2
I 2
	if (skip())
		goto rtn;

	{
		char *memp;
		memp = (char *)savnxf;
		/*
		 *	1 s structure for the macro descriptor
		 *	APERMAC tchar *'s for pointers into the strings
		 *	space for the tchar's themselves
		 */
		memp += sizeof(struct s);
		/*
		 *	CPERMAC (the total # of characters for ALL arguments)
		 *	to a macros, has been carefully chosen
		 *	so that the distance between stack frames is < DELTA 
		 */
#define	CPERMAC	200
#define	APERMAC	9
		memp += APERMAC * sizeof(tchar *);
		memp += CPERMAC * sizeof(tchar);
		nxf = (struct s*)memp;
	}
	lim = (tchar *)nxf;
	argpp = (tchar **)(savnxf + 1);
	argppend = &argpp[APERMAC];
	SPACETEST(argppend, sizeof(tchar *));
	strp = (tchar *)argppend;
	/*
	 *	Zero out all the string pointers before filling them in.
	 */
	for (j = 0; j < APERMAC; j++){
		argpp[j] = (tchar *)0;
	}
#if 0
	fprintf(stderr, "savnxf=0x%x,nxf=0x%x,argpp=0x%x,strp=argppend=0x%x,lim=0x%x,enda=0x%x\n",
		savnxf, nxf, argpp, strp, lim, enda);
#endif 0
E 2
	strflg = 0;
D 2
	if((argppend =
		(argpp = (int **)savnxf+(sizeof(struct s)/sizeof(int **))) + 9)
		> (int **)enda)setbrk(DELTA);
	strp = (int *)argppend;
	for(i=8; i>=0; i--)argpp[i] = 0;
	while((argpp != argppend) && (!skip())){
E 2
I 2
	while ((argpp != argppend) && (!skip())) {
E 2
		*argpp++ = strp;
		quote = 0;
D 2
		if(((i = getch()) & CMASK) == '"')quote++;
			else ch = i;
		while(1){
E 2
I 2
		if (cbits(i = getch()) == '"')
			quote++;
		else 
			ch = i;
		while (1) {
E 2
			i = getch();
D 2
			if( nlflg ||
			  ((!quote) && ((i & CMASK) == ' ')))break;
			if(quote && ((i & CMASK) == '"') &&
			  (((i=getch()) & CMASK) != '"')){
E 2
I 2
			if ( nlflg ||  (!quote && cbits(i) == ' '))
				break;
			if (   quote
			    && (cbits(i) == '"')
			    && (cbits(i = getch()) != '"')) {
E 2
				ch = i;
				break;
			}
			*strp++ = i;
D 2
			if(strflg && (strp >= lim)){
E 2
I 2
			if (strflg && (strp >= lim)) {
#if 0
				fprintf(stderr, "strp=0x%x, lim = 0x%x\n",
					strp, lim);
#endif 0
E 2
				prstrfl("Macro argument too long.\n");
				copyf--;
				edone(004);
			}
D 2
			if((enda-4) <= (char *)strp)setbrk(DELTA);
E 2
I 2
			SPACETEST(strp, 3 * sizeof(tchar));
E 2
		}
		*strp++ = 0;
	}
	nxf = savnxf;
D 2
	nxf->nargs = argpp -(int **)(nxf + 1);
E 2
I 2
	nxf->nargs = argpp - (tchar **)(savnxf + 1);
E 2
	argtop = strp;
rtn:
	copyf--;
}
I 2


E 2
seta()
{
	register i;

	if(((i = (getch() & CMASK) - '0') > 0) &&
D 2
		(i <= 9) && (i <= frame->nargs))ap = *((int **)frame + i-1 + (sizeof(struct s)/sizeof(int **)));
E 2
I 2
		(i <= APERMAC) && (i <= frame->nargs))ap = *((int **)frame + i-1 + (sizeof(struct s)/sizeof(int **)));
E 2
}
caseda(){
	app++;
	casedi();
}
casedi(){
	register i, j;
	register *k;

	lgf++;
	if(skip() || ((i=getrq()) == 0)){
		if(dip != d)wbt(0);
		if(dilev > 0){
			v.dn = dip->dnl;
			v.dl = dip->maxl;
			dip = &d[--dilev];
			offset = dip->op;
		}
		goto rtn;
	}
	if(++dilev == NDI){
		--dilev;
		prstr("Cannot divert.\n");
		edone(02);
	}
	if(dip != d)wbt(0);
	diflg++;
	dip = &d[dilev];
	dip->op = finds(i);
	dip->curd = i;
	clrmn(oldmn);
	k = (int *)&dip->dnl;
	for(j=0; j<10; j++)k[j] = 0;	/*not op and curd*/
rtn:
	app = 0;
	diflg = 0;
}
casedt(){
	lgf++;
	dip->dimac = dip->ditrap = dip->ditf = 0;
	skip();
	dip->ditrap = vnumb((int *)0);
	if(nonumb)return;
	skip();
	dip->dimac = getrq();
}
casetl(){
	register i, j;
	int w1, w2, w3, delim;
	filep begin;
	extern width(), pchar();

	dip->nls = 0;
	skip();
	if(dip != d)wbfl();
	if((offset = begin = alloc()) == 0)return;
	if((delim = getch()) & MOT){
		ch = delim;
		delim = '\'';
	}else delim &= CMASK;
	if(!nlflg)
		while(((i = getch()) & CMASK) != '\n'){
			if((i & CMASK) == delim)i = IMP;
			wbf(i);
		}
	wbf(IMP);wbf(IMP);wbt(0);

	w1 = hseg(width,begin);
	w2 = hseg(width,(filep)0);
	w3 = hseg(width,(filep)0);
	offset = dip->op;
#ifdef NROFF
	if(!offset)horiz(po);
#endif
	hseg(pchar,begin);
	if(w2 || w3)horiz(j=quant((lt - w2)/2-w1,HOR));
	hseg(pchar,(filep)0);
	if(w3){
		horiz(lt-w1-w2-w3-j);
		hseg(pchar,(filep)0);
	}
	newline(0);
	if(dip != d){if(dip->dnl > dip->hnl)dip->hnl = dip->dnl;}
	else{if(v.nl > dip->hnl)dip->hnl = v.nl;}
	ffree(begin);
}
casepc(){
	pagech = chget(IMP);
}
hseg(f,p)
int (*f)();
filep p;
{
	register acc, i;
	static filep q;

	acc = 0;
	if(p)q = p;
	while(1){
		i = rbf0(q);
		q = incoff(q);
		if(!i || (i == IMP))return(acc);
		if((i & CMASK) == pagech){
			nrbits = i & ~CMASK;
			nform = fmt[findr('%')];
			acc += fnumb(v.pn,f);
		}else acc += (*f)(i);
	}
}
casepm(){
	register i, k;
	register char *p;
	int xx, cnt, kk, tot;
	filep j;
	char *kvt();
	char pmline[10];

	kk = cnt = 0;
	tot = !skip();
	for(i = 0; i<NM; i++){
		if(!((xx = contab[i].rq) & MMASK))continue;
		p = pmline;
		j = (((filep)contab[i].x.mx)<<BLKBITS);
		k = 1;
		while((j = blist[blisti(j)]) != -1){k++; j <<= BLKBITS;}
		cnt++;
		kk += k;
		if(!tot){
			*p++ = xx & 0177;
			if(!(*p++ = (xx >> BYTE) & 0177))*(p-1) = ' ';
			*p++ = ' ';
			kvt(k,p);
			prstr(pmline);
		}
	}
	if(tot || (cnt > 1)){
		kvt(kk,pmline);
		prstr(pmline);
	}
}
char *kvt(k,p)
int k;
char *p;
{
	if(k>=100)*p++ = k/100 + '0';
	if(k>=10)*p++ = (k%100)/10 + '0';
	*p++ = k%10 + '0';
	*p++ = '\n';
	*p = 0;
	return(p);
}
dummy(){}
E 1

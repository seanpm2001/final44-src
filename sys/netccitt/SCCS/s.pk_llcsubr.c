h06825
s 00007/00007/00336
d D 8.2 95/02/09 10:42:04 cgd 5 4
c cons_rtrequest returns void, for consistency.
e
s 00002/00002/00341
d D 8.1 93/06/10 22:53:56 bostic 4 3
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00005/00003/00338
d D 7.3 93/06/05 15:57:13 sklower 3 2
c various lintish things come to light by protoypes.
e
s 00121/00000/00220
d D 7.2 93/02/23 00:24:20 dkhusema 2 1
c 
e
s 00220/00000/00000
d D 7.1 92/12/08 17:36:07 sklower 1 0
c date and time created 92/12/08 17:36:07 by sklower
e
u
U
t
T
I 1
/* 
 * Copyright (C) Dirk Husemann, Computer Science Department IV, 
 * 		 University of Erlangen-Nuremberg, Germany, 1990, 1991, 1992
D 4
 * Copyright (c) 1992   Regents of the University of California.
 * All rights reserved.
E 4
I 4
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
E 4
 * 
 * This code is derived from software contributed to Berkeley by
 * Dirk Husemann and the Computer Science Department (IV) of
 * the University of Erlangen-Nuremberg, Germany.
 *
 * %sccs.include.redist.c%
 *
 *	%W% (Berkeley) %G%
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <sys/domain.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/protosw.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/kernel.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_llc.h>
#include <net/if_types.h>
#include <net/route.h>

#include <netccitt/dll.h>
#include <netccitt/x25.h>
#include <netccitt/pk.h>
#include <netccitt/pk_var.h>
#include <netccitt/llc_var.h>


/*
 * Routing support for X.25
 *
 * We distinguish between two cases:
 * RTF_HOST:
 * 	rt_key(rt)	X.25 address of host
 *	rt_gateway	SNPA (MAC+DLSAP) address of host
 *	rt_llinfo	pkcb for rt_key(rt)
 *
 * RTF_GATEWAY
 *	rt_key(rt)	X.25 address of host or suitably masked network
 *	rt_gateway	X.25 address of next X.25 gateway (switch)
 *	rt_llinfo	rtentry for rt_gateway address
 *			ought to be of type RTF_HOST
 *
 *
 * Mapping of X.121 to pkcbs:
 *
 * HDLC uses the DTE-DCE model of X.25, therefore we need a many-to-one
 * relationship, i.e.:
 *	
 * 	{X.121_a, X.121_b, X.121_c, ..., X.121_i} -> pkcb_0
 *
 * LLC2 utilizes the DTE-DTE model of X.25, resulting effectively in a
 * one-to-one relationship, i.e.:
 *
 *	{X.121_j} 	->	pkcb_1a
 *	{X.121_k}	->	pkcb_1b
 *	...
 *	{X.121_q}	->	pkcb_1q
 * 
 * It might make sense to allow a many-to-one relation for LLC2 also,
 * 
 *	{X.121_r, X.121_s, X.121_t, X.121_u} -> pkcb_2a
 *
 * This would make addresses X.121_[r-u] essentially aliases of one
 * address ({X.121_[r-u]} would constitute a representative set).
 *
 * Each one-to-one relation must obviously be entered individually with
 * a route add command, whereas a many-to-one relationship can be 
 * either entered individually or generated by using a netmask.
 * 
 * To facilitate dealings the many-to-one case for LLC2 can only be
 * established via a netmask.
 *
 */

#define XTRACTPKP(rt)	((rt)->rt_flags & RTF_GATEWAY ? \
			 ((rt)->rt_llinfo ? \
			  (struct pkcb *) ((struct rtentry *)((rt)->rt_llinfo))->rt_llinfo : \
			  (struct pkcb *) NULL) : \
			 (struct pkcb *)((rt)->rt_llinfo))

#define equal(a1, a2) (bcmp((caddr_t)(a1), \
			       (caddr_t)(a2), \
			       (a1)->sa_len) == 0)
#define XIFA(rt) ((struct x25_ifaddr *)((rt)->rt_ifa))
I 3
#define SA(s) ((struct sockaddr *)s)
E 3

D 5
int
E 5
I 5
void
E 5
cons_rtrequest(int cmd, struct rtentry *rt, struct sockaddr *dst)
{
	register struct pkcb *pkp;
	register int i;
	register char one_to_one;
	struct pkcb *pk_newlink();
	struct rtentry *npaidb_enter();

	pkp = XTRACTPKP(rt);

	switch(cmd) {
	case RTM_RESOLVE:
	case RTM_ADD:
		if (pkp) 
D 5
			return(EEXIST);
E 5
I 5
			return; /* XXX: EEXIST */
E 5

		if (rt->rt_flags & RTF_GATEWAY) {
			if (rt->rt_llinfo)
				RTFREE((struct rtentry *)rt->rt_llinfo);
			rt->rt_llinfo = (caddr_t) rtalloc1(rt->rt_gateway, 1);
D 5
			return(0);
E 5
I 5
			return; /* XXX: OK */
E 5
		}
		/*
		 * Assumptions:	(1) ifnet structure is filled in
		 *		(2) at least the pkcb created via 
		 *		    x25config (ifconfig?) has been 
		 *		    set up already.
		 *		(3) HDLC interfaces have an if_type of 
		 *		    IFT_X25{,DDN}, LLC2 interfaces 
		 *		    anything else (any better way to 
		 *		    do this?)
		 *
		 */
		if (!rt->rt_ifa)
D 5
			return (ENETDOWN);
E 5
I 5
			return; /* XXX: ENETDOWN */
E 5
	
		/*	
		 * We differentiate between dealing with a many-to-one
		 * (HDLC: DTE-DCE) and a one-to-one (LLC2: DTE-DTE) 
		 * relationship (by looking at the if type).
		 *
		 * Only in case of the many-to-one relationship (HDLC)
		 * we set the ia->ia_pkcb pointer to the pkcb allocated
		 * via pk_newlink() as we will use just that one pkcb for
		 * future route additions (the rtentry->rt_llinfo pointer
		 * points to the pkcb allocated for that route).
		 *
		 * In case of the one-to-one relationship (LLC2) we 
		 * create a new pkcb (via pk_newlink()) for each new rtentry.
		 * 
		 * NOTE: Only in case of HDLC does ia->ia_pkcb point
		 * to a pkcb, in the LLC2 case it doesn't (as we don't 
		 * need it here)!
		 */
		one_to_one = ISISO8802(rt->rt_ifp);

		if (!(pkp = XIFA(rt)->ia_pkcb) && !one_to_one) 
			XIFA(rt)->ia_pkcb = pkp = 
				pk_newlink(XIFA(rt), (caddr_t) 0);
		else if (one_to_one && 
			 !equal(rt->rt_gateway, rt->rt_ifa->ifa_addr)) {
			pkp = pk_newlink(XIFA(rt), (caddr_t) 0);
			/*
			 * We also need another route entry for mapping
			 * MAC+LSAP->X.25 address
			 */
			pkp->pk_llrt = npaidb_enter(rt->rt_gateway, rt_key(rt), rt, 0);
		}
		if (pkp) {
			if (!pkp->pk_rt)
				pkp->pk_rt = rt;
			pkp->pk_refcount++;
		}
		rt->rt_llinfo = (caddr_t) pkp;

D 5
		return(0);
E 5
I 5
		return; /* XXX: OK */
E 5

	case RTM_DELETE:
	{
		/*
		 * The pkp might be empty if we are dealing
		 * with an interface route entry for LLC2, in this 
		 * case we don't need to do anything ...
		 */
		if (pkp) {
			if ( rt->rt_flags & RTF_GATEWAY ) {
				if (rt->rt_llinfo)
					RTFREE((struct rtentry *)rt->rt_llinfo);
D 5
				return(0);
E 5
I 5
				return; /* XXX: OK */
E 5
			}
			
			if (pkp->pk_llrt)
				npaidb_destroy(pkp->pk_llrt);

			pk_dellink (pkp);
			
D 5
			return(0);
E 5
I 5
			return; /* XXX: OK */
E 5
		}
	}
	}
}

I 2
/*
 * Network Protocol Addressing Information DataBase (npaidb) 
 * 
 * To speed up locating the entity dealing with an LLC packet use is made 
 * of a routing tree. This npaidb routing tree is handled 
 * by the normal rn_*() routines just like (almost) any other routing tree. 
 * 
 * The mapping being done by the npaidb_*() routines is as follows: 
 * 
 *     Key:       MAC,LSAP (enhancing struct sockaddr_dl) 
 *     Gateway:   sockaddr_x25 (i.e. X.25 address - X.121 or NSAP) 
 *     Llinfo:    npaidbentry { 
 *                         struct llc_linkcb *npaidb_linkp; 
 *                         struct rtentry *npaidb_rt; 
 *                } 
 * 
 * Using the npaidbentry provided by llinfo we can then access 
 * 
 *       o the pkcb by using (struct pkcb *) (npaidb_rt->rt_llinfo)
 *       o the linkcb via npaidb_linkp 
 * 
 * The following functions are provided 
 * 
 *       o npaidb_enter(struct sockaddr_dl *sdl, struct sockaddr_x25 *sx25, 
 *                      struct struct llc_linkcb *link, struct rtentry *rt) 
 * 
 *       o npaidb_enrich(short type, caddr_t info) 
 * 
 */
E 2

I 2
struct sockaddr_dl npdl_netmask = {
 sizeof(struct sockaddr_dl),					/* _len */
 0,								/* _family */
 0,								/* _index */
 0,								/* _type */
 -1,								/* _nlen */
 -1,								/* _alen */
 -1,								/* _slen */
 { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},		/* _data */
}; 
struct sockaddr npdl_dummy;

int npdl_datasize = sizeof(struct sockaddr_dl)-
		((int)((caddr_t)&((struct sockaddr_dl *)0)->sdl_data[0]));

struct rtentry *
npaidb_enter(struct sockaddr_dl *key, struct sockaddr *value,
	     struct rtentry *rt, struct llc_linkcb *link)
{
	struct rtentry *nprt; register int i;

	USES_AF_LINK_RTS;

D 3
	if ((nprt = rtalloc1(key, 0)) == 0) {
E 3
I 3
	if ((nprt = rtalloc1(SA(key), 0)) == 0) {
E 3
		register u_int size = sizeof(struct npaidbentry);
		register u_char saploc = LLSAPLOC(key, rt->rt_ifp);

		/* 
		 * set up netmask: LLC2 packets have the lowest bit set in
		 * response packets (e.g. 0x7e for command packets, 0x7f for
		 * response packets), to facilitate the lookup we use a netmask
		 * of 11111110 for the SAP position. The remaining positions 
		 * are zeroed out.
		 */
		npdl_netmask.sdl_data[saploc] = NPDL_SAPNETMASK;
		bzero((caddr_t)&npdl_netmask.sdl_data[saploc+1], 
		      npdl_datasize-saploc-1);

		if (value == 0)
			value = &npdl_dummy;

		/* now enter it */
D 3
		rtrequest(RTM_ADD, key, value, &npdl_netmask, 0, &nprt);
E 3
I 3
		rtrequest(RTM_ADD, SA(key), SA(value),
			SA(&npdl_netmask), 0, &nprt);
E 3

		/* and reset npdl_netmask */
		for (i = saploc; i < npdl_datasize; i++)
			npdl_netmask.sdl_data[i] = -1;

		nprt->rt_llinfo = malloc(size , M_PCB, M_WAITOK);
		if (nprt->rt_llinfo) {
			bzero (nprt->rt_llinfo, size);
			((struct npaidbentry *) (nprt->rt_llinfo))->np_rt = rt;
		}
	} else nprt->rt_refcnt--;
	return nprt;
}

struct rtentry *
npaidb_enrich(short type, caddr_t info, struct sockaddr_dl *sdl)
{
	struct rtentry *rt;

	USES_AF_LINK_RTS;

D 3
	if (rt = rtalloc1(sdl, 0)) {
E 3
I 3
	if (rt = rtalloc1((struct sockaddr *)sdl, 0)) {
E 3
		rt->rt_refcnt--;
		switch (type) {
		case NPAIDB_LINK:
			((struct npaidbentry *)(rt->rt_llinfo))->np_link = 
				(struct llc_linkcb *) info;
			break;
		}
		return rt;
	}		

	return ((struct rtentry *) 0);

}

npaidb_destroy(struct rtentry *rt)
{
	USES_AF_LINK_RTS;

	if (rt->rt_llinfo) 
		free((caddr_t) rt->rt_llinfo, M_PCB);
	return(rtrequest(RTM_DELETE, rt_key(rt), rt->rt_gateway, rt_mask(rt), 
			 0, 0));
}


#ifdef LLC
E 2
/*
 * Glue between X.25 and LLC2
 */
int
x25_llcglue(int prc, struct sockaddr *addr)
{
	register struct sockaddr_x25 *sx25 = (struct sockaddr_x25 *)addr;
	register struct x25_ifaddr *x25ifa;
	struct dll_ctlinfo ctlinfo;
	
	if((x25ifa = (struct x25_ifaddr *)ifa_ifwithaddr(addr)) == 0)
		return 0;

	ctlinfo.dlcti_cfg  =
	    (struct dllconfig *)(((struct sockaddr_x25 *)(&x25ifa->ia_xc))+1);
	ctlinfo.dlcti_lsap = LLC_X25_LSAP;

	return ((int)llc_ctlinput(prc, addr, (caddr_t)&ctlinfo));
}
I 2
#endif /* LLC */
E 2
E 1

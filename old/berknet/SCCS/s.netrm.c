h30342
s 00010/00011/00115
d D 4.2 82/09/12 19:28:59 mckusick 2 1
c update to new directory routines
e
s 00126/00000/00000
d D 4.1 82/09/12 17:31:40 mckusick 1 0
c date and time created 82/09/12 17:31:40 by mckusick
e
u
U
t
T
I 1
static char sccsid[] = "%W%	(Berkeley)	%G%";

# include "defs.h"
/* sccs id variable */
static char *netrm_sid = "@(#)netrm.c	1.2";


/*
 *	netrm - remove an entry from the network queue.
 *
 *	first does a creat to truncate the file to zero length in
 *	case it is being sent. (this stops the daemon!) Next the
 *	file is removed. 
 *	must be setuid root
 */

char	path[] =	NETRMPATH;
char	pathname[]=  	NETRMNAME;

int hisuid, hisgid;
static char visit[26];
static struct stat statbuf;
D 2
static struct direct dirbuf;
E 2

main(argc,argv)
char	*argv[];
{
	int cnt, mach, i, all = 0;

	if (argc < 2)
	{
		printf("usage: netrm [-] file1 file2 ... filen\n");
		exit(EX_USAGE);
	}

	hisuid = getuid();
	hisuid = uidmask(hisuid);
	hisgid = getgid();
	
	if(argv[1][0] == '-'){
		all++;
		argv++;
		argc--;
		}
	cnt = 0;

	while (++cnt < argc)rmfile(argv[cnt]);
	if(all){
		visit[chtoinx(local)] = 1;		/* skip this machine */
		for(i = 0; i < MAXINX; i++)
			if((mach = gothru(local,inxtoch(i)))
			&& !visit[chtoinx(mach)]){
				visit[chtoinx(mach)] = 1;
				senddir[strlen(senddir)-1] = mach;
				if(chdir(senddir) < 0) 
					continue;
				pdir(senddir);
			}
	}
}
static pdir(str)
  char *str; {
D 2
	FILE *df;
	df = fopen(str,"r");
E 2
I 2
	DIR *df;
	register struct direct *dp;
	df = opendir(str);
E 2
	if(df == NULL){
		perror(str);
		exit(EX_OSFILE);
		}
D 2
	while(fread(&dirbuf,1,sizeof dirbuf,df) == sizeof dirbuf){
		if(dirbuf.d_ino == 0
		|| dirbuf.d_name[0] != 'd'
		|| dirbuf.d_name[1] != 'f'
		|| stat(dirbuf.d_name,&statbuf) < 0)
E 2
I 2
	while((dp = readdir(df)) != NULL){
		if(dp->d_name[0] != 'd'
		|| dp->d_name[1] != 'f'
		|| stat(dp->d_name,&statbuf) < 0)
E 2
			 continue;
		if(guid(statbuf.st_uid,statbuf.st_gid) != hisuid)
			continue;
		/* kludge in file name */
D 2
		dirbuf.d_name[3] = dirbuf.d_name[2];
		rmfile(dirbuf.d_name+3);
E 2
I 2
		dp->d_name[3] = dp->d_name[2];
		rmfile(dp->d_name+3);
E 2
		}
D 2
	fclose(df);
E 2
I 2
	closedir(df);
E 2
	}
rmfile(str)
  char *str;
  {
	register char *ap, *cp;
	int tt;
	char *ostr,*rem,buf[20];
	ostr = str;
	if(str[0] != 'd' || str[1] != 'f' || str[3] != 'a'){
		strcpy(buf+3,str);
		buf[0] = 'd';
		buf[1] = 'f';
		buf[2] = str[0];
		buf[3] = 'a';
		str = buf;
		}
	cp = path;
	ap = pathname;
	while (*ap++ = *cp++);
	cp = pathname + strlen(pathname) - 10;
	while(*cp++ != 'f');
	cp--;
	cp--;
	rem = cp;
	ap = str;
	while(*cp != '\0' && (*cp++ = *ap++));
	pathname[strlen(pathname) - 11] = str[2];	/* set dir for mach */

	if (stat(pathname,&statbuf) < 0) {
		perror(ostr);
		return;
	}

	tt = guid(statbuf.st_uid,statbuf.st_gid);
	if(tt != hisuid && hisuid != 0) {
		printf("%s: Permission Denied\n",ostr);
		return;
	}

	printf("removing file %s\n",ostr);
	creat(pathname,0600);
	unlink(pathname);
	*rem = 'c';
	unlink(pathname);
}
E 1

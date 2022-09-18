/*
 * Copyright 1994 The President and Fellows of Harvard University.
 * All rights reserved.
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/resourcevar.h>
#include <sys/kernel.h>
#include <sys/namei.h>
#include <sys/vnode.h>
#include <sys/proc.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <ufs/ufs/quota.h>
#include <ufs/ufs/ufsmount.h>
#include <ufs/logging/lffs.h>


struct vnode *
lffs_getvn(p, mnt_name)
	struct proc *p;
	char *mnt_name;
{
	struct nameidata nd;
	struct vnode *vp;
	int error;

	/* Look up pathname of other file system */
	NDINIT(&nd, LOOKUP, FOLLOW, UIO_USERSPACE, mnt_name, p);
	if (error = namei(&nd))
		return (NULL);

	return(nd.ni_vp);
}

int
lffs_log(vp, lwp, base, len, cred, p, ioflag)
	struct vnode *vp;
	struct lwrite_entry *lwp;
	caddr_t base;
	int len;
	struct ucred *cred;
	struct proc *p;
	int ioflag;
{
	struct uio auio;
	struct iovec aiov;
	struct lwrite_entry lwe;
	int error;

	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	aiov.iov_base = base;
	aiov.iov_len = len;
	auio.uio_resid = len;
	auio.uio_offset = 0;
	auio.uio_segflg = UIO_SYSSPACE;
	auio.uio_rw = UIO_WRITE;
	auio.uio_procp = p;
	if (lwp == NULL)
		lwp = &lwe;
	lwp->uio = &auio;
	lwp->logrec_begin = 0;
	lwp->logrec_end = 0;
	error = VOP_IOCTL(vp, FIOLOG, lwp, ioflag, cred, p);
	if (auio.uio_resid && error == 0)
		error = EIO;
	/* If operation was synchronous, then you can update the stable_lsn. */
	if (ioflag)
		VFSTOUFS((VFSTOUFS(vp->v_mount)->um_relmp))->um_stable_lsn =
		    lwp->logrec_end;
	return (error);
}

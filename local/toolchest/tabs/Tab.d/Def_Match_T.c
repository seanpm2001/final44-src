/*

 *      Copyright (c) 1984, 1985 AT&T
 *      All Rights Reserved

 *      THIS IS UNPUBLISHED PROPRIETARY SOURCE 
 *      CODE OF AT&T.
 *      The copyright notice above does not 
 *      evidence any actual or intended
 *      publication of such source code.

 */
#include "stdio.h"
#include "vt.h"
#include "windows.h"
#include "menus.h"

Def_Match_Type (func)
int	(*func) ();
{
	Match_Routine = func;
}

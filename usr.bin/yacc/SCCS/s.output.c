h12804
s 00076/00012/01148
d D 5.7 93/05/24 21:29:29 bostic 8 7
c revision 1.9 from Bob Corbett
e
s 00088/00082/01072
d D 5.6 91/01/20 20:27:44 corbett 7 5
c I added code to implement the -r option.  -rpc
e
s 00014/00000/01154
d R 5.6 91/01/20 20:01:57 corbett 6 5
c I added code to implement the -r option.  -rpc
e
s 00001/00011/01153
d D 5.5 90/06/01 16:13:48 bostic 5 4
c new copyright notice
e
s 00001/00001/01163
d D 5.4 90/03/11 22:34:00 corbett 4 3
c I fixed an off-by-one error in the #line numbers.  -rpc
e
s 00003/00003/01161
d D 5.3 90/03/06 14:44:18 bostic 3 2
c output token #defines after user-specified text
e
s 00039/00041/01125
d D 5.2 90/02/15 09:44:10 bostic 2 1
c update from Bob Corbett
e
s 01166/00000/00000
d D 5.1 89/12/25 14:24:36 bostic 1 0
c date and time created 89/12/25 14:24:36 by bostic
e
u
U
t
T
I 1
/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Robert Paul Corbett.
 *
D 5
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
E 5
I 5
 * %sccs.include.redist.c%
E 5
 */

#ifndef lint
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* not lint */

#include "defs.h"

static int nvectors;
static int nentries;
static short **froms;
static short **tos;
static short *tally;
static short *width;
static short *state_count;
static short *order;
static short *base;
static short *pos;
static int maxtable;
static short *table;
static short *check;
static int lowzero;
static int high;


output()
{
    free_itemsets();
    free_shifts();
    free_reductions();
I 8
    output_prefix();
E 8
D 3
    output_defines();
E 3
    output_stored_text();
I 3
    output_defines();
E 3
D 2
    output_yyconst();
E 2
    output_rule_data();
    output_yydefred();
    output_actions();
    free_parser();
    output_debug();
    output_stype();
I 7
    if (rflag) write_section(tables);
E 7
    write_section(header);
    output_trailing_text();
    write_section(body);
    output_semantic_actions();
    write_section(trailer);
}


I 8
output_prefix()
{
    if (symbol_prefix == NULL)
	symbol_prefix = "yy";
    else
    {
	++outline;
	fprintf(code_file, "#define yyparse %sparse\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yylex %slex\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyerror %serror\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yychar %schar\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyval %sval\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yylval %slval\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yydebug %sdebug\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yynerrs %snerrs\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyerrflag %serrflag\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyss %sss\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyssp %sssp\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyvs %svs\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyvsp %svsp\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yylhs %slhs\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yylen %slen\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yydefred %sdefred\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yydgoto %sdgoto\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yysindex %ssindex\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyrindex %srindex\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yygindex %sgindex\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yytable %stable\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yycheck %scheck\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyname %sname\n", symbol_prefix);
	++outline;
	fprintf(code_file, "#define yyrule %srule\n", symbol_prefix);
    }
    ++outline;
    fprintf(code_file, "#define YYPREFIX \"%s\"\n", symbol_prefix);
}


E 8
D 2
output_yyconst()
{
    outline += 3;
    fprintf(output_file, "#ifndef YYCONST\n#define YYCONST /* const */\n\
#endif\n");
}


E 2
output_rule_data()
{
    register int i;
    register int j;

  
D 2
    fprintf(output_file, "YYCONST short yylhs[] = {%34d,",
E 2
I 2
D 8
    fprintf(output_file, "short yylhs[] = {%42d,",
E 8
I 8
    fprintf(output_file, "short %slhs[] = {%42d,", symbol_prefix,
E 8
E 2
	    symbol_value[start_symbol]);

    j = 10;
    for (i = 3; i < nrules; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
        else
	    ++j;

        fprintf(output_file, "%5d,", symbol_value[rlhs[i]]);
    }
D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");

D 2
    fprintf(output_file, "YYCONST short yylen[] = {%34d,", 2);
E 2
I 2
D 8
    fprintf(output_file, "short yylen[] = {%42d,", 2);
E 8
I 8
    fprintf(output_file, "short %slen[] = {%42d,", symbol_prefix, 2);
E 8
E 2

    j = 10;
    for (i = 3; i < nrules; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
	else
	  j++;

        fprintf(output_file, "%5d,", rrhs[i + 1] - rrhs[i] - 1);
    }
D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");
}


output_yydefred()
{
    register int i, j;

D 2
    fprintf(output_file, "YYCONST short yydefred[] = {%31d,",
E 2
I 2
D 8
    fprintf(output_file, "short yydefred[] = {%39d,",
E 8
I 8
    fprintf(output_file, "short %sdefred[] = {%39d,", symbol_prefix,
E 8
E 2
	    (defred[0] ? defred[0] - 2 : 0));

    j = 10;
    for (i = 1; i < nstates; i++)
    {
	if (j < 10)
	    ++j;
	else
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}

	fprintf(output_file, "%5d,", (defred[i] ? defred[i] - 2 : 0));
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");
}


output_actions()
{
    nvectors = 2*nstates + nvars;

    froms = NEW2(nvectors, short *);
    tos = NEW2(nvectors, short *);
    tally = NEW2(nvectors, short);
    width = NEW2(nvectors, short);

    token_actions();
    FREE(lookaheads);
    FREE(LA);
    FREE(LAruleno);
    FREE(accessing_symbol);

    goto_actions();
    FREE(goto_map + ntokens);
    FREE(from_state);
    FREE(to_state);

    sort_actions();
    pack_table();
    output_base();
    output_table();
    output_check();
}


token_actions()
{
    register int i, j;
    register int shiftcount, reducecount;
    register int max, min;
    register short *actionrow, *r, *s;
    register action *p;

    actionrow = NEW2(2*ntokens, short);
    for (i = 0; i < nstates; ++i)
    {
	if (parser[i])
	{
	    for (j = 0; j < 2*ntokens; ++j)
	    actionrow[j] = 0;

	    shiftcount = 0;
	    reducecount = 0;
	    for (p = parser[i]; p; p = p->next)
	    {
		if (p->suppressed == 0)
		{
		    if (p->action_code == SHIFT)
		    {
			++shiftcount;
			actionrow[p->symbol] = p->number;
		    }
		    else if (p->action_code == REDUCE && p->number != defred[i])
		    {
			++reducecount;
			actionrow[p->symbol + ntokens] = p->number;
		    }
		}
	    }

	    tally[i] = shiftcount;
	    tally[nstates+i] = reducecount;
	    width[i] = 0;
	    width[nstates+i] = 0;
	    if (shiftcount > 0)
	    {
		froms[i] = r = NEW2(shiftcount, short);
		tos[i] = s = NEW2(shiftcount, short);
		min = MAXSHORT;
		max = 0;
		for (j = 0; j < ntokens; ++j)
		{
		    if (actionrow[j])
		    {
			if (min > symbol_value[j])
			    min = symbol_value[j];
			if (max < symbol_value[j])
			    max = symbol_value[j];
			*r++ = symbol_value[j];
			*s++ = actionrow[j];
		    }
		}
		width[i] = max - min + 1;
	    }
	    if (reducecount > 0)
	    {
		froms[nstates+i] = r = NEW2(reducecount, short);
		tos[nstates+i] = s = NEW2(reducecount, short);
		min = MAXSHORT;
		max = 0;
		for (j = 0; j < ntokens; ++j)
		{
		    if (actionrow[ntokens+j])
		    {
			if (min > symbol_value[j])
			    min = symbol_value[j];
			if (max < symbol_value[j])
			    max = symbol_value[j];
			*r++ = symbol_value[j];
			*s++ = actionrow[ntokens+j] - 2;
		    }
		}
		width[nstates+i] = max - min + 1;
	    }
	}
    }
    FREE(actionrow);
}

goto_actions()
{
    register int i, j, k;

    state_count = NEW2(nstates, short);

    k = default_goto(start_symbol + 1);
D 2
    fprintf(output_file, "YYCONST short yydgoto[] = {%32d,", k);
E 2
I 2
D 8
    fprintf(output_file, "short yydgoto[] = {%40d,", k);
E 8
I 8
    fprintf(output_file, "short %sdgoto[] = {%40d,", symbol_prefix, k);
E 8
E 2
    save_column(start_symbol + 1, k);

    j = 10;
    for (i = start_symbol + 2; i < nsyms; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
	else
	    ++j;

	k = default_goto(i);
	fprintf(output_file, "%5d,", k);
	save_column(i, k);
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");
    FREE(state_count);
}

int
default_goto(symbol)
int symbol;
{
    register int i;
    register int m;
    register int n;
    register int default_state;
    register int max;

    m = goto_map[symbol];
    n = goto_map[symbol + 1];

    if (m == n) return (0);

    for (i = 0; i < nstates; i++)
	state_count[i] = 0;

    for (i = m; i < n; i++)
	state_count[to_state[i]]++;

    max = 0;
    default_state = 0;
    for (i = 0; i < nstates; i++)
    {
	if (state_count[i] > max)
	{
	    max = state_count[i];
	    default_state = i;
	}
    }

    return (default_state);
}



save_column(symbol, default_state)
int symbol;
int default_state;
{
    register int i;
    register int m;
    register int n;
    register short *sp;
    register short *sp1;
    register short *sp2;
    register int count;
    register int symno;

    m = goto_map[symbol];
    n = goto_map[symbol + 1];

    count = 0;
    for (i = m; i < n; i++)
    {
	if (to_state[i] != default_state)
	    ++count;
    }
    if (count == 0) return;

    symno = symbol_value[symbol] + 2*nstates;

    froms[symno] = sp1 = sp = NEW2(count, short);
    tos[symno] = sp2 = NEW2(count, short);

    for (i = m; i < n; i++)
    {
	if (to_state[i] != default_state)
	{
	    *sp1++ = from_state[i];
	    *sp2++ = to_state[i];
	}
    }

    tally[symno] = count;
    width[symno] = sp1[-1] - sp[0] + 1;
}

sort_actions()
{
  register int i;
  register int j;
  register int k;
  register int t;
  register int w;

  order = NEW2(nvectors, short);
  nentries = 0;

  for (i = 0; i < nvectors; i++)
    {
      if (tally[i] > 0)
	{
	  t = tally[i];
	  w = width[i];
	  j = nentries - 1;

	  while (j >= 0 && (width[order[j]] < w))
	    j--;

	  while (j >= 0 && (width[order[j]] == w) && (tally[order[j]] < t))
	    j--;

	  for (k = nentries - 1; k > j; k--)
	    order[k + 1] = order[k];

	  order[j + 1] = i;
	  nentries++;
	}
    }
}


pack_table()
{
    register int i;
    register int place;
    register int state;

    base = NEW2(nvectors, short);
    pos = NEW2(nentries, short);

    maxtable = 1000;
    table = NEW2(maxtable, short);
    check = NEW2(maxtable, short);

    lowzero = 0;
    high = 0;

    for (i = 0; i < maxtable; i++)
	check[i] = -1;

    for (i = 0; i < nentries; i++)
    {
	state = matching_vector(i);

	if (state < 0)
	    place = pack_vector(i);
	else
	    place = base[state];

	pos[i] = place;
	base[order[i]] = place;
    }

    for (i = 0; i < nvectors; i++)
    {
D 2
	FREE(froms[i]);
	FREE(tos[i]);
E 2
I 2
	if (froms[i])
	    FREE(froms[i]);
	if (tos[i])
	    FREE(tos[i]);
E 2
    }

    FREE(froms);
    FREE(tos);
    FREE(pos);
}


/*  The function matching_vector determines if the vector specified by	*/
/*  the input parameter matches a previously considered	vector.  The	*/
/*  test at the start of the function checks if the vector represents	*/
/*  a row of shifts over terminal symbols or a row of reductions, or a	*/
/*  column of shifts over a nonterminal symbol.  Berkeley Yacc does not	*/
/*  check if a column of shifts over a nonterminal symbols matches a	*/
/*  previously considered vector.  Because of the nature of LR parsing	*/
/*  tables, no two columns can match.  Therefore, the only possible	*/
/*  match would be between a row and a column.  Such matches are	*/
/*  unlikely.  Therefore, to save time, no attempt is made to see if a	*/
/*  column matches a previously considered vector.			*/
/*									*/
/*  Matching_vector is poorly designed.  The test could easily be made	*/
/*  faster.  Also, it depends on the vectors being in a specific	*/
/*  order.								*/

int
matching_vector(vector)
int vector;
{
    register int i;
    register int j;
    register int k;
    register int t;
    register int w;
    register int match;
    register int prev;

    i = order[vector];
    if (i >= 2*nstates)
	return (-1);

    t = tally[i];
    w = width[i];

    for (prev = vector - 1; prev >= 0; prev--)
    {
	j = order[prev];
	if (width[j] != w || tally[j] != t)
	    return (-1);

	match = 1;
	for (k = 0; match && k < t; k++)
	{
	    if (tos[j][k] != tos[i][k] || froms[j][k] != froms[i][k])
		match = 0;
	}

	if (match)
	    return (j);
    }

    return (-1);
}



int
pack_vector(vector)
int vector;
{
    register int i, j, k, l;
    register int t;
    register int loc;
    register int ok;
    register short *from;
    register short *to;
    int newmax;

    i = order[vector];
    t = tally[i];
    assert(t);

    from = froms[i];
    to = tos[i];

    j = lowzero - from[0];
    for (k = 1; k < t; ++k)
	if (lowzero - from[k] > j)
	    j = lowzero - from[k];
    for (;; ++j)
    {
	if (j == 0)
	    continue;
	ok = 1;
	for (k = 0; ok && k < t; k++)
	{
	    loc = j + from[k];
	    if (loc >= maxtable)
	    {
		if (loc >= MAXTABLE)
		    fatal("maximum table size exceeded");

		newmax = maxtable;
		do { newmax += 200; } while (newmax <= loc);
D 7
		table = (short *) realloc(table, newmax*sizeof(short));
E 7
I 7
		table = (short *) REALLOC(table, newmax*sizeof(short));
E 7
		if (table == 0) no_space();
D 7
		check = (short *) realloc(check, newmax*sizeof(short));
E 7
I 7
		check = (short *) REALLOC(check, newmax*sizeof(short));
E 7
		if (check == 0) no_space();
		for (l  = maxtable; l < newmax; ++l)
		{
		    table[l] = 0;
		    check[l] = -1;
		}
		maxtable = newmax;
	    }

	    if (check[loc] != -1)
		ok = 0;
	}
	for (k = 0; ok && k < vector; k++)
	{
	    if (pos[k] == j)
		ok = 0;
	}
	if (ok)
	{
	    for (k = 0; k < t; k++)
	    {
		loc = j + from[k];
		table[loc] = to[k];
		check[loc] = from[k];
		if (loc > high) high = loc;
	    }

	    while (check[lowzero] != -1)
		++lowzero;

	    return (j);
	}
    }
}



output_base()
{
    register int i, j;

D 2
    fprintf(output_file, "YYCONST short yysindex[] = {%31d,", base[0]);
E 2
I 2
D 8
    fprintf(output_file, "short yysindex[] = {%39d,", base[0]);
E 8
I 8
    fprintf(output_file, "short %ssindex[] = {%39d,", symbol_prefix, base[0]);
E 8
E 2

    j = 10;
    for (i = 1; i < nstates; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
	else
	    ++j;

	fprintf(output_file, "%5d,", base[i]);
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
D 2
    fprintf(output_file, "\n};\nYYCONST short yyrindex[] = {%31d,",
E 2
I 2
D 8
    fprintf(output_file, "\n};\nshort yyrindex[] = {%39d,",
E 8
I 8
    fprintf(output_file, "\n};\nshort %srindex[] = {%39d,", symbol_prefix,
E 8
E 2
	    base[nstates]);

    j = 10;
    for (i = nstates + 1; i < 2*nstates; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
	else
	    ++j;

	fprintf(output_file, "%5d,", base[i]);
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
D 2
    fprintf(output_file, "\n};\nYYCONST short yygindex[] = {%31d,",
E 2
I 2
D 8
    fprintf(output_file, "\n};\nshort yygindex[] = {%39d,",
E 8
I 8
    fprintf(output_file, "\n};\nshort %sgindex[] = {%39d,", symbol_prefix,
E 8
E 2
	    base[2*nstates]);

    j = 10;
    for (i = 2*nstates + 1; i < nvectors - 1; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
	else
	    ++j;

	fprintf(output_file, "%5d,", base[i]);
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");
    FREE(base);
}



output_table()
{
    register int i;
    register int j;

    ++outline;
D 2
    fprintf(output_file, "#define\tYYTABLESIZE\t\t%d\n", high);
    fprintf(output_file, "YYCONST short yytable[] = {%32d,", table[0]);
E 2
I 2
D 7
    fprintf(output_file, "#define YYTABLESIZE %d\n", high);
E 7
I 7
    fprintf(code_file, "#define YYTABLESIZE %d\n", high);
E 7
D 8
    fprintf(output_file, "short yytable[] = {%40d,", table[0]);
E 8
I 8
    fprintf(output_file, "short %stable[] = {%40d,", symbol_prefix,
	    table[0]);
E 8
E 2

    j = 10;
    for (i = 1; i <= high; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
	else
	    ++j;

	fprintf(output_file, "%5d,", table[i]);
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");
    FREE(table);
}



output_check()
{
    register int i;
    register int j;

D 2
    fprintf(output_file, "YYCONST short yycheck[] = {%32d,", check[0]);
E 2
I 2
D 8
    fprintf(output_file, "short yycheck[] = {%40d,", check[0]);
E 8
I 8
    fprintf(output_file, "short %scheck[] = {%40d,", symbol_prefix,
	    check[0]);
E 8
E 2

    j = 10;
    for (i = 1; i <= high; i++)
    {
	if (j >= 10)
	{
D 7
	    ++outline;
E 7
I 7
	    if (!rflag) ++outline;
E 7
	    putc('\n', output_file);
	    j = 1;
	}
	else
	    ++j;

	fprintf(output_file, "%5d,", check[i]);
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");
    FREE(check);
}


int
is_C_identifier(name)
char *name;
{
    register char *s;
    register int c;

    s = name;
    c = *s;
    if (c == '"')
    {
	c = *++s;
	if (!isalpha(c) && c != '_' && c != '$')
	    return (0);
	while ((c = *++s) != '"')
	{
	    if (!isalnum(c) && c != '_' && c != '$')
		return (0);
	}
	return (1);
    }

    if (!isalpha(c) && c != '_' && c != '$')
	return (0);
    while (c = *++s)
    {
	if (!isalnum(c) && c != '_' && c != '$')
	    return (0);
    }
    return (1);
}


output_defines()
{
    register int c, i;
    register char *s;

    for (i = 2; i < ntokens; ++i)
    {
	s = symbol_name[i];
	if (is_C_identifier(s))
	{
D 7
	    fprintf(output_file, "#define ");
E 7
I 7
	    fprintf(code_file, "#define ");
E 7
	    if (dflag) fprintf(defines_file, "#define ");
	    c = *s;
	    if (c == '"')
	    {
		while ((c = *++s) != '"')
		{
D 7
		    putc(c, output_file);
E 7
I 7
		    putc(c, code_file);
E 7
		    if (dflag) putc(c, defines_file);
		}
	    }
	    else
	    {
		do
		{
D 7
		    putc(c, output_file);
E 7
I 7
		    putc(c, code_file);
E 7
		    if (dflag) putc(c, defines_file);
		}
		while (c = *++s);
	    }
	    ++outline;
D 7
	    fprintf(output_file, " %d\n", symbol_value[i]);
E 7
I 7
	    fprintf(code_file, " %d\n", symbol_value[i]);
E 7
	    if (dflag) fprintf(defines_file, " %d\n", symbol_value[i]);
	}
    }

    ++outline;
D 7
    fprintf(output_file, "#define YYERRCODE %d\n", symbol_value[1]);
E 7
I 7
    fprintf(code_file, "#define YYERRCODE %d\n", symbol_value[1]);
E 7

    if (dflag && unionized)
    {
	fclose(union_file);
	union_file = fopen(union_file_name, "r");
	if (union_file == NULL) open_error(union_file_name);
	while ((c = getc(union_file)) != EOF)
	    putc(c, defines_file);
D 8
	fprintf(defines_file, " YYSTYPE;\nextern YYSTYPE yylval;\n");
E 8
I 8
	fprintf(defines_file, " YYSTYPE;\nextern YYSTYPE %slval;\n",
		symbol_prefix);
E 8
    }
}


output_stored_text()
{
    register int c;
    register FILE *in, *out;

    fclose(text_file);
    text_file = fopen(text_file_name, "r");
D 7
    if (text_file == NULL) open_error(text_file_name);
E 7
I 7
    if (text_file == NULL)
	open_error(text_file_name);
E 7
    in = text_file;
D 7
    out = output_file;
E 7
    if ((c = getc(in)) == EOF)
	return;
D 7
    if (c == '\n') ++outline;
E 7
I 7
    out = code_file;
    if (c ==  '\n')
	++outline;
E 7
    putc(c, out);
    while ((c = getc(in)) != EOF)
    {
D 7
	if (c == '\n') ++outline;
E 7
I 7
	if (c == '\n')
	    ++outline;
E 7
	putc(c, out);
    }
    if (!lflag)
D 7
    {
	++outline;
D 3
	fprintf(out, line_format, outline, output_file_name);
E 3
I 3
	fprintf(out, line_format, outline + 1, output_file_name);
E 3
    }
E 7
I 7
	fprintf(out, line_format, ++outline + 1, code_file_name);
E 7
}


output_debug()
{
    register int i, j, k, max;
    char **symnam, *s;

    ++outline;
D 7
    fprintf(output_file, "#define YYFINAL %d\n", final_state);
E 7
I 7
    fprintf(code_file, "#define YYFINAL %d\n", final_state);
E 7
    outline += 3;
D 7
    fprintf(output_file, "#ifndef YYDEBUG\n#define YYDEBUG %d\n#endif\n",
E 7
I 7
    fprintf(code_file, "#ifndef YYDEBUG\n#define YYDEBUG %d\n#endif\n",
E 7
	    tflag);
I 7
    if (rflag)
	fprintf(output_file, "#ifndef YYDEBUG\n#define YYDEBUG %d\n#endif\n",
		tflag);
E 7

    max = 0;
    for (i = 2; i < ntokens; ++i)
	if (symbol_value[i] > max)
	    max = symbol_value[i];
    ++outline;
D 7
    fprintf(output_file, "#define YYMAXTOKEN %d\n", max);
E 7
I 7
    fprintf(code_file, "#define YYMAXTOKEN %d\n", max);
E 7

D 2
    symnam = (char **) MALLOC(max*sizeof(char *));
E 2
I 2
    symnam = (char **) MALLOC((max+1)*sizeof(char *));
E 2
    if (symnam == 0) no_space();
I 7

    /* Note that it is  not necessary to initialize the element		*/
    /* symnam[max].							*/
E 7
    for (i = 0; i < max; ++i)
	symnam[i] = 0;
    for (i = ntokens - 1; i >= 2; --i)
	symnam[symbol_value[i]] = symbol_name[i];
    symnam[0] = "end-of-file";

D 7
    ++outline;
E 7
I 7
    if (!rflag) ++outline;
E 7
D 2
    fprintf(output_file, "#if YYDEBUG\nYYCONST char *yyname[] = {");
E 2
I 2
D 8
    fprintf(output_file, "#if YYDEBUG\nchar *yyname[] = {");
E 8
I 8
    fprintf(output_file, "#if YYDEBUG\nchar *%sname[] = {", symbol_prefix);
E 8
E 2
    j = 80;
    for (i = 0; i <= max; ++i)
    {
	if (s = symnam[i])
	{
	    if (s[0] == '"')
	    {
		k = 7;
		while (*++s != '"')
		{
I 7
		    ++k;
E 7
		    if (*s == '\\')
		    {
			k += 2;
			if (*++s == '\\')
D 7
			    k += 2;
			else
E 7
			    ++k;
		    }
D 7
		    else
			++k;
E 7
		}
		j += k;
		if (j > 80)
		{
D 7
		    ++outline;
E 7
I 7
		    if (!rflag) ++outline;
E 7
		    putc('\n', output_file);
		    j = k;
		}
		fprintf(output_file, "\"\\\"");
		s = symnam[i];
		while (*++s != '"')
		{
		    if (*s == '\\')
		    {
			fprintf(output_file, "\\\\");
			if (*++s == '\\')
			    fprintf(output_file, "\\\\");
			else
			    putc(*s, output_file);
		    }
		    else
			putc(*s, output_file);
		}
		fprintf(output_file, "\\\"\",");
	    }
	    else if (s[0] == '\'')
	    {
		if (s[1] == '"')
		{
		    j += 7;
		    if (j > 80)
		    {
D 7
			++outline;
E 7
I 7
			if (!rflag) ++outline;
E 7
			putc('\n', output_file);
			j = 7;
		    }
		    fprintf(output_file, "\"'\\\"'\",");
		}
		else
		{
		    k = 5;
		    while (*++s != '\'')
		    {
I 7
			++k;
E 7
			if (*s == '\\')
			{
			    k += 2;
D 7
			    ++s;
E 7
			    if (*++s == '\\')
D 7
				k += 2;
			    else
E 7
				++k;
			}
D 7
			else
			    ++k;
E 7
		    }
		    j += k;
		    if (j > 80)
		    {
D 7
			++outline;
E 7
I 7
			if (!rflag) ++outline;
E 7
			putc('\n', output_file);
			j = k;
		    }
		    fprintf(output_file, "\"'");
		    s = symnam[i];
		    while (*++s != '\'')
		    {
			if (*s == '\\')
			{
			    fprintf(output_file, "\\\\");
			    if (*++s == '\\')
				fprintf(output_file, "\\\\");
			    else
				putc(*s, output_file);
			}
			else
			    putc(*s, output_file);
		    }
		    fprintf(output_file, "'\",");
		}
	    }
	    else
	    {
		k = strlen(s) + 3;
		j += k;
		if (j > 80)
		{
D 7
		    ++outline;
E 7
I 7
		    if (!rflag) ++outline;
E 7
		    putc('\n', output_file);
		    j = k;
		}
		putc('"', output_file);
		do { putc(*s, output_file); } while (*++s);
		fprintf(output_file, "\",");
	    }
	}
	else
	{
	    j += 2;
	    if (j > 80)
	    {
D 7
		++outline;
E 7
I 7
		if (!rflag) ++outline;
E 7
		putc('\n', output_file);
		j = 2;
	    }
	    fprintf(output_file, "0,");
	}
    }
D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "\n};\n");
    FREE(symnam);

D 7
    ++outline;
E 7
I 7
    if (!rflag) ++outline;
E 7
D 2
    fprintf(output_file, "YYCONST char *yyrule[] = {\n");
E 2
I 2
D 8
    fprintf(output_file, "char *yyrule[] = {\n");
E 8
I 8
    fprintf(output_file, "char *%srule[] = {\n", symbol_prefix);
E 8
E 2
    for (i = 2; i < nrules; ++i)
    {
	fprintf(output_file, "\"%s :", symbol_name[rlhs[i]]);
	for (j = rrhs[i]; ritem[j] > 0; ++j)
	{
	    s = symbol_name[ritem[j]];
	    if (s[0] == '"')
	    {
		fprintf(output_file, " \\\"");
		while (*++s != '"')
		{
		    if (*s == '\\')
		    {
			if (s[1] == '\\')
			    fprintf(output_file, "\\\\\\\\");
			else
			    fprintf(output_file, "\\\\%c", s[1]);
			++s;
		    }
		    else
			putc(*s, output_file);
		}
		fprintf(output_file, "\\\"");
	    }
	    else if (s[0] == '\'')
	    {
		if (s[1] == '"')
		    fprintf(output_file, " '\\\"'");
		else if (s[1] == '\\')
		{
		    if (s[2] == '\\')
			fprintf(output_file, " '\\\\\\\\");
		    else
			fprintf(output_file, " '\\\\%c", s[2]);
		    s += 2;
		    while (*++s != '\'')
			putc(*s, output_file);
		    putc('\'', output_file);
		}
		else
		    fprintf(output_file, " '%c'", s[1]);
	    }
	    else
		fprintf(output_file, " %s", s);
	}
D 7
	++outline;
E 7
I 7
	if (!rflag) ++outline;
E 7
	fprintf(output_file, "\",\n");
    }

D 7
    outline += 2;
E 7
I 7
    if (!rflag) outline += 2;
E 7
    fprintf(output_file, "};\n#endif\n");
}


output_stype()
{
D 2
    if (!unionized)
E 2
I 2
    if (!unionized && ntags == 0)
E 2
    {
	outline += 3;
D 7
	fprintf(output_file, "#ifndef YYSTYPE\ntypedef int YYSTYPE;\n#endif\n");
E 7
I 7
	fprintf(code_file, "#ifndef YYSTYPE\ntypedef int YYSTYPE;\n#endif\n");
E 7
    }
}


output_trailing_text()
{
    register int c, last;
I 7
    register FILE *in, *out;
E 7

    if (line == 0)
	return;

I 7
    in = input_file;
    out = code_file;
E 7
    c = *cptr;
    if (c == '\n')
    {
	++lineno;
D 7
	if ((c = getc(input_file)) == EOF)
E 7
I 7
	if ((c = getc(in)) == EOF)
E 7
	    return;
	if (!lflag)
	{
	    ++outline;
D 7
	    fprintf(output_file, line_format, lineno, input_file_name);
E 7
I 7
	    fprintf(out, line_format, lineno, input_file_name);
E 7
	}
D 7
	if (c == '\n') ++outline;
	putc(c, output_file);
E 7
I 7
	if (c == '\n')
	    ++outline;
	putc(c, out);
E 7
	last = c;
    }
    else
    {
	if (!lflag)
	{
	    ++outline;
D 7
	    fprintf(output_file, line_format, lineno, input_file_name);
E 7
I 7
	    fprintf(out, line_format, lineno, input_file_name);
E 7
	}
D 7
	do { putc(c, output_file); } while ((c = *++cptr) != '\n');
E 7
I 7
	do { putc(c, out); } while ((c = *++cptr) != '\n');
E 7
	++outline;
D 7
	putc('\n', output_file);
E 7
I 7
	putc('\n', out);
E 7
	last = '\n';
    }

D 7
    while ((c = getc(input_file)) != EOF)
E 7
I 7
    while ((c = getc(in)) != EOF)
E 7
    {
D 7
	if (c == '\n') ++outline;
	putc(c, output_file);
E 7
I 7
	if (c == '\n')
	    ++outline;
	putc(c, out);
E 7
	last = c;
    }

    if (last != '\n')
    {
	++outline;
D 7
	putc('\n', output_file);
E 7
I 7
	putc('\n', out);
E 7
    }
    if (!lflag)
D 7
    {
	++outline;
D 3
	fprintf(output_file, line_format, outline, output_file_name);
E 3
I 3
	fprintf(output_file, line_format, outline + 1, output_file_name);
E 3
    }
E 7
I 7
	fprintf(out, line_format, ++outline + 1, code_file_name);
E 7
}


output_semantic_actions()
{
    register int c, last;
I 7
    register FILE *out;
E 7

    fclose(action_file);
    action_file = fopen(action_file_name, "r");
D 7
    if (action_file == NULL) open_error(action_file_name);
E 7
I 7
    if (action_file == NULL)
	open_error(action_file_name);
E 7

    if ((c = getc(action_file)) == EOF)
	return;
I 7

    out = code_file;
E 7
    last = c;
D 7
    if (c == '\n') ++outline;
    putc(c, output_file);
E 7
I 7
    if (c == '\n')
	++outline;
    putc(c, out);
E 7
    while ((c = getc(action_file)) != EOF)
    {
D 7
	if (c == '\n') ++outline;
	putc(c, output_file);
E 7
I 7
	if (c == '\n')
	    ++outline;
	putc(c, out);
E 7
	last = c;
    }

    if (last != '\n')
    {
	++outline;
D 7
	putc('\n', output_file);
E 7
I 7
	putc('\n', out);
E 7
    }
I 7

E 7
    if (!lflag)
D 7
    {
	++outline;
D 4
	fprintf(output_file, line_format, outline, output_file_name);
E 4
I 4
	fprintf(output_file, line_format, outline + 1, output_file_name);
E 4
    }
E 7
I 7
	fprintf(out, line_format, ++outline + 1, code_file_name);
E 7
}


free_itemsets()
{
D 2
  register core *cp;
E 2
I 2
    register core *cp, *next;
E 2

D 2
  FREE(state_table);

  for (cp = first_state; cp; cp = cp->next)
    FREE(cp);
E 2
I 2
    FREE(state_table);
    for (cp = first_state; cp; cp = next)
    {
	next = cp->next;
	FREE(cp);
    }
E 2
}


D 2

E 2
free_shifts()
{
D 2
  register shifts *sp;
E 2
I 2
    register shifts *sp, *next;
E 2

D 2
  FREE(shift_table);

  for (sp = first_shift; sp; sp = sp->next)
    FREE(sp);
E 2
I 2
    FREE(shift_table);
    for (sp = first_shift; sp; sp = next)
    {
	next = sp->next;
	FREE(sp);
    }
E 2
}



free_reductions()
{
D 2
  register reductions *rp;
E 2
I 2
    register reductions *rp, *next;
E 2

D 2
  FREE(reduction_table);

  for (rp = first_reduction; rp; rp = rp->next)
    FREE(rp);
E 2
I 2
    FREE(reduction_table);
    for (rp = first_reduction; rp; rp = next)
    {
	next = rp->next;
	FREE(rp);
    }
E 2
}
E 1
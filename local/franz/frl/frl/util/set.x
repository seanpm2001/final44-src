(File |frl/util/set.l|)
(sremove lambda append delete member cond)
(sremoveq lambda append delq memq cond)
(sadd lambda cons member null cond)
(saddq lambda cons memq null cond)
(intersect2 lambda cdr cons setq car member and nreverse null do)
(intersectq2 lambda cdr cons setq car memq and nreverse null do)
(intersectq? lambda cdr return car memq and if do null cond)
(intersect? lambda cdr return car member and if do null cond)
(intersectq lexpr |1+| cdr setq declare > do arg intersectq2 eq = cond)
(intersect lexpr |1+| cdr setq declare > do arg intersect2 eq = cond)
(union2 lambda cdr cons setq car member or null do setminus2 nreverse nconc)
(unionq2 lambda cdr cons setq car memq or null do setminusq2 nreverse nconc)
(unionq lexpr |1+| cdr setq declare > do arg unionq2 eq = cond)
(union lexpr |1+| cdr setq declare > do arg union2 eq = cond)
(setminusq lambda cdr cons setq car memq or null do setminusq2 nreverse)
(setminus lambda cdr cons setq car member or null do setminus2 nreverse)
(setminus2 macro cons quote list caddr cadr)
(setminusq2 macro cons quote list caddr cadr)
(setifyq lambda cons setq memq not nreverse cdr car list do null cond)
(setify lambda cons setq member not nreverse cdr car list do null cond)

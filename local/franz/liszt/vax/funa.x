(File ../funa.l)
(cc-cxxr lambda d-inreg d-allocreg cons e-cvt not cddr append list e-write2 e-tst d-movespec do eq or dtpr d-exp let d-simple caddr cadr car d-bestreg setq symbolp and return e-goto cdr d-handlecc quote d-move null cond if prog)
(d-superrplacx lambda |1-| decr cdr unpush |1+| incr cons Push e-move d-clearreg e-cvt e-write3 d-structgen d-exp d-move d-semisimple and d-simple not eq or d-fixnumexp quote list setq fixp cond if cadddr caddr cadr let)
(c-rplacx lambda quote d-superrplacx)
(d-structgen lambda cons cdr * setq d-clearreg atom not cadr list e-cvt append quote d-move car eq dtpr and cond if)
(d-semisimple lambda caadadr cadadr caadr cadr car quote eq dtpr and d-simple or)
(d-supercxr lambda e-tst d-handlecc e-call-qnewint e-goto car d-move d-clearreg e-quick-call e-write3 d-structgen e-move C-pop d-exp e-cvt e-write2 C-push d-semisimple eq and d-simple not d-fixnumexp quote list setq fixp cond if caddr cadr let)
(cc-cxr lambda d-supercxr)
(c-cons lambda d-clearreg - cddr setq quote e-write2 e-quick-call cdr d-pushargs)
(c-cond lambda d-clearreg e-goto copy d-exps equal eq caar d-exp and let cdar *throw throw |1+| terpri niceprint patom comp-msg progn comp-err car atom e-label d-move if null or d-genlab cdr do quote cons setq cond makecomment)
(d-popframe lambda cdr + e-write4 e-add3 list e-move quote let)
(d-pushframe lambda cons setq setf push e-quick-call e-move null cond if list e-cvt quote e-write2 C-push)
(c-*catch lambda d-clearreg cdr unpush d-popframe caddr e-label e-cvt e-move e-write2 e-tst cons setq setf push d-pushframe cadr d-exp d-genlab quote let)
(d-boolexlate lambda list cons setq makecomment cadddr caddr d-boolexlate let cadr fixp car quote eq and atom cond if)
(d-booleexpand lambda cddddr append cadddr caddr list cadr cons d-booleexpand =& = *throw throw |1+| setq terpri niceprint patom comp-msg progn comp-err length cdr <& < car quote eq dtpr and cond if)
(c-boole lambda d-exp let cdr d-callbig car quote eq d-booleexpand d-boolexlate setq cadr fixp cond)
(cc-bigp lambda cadr d-typesimp)
(cc-bcdp lambda cadr d-typesimp)
(c-bcdcall lambda cdr d-callbig)
(cc-atom lambda cadr d-typecmplx)
(cm-assq lambda caddr cadr list quote cons)
(c-assembler-code lambda car e-write1 null cdr do quote cons cond makecomment setq)
(cc-arg lambda e-label e-gotonil e-write3 e-cmp d-cmp d-exp cons let e-goto =& or d-handlecc e-write2 e-tst e-cvt e-move e-write4 e-sub3 quote list d-move >& fixp return null cadr car cdr length and *throw throw |1+| terpri niceprint patom comp-msg progn comp-err eq not cond if d-genlab setq prog)
(cc-and lambda d-clearreg d-move e-label e-goto setq car cons do d-exp null quote cdr cond if d-genlab let)

(File |talk/frmish.l|)
(ftalk lambda talk cdr car #fload null quote setq liszt-internal-do mapc)
(framish-listen-test lambda cdr terpri $prpr car null readl2 nreverse bracket-string feed-string-to-pidgin setq liszt-internal-do mapc #print do *catch catch quote shout0)
(framish-listen lambda cdr terpri $prpr car null request-pidgin setq liszt-internal-do mapc #print do *catch catch quote shout0)
(request-choice lambda go shout0 return member and if quote request2 setq prog)
(select-data lambda nreverse cdr cons setq null liszt-internal-do mapcar findicators1 fextract-data |1st| car findicator fextract-datum quote fcomment? list atom cond)
(select-datum lambda request-choice cdr |1st| car findicator fextract-datum quote fcomment? atom cond)
(convert-path-all lambda fix-quote-sign construct-indicator-path quote list path? cond)
(convert-path lambda fix-quote-sign construct-indicator-path quote list path? cond)
(quotable-atom lambda quote get not atom and)
(kwote-if-atom lambda quote list kwote quotable-atom cond)
(check-unquote lambda list quote get atom and cond)
(unquote macro cadr)
(fix-quote-sign lambda cadr |2nd| car eq quote list kwote quotable-atom cond)
(process-indicators lambda nreverse cdr car fix-quote-sign cons setq null merge-framish-context liszt-internal-do mapcar)
(construct-indicator-path* lambda process-indicators cdddr rplaca rplac4 cadddr |4th| pathfacet null and if)
(construct-indicator-path lambda pathify process-indicators)
(framish-display macro kwote-if-atom |1st| eq |3rd| list caddr car construct-indicator-path for quote cdr member cadr |2nd| dtpr and cond progn)
(framish-remove macro eq append cons car |1st| list quote cadr |2nd| equal or cond caddr |3rd| for progn)
(framish-replace macro caddr |3rd| car |1st| quote list cadr |2nd| for progn)
(framish-put macro quote list caddr cadr)
(framish-say macro caddr |3rd| car |1st| quote list cadr |2nd| for progn)
(framish-use macro car |1st| eq cond cadr |2nd| caseq quote list caddr |3rd| for progn)
(framish-get macro caddr |3rd| car |1st| quote list cadr |2nd| for progn)
(framish-quantify macro cadddr |4th| car |1st| list append caddr |3rd| quote cadr |2nd| cond for progn)
(framish-select macro car |1st| cadddr |4th| cons list caddr |3rd| quote cadr |2nd| cond for progn)
(framish-are-? macro car |1st| caddr |3rd| quote list cadr |2nd| for progn)
(framish-is-? macro car |1st| caddr |3rd| quote list cadr |2nd| for progn)
(framish-are macro caddr |3rd| car |1st| cons equal cond quote list cadr |2nd| for progn)
(framish-is macro caddr |3rd| car |1st| list quote equal cond cadr |2nd| for progn)
(comparative-with-is lambda saddq setq)
(comparative-left-argument lambda pathify atom cond)
(reset-framish-context lambda quote setq)
(display-framish-context lambda symeval boundp cond shout-indented indent-to shout0 car setq null quote do do-foreach linel cdr - for)
(merge-current-facet lambda or)
(merge-current-slot lambda or)
(merge-current-frame lambda or)
(merge-framish-context lambda cadddr |4th| merge-current-facet caddr |3rd| merge-current-slot cadr |2nd| merge-current-frame list)
(current-framish-context lambda merge-framish-context)
(set-framish-context lambda current-framish-context cdr set and if car setq null quote do)
(path macro construct-indicator-path quote list progn)
(*path macro construct-indicator-path* quote list progn)
(recognize-facet lambda car return caar eq cond null do assq cdr)
(newframe lambda check-unquote cdr rplaca rplac2 cadr |2nd| setq path? and if)
(newslot lambda check-unquote cddr rplaca rplac3 caddr |3rd| setq path? and if)
(newfacet lambda recognize-facet or check-unquote cdddr rplaca rplac4 cadddr |4th| setq path? and if)
(*pathify lambda rplaca rplac1)
(pathify lambda check-unquote buildpath recognize-facet setq return path? cond prog)
(buildpath lambda recognize-facet or cadddr |4th| caddr |3rd| cadr |2nd| path? cond check-unquote quote list)
(path? lambda car typep eq listp and)
(pathfacet macro quote list cadr)
(pathslot macro quote list cadr)
(pathframe macro quote list cadr)
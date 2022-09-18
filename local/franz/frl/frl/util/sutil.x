(File |frl/util/sutil.l|)
(instances lambda nreverse cdr atom car cons setq null quote *fdata liszt-internal-do mapcar findicators1 *fdata-only *fvalues-only frame? cond)
(name-val lambda error return caar null do assq cdr cons setq car cadr eq quote list request6 for atom cond)
(test-function nlambda request6 name-val cons nreverse null foreach apply setq quote list shout affirmative? not do cdr car for)
(isa? lambda ako? frame? and)
(has-property lambda nreverse cdr atom cond car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only fvalues-only member)
(has-properties nlambda cdddr cons quote apply caddr eval cadr car has-property cdr null cond)
(find-or-make nlambda null cond quote apply for)
(find-frames nlambda cdr cons apply and setq nreverse null quote car fdescendants do filter)
(find-frame nlambda return cdr cons apply and setq null quote car fdescendants do exists)
(add-to-frame nlambda cdddr cons apply quote cadr fput-datum fput caddr eval cdr null cond car for)
(make-frame nlambda cdr cons quote apply cadar car atom cond myinstantiate for)
(myinstantiate lexpr list request6 setarg cons |1-| + cdr < do minusp eq let listify quote apply arg frame? cond)
(new-example nlambda quote apply ask-about-frame)
(definition lambda caddr cadr cond cdr return equal or atom not and car setq null quote finherit do exists for)
(slots-of lambda fheritage-slots setminus)
(feature? lambda nreverse cdr atom cond car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only member)
(features lambda nreverse cdr atom cond car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only)
(feature lambda quote fput-datum fput)
(ask-and-add-n lambda ask-and-add-1 another-fn quote list request6 cond)
(ask-and-add-1 lambda ask-and-add-1 quote list shout fadd-value-if-reqs-ok cond and ask-about-slot for)
(ask-and-add-slots lambda cdr ask-and-add car setq null do do-foreach)
(ask-and-add lambda ask-and-add-1 cons null ask-and-add-n do quote feature? cond)
(fadd-value-if-reqs-ok lambda quote fput-datum fput check-reqs-ok cond)
(check-reqs-ok lambda complain list shout and nreverse cdr atom car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only fpoll-summary cond)
(between-complain lambda quote list shout lessp cond)
(between-req lambda greaterp lessp or not)
(ako-complain lambda quote list shout)
(ako-req lambda ako? frame? and)
(complain-fn lambda caddr cadr list quote car equal cond null not and)
(complain lambda terpri pprin1 mapc list shout not setf push eval get-msg complain-fn or foreach return caar eq do assq cadr fpoll nreverse cdr atom cond car cons setq null liszt-internal-do mapcar findicators1 fdata-only quote fdata for)
(get-msg lambda fcomment? cadr cdr return equal and car setq null do exists for)
(exists-named? lambda return nreverse cdr atom cond cons fdata liszt-internal-do mapcar findicators1 fdata-only fvalues-only inspect-datum fvalue-only equal and car setq null quote fdescendants do exists)
(named lambda ask-about-frame quote fput-datum fput finstantiate for exists-named? cond)
(process-answer lambda named quote list request6 affirmative? exists-named? or frame? numberp not atom and cond definition for)
(ask-about-slot lambda default-slot-ask eval process-answer nreverse cdr atom cond car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only inspect-datum fdatum-only for)
(default-slot-ask lambda another-fn list request6 quote feature? cond)
(another-fn lambda nreverse cdr atom car cons setq quote fdata liszt-internal-do mapcar findicators1 fdata-only fvalues-only null cond)
(default-frame-ask lambda slots-of fill-slots)
(ask-about-frame lambda default-frame-ask eval nreverse cdr atom cond car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only inspect-datum fdatum-only for)
(fill-slots lambda cdr fill-slot car setq null do do-foreach)
(fill-slot lambda ask-and-add nreverse cdr atom car cons setq null fdata liszt-internal-do mapcar findicators1 fdata-only fvalues-only not quote feature? or cond)
(ask-for-type lambda ask-about-frame list shout quote fput-datum fput finstantiate for)
(describe-frame lexpr default-frame-describe eval nreverse cdr atom car cons setq null fdata liszt-internal-do mapcar findicators1 fdata-only inspect-datum fdatum-only quote list shout eq cond arg for)
(default-frame-describe lambda cdr describe-slot cons car setq nreverse null slots-of do foreach)
(default-slot-describe lambda list append shout nreverse cdr atom cond car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only fvalues-only for)
(describe-slots lambda cdr describe-slot cons car setq nreverse null do foreach)
(describe-slot lambda default-slot-describe eval nreverse cdr atom cond car cons setq null quote fdata liszt-internal-do mapcar findicators1 fdata-only inspect-datum fdatum-only for)
(fgensym lambda explode append implode |1+| setq)

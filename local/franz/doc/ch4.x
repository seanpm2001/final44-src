.In 4 2 and "[g_arg1 ...]"
.In 4 2 apply "'u_func 'l_args"
.In 4 1 arg "['x_numb]"
.In 4 1 break "[g_message ['g_pred]]"
.In 4 1 *break "'g_pred 'g_message"
.In 4 1 caseq "'g_key-form l_clause1 ..."
.In 4 2 catch "g_exp [ls_tag]"
.In 4 2 *catch "'ls_tag g_exp"
.In 4 3 comment "[g_arg ...]"
.In 4 3 cond "[l_clause1 ...]"
.In 4 3 cvttointlisp ""
.In 4 3 cvttofranzlisp ""
.In 4 3 cvttomaclisp ""
.In 4 3 cvttoucilisp ""
.In 4 4 debug "s_msg"
.In 4 4 debugging "'g_arg"
.In 4 4 declare "[g_arg ...]"
.In 4 4 def "s_name (s_type l_argl g_exp1 ...)"
.In 4 4 defmacro "s_name l_arg g_exp1 ..."
.In 4 4 defcmacro "s_name l_arg g_exp1 ..."
.In 4 4 defun "s_name [s_mtype] ls_argl g_exp1 ... "
.In 4 5 defvar "s_variable ['g_init]"
.In 4 5 do "l_vrbs l_test g_exp1 ..."
.In 4 6 do "s_name g_init g_repeat g_test g_exp1 ..."
.In 4 7 environment "[l_when1 l_what1 l_when2 l_what2 ...]"
.In 4 7 environment-maclisp "[l_when1 l_what1 l_when2 l_what2 ...]"
.In 4 7 environment-lmlisp "[l_when1 l_what1 l_when2 l_what2 ...]"
.In 4 7 err "['s_value [nil]]"
.In 4 7 error "['s_message1 ['s_message2]]"
.In 4 7 errset "g_expr [s_flag]"
.In 4 8 eval "'g_val ['x_bind-pointer]"
.In 4 8 evalframe "'x_pdlpointer"
.In 4 9 evalhook "'g_form 'su_evalfunc ['su_funcallfunc]"
.In 4 9 eval-when "l_times g_exp1 ... g_expn"
.In 4 9 exec "s_arg1 ..."
.In 4 10 exece "'s_fname ['l_args ['l_envir]]"
.In 4 10 freturn "'x_pdl-pointer 'g_retval"
.In 4 10 frexp "'f_arg"
.In 4 10 funcall "'u_func ['g_arg1 ...]"
.In 4 10 funcallhook "'l_form 'su_funcallfunc ['su_evalfunc]"
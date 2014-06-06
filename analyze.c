#include "analyze.h"

int _countSymbol(struct symtab *from, char *this){
	int i = 0;
	Elem *e;
	for(e = list_begin(from->symbols);
		e != list_end(from->symbols);
		e = list_next(e)){
			struct _symbol_common *c = list_entry(e, struct _symbol_common, symelem);
			if(strcmp(c->name, this) == 0){
				i++;
			}
	}
	return i;
}

void _typeCheck(struct _common *data, struct symtab *_context, bool _isAbleVoidCall){
	if(data)
	switch(data->type){
		case var_declaration:
			{
				struct var_declaration *vd = (struct var_declaration *)data;
			  	// 1. 같은 scope에 중복된 이름이 있는지?
				if(_countSymbol(_context, vd->name) > 1){
					printf("%3d:%2d\t같은 이름(%s)의 변수가 있습니다.\n", vd->line+1, vd->cur+1, vd->name);
				}
				// 2. void type이면 안됨.
				if(strcmp(vd->type_specifier, "void") == 0){
					printf("%3d:%2d\tVoid형의 변수(%s)는 선언할 수 없습니다.\n", vd->line+1, vd->cur+1, vd->name);
				}
			}
			break;
		case fun_declaration:
			{
				struct fun_declaration *fd = (struct fun_declaration *)data;
				// 1. 같은 scope에 중복된 이름이 있는지?
				if(_countSymbol(_context, fd->name) > 1){
					printf("%3d:%2d\t같은 이름(%s)의 함수가 있습니다.\n", fd->line+1, fd->cur+1, fd->name);
				}
				// 2. global에서 함수선언인지?
				if(strcmp(_context->name, "Global")){
					printf("%3d:%2d\t %s 함수는 Global에서 선언해야 합니다.\n", fd->line+1, fd->cur+1, fd->name);
				}
				// 3. main함수일경우
				if(strcmp(fd->name, "main") == 0){
					// 3-1. 가장 마지막에
					// TODO
					// 3-2. 리턴값이 void
					if(strcmp(fd->type_specifier, "void")){
						printf("%3d:%2d\tMain함수의 리턴값은 Void여야합니다.\n", fd->line+1, fd->cur+1);
					}
					// 3-3. 파라메터가 없어야함.
					if(fd->params){
						printf("%3d:%2d\tMain함수의 파라메터는 없어야합니다.\n", fd->line+1, fd->cur+1);
					}
				}
			}
			break;
		case param:
			{
				struct param *p = (struct param *)data;
				// 1. 같은 scope에 중복된 이름이 있는지?
				if(_countSymbol(_context, p->name) > 1){
					printf("%3d:%2d\t중복 변수(%s)입니다.\n", p->line+1, p->cur+1, p->name);
				}
				// 2. void타입인지?
				if(strcmp(p->type_specifier, "void") == 0){
					printf("%3d:%2d\t파라메타(%s)는 Void타입이면 안됩니다.\n", p->line+1, p->cur+1, p->name);
				}
			}
			break;
		case return_stmt:
			{
				struct return_stmt *rs = (struct return_stmt *)data;
				// 1. 함수선언부와 return 타입이 일치하는지
				struct symtab *func_where = searchSymtabWhere(_context, _context->name);
				struct fun_declaration *func = searchSymtab(func_where, _context->name);
				if(strcmp(func->type_specifier, "int") == 0){
					if(!rs->return_expression){
						printf("%3d:%2d\t함수(%s) 선언은 Int이나 리턴값이 없습니다.\n", rs->line+1, rs->cur+1, _context->name);
					}
				}else if(strcmp(func->type_specifier, "void") == 0){
					if(rs->return_expression){
						printf("%3d:%2d\t함수(%s) 선언은 Void이나 리턴값이 있습니다.\n", rs->line+1, rs->cur+1, _context->name);
					}
				}
			}
			break;
		case var:
			{
				struct var *v = (struct var *)data;
				// 0. 있는 변수인지.
				struct symtab *found_where = searchSymtabWhere(_context, v->name);
				struct _symbol_common *found = searchSymtabWhere(found_where, v->name);
				if(!found){
					printf("%3d:%2d\t없는 변수(%s)입니다.\n", v->line+1, v->cur+1, v->name);
				}else{
					// 1. Array로 선언 안되어있는데 Array로 썻는지.
					bool isArray;
					if(found->type == var_declaration){
						if(((struct var_declaration *)found)->size)
							isArray = true;
					}else if(found->type == param){
						isArray = ((struct param *)found)->isArray;
					}
					if(!isArray){
						if(v->array){
							printf("%3d:%2d\t배열이 아닌 변수(%s)를 배열로 사용하였습니다.\n", v->line+1, v->cur+1, v->name);
						}else{
							// XXX 배열이 아닌 변수를 배열로 사용하지 않았습니다.
						}
					}else{
						;// TODO 배열로 선언한 변수입니다. 
					}
				}
			}
			break;
		case factor:
			if(!_isAbleVoidCall){
				/* VoidCall이 안되는 다음과 같은 상황일때!
				 * 1. while/if
				 * 2. mulop/addop/..
				 */
				struct factor *f = (struct factor *)data;
				// 1. call일때!
				if(f->contenttype == call){
					char *funcname = f->content.call->name;
					struct symtab *func_where = searchSymtabWhere(_context, funcname);
					struct fun_declaration *func = searchSymtab(func_where, funcname);
					// 선언이 안되어있으면 안됨.
					if(!func){
						if(strcmp(funcname, "input") == 0){
							;
						}else if(strcmp(funcname, "output") == 0){
							;
						}else{
							printf("%3d:%2d\t선언이 안되어 있는 call(%s)입니다.!\n", f->line+1, f->cur+1, funcname);
						}
					}else{
						// 리턴값이 void면 안됨.
						if(strcmp(func->type_specifier, "void") == 0){
							printf("%3d:%2d\t%s 함수가 void를 리턴하려고 했습니다.!\n", f->line+1, f->cur+1, funcname);
						}
					}
				}
			}
			break;
		case call:
			{
				;
			}
			break;
		default:
			break;
	}
}

void _typeCheckLoop(struct _common *data, struct symtab *_context, bool _isAbleVoidCall){
	if(data)
	switch(data->type){
		case declaration_list:
			{
				struct declaration_list *dl = (struct declaration_list *)data;
				Elem *find_declaration;
				for(find_declaration = list_begin(dl->list);
					find_declaration != list_end(dl->list);
					find_declaration = list_next(find_declaration)){
						struct _declaration *this = list_entry(find_declaration, struct _declaration, elem);
						_typeCheckLoop((struct _common *)this, dl->symtab, _isAbleVoidCall);
				}
			}
			break;
		case var_declaration:
			{
				_typeCheck(data, _context, _isAbleVoidCall);
			}
			break;
		case fun_declaration:
			{
				_typeCheck(data, _context, _isAbleVoidCall);
				struct fun_declaration *fd = (struct fun_declaration *)data;
				_typeCheckLoop((struct _common *)fd->params, fd->symtab, _isAbleVoidCall);
				_typeCheckLoop((struct _common *)fd->compound_stmt, fd->symtab, _isAbleVoidCall);
			}
			break;
		case param_list:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct param_list *pl = (struct param_list *)data;
				Elem *find_param;
				for(find_param = list_begin(pl->list);
					find_param != list_end(pl->list);
					find_param = list_next(find_param)){
						struct param *this = list_entry(find_param, struct param, elem);
						_typeCheckLoop((struct _common *)this, _context, _isAbleVoidCall);
				}
			}
			break;
		case param:
			_typeCheck(data, _context, _isAbleVoidCall);
			break;
		case compound_stmt:
			{
				struct compound_stmt *cs = (struct compound_stmt *)data;
				_typeCheck(data, _context, _isAbleVoidCall);
				if(cs->symtab)
					_typeCheckLoop((struct _common *)cs->statement_list, cs->symtab, _isAbleVoidCall);
				else
					_typeCheckLoop((struct _common *)cs->statement_list, _context, _isAbleVoidCall);
			}
			break;
		case local_declarations:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct local_declarations *ld = (struct local_declarations *)data;
				Elem *find_local_declarations;
				for(find_local_declarations = list_begin(ld->list);
					find_local_declarations != list_end(ld->list);
					find_local_declarations = list_next(find_local_declarations)){
						struct var_declaration *vd = list_entry(find_local_declarations, struct var_declaration, elem);
						_typeCheckLoop((struct _common *)vd, _context, _isAbleVoidCall);
				}
			}
			break;
		case statement_list:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct statement_list *sl = (struct statement_list *)data;
				Elem *find_statement_list;
				for(find_statement_list = list_begin(sl->list);
					find_statement_list != list_end(sl->list);
					find_statement_list = list_next(find_statement_list)){
						struct _statement *s = list_entry(find_statement_list, struct _statement, elem);
						_typeCheckLoop((struct _common *)s, _context, _isAbleVoidCall);
				}
			}
			break;
		case expression_stmt:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct expression_stmt *es = (struct expression_stmt *)data;
				if(es->expression)
					_typeCheckLoop((struct _common *)es->expression, _context, _isAbleVoidCall);
			}
			break;
		case selection_stmt:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct selection_stmt *ss = (struct selection_stmt *)data;
				if(ss->condition)
					_typeCheckLoop((struct _common *)ss->condition, _context, false);
				if(ss->action)
					_typeCheckLoop((struct _common *)ss->action, _context, _isAbleVoidCall);
				if(ss->else_action)
					_typeCheckLoop((struct _common *)ss->else_action, _context, _isAbleVoidCall);
			}
			break;
		case iteration_stmt:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct iteration_stmt *is = (struct iteration_stmt *)data;
				if(is->action)
					_typeCheckLoop((struct _common *)is->action, _context, false);
			}
			break;
		case return_stmt:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct return_stmt *rs = (struct return_stmt *)data;
				if(rs->return_expression)
					_typeCheckLoop((struct _common *)rs->return_expression, _context, _isAbleVoidCall);
			}
			break;
		case expression:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct expression *e = (struct expression *)data;
				if(e->isAssign){
					if(e->var)
						_typeCheckLoop((struct _common *)e->var, _context, false);
					if(e->expression)
						_typeCheckLoop((struct _common *)e->expression, _context, false);
				}else{
					if(e->simple_expression)
						_typeCheckLoop((struct _common *)e->simple_expression, _context, _isAbleVoidCall);
				}
			}
			break;
		case var:
			_typeCheck(data, _context, _isAbleVoidCall);
			break;
		case simple_expression:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct simple_expression *se = (struct simple_expression *)data;
				if(se->left){
					_typeCheckLoop((struct _common *)se->left, _context, se->relop ? false : _isAbleVoidCall);
				}
				if(se->relop){
					if(se->right){
						_typeCheckLoop((struct _common *)se->right, _context, false);
					}
				}
			}
			break;
		case additive_expression:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct additive_expression *ae = (struct additive_expression *)data;
				if(ae->addop){
					if(ae->additive_expression){
						_typeCheckLoop((struct _common *)ae->additive_expression, _context, false);
					}
				}
				if(ae->term){
					_typeCheckLoop((struct _common *)ae->term, _context, ae->addop ? false : _isAbleVoidCall);
				}
			}
			break;
		case term:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct term *t = (struct term *)data;
				if(t->mulop){
					if(t->term){
						_typeCheckLoop((struct _common *)t->term, _context, _isAbleVoidCall);
					}
				}
				if(t->factor){
					_typeCheckLoop((struct _common *)t->factor, _context, t->mulop ? false : _isAbleVoidCall);
				}
			}
			break;
		case factor:
			{
			_typeCheck(data, _context ,_isAbleVoidCall);
				struct factor *f = (struct factor *)data;
				switch(f->contenttype){
					case expression:
						_typeCheckLoop((struct _common *)f->content.expression, _context, _isAbleVoidCall);
						break;
					case var:
						_typeCheckLoop((struct _common *)f->content.var, _context, _isAbleVoidCall);
						break;
					case call:
						_typeCheckLoop((struct _common *)f->content.call, _context, _isAbleVoidCall);
						break;
					case num:
						break;
				}
			}
			break;
		case call:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct call *c = (struct call *)data;
				if(c->args){
					_typeCheckLoop((struct _common *)c->args, _context, _isAbleVoidCall);
				}
			}
			break;
		case arg_list:
			{
			_typeCheck(data, _context, _isAbleVoidCall);
				struct arg_list *al = (struct arg_list *)data;
				Elem *find_arg_list;
				for(find_arg_list = list_begin(al->list);
					find_arg_list != list_end(al->list);
					find_arg_list = list_next(find_arg_list)){
						struct expression *e = list_entry(find_arg_list, struct expression, elem);
						_typeCheckLoop((struct _common *)e, _context, _isAbleVoidCall);
				}
			}
			break;
		default:
			break;
	}
}

void typeCheck(Program *prog){
	_typeCheckLoop((struct _common *)prog, NULL, true);
}


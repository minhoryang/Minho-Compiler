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

void _typeCheck(struct _common *data, struct symtab *_context){
	if(data)
	switch(data->type){
		case var_declaration:
			{
				struct var_declaration *vd = (struct var_declaration *)data;
			  	// 1. 같은 scope에 중복된 이름이 있는지?
				if(_countSymbol(_context, vd->name) > 1){
					printf("ERROR!\n");
				}
				// 2. void type이면 안됨.
				if(strcmp(vd->type_specifier, "void") == 0){
					printf("ERROR!\n");
				}
			}
			break;
		case fun_declaration:
			{
				struct fun_declaration *fd = (struct fun_declaration *)data;
				// 1. 같은 scope에 중복된 이름이 있는지?
				if(_countSymbol(_context, fd->name) > 1){
					printf("ERROR!\n");
				}
				// 2. global에서 함수선언인지?
				if(strcmp(_context->name, "Global")){
					printf("ERROR!\n");
				}
				// 3. main함수일경우
				if(strcmp(fd->name, "main") == 0){
					// 3-1. 가장 마지막에
					// TODO
					// 3-2. 리턴값이 void
					if(strcmp(fd->type_specifier, "void")){
						printf("ERROR!\n");
					}
					// 3-3. 파라메터가 없어야함.
					if(fd->params){
						printf("ERROR!\n");
					}
				}
			}
			break;
		case param:
			{
				struct param *p = (struct param *)data;
				// 1. 같은 scope에 중복된 이름이 있는지?
				if(_countSymbol(_context, p->name) > 1){
					printf("ERROR!\n");
				}
				// 2. void타입인지?
				if(strcmp(p->type_specifier, "void") == 0){
					printf("ERROR!\n");
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
						printf("ERROR!\n");
					}
				}else if(strcmp(func->type_specifier, "void") == 0){
					if(rs->return_expression){
						printf("ERROR!\n");
					}
				}
			}
			break;
		case factor:
			/*{
				struct factor *f = (struct factor *)data;
				// 1. call일때!
				if(f->contenttype == call){
					char *funcname = f->content.call->name;
					struct symtab *func_where = searchSymtabWhere(_context, funcname);
					struct fun_declaration *func = searchSymtab(func_where, funcname);
					// 선언이 안되어있으면 안됨.
					if(!func){
						printf("ERROR!\n");
					}else{
						// 리턴값이 void면 안됨.
						if(strcmp(func->type_specifier, "void") == 0){
							printf("ERROR!\n");
						}
					}
				}
			}*/
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

void _typeCheckLoop(struct _common *data, struct symtab *_context){
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
						_typeCheckLoop((struct _common *)this, dl->symtab);
				}
			}
			break;
		case var_declaration:
			{
				_typeCheck(data, _context);
			}
			break;
		case fun_declaration:
			{
				_typeCheck(data, _context);
				struct fun_declaration *fd = (struct fun_declaration *)data;
				_typeCheckLoop((struct _common *)fd->params, fd->symtab);
				_typeCheckLoop((struct _common *)fd->compound_stmt, fd->symtab);
			}
			break;
		case param_list:
			{
			_typeCheck(data, _context);
				struct param_list *pl = (struct param_list *)data;
				Elem *find_param;
				for(find_param = list_begin(pl->list);
					find_param != list_end(pl->list);
					find_param = list_next(find_param)){
						struct param *this = list_entry(find_param, struct param, elem);
						_typeCheckLoop((struct _common *)this, _context);
				}
			}
			break;
		case param:
			_typeCheck(data, _context);
			break;
		case compound_stmt:
			{
				struct compound_stmt *cs = (struct compound_stmt *)data;
				_typeCheck(data, _context);
				if(cs->symtab)
					_typeCheckLoop((struct _common *)cs->statement_list, cs->symtab);
				else
					_typeCheckLoop((struct _common *)cs->statement_list, _context);
			}
			break;
		case local_declarations:
			{
			_typeCheck(data, _context);
				struct local_declarations *ld = (struct local_declarations *)data;
				Elem *find_local_declarations;
				for(find_local_declarations = list_begin(ld->list);
					find_local_declarations != list_end(ld->list);
					find_local_declarations = list_next(find_local_declarations)){
						struct var_declaration *vd = list_entry(find_local_declarations, struct var_declaration, elem);
						_typeCheckLoop((struct _common *)vd, _context);
				}
			}
			break;
		case statement_list:
			{
			_typeCheck(data, _context);
				struct statement_list *sl = (struct statement_list *)data;
				Elem *find_statement_list;
				for(find_statement_list = list_begin(sl->list);
					find_statement_list != list_end(sl->list);
					find_statement_list = list_next(find_statement_list)){
						struct _statement *s = list_entry(find_statement_list, struct _statement, elem);
						_typeCheckLoop((struct _common *)s, _context);
				}
			}
			break;
		case expression_stmt:
			{
			_typeCheck(data, _context);
				struct expression_stmt *es = (struct expression_stmt *)data;
				if(es->expression)
					_typeCheckLoop((struct _common *)es->expression, _context);
			}
			break;
		case selection_stmt:
			{
			_typeCheck(data, _context);
				struct selection_stmt *ss = (struct selection_stmt *)data;
				if(ss->condition)
					_typeCheckLoop((struct _common *)ss->condition, _context);
				if(ss->action)
					_typeCheckLoop((struct _common *)ss->action, _context);
				if(ss->else_action)
					_typeCheckLoop((struct _common *)ss->else_action, _context);
			}
			break;
		case iteration_stmt:
			{
			_typeCheck(data, _context);
				struct iteration_stmt *is = (struct iteration_stmt *)data;
				if(is->action)
					_typeCheckLoop((struct _common *)is->action, _context);
			}
			break;
		case return_stmt:
			{
			_typeCheck(data, _context);
				struct return_stmt *rs = (struct return_stmt *)data;
				if(rs->return_expression)
					_typeCheckLoop((struct _common *)rs->return_expression, _context);
			}
			break;
		case expression:
			{
			_typeCheck(data, _context);
				struct expression *e = (struct expression *)data;
				if(e->isAssign){
					if(e->var)
						_typeCheckLoop((struct _common *)e->var, _context);
					if(e->expression)
						_typeCheckLoop((struct _common *)e->expression, _context);
				}else{
					if(e->simple_expression)
						_typeCheckLoop((struct _common *)e->simple_expression, _context);
				}
			}
			break;
		case var:
			_typeCheck(data, _context);
			break;
		case simple_expression:
			{
			_typeCheck(data, _context);
				struct simple_expression *se = (struct simple_expression *)data;
				if(se->left){
					_typeCheckLoop((struct _common *)se->left, _context);
				}
				if(se->relop){
					if(se->right){
						_typeCheckLoop((struct _common *)se->right, _context);
					}
				}
			}
			break;
		case additive_expression:
			{
			_typeCheck(data, _context);
				struct additive_expression *ae = (struct additive_expression *)data;
				if(ae->addop){
					if(ae->additive_expression){
						_typeCheckLoop((struct _common *)ae->additive_expression, _context);
					}
				}
				if(ae->term){
					_typeCheckLoop((struct _common *)ae->term, _context);
				}
			}
			break;
		case term:
			{
			_typeCheck(data, _context);
				struct term *t = (struct term *)data;
				if(t->mulop){
					if(t->term){
						_typeCheckLoop((struct _common *)t->term, _context);
					}
				}
				if(t->factor){
					_typeCheckLoop((struct _common *)t->factor, _context);
				}
			}
			break;
		case factor:
			{
			_typeCheck(data, _context);
				struct factor *f = (struct factor *)data;
				switch(f->contenttype){
					case expression:
						_typeCheckLoop((struct _common *)f->content.expression, _context);
						break;
					case var:
						_typeCheckLoop((struct _common *)f->content.var, _context);
						break;
					case call:
						_typeCheckLoop((struct _common *)f->content.call, _context);
						break;
					case num:
						break;
				}
			}
			break;
		case call:
			{
			_typeCheck(data, _context);
				struct call *c = (struct call *)data;
				if(c->args){
					_typeCheckLoop((struct _common *)c->args, _context);
				}
			}
			break;
		case arg_list:
			{
			_typeCheck(data, _context);
				struct arg_list *al = (struct arg_list *)data;
				Elem *find_arg_list;
				for(find_arg_list = list_begin(al->list);
					find_arg_list != list_end(al->list);
					find_arg_list = list_next(find_arg_list)){
						struct expression *e = list_entry(find_arg_list, struct expression, elem);
						_typeCheckLoop((struct _common *)e, _context);
				}
			}
			break;
		default:
			break;
	}
}

void typeCheck(Program *prog){
	_typeCheckLoop((struct _common *)prog, NULL);
}


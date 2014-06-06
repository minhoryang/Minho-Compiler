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
				_typeCheckLoop((struct _common *)fd->compound_stmt, fd->symtab);
			}
			break;
		case param_list:
			_typeCheck(data, _context);
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
			_typeCheck(data, _context);
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
			_typeCheck(data, _context);
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
				printf("return %s\n", _context->name);
			}
			break;
		case expression:
			_typeCheck(data, _context);
			break;
		case var:
			_typeCheck(data, _context);
			break;
		case simple_expression:
			_typeCheck(data, _context);
			break;
		case additive_expression:
			_typeCheck(data, _context);
			break;
		case term:
			_typeCheck(data, _context);
			break;
		case factor:
			_typeCheck(data, _context);
			break;
		case call:
			_typeCheck(data, _context);
			break;
		case arg_list:
			_typeCheck(data, _context);
			break;
		default:
			break;
	}
}

void typeCheck(Program *prog){
	_typeCheckLoop((struct _common *)prog, NULL);
}


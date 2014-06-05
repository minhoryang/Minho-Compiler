#include "symtab.h"

struct symtab * _allocSymtab(const char *name, struct symtab *_parent){
	struct symtab *st = _ALLOC(struct symtab);
	st->name = strdup(name);
	st->symbols = NULL;
	st->usings = NULL;
	st->parent = _parent;
	return st;
}

void _buildSymtab(struct _common *data, struct symtab *_context, bool func_exception){
	if(data)
	switch(data->type){
		case declaration_list:
			{
				struct declaration_list *dl = (struct declaration_list *)data;
				dl->symtab = _allocSymtab("Global", _context);
				{
					struct _common *now = dl->list;
					while(now){
						_buildSymtab(now, dl->symtab, false);
						now = ((struct _declaration *)now)->list;
					}
				}
			}
			break;
		case var_declaration:
			{
				struct var_declaration *vd = (struct var_declaration *)data;
				if(_context->symbols){
					struct _symbol_common *now = (struct _symbol_common *)_context->symbols;
					while(now){
						if(now->symbols){
							now = now->symbols;
						}else{
							now->symbols = (struct _symbol_common *)vd;
							break;
						}
					}
				}else{
					_context->symbols = (struct _symbol_common *)vd;
				}
			}
			break;
		case fun_declaration:
			{
				struct fun_declaration *fd = (struct fun_declaration *)data;
				if(_context->symbols){
					struct _symbol_common *now = (struct _symbol_common *)_context->symbols;
					while(now){
						if(now->symbols){
							now = now->symbols;
						}else{
							now->symbols = (struct _symbol_common *)fd;
							break;
						}
					}
				}else{
					_context->symbols = (struct _symbol_common *)fd;
				}
				fd->symtab = _allocSymtab(fd->name, _context);
				_buildSymtab((struct _common *)fd->params, fd->symtab, false);
				_buildSymtab((struct _common *)fd->compound_stmt, fd->symtab, true);
			}
			break;
		case param_list:
			{
				struct param_list *pl = (struct param_list *)data;
				if(pl->list){
					_buildSymtab(pl->list, _context, false);
					struct _common *now = ((struct param *)pl->list)->list;
					while(now){
						_buildSymtab(now, _context, false);
						now = ((struct param *)now)->list;
					}
				}
			}
			break;
		case param:
			{
				struct param *p = (struct param *)data;
				if(_context->symbols){
					struct _symbol_common *now = (struct _symbol_common *)_context->symbols;
					while(now){
						if(now->symbols){
							now = now->symbols;
						}else{
							now->symbols = (struct _symbol_common *)p;
							break;
						}
					}
				}else{
					_context->symbols = (struct _symbol_common *)p;
				}
			}
		case compound_stmt:
			{
				struct compound_stmt *cs = (struct compound_stmt *)data;
				struct symtab *context;
				if(!func_exception){
					context = (cs->symtab = _allocSymtab(_context->name, _context));
				}else{
					context = _context;
					cs->symtab = NULL;
				}
				_buildSymtab((struct _common *)cs->local_declarations, context, false);
				_buildSymtab((struct _common *)cs->statement_list, context, false);
			}
			break;
		case local_declarations:
			{
				struct local_declarations *ld = (struct local_declarations *)data;
				struct _common *now = ld->list;
				while(now){
					_buildSymtab(now, _context, false);
					now = ((struct var_declaration *)now)->list;
				}
			}
			break;
		case statement_list:
			{
				struct statement_list *sl = (struct statement_list *)data;
				struct _common *now = sl->list;
				while(now){
					_buildSymtab(now, _context, false);
					//printf("%d\n", now->type);
					now = ((struct _statement *)now)->list;
				}
			}
			break;
		case expression_stmt:
			{
				struct expression_stmt *e = (struct expression_stmt *)data;
				if(e->expression)
					_buildSymtab((struct _common *)e->expression, _context, false);
			}
			break;
		case selection_stmt:
			{
				struct selection_stmt *ss = (struct selection_stmt *)data;
				if(ss->condition)
					_buildSymtab((struct _common *)ss->condition, _context, false);
				if(ss->action)
					_buildSymtab((struct _common *)ss->action, _context, false);
				if(ss->else_action)
					_buildSymtab((struct _common *)ss->else_action, _context, false);
			}
			break;
		case iteration_stmt:
			{
				struct iteration_stmt *is = (struct iteration_stmt *)data;
				if(is->condition)
					_buildSymtab((struct _common *)is->condition, _context, false);
				if(is->action)
					_buildSymtab((struct _common *)is->action, _context, false);
			}
			break;
		case return_stmt:
			{
				struct return_stmt *rs = (struct return_stmt *)data;
				if(rs->return_expression)
					_buildSymtab((struct _common *)rs->return_expression, _context, false);
			}
			break;
		case expression:
			{
				struct expression *e = (struct expression *)data;
				if(e->isAssign){
					if(e->var)
						_buildSymtab((struct _common *)e->var, _context, false);
					if(e->expression)
						_buildSymtab((struct _common *)e->expression, _context, false);
				}else{
					if(e->simple_expression)
						_buildSymtab((struct _common *)e->simple_expression, _context, false);
				}
			}
			break;
		case var:
			{
				struct var *v = (struct var *)data;
				{
					struct symtab *found = searchSymtabWhere(_context, v->name);
					if(found){
						//TODO list_push_back(found->usings, &(v->symelem));
					}else{
						//TODO list_push_back(_context->usings, &(v->symelem));
					}
				}
				if(v->array)
					_buildSymtab((struct _common *)v->array, _context, false);
			}
			break;
		case simple_expression:
			{
				struct simple_expression *se = (struct simple_expression *)data;
				if(se->left){
					_buildSymtab((struct _common *)se->left, _context, false);
				}
				if(se->relop){
					if(se->right){
						_buildSymtab((struct _common *)se->right, _context, false);
					}
				}
			}
			break;
		case additive_expression:
			{
				struct additive_expression *ae = (struct additive_expression *)data;
				if(ae->addop){
					if(ae->additive_expression){
						_buildSymtab((struct _common *)ae->additive_expression, _context, false);
					}
				}
				if(ae->term){
					_buildSymtab((struct _common *)ae->term, _context, false);
				}
			}
			break;
		case term:
			{
				struct term *t = (struct term *)data;
				if(t->mulop){
					if(t->term){
						_buildSymtab((struct _common *)t->term, _context, false);
					}
				}
				if(t->factor){
					_buildSymtab((struct _common *)t->factor, _context, false);
				}
			}
			break;
		case factor:
			{
				struct factor *f = (struct factor *)data;
				switch(f->contenttype){
					case expression:
						_buildSymtab((struct _common *)f->content.expression, _context, false);
					case var:
						_buildSymtab((struct _common *)f->content.var, _context, false);
					case call:
						_buildSymtab((struct _common *)f->content.call, _context, false);
						break;
					case num:
						break;
				}
			}
			break;
		case call:
			{
				struct call *c = (struct call *)data;
				if(c->args){
					_buildSymtab((struct _common *)c->args, _context, false);
				}
				{
					struct symtab *found = searchSymtabWhere(_context, c->name);
					if(found){
						//TODO list_push_back(found->usings, &(c->symelem));
					}else{
						//TODO list_push_back(_context->usings, &(c->symelem));
					}
				}
			}
			break;
		case arg_list:
			{
				struct arg_list *al = (struct arg_list *)data;
				struct _common *now = al->list;
				while(now){
					_buildSymtab(now, _context, false);
					now = ((struct expression *)now)->list;
				}
			}
			break;
		default:
			break;
	}
}

#define TAPING do{ \
	int i; \
	for(i=0;i<level;i++) \
		printf("\t"); \
}while(false);
void _dumpSymtab(struct symtab *this, int level){
	// TODO list sort.
	struct _symbol_common *now = this->symbols;
	while(now){
			struct _declaration *c = (struct _declaration *)now;
			TAPING printf("%3d:%2d\t%s\t%s", c->line+1, c->cur+1, c->type_specifier, c->name);
			switch(c->type){
				case var_declaration:
					{
						struct var_declaration *vd = (struct var_declaration *)c;
						if(vd->size)
							printf("[%s]", vd->size);
					}
					break;
				case fun_declaration:
					{
						// TODO PARAM?
						printf("()");
					}
					break;
				case param:
					{
						struct param *p = (struct param *)c;
						if(p->isArray)
							printf("[]");
					}
					break;
			}
			printf(";\n");
			/*{
				Elem *find_usage;
				for(find_usage = list_begin(this->usings);
					find_usage != list_end(this->usings);
					find_usage = list_next(find_usage)){
					//	struct _symbol_common *sc = list_entry(find_usage, struct _symbol_common, symelem);
				}
			}
			printf("\n");
			*/
		now = now->symbols;
	}
}

void _traceSymtab(struct _common *data, int level, int *cnt){
	if(data)
	switch(data->type){
		case declaration_list:
			{
				struct declaration_list *dl = (struct declaration_list *)data;
				{  // XXX IN!
					TAPING printf("%3d:%2d\tSCOPE %d: \"%s\"\n", dl->line+1, dl->cur+1, (*cnt)++, dl->symtab->name);
					level+=1;
					_dumpSymtab(dl->symtab, level);
				}
				if(dl->list){
					_traceSymtab(dl->list, level, cnt);
					struct _common *now = ((struct _declaration *)dl->list)->list;
					while(now){
						_traceSymtab(now, level, cnt);
						now = ((struct _declaration *)now)->list;
					}
				}
				{	// XXX OUT!
					level-=1;
				}
			}
			break;
		// var_declaration ignore.
		case fun_declaration:
			{
				struct fun_declaration *fd = (struct fun_declaration *)data;
				{	// XXX IN!
					TAPING printf("%3d:%2d\tSCOPE %d: \"%s\"\n", fd->line+1, fd->cur+1, (*cnt)++, fd->symtab->name);
					level+=1;
					_dumpSymtab(fd->symtab, level);
				}
				_traceSymtab((struct _common *)fd->compound_stmt, level, cnt);
				{	// XXX OUT!
					level-=1;
				}
			}
			break;
		// param_list ignore.
		// param ignore.
		case compound_stmt:
			{
				struct compound_stmt *cs = (struct compound_stmt *)data;
				if(cs->symtab){  // XXX IN!
					TAPING printf("%3d:%2d\tSCOPE %d: \"%s\"\n", cs->line+1, cs->cur+1, (*cnt)++, cs->symtab->name);
					level+=1;
					_dumpSymtab(cs->symtab, level);
				}
				_traceSymtab((struct _common *)cs->statement_list, level, cnt);
				if(cs->symtab){  // XXX OUT!
					level-=1;
				}
			}
			break;
		// local_declarations ignore.
		case statement_list:
			{
				struct statement_list *sl = (struct statement_list *)data;
				if(sl->list){
					_traceSymtab(sl->list, level, cnt);
					struct _common *now = ((struct _statement *)sl->list)->list;
					while(now){
						_traceSymtab(now, level, cnt);
						now = ((struct _statement *)now)->list;
					}
				}
			}
			break;
		// expression_stmt ignore.
		case selection_stmt:
			{
				struct selection_stmt *ss = (struct selection_stmt *)data;
				if(ss->action)
					_traceSymtab((struct _common *)ss->action, level, cnt);
				if(ss->else_action)
					_traceSymtab((struct _common *)ss->else_action, level, cnt);
			}
			break;
		case iteration_stmt:
			{
				struct iteration_stmt *is = (struct iteration_stmt *)data;
				if(is->action)
					_traceSymtab((struct _common *)is->action, level, cnt);
			}
			break;
		// return_stmt ignore.
		// expression ignore.
		// var ignore.
		// simple_expression ignore.
		// additive_expression ignore.
		// term ignore.
		// factor ignore.
		// call ignore.
		// arg_list ignore.
		default:
			break;
	}
}

void buildSymtab(Program *prog){
	_buildSymtab((struct _common *)prog, NULL, false);
	if(TraceAnalyze){
		ALLOC(i, int);
		_traceSymtab((struct _common *)prog, 0, i);
		free(i);
	}
}

struct symtab *searchSymtabWhere(struct symtab *from, char *this){
	while(from){
		struct _symbol_common *found = searchSymtab(from, this);
		if(found){
			return from;
		}
		from = from->parent;
	}
	return NULL;
}

struct _symbol_common *searchSymtab(struct symtab *from, char *this){
	if(from->symbols){
		if(strcmp(from->symbols->name, this))
			return from->symbols;
		struct _symbol_common *now = ((struct _symbol_common *)from->symbols)->symbols;
		while(now){
			if(strcmp(now->name, this))
				return now;
		}
	}
	return NULL;
}

void typeCheck(Program *prog){
	;
}

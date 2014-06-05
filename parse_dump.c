void dump_declaration_list(struct declaration_list *dl, int level){
	if(dl->list){
		LEVEL_UP;
		printTree(dl->list, level);
		LEVEL_DOWN;
		struct _common *now = ((struct _declaration *)(dl->list))->list;
		while(now){
			LEVEL_UP;
			printTree((struct _common *)now, level);
			LEVEL_DOWN;
			now = ((struct _declaration *)now)->list;
		}
	}
}

void dump_var_declaration(struct var_declaration *vd, int level){
	SPACING printf("[Variable Declared]\n");
	LEVEL_UP;
	SPACING printf("TYPE\t\"%s\"\n", vd->type_specifier);
	SPACING printf("ID\t\"%s\"\n", vd->name);
	if(vd->size){
		SPACING printf("ARRAY\t[%s]\n", vd->size);
	}
	LEVEL_DOWN;
}

void dump_fun_declaration(struct fun_declaration *fd, int level){
	SPACING printf("[Function Declared]\n");
	LEVEL_UP;
	SPACING printf("TYPE\t\"%s\"\n", fd->type_specifier);
	SPACING printf("ID\t\"%s\"\n", fd->name);
	if(fd->params){
		printTree((struct _common *)fd->params, level);
	}
	if(fd->compound_stmt)
		printTree((struct _common *)fd->compound_stmt, level);
	LEVEL_DOWN;
}

void dump_param_list(struct param_list *pl, int level){
	SPACING printf("[Param List]\n");
	if(pl->list){
		LEVEL_UP;
		printTree((struct _common *)pl->list, level);
		LEVEL_DOWN;
		struct _common *now = ((struct param *)(pl->list))->list;
		while(now){
			LEVEL_UP;
			printTree((struct _common *)now, level);
			LEVEL_DOWN;
			now = ((struct param *)now)->list;
		}
	}
}

void dump_param(struct param *p, int level){
	SPACING printf("[Param]\n");
	LEVEL_UP;
	SPACING printf("TYPE\t\"%s\"\n", p->type_specifier);
	SPACING printf("ID\t\"%s\"\n", p->name);
	if(p->isArray){
		SPACING printf("isArray []\n");
	}
	LEVEL_DOWN;
}

void dump_compound_stmt(struct compound_stmt *cs, int level){
	SPACING printf("[Compound Statement]\n");
	LEVEL_UP;
	if(cs->local_declarations)
		printTree((struct _common *)cs->local_declarations, level);
	if(cs->statement_list)
		printTree((struct _common *)cs->statement_list, level);
	LEVEL_DOWN;
}

void dump_local_declarations(struct local_declarations *ld, int level){
	SPACING printf("[Local Declared]\n");
	if(ld->list){
		LEVEL_UP;
		printTree(ld->list, level);
		LEVEL_DOWN;
		struct _common *now = ((struct var_declaration *)(ld->list))->list;
		while(now){
			LEVEL_UP;
			printTree((struct _common *)now, level);
			LEVEL_DOWN;
			now = ((struct var_declaration *)now)->list;
		}
	}
}

void dump_statement_list(struct statement_list *sl, int level){
	SPACING printf("[Statement List]\n");
	if(sl->list){
		LEVEL_UP;
		printTree(sl->list, level);
		LEVEL_DOWN;
		struct _common *now = ((struct _statement *)sl->list)->list;
		while(now){
			LEVEL_UP;
			printTree(now, level);
			LEVEL_DOWN;
			now = ((struct _statement *)now)->list;
		}
	}
}

void dump_expression_stmt(struct expression_stmt *ex, int level){
	SPACING printf("[Expression Statement]\n");
	LEVEL_UP;
	if(ex->expression)
		printTree((struct _common *)ex->expression, level);
	LEVEL_DOWN;
}

void dump_selection_stmt(struct selection_stmt *ss, int level){
	SPACING printf("[Selection Statement]\n");
	LEVEL_UP;
	if(ss->condition){
		SPACING printf("[IF]\n");
		LEVEL_UP;
		printTree((struct _common *)ss->condition, level);
		LEVEL_DOWN;
	}
	if(ss->action){
		SPACING printf("[Then]\n");
		LEVEL_UP;
		printTree((struct _common *)ss->action, level);
		LEVEL_DOWN;
	}
	if(ss->else_action){
		SPACING printf("[ELSE]\n");
		LEVEL_UP;
		printTree((struct _common *)ss->else_action, level);
		LEVEL_DOWN;
	}
	LEVEL_DOWN;
}

void dump_iteration_stmt(struct iteration_stmt *is, int level){
	SPACING printf("[Iteration Statement]\n");
	LEVEL_UP;
	if(is->condition){
		SPACING printf("[WHILE]\n");
		LEVEL_UP;
		printTree((struct _common *)is->condition, level);
		LEVEL_DOWN;
	}
	if(is->action){
		SPACING printf("[Then]\n");
		LEVEL_UP;
		printTree((struct _common *)is->action, level);
		LEVEL_DOWN;
	}
	LEVEL_DOWN;
}

void dump_return_stmt(struct return_stmt *rs, int level){
	SPACING printf("[Return Statement]\n");
	LEVEL_UP;
	if(rs->return_expression){
		printTree((struct _common *)rs->return_expression, level);
	}
	LEVEL_DOWN;
}

void dump_expression(struct expression *e, int level){
	SPACING printf("[Expression]\n");
	LEVEL_UP;
	if(e->isAssign){
		SPACING printf("[Assign]\n");
		LEVEL_UP;
		if(e->var){
			printTree((struct _common *)e->var, level);
		}
		SPACING printf("=\n");
		if(e->expression){
			printTree((struct _common *)e->expression, level);
		}
		LEVEL_DOWN;
	}else{
		if(e->simple_expression)
			printTree((struct _common *)e->simple_expression, level);
	}
	LEVEL_DOWN;
}

void dump_var(struct var *v, int level){
	SPACING printf("[Var]\n");
	LEVEL_UP;
	if(v->name){
		SPACING printf("NAME\t\"%s\"\n", v->name);
	}
	if(v->array){
		SPACING printf("[Array]\n");
		LEVEL_UP;
		printTree((struct _common *)v->array, level);
		LEVEL_DOWN;
	}
	LEVEL_DOWN;
}

void dump_simple_expression(struct simple_expression *se, int level){
	SPACING printf("[Simple Expression]\n");
	LEVEL_UP;
	if(se->left){
		SPACING printf("[Left]\n");
		LEVEL_UP;
		printTree((struct _common *)se->left, level);
		LEVEL_DOWN;
	}
	if(se->relop){
		SPACING printf("RELOP\t%s\n", se->relop);
		if(se->right){
			SPACING printf("[Right]\n");
			LEVEL_UP;
			printTree((struct _common *)se->right, level);
			LEVEL_DOWN;
		}
	}
	LEVEL_DOWN;
}

void dump_additive_expression(struct additive_expression *ae, int level){
   SPACING printf("[Additive Expression]\n");
   LEVEL_UP;
   if(ae->addop){
	   if(ae->additive_expression){
		   printTree((struct _common *)ae->additive_expression, level);
	   }
	   SPACING printf("ADDOP\t%s\n", ae->addop);
   }
   if(ae->term){
	   printTree((struct _common *)ae->term, level);
   }
   LEVEL_DOWN;
}

void dump_term(struct term *t, int level){
	SPACING printf("[Term]\n");
	LEVEL_UP;
	if(t->mulop){
		if(t->term){
			printTree((struct _common *)t->term, level);
		}
		SPACING printf("MULOP\t%s\n", t->mulop);
	}
	if(t->factor){
		printTree((struct _common *)t->factor, level);
	}
	LEVEL_DOWN;
}

void dump_factor(struct factor *f, int level){
	SPACING printf("[Factor]\n");
	LEVEL_UP;
	switch(f->contenttype){
		case expression:
			printTree((struct _common *)f->content.expression, level);
			break;
		case var:
			printTree((struct _common *)f->content.var, level);
			break;
		case call:
			printTree((struct _common *)f->content.call, level);
			break;
		case num:
			SPACING printf("%s\n", (char *)(f->content.num));
			break;
	}
	LEVEL_DOWN;
}

void dump_call(struct call *c, int level){
	SPACING printf("[Call]\n");
	LEVEL_UP;
	SPACING printf("\"%s\"\n", c->name);
	if(c->args){
		printTree((struct _common *)c->args, level);
	}else{
		SPACING printf("[No Arg List]\n");
	}
	LEVEL_DOWN;
}

void dump_arg_list(struct arg_list *al, int level){
	{
		SPACING printf("[Arg List]\n");
		if(al->list){
			//LEVEL_UP;
			printTree(al->list, level);
			//LEVEL_DOWN;
			struct _common *now = ((struct expression *)al->list)->list;
			while(now){
				//LEVEL_UP;
				printTree(now, level);
				//LEVEL_DOWN;
				now = ((struct expression *)now)->list;
			}
		}
	}
}

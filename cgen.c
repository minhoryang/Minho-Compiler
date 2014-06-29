#include "globals.h"
#include "parse.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

static int tmpOffset = 0;
static int fpOffset = 0;
static int oldfpOffset = 0;

void cGen(struct _common *data){
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
						cGen((struct _common *)this);
				}
			}
			break;
		//case var_declaration:
		//	break;
		case fun_declaration:
			// offset_for_call;
			{
				struct fun_declaration *fd = (struct fun_declaration *)data;

			}
			break;
		//case param_list:
		//	break;
		//case param:
		//	break;
		case compound_stmt:
			break;
		case local_declarations:
			break;
		case statement_list:
			break;
		case expression_stmt:
			break;
		case selection_stmt:
			break;
		case iteration_stmt:
			break;
		case return_stmt:
			break;
		case expression:
			break;
		case var:
			break;
		case simple_expression:
			break;
		case additive_expression:
			break;
		case term:
			break;
		case factor:
			break;
		case call:
			break;
		case arg_list:
			break;
	}
}

void codeGen(Program *prog, char * codefile)
{  char * s = malloc(strlen(codefile)+7);
   strcpy(s,"File: ");
   strcat(s,codefile);
   emitComment("C-Minus Compilation to TM Code");
   emitComment(s);
   /* generate standard prelude */
   emitComment("Standard prelude:");
   emitRM("LD",gp,0,ac,"load maxaddress from location 0");
   emitRM("ST",ac,0,ac,"clear location 0");
   emitComment("End of standard prelude.");
   /* generate code for TINY program */
   cGen((struct _common *)prog);
   /* finish */
   emitComment("End of execution.");
   emitRO("HALT",0,0,0,"");
}


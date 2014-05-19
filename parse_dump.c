void dump_declaration_list(struct declaration_list *dl, int level){
	Elem *find_declaration;
	for(find_declaration = list_begin(dl->list);
		find_declaration != list_end(dl->list);
		find_declaration = list_next(find_declaration)){
			struct fun_declaration *this = list_entry(find_declaration, struct fun_declaration, elem);
			LEVEL_UP;
			printTree((struct _common *)this, level);
			//printf("search %x\n", find_declaration);
			LEVEL_DOWN;
	}
}

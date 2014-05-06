int yywrap(void){
	// http://dinosaur.compilertools.net/flex/flex_10.html
	/*
	 When the scanner receives an end-of-file indication from YY_INPUT, it then checks the `yywrap()' function. If `yywrap()' returns false (zero), then it is assumed that the function has gone ahead and set up yyin to point to another input file, and scanning continues. If it returns true (non-zero), then the scanner terminates, returning 0 to its caller. Note that in either case, the start condition remains unchanged; it does not revert to INITIAL.
	 If you do not supply your own version of `yywrap()', then you must either use `%option noyywrap' (in which case the scanner behaves as though `yywrap()' returned 1), or you must link with `-lfl' to obtain the default version of the routine, which always returns 1.
	 */
	return 1;
}

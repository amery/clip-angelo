#command TEXT								;
												=> text QOut, QQOut

#command TEXT TO FILE <(file)>		;
												=> __TextSave( <(file)> )			;
													; text QOut, __TextRestore

#command TEXT TO PRINTER			;
											=> __TextSave("PRINTER")			;
												; text QOut, __TextRestore

#xcommand TEXTBLOCK := <xpr>			=> ( TEXTBLOCK := <xpr> )
#xcommand TEXTBLOCK = <xpr>			=> ( TEXTBLOCK = <xpr> )
#xcommand TEXTBLOCK : <xpr>			=> ( TEXTBLOCK : <xpr> )
#xcommand TEXTBLOCK <*cText*>			=> // <cText>

#command TOTAL [TO <(file)>] [ON <key>]	;
			[FIELDS <fields,...>]				;
			[FOR <for>]								;
			[WHILE <while>]						;
			[NEXT <next>]							;
			[RECORD <rec>]							;
			[<rest:REST>]							;
			[ALL]										;
			[<noopt:NOOPTIMIZE>]					;
													=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )							;
														;		__dbTotalOptimize( <(file)>, <{key}>, { <(fields)> },		;
														[codestr( <for> )], <{for}>, <{while}>, <next>, <rec>,			;
														<.rest.> )																		;
														;	else																			;
														;		__dbTotal( <(file)>, <{key}>, { <(fields)> },				;
														<{for}>, <{while}>, <next>, <rec>, <.rest.> )						;
														;	endif

#command TOTAL [TO <(file)>] [ON <key>]	;
			[FIELDS <fields,...>]				;
			[FOR <for>]								;
			[WHILE <while>]						;
			[NEXT <next>]							;
			[RECORD <rec>]							;
			[<rest:REST>]							;
			[ALL]										;
													=> __dbTotal( <(file)>, <{key}>, { <(fields)> }, <{for}>, <{while}>, <next>, <rec>, <.rest.> )

#command TYPE <(file)> [<print: TO PRINTER>] [TO FILE <(dest)>]	;
													=> __TypeFile( <(file)>, <.print.> )	;
														[; COPY FILE <(file)> TO <(dest)> ]

#command TYPE <(file)> [<print: TO PRINTER>]				;
													=> __TypeFile( <(file)>, <.print.> )


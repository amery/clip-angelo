#command LABEL FORM <lbl>				;
			[<sample: SAMPLE>]			;
			[<noconsole: NOCONSOLE>]	;
			[<print: TO PRINTER>]		;
			[TO FILE <(toFile)>]			;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[ALL]								;
			[<noopt:NOOPTIMIZE>]			;
												=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )					;
													;		__LabelFormOptimize(											;
													<(lbl)>, <.print.>, <(toFile)>, <.noconsole.>,				;
													[codestr( <for> )], <{for}>, <{while}>, <next>, <rec>,	;
													<.rest.>, <.sample.> )												;
													;	else																	;
													;		__LabelForm(													;
													<(lbl)>, <.print.>, <(toFile)>, <.noconsole.>,				;
													<{for}>, <{while}>, <next>, <rec>, <.rest.>,					;
													<.sample.> )															;
													;	endif


#command LABEL FORM <lbl>				;
			[<sample: SAMPLE>]			;
			[<noconsole: NOCONSOLE>]	;
			[<print: TO PRINTER>]		;
			[TO FILE <(toFile)>]			;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[ALL]								;
												=> __LabelForm(												;
													<(lbl)>, <.print.>, <(toFile)>, <.noconsole.>,	;
													<{for}>, <{while}>, <next>, <rec>, <.rest.>,		;
													<.sample.> )

#command LIST := <xpr>					=> ( list := <xpr> )
#command LIST = <xpr>					=> ( list := <xpr> )
#command LIST : <xpr>					=> ( list : <xpr> )

#command LIST [<list,...>]			;
			[<off:OFF>]					;
			[<toPrint: TO PRINTER>]	;
			[TO FILE <(toFile)>]		;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[ALL]							;
			[<noopt:NOOPTIMIZE>]		;
											=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )			;
												;		__dbListOptimize(<.off.>, { <{list}> }, .t.,	;
												[codestr( <for> )], <{for}>, <{while}>, <next>,		;
												<rec>, <.rest.>, <.toPrint.>, <(toFile)> )			;
												;	else															;
												;		__dbList(<.off.>, { <{list}> }, .t.,			;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>,			;
												<.toPrint.>, <(toFile)> )									;
												;	endif

#command LIST [<list,...>]			;
			[<off:OFF>]					;
			[<toPrint: TO PRINTER>]	;
			[TO FILE <(toFile)>]		;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[ALL]							;
											=> __dbList(												;
												<.off.>, { <{list}> }, .t.,						;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>,	;
												<.toPrint.>, <(toFile)> )

#command LOCATE					;
			[FOR <for>]				;
			[WHILE <while>]		;
			[NEXT <next>]			;
			[RECORD <rec>]			;
			[<rest:REST>]			;
			[ALL]						;
			[<noopt:NOOPTIMIZE>]	;
		 									=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )								;
												;		__dbLocateOptimize( [codestr( <for> )], <{for}>, <{while}>,	;
												<next>, <rec>, <.rest.> )														;
												; else																				;
												;		__dbLocate( <{for}>, <{while}>, <next>, <rec>, <.rest.> )	;
												; end

#command LOCATE				;
			[FOR <for>]			;
			[WHILE <while>]	;
			[NEXT <next>]		;
			[RECORD <rec>]		;
			[<rest:REST>]		;
			[ALL]					;
									=> __dbLocate( <{for}>, <{while}>, <next>, <rec>, <.rest.> )


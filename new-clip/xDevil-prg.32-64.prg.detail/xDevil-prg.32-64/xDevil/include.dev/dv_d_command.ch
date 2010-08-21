#command DEF <p> RANGE <x1> [,<x2>] TO <v> 	;
										=> <p> := iif(<p> == NIL, <v>, iif( <p> \< <x1> [.or. <p> \> <x2> ], <v>, <p>))

#xcommand DEFAULT <p> TO <v> [, <p2> TO <v2> ] 	;
										=> <p> := IIF(<p> == NIL, <v>, <p>) [; <p2> := IIF (<p2> == NIL, <v2>, <p2>) ]

#xcommand DEFAULT <uVar1> := <uVal1> ;
	       [, <uVarN> := <uValN> ] 		;
	       									=>		<uVar1> := If( <uVar1> == nil, <uVal1>, <uVar1> ) 			;
														[  ; <uVarN> := If( <uVarN> == nil, <uValN>, <uVarN> ) ]

#command DELETE FILE <(file)>				 => FErase( <(file)> )

#command DELETE TAG <(cOrdName1)> [ IN <(cOrdBag1)> ]		;
			 [, <(cOrdNameN)> [ IN <(cOrdBagN)> ] ]			;
									=> ordDestroy( <(cOrdName1)>, <(cOrdBag1)> ) [; ordDestroy( <(cOrdNameN)>, <(cOrdBagN)> ) ]

#command DELETE						;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[ALL]							;
			[<noopt:NOOPTIMIZE>]		;
											=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.>)		;
												; DBEvalOptimize(										;
												{|| dbDelete()},										;
												[codestr( <for> )],									;
												<{while}>, <next>, <rec>, <.rest.>				;
												)															;
												; else													;
												; DBEval(												;
												{|| dbDelete()},										;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>	;
												)						;
												; end

#command DELETE				;
			[FOR <for>]			;
			[WHILE <while>]	;
			[NEXT <next>]		;
			[RECORD <rec>]		;
			[<rest:REST>]		;
			[ALL]					;
									=> DBEval( {|| dbDelete()}, <{for}>, <{while}>, <next>, <rec>, <.rest.> )

#command DELETE						 =>  dbDelete()

#command DIR [<(spec)>]						 => __Dir( <(spec)> )

#command DISPLAY [<list,...>]			;
			[<off:OFF>]						;
			[<toPrint: TO PRINTER>]		;
			[TO FILE <(toFile)>]			;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[<all:ALL>]						;
			[<noopt:NOOPTIMIZE>]			;
												=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )					;
													;		__dbListOptimize(<.off.>, { <{list}> }, <.all.>,	;
													[codestr( <for> )], <{for}>, <{while}>, <next>,				;
													<rec>, <.rest.>, <.toPrint.>, <(toFile)> )					;
													;	else																	;
													;		__dbList(<.off.>, { <{list}> }, <.all.>,				;
													<{for}>, <{while}>, <next>, <rec>, <.rest.>,					;
													<.toPrint.>, <(toFile)> )											;
													;	endif

#command DISPLAY [<list,...>]			;
			[<off:OFF>]						;
			[<toPrint: TO PRINTER>]		;
			[TO FILE <(toFile)>]			;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[<all:ALL>]						;
												=> __DBList( <.off.>, { <{list}> }, <.all.>, <{for}>, <{while}>, <next>, <rec>, <.rest.>, <.toPrint.>, <(toFile)> )

#command DO SWITCH <exp>			=> switch <exp>

#command DO WHILE <exp>				=> while <exp>

#command DO <proc>.PRG [WITH <list,...>]					;
											=> do <proc> [ WITH <list>]


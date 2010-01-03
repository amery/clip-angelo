#command READ [MENU <oMenu>] 							;
			[MSG AT <nRow>, <nLeft>, <nRight>		;
			[MSG COLOR <cColor>]]						;
												=> ReadModal( GetList,	;
													NIL, <oMenu>, <nRow>, <nLeft>, <nRight>, <cColor> )	;
													; GetList := {}

#command READ SAVE [MENU <oMenu>] 				;
			[MSG AT <nRow>, <nLeft>, <nRight> 	;
			[MSG COLOR <cColor>]]					;
															=> ReadModal( GetList, NIL, <oMenu>, <nRow>, <nLeft>, <nRight>, <cColor> )

#command READ SAVE	;
								=> ReadModal(GetList)

#command READ			;
							=> ReadModal(GetList)															;
								; GetList := {}

#command RECALL						;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[<noopt:NOOPTIMIZE>]		;
			[ALL]							;
												=> if ( set(_SET_OPTIMIZE) )								;
													; DBEvalOptimize(											;
													{|| dbRecall()},											;
													[codestr( <for> )],										;
													<{while}>, <next>, <rec>, <.rest.> )				;
													; else														;
													; DBEval(													;
													{|| dbRecall()},											;
													<{for}>, <{while}>, <next>, <rec>, <.rest.> )	;
													; end

#command RECALL				;
			[FOR <for>]			;
			[WHILE <while>]	;
			[NEXT <next>]		;
			[RECORD <rec>]		;
			[<rest:REST>]		;
			[ALL]					;
									=> DBEval( {|| dbRecall()}, <{for}>, <{while}>, <next>, <rec>, <.rest.> )

#command RECALL				=>  dbRecall()

#command REINDEX						;
			[EVAL <eval>]				;
			[EVERY <every>]			;
			[<lNoOpt: NOOPTIMIZE>]	;
											=> ordCondSet(,,,, <{eval}>, <every>,,,,,,,,,, <.lNoOpt.>)		;
												;  ordListRebuild()

#command REINDEX									=> ordListRebuild()

#command RELEASE ALL LIKE <skel>			 	=> __MRelease( #<skel>, .t. )
#command RELEASE ALL EXCEPT <skel>			=> __MRelease( #<skel>, .f. )
#command RELEASE ALL							 	=> __MRelease("*", .t.)
#command RELEASE <vars,...>					=> __MXRelease( <"vars"> )

#command RENAME <(old)> TO <(new)>			=> FRename( <(old)>, <(new)> )

#command REPEAT 									=> while .t.

#command REPLACE [ <f1> WITH <x1> 	;
			[, <fn> WITH <xn>] ]			;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[ALL]								;
			[<noopt:NOOPTIMIZE>]			;
									=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.>)						;
										; DBEvalOptimize(														;
										{|| _FIELD-><f1> := <x1> [, _FIELD-><fn> := <xn>]},		;
										[codestr( <for> )], <{while}>, <next>, <rec>, <.rest.>	;
										)																			;
										; else																	;
										;	DBEval(																;
										{|| _FIELD-><f1> := <x1> [, _FIELD-><fn> := <xn>]},		;
											<{for}>, <{while}>, <next>, <rec>, <.rest.> )			;
										; end

#command REPLACE [ <f1> WITH <x1> 	;
			[, <fn> WITH <xn>] ]			;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[ALL]								;
									=> DBEval(																;
										{|| _FIELD-><f1> := <x1> [, _FIELD-><fn> := <xn>]},	;
										<{for}>, <{while}>, <next>, <rec>, <.rest.> )

#command REPLACE <f1> WITH <v1> [, <fN> WITH <vN> ]				;
									=> _FIELD-><f1> := <v1> [; _FIELD-><fN> := <vN>]

#command REPORT FORM <frm>				;
			[HEADING <heading>]			;
			[<plain: PLAIN>]				;
			[<noeject: NOEJECT>]			;
			[<summary: SUMMARY>]			;
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
													;		__ReportFormOptimize(										;
													<(frm)>, <.print.>, <(toFile)>, <.noconsole.>,				;
													[codestr( <for> )], <{for}>, <{while}>, <next>, <rec>,	;
													<.rest.>, <.plain.>, <heading>,									;
													<.noeject.>, <.summary.> )											;
													;	else																	;
													;		__ReportForm(													;
													<(frm)>, <.print.>, <(toFile)>, <.noconsole.>,				;
													<{for}>, <{while}>, <next>, <rec>, <.rest.>,					;
													<.plain.>, <heading>,												;
													<.noeject.>, <.summary.> )											;
													;	endif

#command REPORT FORM <frm>				;
			[HEADING <heading>]			;
			[<plain: PLAIN>]				;
			[<noeject: NOEJECT>]			;
			[<summary: SUMMARY>]			;
			[<noconsole: NOCONSOLE>]	;
			[<print: TO PRINTER>]		;
			[TO FILE <(toFile)>]			;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[ALL]								;
										=> __ReportForm(												;
											<(frm)>, <.print.>, <(toFile)>, <.noconsole.>,	;
											<{for}>, <{while}>, <next>, <rec>, <.rest.>, 	;
											<.plain.>, <heading>,									;
											<.noeject.>, <.summary.> )

#command REQUEST <vars,...>	=> EXTERNAL <vars>

#command RESTORE [FROM <(file)>] [<add: ADDITIVE>]				;
										=> __MRestore( <(file)>, <.add.> )

#command RESTORE SCREEN FROM <c>						;
		=> RestScreen( 0, 0, Maxrow(), Maxcol(), <c> )

#command RESTORE SCREEN			=> __XRestScreen()

#command RUN = <xpr>				=> ( run := <xpr> )
#command RUN := <xpr>				 => ( run := <xpr> )
#command RUN : <xpr>				=> ( run : <xpr> )
#command RUN ( <c> )				=> __Run( <c> )
#command RUN <*cmd*>				=> __Run( #<cmd> )


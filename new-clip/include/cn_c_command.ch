#command CALL <proc>() [WITH <list,...>]		;
														=> call <proc> [ WITH <list>]

#command CANCEL						 => __Quit()

#command CLEAR ALL		;
							=> CLOSE DATABASES		;
								; CLOSE FORMAT			;
								; CLEAR MEMORY			;
								; CLEAR GETS			;
								; SET ALTERNATE OFF	;
								; SET ALTERNATE TO

#command CLEAR GETS		;
							=> ReadKill(.T.)		;
								; GetList := {}

#command CLEAR MEMORY		=> __MClear()

#command CLEAR SCREEN 		=> CLS

#command CLEAR TYPEAHEAD	=> __Keyboard()

#command CLEAR 			;
							=> CLEAR SCREEN	;
								; CLEAR GETS	;

#command CLOSE ALL	;
								=> CLOSE DATABASES	;
									; SELECT 1			;
									; CLOSE FORMAT

#command CLOSE ALTERNATE		=> Set(_SET_ALTFILE, "")

#command CLOSE DATABASES		=> dbCloseAll()

#command CLOSE FORMAT			=> __SetFormat(NIL)

#command CLOSE INDEXES			=> dbClearIndex()

#command CLOSE PROCEDURE		=>

#command CLOSE <alias>			=> <alias>->( dbCloseArea() )

#command CLOSE						=> dbCloseArea()

#command CLS		;
						=> Scroll()			;
							; SetPos(0,0)

#command COMMIT						 => dbCommitAll()

#command CONNECT								;
			[RDBMS <rdbms>]					;
			[USER <user>]						;
			[PASSWORD <password>]			;
			[DATABASE <(db)>]					;
			[CHARSET <cs>]						;
			[ISOLATION LEVEL <trans>]		;
			[HOST <host>]						;
			[PORT <port>]						;
			[DUMP <tty>]						;
			[OPTIONS <opts>]					;
			TO <conn>							;
									=> <conn> := ConnectNew(<"rdbms">,<"host">,<"port">,<"user">,		;
										<"password">,<(db)>,<"tty">,<"opts">,<"cs">,<"trans">)

#command CONTINUE					=> __dbContinue()

#command COPY FILE <(src)> TO <(dest)>  => __CopyFile( <(src)>, <(dest)> )

#command COPY TO ARRAY <var>		;
			[FIELDS <fields,...>]	;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[ALL]							;
			[<x:OFF>]					;
								=> <var>:={}																		;
									;	m6_CopyToArray( @<var>, { <(fields)> }, <"for">, <{for}>,	;
									<{while}>, <next>, <rec>, <.rest.>, !<.x.>)

#command COPY [STRUCTURE] [EXTENDED] [TO <(file)>]				;
								=> __dbCopyXStruct( <(file)> )

#command COPY [STRUCTURE] [TO <(file)>] [FIELDS <fields,...>]			;
								=> __dbCopyStruct( <(file)>, { <(fields)> } )

#command COPY [TO <(file)>] 				;
			[DELIMITED [WITH <*delim*>]]	;
			[FIELDS <fields,...>]			;
			[FOR <for>]							;
			[WHILE <while>]					;
			[NEXT <next>]						;
			[RECORD <rec>]						;
			[<rest:REST>]						;
			[ALL]									;
			[<noopt:NOOPTIMIZE>]				;
											=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )								;
												;		__dbDelimOptimize(.t.,<(file)>, <(delim)>, { <(fields)> },	;
												[codestr( <for> )], <{for}>, <{while}>, <next>, <rec>, 				;
												<.rest.> )																			;
												;	else																				;
												;		__dbDelim(.t.,<(file)>, <(delim)>, { <(fields)> },				;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>	)							;
												;	endif

#command COPY [TO <(file)>] 				;
			[DELIMITED [WITH <*delim*>]]	;
			[FIELDS <fields,...>]			;
			[FOR <for>]							;
			[WHILE <while>]					;
			[NEXT <next>]						;
			[RECORD <rec>]						;
			[<rest:REST>]						;
			[ALL]									;
											=> __dbDelim(												;
												.t.,<(file)>, <(delim)>, { <(fields)> },		;
												<{for}>, <{while}>, <next>, <rec>, <.rest.> 	;
												)

#command COPY [TO <(file)>] [SDF]	;
			[FIELDS <fields,...>]		;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[ALL]								;
			[<noopt:NOOPTIMIZE>]			;
											=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )									;
												;		__dbSDFOptimize(.t.,<(file)>, { <(fields)> },						;
												[codestr( <for> )], <{for}>, <{while}>, <next>, <rec>, <.rest.> )		;
												;	else																					;
												;		__dbSDF(.t.,<(file)>, { <(fields)> },									;
												<{for}>, <{while}>, <next>, <rec>, <.rest.> )								;
												;	endif

#command COPY [TO <(file)>] [SDF]	;
			[FIELDS <fields,...>]		;
			[FOR <for>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[ALL]								;
											=> __dbSDF(													;
												.t.,<(file)>, { <(fields)> },						;
												<{for}>, <{while}>, <next>, <rec>, <.rest.> 	;
												)

#command COPY [TO <(file)>]		;
			[FIELDS <fields,...>]	;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[VIA <rdd>]					;
			[ALL]							;
			[<noopt:NOOPTIMIZE>]		;
											=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )					;
												;		__dbCopyOptimize(<(file)>, { <(fields)> },			;
												[codestr( <for> )], <{for}>, <{while}>, <next>, <rec>,	;
												<.rest.>, <rdd> )														;
												;	else																	;
												;		__dbCopy(<(file)>, { <(fields)> },						;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>, <rdd> )		;
												;	endif

#command COPY [TO <(file)>]		;
			[FIELDS <fields,...>]	;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[VIA <rdd>]					;
			[ALL]							;
											=> __dbCopy(														;
												<(file)>, { <(fields)> },									;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>, <rdd>	;
												)

#command COUNT [TO <var>]			;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[<noopt:NOOPTIMIZE>]		;
			[ALL]							;
		 									=> <var> := 0																;
												; if ( set(_SET_OPTIMIZE) )										;
												; DBEvalOptimize(														;
												{|| local( _count :=@ <var>), _count := _count + 1},		;
												[codestr( <for> )], <{while}>, <next>, <rec>, <.rest.>	;
												)																			;
												; else																	;
												;  DBEval(																;
												{|| local( _count :=@ <var>), _count := _count + 1},		;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>					;
												)																			;
												; end

#command COUNT [TO <var>]	;
			[FOR <for>]			;
			[WHILE <while>]	;
			[NEXT <next>]		;
			[RECORD <rec>]		;
			[<rest:REST>]		;
			[ALL]					;
									=> <var> := 0												;
										; DBEval(												;
										{|| <var> := <var> + 1},							;
										<{for}>, <{while}>, <next>, <rec>, <.rest.>	;
										)

#command CREATE ROWSET 					;
			[FOR [CONNECT]] <conn>		;
			[QUERY <select>]				;
			[PARAMETERS <pars>]			;
			[ON INSERT <insert>]			;
			[ON DELETE <delete>]			;
			[ON UPDATE <update>]			;
			[ON REFRESH <refresh>]		;
			[ID <id>]						;
			[ORDERS <orders>]				;
			[ID_GENERATOR <idgen>]		;
			[<nofetch:NOFETCH>]			;
			[EVAL <eval>]					;
			[EVERY <every>]				;
			[TO <rs>]						;
												=> <rs> := <conn>:createrowset(<select>,<pars>,				;
													<insert>,<delete>,<update>,<refresh>,						;
													<"id">,<orders>,<idgen>,<.nofetch.>,<{eval}>,<every>)

#command CREATE <(file1)>		;
			[FROM <(file2)>]		;
			[VIA <rdd>]				;
			[ALIAS <a>]				;
			[<new: NEW>]			;
										=> __dbCreate( <(file1)>, <(file2)>, <rdd>, <.new.>, <(a)> )


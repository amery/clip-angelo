#command INDEX ON <key> 			;
			[TAG <(cOrderName)> ] 	;
			TO <(cOrderBagName)>		;
			[FOR <for>]					;
			[<all:ALL>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[EVAL <eval>]				;
			[EVERY <every>]			;
			[<unique: UNIQUE>]		;
			[<ascend: ASCENDING>]	;
			[<descend: DESCENDING>]	;
			[<custom: CUSTOM>]		;
		 									=> ordCondSet( <"for">, <{for}>, [<.all.>], <{while}>, <{eval}>, <every>, RECNO(), 	;
		 									<next>, <rec>, [<.rest.>], [<.descend.>],,, [<.custom.>] )									;
		 									;  ordCreate(<(cOrderBagName)>, <(cOrderName)>,													;
		 									<"key">, <{key}>, [<.unique.>]		)

#command INDEX ON <key> 				;
			[TAG <(cOrderName)> ] 		;
			TO <(cOrderBagName)>			;
			[FOR <for>]						;
			[<all:ALL>]						;
			[WHILE <while>]				;
			[NEXT <next>]					;
			[RECORD <rec>]					;
			[<rest:REST>]					;
			[EVAL <eval>]					;
			[EVERY <every>]				;
			[<unique: UNIQUE>]			;
			[<ascend: ASCENDING>]		;
			[<descend: DESCENDING>]		;
												=> ordCondSet( <"for">, <{for}>,								;
													[<.all.>],														;
													<{while}>,														;
													<{eval}>, <every>,											;
													RECNO(), <next>, <rec>,										;
													[<.rest.>], [<.descend.>] )								;
													;  ordCreate(<(cOrderBagName)>, <(cOrderName)>,		;
													<"key">, <{key}>, [<.unique.>]		)

#command INDEX ON <key> TO <(file)> [<u: UNIQUE>]				;
							=> dbCreateIndex( <(file)>, <"key">, <{key}>, iif( <.u.>, .t., NIL ) )
#command INPUT [<c>] TO <var>		;
											=> if ( !Empty(__Accept(<c>)) ) 			;
												;		<var> := &( __AcceptStr() )	;
												; end


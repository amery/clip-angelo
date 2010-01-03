#command JOIN [WITH <(alias)>] [TO <file>]	;
			[FIELDS <fields,...>]					;
			[FOR <for>]									;
															=> __dbJoin( <(alias)>, <(file)>, { <(fields)> },			;
																IIF( EMPTY( #<for> ), { || .T. }, <{for}> ) )


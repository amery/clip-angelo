#command UNLOCK ALL						=> dbUnlockAll()

#command UNLOCK							=> dbUnlock()

#command UNTIL <val> 					=> if !<val>; exit; endif; enddo

#command UPDATE [FROM <(alias)>] [ON <key>]				;
			[REPLACE <f1> WITH <x1> [, <fn> WITH <xn>]]	;
			[<rand:RANDOM>]										;
													=> __dbUpdate( <(alias)>, <{key}>, <.rand.>, { || _FIELD-><f1> := <x1> [, _FIELD-><fn> := <xn>] } )

#command USE <(db)>									;
			[VIA <rdd>]									;
			[ALIAS <a>]									;
			[<new: NEW>]								;
			[<ex: EXCLUSIVE>]							;
			[<sh: SHARED>]								;
			[<ro: READONLY>]							;
			[<tmp: TEMPORARY>]						;
			[INDEX <(index1)> [, <(indexn)>]]	;
													=> dbUseArea(																;
														<.new.>, <rdd>, <(db)>, <(a)>,									;
														iif(<.sh.> .or. <.ex.>, !<.ex.>, NIL), <.ro.>,<.tmp.> )	;
														[; dbSetIndex( <(index1)> )]										;
														[; dbSetIndex( <(indexn)> )]

#command USE									=> dbCloseArea()


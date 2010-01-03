#command SAVE ALL EXCEPT <skel> TO <(file)>					;
										=> __MSave( <(file)>, <(skel)>, .f. )

#command SAVE ALL LIKE <skel> TO <(file)>					;
										=> __MSave( <(file)>, <(skel)>, .t. )

#command SAVE SCREEN TO <var>						;
										=> <var> := SaveScreen( 0, 0, Maxrow(), Maxcol() )

#command SAVE SCREEN				=> __XSaveScreen()

#command SAVE TO <(file)> ALL EXCEPT <skel>					;
										=> __MSave( <(file)>, <(skel)>, .f. )

#command SAVE TO <(file)> ALL LIKE <skel>					;
										=> __MSave( <(file)>, <(skel)>, .t. )

#command SAVE [TO <(file)>] [ALL]													;
										=> __MSave( <(file)>, "*", .t. )

#command SEARCH [FOR <for>] [TO <var>]											;
										=> <var> := {}							;
											;	m6_Search( <"for">, <{for}>, @<var> )

#command SEEK EVAL <expB>							;
										=> clip_evalseek( <expB> )

#command SEEK REGULAR <regexp>	;
			[<next:NEXT>]				;
											=> clip_regexseek( <regexp>, <.next.> )

#command SEEK <xpr>				;
			[<soft: SOFTSEEK>]	;
			[<last: LAST>]			;
										=> dbSeek( <xpr>, iif( <.soft.>, .T., NIL ), iif( <.last.>, .T., NIL ) )

#command SELECT <whatever>					=> dbSelectArea( <(whatever)> )
#command SELECT <f>([<list,...>])		=> dbSelectArea( <f>(<list>) )

#command SET ALTERNATE TO <(file)> [<add: ADDITIVE>]				;
													=> Set( _SET_ALTFILE, <(file)>, <.add.> )
#command SET ALTERNATE TO					=> Set( _SET_ALTFILE, "" )
#command SET ALTERNATE <x:ON,OFF,&>		=> Set( _SET_ALTERNATE, <(x)> )
#command SET ALTERNATE (<x>)				=> Set( _SET_ALTERNATE, <x> )

#command SET AUTORDER TO <x>				=> Set( _SET_AUTORDER, <x> )
#command SET AUTORDER TO					=> Set( _SET_AUTORDER,  0  )
#command SET AUTOORDER TO <x>				=> Set( _SET_AUTORDER, <x> )
#command SET AUTOORDER TO					=> Set( _SET_AUTORDER,  0  )

#command SET AUTOPEN <x:ON,OFF,&>		=> Set( _SET_AUTOPEN, <(x)> )
#command SET AUTOPEN (<x>)					=> Set( _SET_AUTOPEN, <x> )
#command SET AUTOOPEN <x:ON,OFF,&>		=> Set( _SET_AUTOPEN, <(x)> )
#command SET AUTOOPEN (<x>)				=> Set( _SET_AUTOPEN, <x> )

#command SET AUTOSHARE TO <x>				=> Set( _SET_AUTOSHARE, <x> )
#command SET AUTOSHARE TO					=> Set( _SET_AUTOSHARE,  0  )

#command SET BELL <x:ON,OFF,&>			=> Set( _SET_BELL, <(x)> )
#command SET BELL (<x>)						=> Set( _SET_BELL, <x> )

#command SET BUFFERING <x:ON,OFF,&>		=> Set( _SET_BUFFERING, <(x)> )
#command SET BUFFERING (<x>)				=> Set( _SET_BUFFERING, <x> )

#command SET CANCEL <x:ON,OFF,&>			=> Set( _SET_CANCEL, <(x)> )
#command SET CANCEL (<x>)					=> Set( _SET_CANCEL, <x> )

#command SET CENTURY <x:ON,OFF,&>		=> __SetCentury( <(x)> )
#command SET CENTURY (<x>)					=> __SetCentury( <x> )

#command SET COLOUR TO [<*spec*>]		=> SET COLOR TO [<spec>]
#command SET COLOR TO [<*spec*>]			=> SetColor( #<spec> )
#command SET COLOR TO ( <c> )				=> SetColor( <c> )

#command SET CONSOLE <x:ON,OFF,&>		=> Set( _SET_CONSOLE, <(x)> )
#command SET CONSOLE (<x>)					=> Set( _SET_CONSOLE, <x> )

#command SET CONFIRM <x:ON,OFF,&>		=> Set( _SET_CONFIRM, <(x)> )
#command SET CONFIRM (<x>)					=> Set( _SET_CONFIRM, <x> )

#command SET CURSOR <x:ON,OFF,&> 		=> SetCursor( iif(Upper(<(x)>) == "ON", 1, 0) )
#command SET CURSOR (<x>) 					=> SetCursor( iif(<x>, 1, 0) )

#command SET DATE FORMAT [TO] <c>		=> Set( _SET_DATEFORMAT, <c> )
#command SET DATE [TO] AMERICAN			=> _DFSET( "mm/dd/yyyy", "mm/dd/yy" )
#command SET DATE [TO] ANSI				=> _DFSET( "yyyy.mm.dd", "yy.mm.dd" )
#command SET DATE [TO] BRITISH			=> _DFSET( "dd/mm/yyyy", "dd/mm/yy" )
#command SET DATE [TO] FRENCH				=> _DFSET( "dd/mm/yyyy", "dd/mm/yy" )
#command SET DATE [TO] GERMAN				=> _DFSET( "dd.mm.yyyy", "dd.mm.yy" )
#command SET DATE [TO] ITALIAN			=> _DFSET( "dd-mm-yyyy", "dd-mm-yy" )
#command SET DATE [TO] JAPANESE			=> _DFSET( "yyyy/mm/dd", "yy/mm/dd" )
#command SET DATE [TO] USA					=> _DFSET( "mm-dd-yyyy", "mm-dd-yy" )

#command SET DBF CHARSET TO <(csname)>					;
		=> Set( "DBF_CHARSET", <(csname)> )

#command SET DECIMALS TO <x>				=> Set( _SET_DECIMALS, <x> )
#command SET DECIMALS TO					=> Set( _SET_DECIMALS, 0 )

#command SET DEFAULT TO <(path)>			=> Set( _SET_DEFAULT, <(path)> )
#command SET DEFAULT TO						=> Set( _SET_DEFAULT, "" )

#command SET DELETED <x:ON,OFF,&>		=>  Set( _SET_DELETED, <(x)> )
#command SET DELETED (<x>)					=>  Set( _SET_DELETED, <x> )

#command SET DELIMITERS <x:ON,OFF,&>	=> Set( _SET_DELIMITERS, <(x)> )
#command SET DELIMITERS (<x>)				=> Set( _SET_DELIMITERS, <x> )

#command SET DELIMITERS TO DEFAULT		=> Set( _SET_DELIMCHARS, "::" )
#command SET DELIMITERS TO <c>			=> Set( _SET_DELIMCHARS, <c> )
#command SET DELIMITERS TO					=> Set( _SET_DELIMCHARS, "::" )

#command SET DESCENDING OFF				=> ordDescend( ,, .F. )
#command SET DESCENDING ON					=> ordDescend( ,, .T. )

#command SET DEVICE TO PRINTER			=> Set( _SET_DEVICE, "PRINTER" )
#command SET DEVICE TO SCREEN				=> Set( _SET_DEVICE, "SCREEN" )

#command SET DIRECTORY CREATE MODE <n> => Set( _SET_DIRCREATEMODE, <"n"> )

#command SET DISPBOX <x:ON,OFF,&>		=> Set( _SET_DISPBOX, <(x)> )
#command SET DISPBOX (<x>)					=> Set( _SET_DISPBOX, <x> )

#command SET ECHO <*x*>						=>

#command SET EPOCH TO <year>				=> Set( _SET_EPOCH, <year> )

#command SET ESCAPE <x:ON,OFF,&>			=> Set( _SET_ESCAPE, <(x)> )
#command SET ESCAPE (<x>)					=> Set( _SET_ESCAPE, <x> )

#command SET EVENTMASK TO <x>				=> Set( _SET_EVENTMASK, <x> )

#command SET EXACT <x:ON,OFF,&>			=> Set( _SET_EXACT, <(x)> )
#command SET EXACT (<x>)					=> Set( _SET_EXACT, <x> )

#command SET EXCLUSIVE <x:ON,OFF,&>		=>  Set( _SET_EXCLUSIVE, <(x)> )
#command SET EXCLUSIVE (<x>)				=>  Set( _SET_EXCLUSIVE, <x> )

#command SET FILE CREATE MODE <n>		=> Set( _SET_FILECREATEMODE, <"n"> )

#command SET FILEIO MODE RAW				=> Set( _SET_BUFFERING, .f. )
#command SET FILEIO MODE BUFFERING		=> Set( _SET_BUFFERING, .t. )
#command SET FILEIO MODE BUFFERED		=> Set( _SET_BUFFERING, .t. )

#command SET FILTER TO <xpr>		;
			[<noopt:NOOPTIMIZE>]		;
													=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.>)				;
														;		dbSetFilter( <{xpr}>, codestr( <xpr> ) )				;
														; else								;
														;		dbSetFilter( <{xpr}>, <"xpr">, .t. )								;
														; end

#command SET FILTER TO <xpr>				=> dbSetFilter( <{xpr}>, <"xpr"> )

#command SET FILTER TO <x:&> 				=> if ( Empty(<(x)>) )						;
														;		dbClearFilter()															;
														; else								;
														;		dbSetFilter( <{x}>, <(x)> )											;
														; end

#command SET FILTER TO						=> dbClearFilter(NIL)

#command SET FIXED <x:ON,OFF,&>			=> Set( _SET_FIXED, <(x)> )
#command SET FIXED (<x>)					=> Set( _SET_FIXED, <x> )

#command SET FOPEN MODE <x:ON,OFF,&>	=> Set( _SET_FOPENMODE, <(x)> )
#command SET FOPEN MODE [TO] DOS			=> Set( _SET_FOPENMODE, .f. )
#command SET FOPEN MODE [TO] POSIX		=> Set( _SET_FOPENMODE, .t. )
#command SET FOPEN MODE [TO] UNIX		=> Set( _SET_FOPENMODE, .t. )
#command SET FOPEN MODE (<x>)				=> Set( _SET_FOPENMODE, <x> )

#command SET FORMAT TO <proc>.<ext>		=> _ProcReq_( <(proc)> + "." + <(ext)> )				;
														; __SetFormat( {|| <proc>()} )
#command SET FORMAT TO <proc>				=> _ProcReq_( <(proc)> + ".FMT" )											;
														; __SetFormat( {|| <proc>()} )
#command SET FORMAT TO <x:&>				=> if ( Empty(<(x)>) )						;
														;	SET FORMAT TO																;
														; else								;
														;	__SetFormat( &("{||" + <(x)> + "()}") )				;
														; end
#command SET FORMAT TO 						=> __SetFormat()

#command SET FUNCTION <n> [TO] [<c>]	=> __SetFunction( <n>, <c> )

#command SET HEADING <*x*>					=>

/* Visual FoxPro */
/* usa */
#command SET HOURS [TO] 12					=> Set( _SET_HOURS, 0 )
/* euro */
#command SET HOURS [TO] 24					=> Set( _SET_HOURS, 1 )

#command SET IDLE INKEY <x:ON,OFF,&>	=> Set( _SET_IDLE_INKEY, <(x)> )

#command SET INDEX BUFFER LIMIT [TO] <x> => Set( _SET_INDEX_BUFFER_LIMIT, <x> )

#command SET INDEX TO [ <(index1)> [, <(indexn)>]] [<add: ADDITIVE>]	;
													=> if !<.add.>	; ordListClear()	; end					;
														[; ordListAdd( <(index1)> )]						;
														[; ordListAdd( <(indexn)> )]

#command SET INKEY IDLE <x:ON,OFF,&>	=> Set( _SET_IDLE_INKEY, <(x)> )

#command SET INTENSITY <x:ON,OFF,&>		=> Set( _SET_INTENSITY, <(x)> )
#command SET INTENSITY (<x>)				=> Set( _SET_INTENSITY, <x> )

#command SET KEY <n> TO <proc>			=> SetKey( <n>, {|p, l, v| <proc>(p, l, v)} )

#command SET KEY <n> TO <proc> ( [<list,...>] )		=> SET KEY <n> TO <proc>

#command SET KEY <n> TO <proc:&>			=> if ( Empty(<(proc)>) )						;
														;	SetKey( <n>, NIL )						;
														; else								;
														;	SetKey( <n>, {|p, l, v| <proc>(p, l, v)} )			;
														; end

#command SET KEY <n> [TO]					=> SetKey( <n>, NIL )

#command SET LOCKSTYLE [TO] CLIPPER		=> Set( _SET_LOCKSTYLE, 0 )
#command SET LOCKSTYLE [TO] CLIPPER50	=> Set( _SET_LOCKSTYLE, 0 )
#command SET LOCKSTYLE [TO] CLIPPER53	=> Set( _SET_LOCKSTYLE, 0 )
#command SET LOCKSTYLE [TO] CLIPPER52	=> Set( _SET_LOCKSTYLE, 1 )
#command SET LOCKSTYLE [TO] FOXPRO		=> Set( _SET_LOCKSTYLE, 1 )
#command SET LOCKSTYLE [TO] SIX			=> Set( _SET_LOCKSTYLE, 1 )

#command SET LOGFILE TO <(fname)>		=> Set( _SET_LOGFILE, <(fname)> )

#command SET LOGLEVEL TO <n>				=> Set( _SET_LOGLEVEL, <n> )

#command SET LOGLEVEL <n>					=> Set( _SET_LOGLEVEL, <n> )

#command SET MACRO_IN_STRING <x:ON,OFF,&>		=> Set( _SET_MACRO_IN_STRING, <(x)> )
#command SET MACRO_IN_STRING (<x>)				=> Set( _SET_MACRO_IN_STRING, <x> )

#command SET MAP FILE [TO] <x:ON,OFF,&>  		=> Set( _SET_MAP_FILE, <(x)> )

#command SET MARGIN TO <x>					=> Set( _SET_MARGIN, <x> )
#command SET MARGIN TO						=> Set( _SET_MARGIN, 0 )

#command SET MBLOCKSIZE TO <x>			=> Set( _SET_MBLOCKSIZE, <x> )

#command SET MESSAGE TO <n> [<cent: CENTER, CENTRE>]	=> Set( _SET_MESSAGE, <n> )													;
																			; Set( _SET_MCENTER, <.cent.> )

#command SET MESSAGE TO						=> Set( _SET_MESSAGE, 0 )						;
														; Set( _SET_MCENTER, .f. )

#command SET MFILEEXT TO <x>				=> Set( _SET_MFILEEXT, <x> )

#command SET MEMOBLOCK TO <x>				=> Set( _SET_MBLOCKSIZE, <x> )

#command SET MENU <*x*>						=>

#command SET MULTILOCKS [TO] <x:ON,OFF,&> => Set(_SET_MULTILOCKS, <(x)> )

#command SET NEAR <x:ON,OFF,&>			=> Set( _SET_SOFTSEEK, <(x)> )
#command SET NEAR (<x>)						=> Set( _SET_SOFTSEEK, <x> )

#command SET OPTIMIZE <x:ON,OFF,&>		=> Set( _SET_OPTIMIZE, <(x)> )
#command SET OPTIMIZE (<x>)				=> Set( _SET_OPTIMIZE, <x> )

#command SET OPTIMIZE LEVEL [TO] <x>	=> Set( _SET_OPTIMIZELEVEL, <x> )

#command SET ORDER TO TAG <(cOrder)>	;
			[IN <(cOrdBag)>]					;
													=> ordSetFocus( <(cOrder)> [, <(cOrdBag)>] )

#command SET ORDER TO <xOrder>	;
			[IN <(cOrdBag)>]			;
													=> ordSetFocus( <xOrder> [, <(cOrdBag)>] )

#command SET ORDER TO						=> ordSetFocus(0)

#command SET PATH TO <*path*>				=> Set( _SET_PATH, <(path)> )
#command SET PATH TO							=> Set( _SET_PATH, "" )

#command SET PRINTER CHARSET TO <(csname)>	=> Set( "PRINTER_CHARSET", <(csname)> )

#command SET PRINTER <x:ON,OFF,&>		=> Set( _SET_PRINTER, <(x)> )
#command SET PRINTER (<x>)					=> Set( _SET_PRINTER, <x> )

#command SET PRINTER TO						=> Set( _SET_PRINTFILE, "" )

#command SET PRINTER TO MEMBUF			=> Set( _SET_PRINTFILE, "MEMBUF" )

#command SET PRINTER TO <(file)> [<add: ADDITIVE>]	=> Set( _SET_PRINTFILE, <(file)>, <.add.> )

#command SET PROCEDURE TO					=>
#command SET PROCEDURE TO <*f*>			=>  //_ProcReq_( <(f)> )

#command SET RATIONAL <x:ON,OFF,&>		=> Set( _SET_RATIONAL, <(x)> )
#command SET RATIONAL (<x>)				=> Set( _SET_RATIONAL, <x> )

#command SET RELATION							;
			[<add:ADDITIVE>]						;
			[TO <key1> INTO <(alias1)> 		;
			[, [TO] <keyn> INTO <(aliasn)>]]	;
													=> if ( !<.add.> )													;
														;		dbClearRel()												;
														; end																	;
														; dbSetRelation( <(alias1)>, <{key1}>, <"key1"> )		;
														[; dbSetRelation( <(aliasn)>, <{keyn}>, <"keyn"> )]

#command SET RELATION TO			=> dbClearRel()

#command SET SAFETY <*x*>			=>

#command SET SCOPE TO							=> OrdScope( 0, nil )	;
						; OrdScope( 1, nil )
#command SET SCOPE TO <x>, <y>				=> OrdScope( 0, <x> );
						; OrdScope( 1, <y> )
#command SET SCOPE TO <x>						=> OrdScope( 0, <x> );
						; OrdScope( 1, <x> )
#command SET SCOPE TO ,<x>						=> OrdScope( 1, <x> )

#command SET SCOPEBOTTOM TO					=> OrdScope( 1, nil )
#command SET SCOPEBOTTOM TO <x>				=> OrdScope( 1, <x> )

#command SET SCOPETOP TO						=> OrdScope( 0, nil )
#command SET SCOPETOP TO <x>					=> OrdScope( 0, <x> )

#command SET SCOREBOARD <x:ON,OFF,&>		=> Set( _SET_SCOREBOARD, <(x)> )
#command SET SCOREBOARD (<x>)				 	=> Set( _SET_SCOREBOARD, <x> )

#command SET SECONDS OFF						=> Set( _SET_SECONDS, .f. )
#command SET SECONDS ON							=> Set( _SET_SECONDS, .t. )

#command SET SOFTSEEK <x:ON,OFF,&>			=>  Set( _SET_SOFTSEEK, <(x)> )
#command SET SOFTSEEK (<x>)					=>  Set( _SET_SOFTSEEK, <x> )

#command SET STATUS <*x*>						=>

#command SET STEP <*x*>							=>

#command SET STRICTREAD <x:ON,OFF,&>		=> Set( _SET_STRICTREAD, <(x)> )
#command SET STRICTREAD (<x>)				 	=> Set( _SET_STRICTREAD, <x> )

#command SET TALK <*x*>							=>

#command SET TRANSLATE <x:ON,OFF,&>			=> Set( _SET_TRANSLATE_PATH, <(x)> )
#command SET TRANSLATE (<x>)					=> Set( _SET_TRANSLATE_PATH, <x> )
#command SET TRANSLATE PATH <x:ON,OFF,&>	=> Set( _SET_TRANSLATE_PATH, <(x)> )
#command SET TRANSLATE PATH (<x>)			=> Set( _SET_TRANSLATE_PATH, <x> )

#command SET TYPEAHEAD TO <n>					=> Set( _SET_TYPEAHEAD, <n> )

#command SET UNIQUE <x:ON,OFF,&>				=>  Set( _SET_UNIQUE, <(x)> )
#command SET UNIQUE (<x>)						=>  Set( _SET_UNIQUE, <x> )

#command SET UTF8TERM <x:ON,OFF,&>			=> Set( _SET_UTF8TERM, <(x)> )
#command SET UTF8TERM (<x>)					=> Set( _SET_UTF8TERM, <x> )

#command SET VIDEOMODE TO <x>					=> Set( _SET_VIDEOMODE, <x> )

#command SET WRAP <x:ON,OFF,&>  				=> Set( _SET_WRAP, <(x)> )
#command SET WRAP (<x>)							=> Set( _SET_WRAP, <x> )

#command SKIP ALIAS <a>							=> <a> -> ( dbSkip(1) )
#command SKIP <n> ALIAS <a>		 			=> <a> -> ( dbSkip(<n>) )
#command SKIP										=> dbSkip(1)
#command SKIP <n>									=> dbSkip( <n> )

#command SORT [TO <(file)>] 		;
			[ON <fields,...>]			;
			[FOR <for>]					;
			[WHILE <while>]			;
			[NEXT <next>]				;
			[RECORD <rec>]				;
			[<rest:REST>]				;
			[ALL]							;
			[<noopt:NOOPTIMIZE>]		;
													=> if ( set(_SET_OPTIMIZE) .and. !<.noopt.> )					;
														;		__dbSortOptimize( <(file)>, { <(fields)> },			;
														[codestr( <for> )], <{for}>, <{while}>, <next>, <rec>,	;
														<.rest.> )																;
														;	else																	;
														;		__dbSort(  <(file)>, { <(fields)> },					;
														<{for}>, <{while}>, <next>, <rec>, <.rest.> )				;
														;	endif

#command SORT [TO <(file)>] 	;
			[ON <fields,...>]		;
			[FOR <for>]				;
			[WHILE <while>]		;
			[NEXT <next>]			;
			[RECORD <rec>]			;
			[<rest:REST>]			;
			[ALL]						;
													=> __dbSort( <(file)>, { <(fields)> }, <{for}>, <{while}>, <next>, <rec>, <.rest.> )

#command STORE <value> TO <var1> [, <varN> ]	;
													=> <var1> := [ <varN> := ] <value>

#command SUM [ <x1> [, <xn>]  TO  <v1> [, <vn>] ]	;
			[FOR <for>]											;
			[WHILE <while>]									;
			[NEXT <next>]										;
			[RECORD <rec>]										;
			[<rest:REST>]										;
			[ALL]													;
			[<noopt:NOOPTIMIZE>]								;
													=> <v1> := [ <vn> := ] 0												;
														; if ( set(_SET_OPTIMIZE) )										;
														;		DBEvalOptimize(												;
														{|| local( <#v1#>:=@<v1> [, <#vn#>:=@<vn>]),					;
														<#v1#> := <#v1#> + <x1> [, <#vn#> := <#vn#> + <xn> ]},	;
														[codestr( <for> )], <{while}>, <next>, <rec>, <.rest.>	;
															)																		;
														; else																	;
														;		DBEval(															;
														{|| local( <#v1#>:=@<v1> [, <#vn#>:=@<vn>]),					;
														<#v1#> := <#v1#> + <x1> [, <#vn#> := <#vn#> + <xn> ]},	;
														<{for}>, <{while}>, <next>, <rec>, <.rest.>					;
														)																			;
														; end

#command SUM [ <x1> [, <xn>]  TO  <v1> [, <vn>] ]	;
			[FOR <for>]											;
			[WHILE <while>]									;
			[NEXT <next>]										;
			[RECORD <rec>]										;
			[<rest:REST>]										;
			[ALL]													;
													=> <v1> := [ <vn> := ] 0											;
														; DBEval(															;
														{|| <v1> := <v1> + <x1> [, <vn> := <vn> + <xn> ]}, 	;
														<{for}>, <{while}>, <next>, <rec>, <.rest.> )


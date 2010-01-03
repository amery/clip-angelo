#command APPEND BLANK				=> dbAppend()

#command APPEND [FROM <(file)>] [DELIMITED [WITH <*delim*>]]			;
		[FIELDS <fields,...>]														;
		[FOR <for>]																		;
		[WHILE <while>]																;
		[NEXT <next>]																	;
		[RECORD <rec>]																	;
		[<rest:REST>]																	;
		[ALL]																				;
											=> __dbDelim(													;
												.f., <(file)>, <(delim)>, { <(fields)> },			;
												<{for}>, <{while}>, <next>, <rec>, <.rest.>		;
												)


#command APPEND [FROM <(file)>] [SDF]	;
		[FIELDS <fields,...>]				;
		[FOR <for>]								;
		[WHILE <while>]						;
		[NEXT <next>]							;
		[RECORD <rec>]							;
		[<rest:REST>]							;
		[ALL]										;
									=> __dbSDF(														;
										.f., <(file)>, { <(fields)> },						;
										<{for}>, <{while}>, <next>, <rec>, <.rest.>		;
										)

#command APPEND [FROM <(file)>]	;
		[FIELDS <fields,...>]		;
		[FOR <for>]						;
		[WHILE <while>]				;
		[NEXT <next>]					;
		[RECORD <rec>]					;
		[<rest:REST>]					;
		[VIA <rdd>]						;
		[ALL]								;
									=> __dbApp(							;
										<(file)>, { <(fields)> },					;
										<{for}>, <{while}>, <next>, <rec>, <.rest.>, <rdd>	;
										)

#command AT <top>, <left> CLEAR TO <bottom>, <right>						;
							COLOR <color>												;
														=> DispBox(																		;
															<top>, <left>, <bottom>, <right>, " "								;
															, <color> )																	;
															; SetPos( <top>, <left> )

#command AT <top>, <left> CLEAR TO <bottom>, <right>						;
																	=> Scroll( <top>, <left>, <bottom>, <right> )						;
																		; SetPos( <top>, <left> )

#command AT <top>, <left> CLEAR		;
												=> Scroll( <top>, <left> )													;
													; SetPos( <top>, <left> )

#command AT <row>, <col> GET <var>			;
			CHECKBOX									;
			[VALID <valid>]						;
			[WHEN <when>]							;
			[CAPTION <caption>]					;
			[MESSAGE <message>]					;
			[COLOR <color>]						;
			[FOCUS <fblock>]						;
			[STATE <sblock>]						;
			[STYLE <style>]						;
			[SEND <msg>]							;
			[GUISEND <guimsg>]					;
			[BITMAPS <aBitmaps>]					;
														=> aadd(GetList,getnew(<row>,<col>,										;
															{|_1| local(_p:=@<var>), iif(_1==NIL,_p,_p:=_1 ) },			;
															<(var)>,NIL,<color>,<var>,<{valid}>,<{when}>) )					;
															; ATail(GetList):Control := _CheckBox_( <var>, <caption>,	;
															<message>, <color>, <{fblock}>, <{sblock}>,						;
															<style>, <aBitmaps>,<row>,<col>-1 )									;
															; ATail(GetList):reader  := { | a, b, c, d |						;
															GuiReader( a, b, c, d ) }												;
		 													[; ATail(GetList):<msg>]												;
		 													[; ATail(GetList):Control:<guimsg>]									;
															; ATail(GetList):Control:Display()

#command AT <top>, <left>, <bottom>, <right> BOX <string>	;
							[COLOR <color>]							;
																			=> DispBox(																		;
																				<top>, <left>, <bottom>, <right>, <string>						;
																				[, <color> ]																;
																				)

#command AT <top>, <left> TO <bottom>, <right> [DOUBLE]	;
							[COLOR <color>]							;
																			=> DispBox(																		;
																				<top>, <left>, <bottom>, <right>, 2									;
																				[, <color> ]																;
																				)

#command AT <top>, <left> TO <bottom>, <right> [COLOR <color>]	;
																					=> DispBox(																		;
																						<top>, <left>, <bottom>, <right>, 1									;
																						[, <color> ]																;
																						)

#command AT <top>, <left>, <bottom>, <right> GET <var>	;
			LISTBOX		<items>									;
			[VALID <valid>]										;
			[WHEN <when>]											;
			[CAPTION <caption>]									;
			[MESSAGE <message>]									;
			[COLOR <color>]										;
			[FOCUS <fblock>]										;
			[STATE <sblock>]										;
			[<drop: DROPDOWN>]									;
			[<scroll: SCROLLBAR>]								;
			[SEND <msg>]											;
			[GUISEND <guimsg>]									;
			[BITMAP <cBitmap>]									;
													=> SetPos( <top>, <left> )													;
														; AAdd( GetList,															;
														_GET_( <var>, <(var)>, NIL, <{valid}>, <{when}> ) )			;
														; ATail(GetList):Control := _ListBox_( ATail(Getlist):row,	;
														ATail(Getlist):col,														;
														<bottom>, <right>, <var>, <items>, <caption>, <message>,		;
														<color>, <{fblock}>, <{sblock}>, <.drop.>, 						;
														<.scroll.>, <cBitmap> )													;
														; ATail(GetList):reader  := { | a, b, c, d |						;
														GuiReader( a, b, c, d ) }												;
														[; ATail(GetList):<msg>]												;
														[; ATail(GetList):Control:<guimsg>]									;
														; ATail(GetList):Control:Display()

#command AT <top>, <left>, <bottom>, <right> GET <var>	;
			TBROWSE <oBrowse>										;
			ALIAS <alias>											;
			[VALID <preBlock>]									;
			[WHEN <postBlock>]									;
			[MESSAGE <message>]									;
			[SEND <msg>]											;
			[GUISEND <guimsg>]									;
												=> SetPos( <top>, <left> )													;
													; AAdd( GetList,															;
													_GET_( <var>, <(var)>, NIL, <{preBlock}>, <{postBlock}> ) )	;
													; <oBrowse>:ntop			:= ATail(Getlist):row					;
													; <oBrowse>:nleft			 := ATail(Getlist):col					;
													; <oBrowse>:nbottom		:= <bottom>									;
													; <oBrowse>:nright			:= <right>								;
													; <oBrowse>:Configure()													;
													; ATail(GetList):Control := <oBrowse>								;
													; ATail(GetList):reader  := { | a, b, c, d |						;
													<alias>->( TBReader( a, b, c, d ) ) }								;
													[; ATail(GetList):Control:Message := <message>]					;
													[; ATail(GetList):<msg>]												;
													[; ATail(GetList):Control:<guimsg>]

#command AT <top>, <left>, <bottom>, <right> GET <var>		;
			RADIOGROUP <buttons>										;
			[VALID <valid>]											;
			[WHEN <when>]												;
			[CAPTION <caption>]										;
			[MESSAGE <message>]										;
			[COLOR <color>]											;
			[FOCUS <fblock>]											;
			[STYLE <style>]											;
			[SEND <msg>]												;
			[GUISEND <guimsg>]										;
												=> SetPos( <top>, <left> )														;
													; AAdd( GetList,																;
													_GET_( <var>, <(var)>, NIL, <{valid}>, <{when}> ) )				;
													; ATail(GetList):Control := _RadioGrp_( ATail(Getlist):row,		;
													ATail(Getlist):col,															;
													<bottom>, <right>, <var>, <buttons>, <caption>, <message>, 	;
													<color>, <{fblock}>, <style> )											;
													; ATail(GetList):reader  := { | a, b, c, d |							;
													GuiReader( a, b, c, d ) }													;
													[; ATail(GetList):<msg>]													;
													[; ATail(GetList):Control:<guimsg>]										;
													; ATail(GetList):Control:Display()

#command AT <top>, <left>, <bottom>, <right> GET <var>	;
			TBROWSE <oBrowse>										;
			[VALID <preBlock>]									;
			[WHEN <postBlock>]									;
			[MESSAGE <message>]									;
			[SEND <msg>]											;
			[GUISEND <guimsg>]									;
												=> SetPos( <top>, <left> )														;
													; AAdd( GetList,																;
													_GET_( <var>, <(var)>, NIL, <{preBlock}>, <{postBlock}> ) )		;
													; <oBrowse>:ntop			:= ATail(Getlist):row						;
													; <oBrowse>:nleft			 := ATail(Getlist):col						;
													; <oBrowse>:nbottom		:= <bottom>										;
													; <oBrowse>:nright			:= <right>									;
													; <oBrowse>:Configure()														;
													; ATail(GetList):Control := <oBrowse>									;
													; ATail(GetList):reader  := { | a, b, c, d |							;
													TBReader( a, b, c, d ) }													;
													[; ATail(GetList):Control:Message := <message>]						;
													[; ATail(GetList):<msg>]													;
													[; ATail(GetList):Control:<guimsg>]

#command AT <top>, <left>, <bottom>, <right> GET <var>	;
			TEXT														;
			[VALID <valid>]										;
			[WHEN <when>]											;
			[SEND <msg>]											;
												=> aadd(GetList,textgetnew(<top>,<left>,<bottom>,<right>, 	;
													{|_1| local(_p:=@<var>), iif(_1==NIL,_p,_p:=_1 ) },		;
													<(var)>,setcolor(),<var>,<{valid}>,<{when}>) )				;
													[; ATail(GetList):<msg>]

#command AT <row>, <col> GET <var>		;
			CHECKBOX								;
			[VALID <valid>]					;
			[WHEN <when>]						;
			[CAPTION <caption>]				;
			[MESSAGE <message>]				;
			[COLOR <color>]					;
			[FOCUS <fblock>]					;
			[STATE <sblock>]					;
			[STYLE <style>]					;
			[SEND <msg>]						;
			[GUISEND <guimsg>]				;
			[BITMAPS <aBitmaps>]				;
													=> SetPos( <row>, <col> )													;
														; AAdd( GetList,															;
														_GET_( <var>, <(var)>, NIL, <{valid}>, <{when}> ) )			;
														; ATail(GetList):Control := _CheckBox_( <var>, <caption>,	;
														<message>, <color>, <{fblock}>, <{sblock}>,						;
														<style>, <aBitmaps> )													;
														; ATail(GetList):reader  := { | a, b, c, d |						;
														GuiReader( a, b, c, d ) }												;
														[; ATail(GetList):<msg>]												;
														[; ATail(GetList):Control:<guimsg>]									;
														; ATail(GetList):Control:Display()

#command AT <row>, <col> GET <var>				;
			PUSHBUTTON									;
			[VALID <valid>]							;
			[WHEN <when>]								;
			[CAPTION <caption>]						;
			[MESSAGE <message>]						;
			[COLOR <color>]							;
			[FOCUS <fblock>]							;
			[STATE <sblock>]							;
			[STYLE <style>]							;
			[SEND <msg>]								;
			[GUISEND <guimsg>]						;
			[SIZE X <sizex> Y <sizey>]				;
			[CAPOFF X <capxoff> Y <capyoff>]		;
			[BITMAP <bitmap>]							;
			[BMPOFF X <bmpxoff> Y <bmpyoff>]		;
															=> SetPos( <row>, <col> )															;
																; AAdd( GetList,																	;
																_GET_( <var>, <(var)>, NIL, <{valid}>, <{when}> ) )					;
																; ATail(GetList):Control := _PushButt_( <caption>, <message>,		;
																<color>, <{fblock}>, <{sblock}>, <style>,									;
																<sizex>, <sizey>, <capxoff>, <capyoff>,									;
																<bitmap>, <bmpxoff>, <bmpyoff> )												;
																; ATail(GetList):reader  := { | a, b, c, d |								;
																GuiReader( a, b, c, d ) }														;
																[; ATail(GetList):<msg>]														;
																[; ATail(GetList):Control:<guimsg>]											;
																; ATail(GetList):Control:Display()

#command AT <row>, <col> GET <var>		;
			[PICTURE <pic>]					;
			[VALID <valid>]					;
			[WHEN <when>]						;
			[CAPTION <caption>]				;
			[MESSAGE <message>]				;
			[SEND <msg>]						;
													=> SetPos( <row>, <col> )												;
														; AAdd( GetList,														;
														_GET_( <var>, <"var">, <pic>, <{valid}>, <{when}> ) ) 	;
														[; ATail(GetList):Caption := <caption>]						;
														[; ATail(GetList):CapRow  := ATail(Getlist):row				;
														; ATail(GetList):CapCol  := ATail(Getlist):col -			;
														__CapLength(<caption>) - 1]										;
														[; ATail(GetList):message := <message>]						;
														[; ATail(GetList):<msg>]											;
														; ATail(GetList):Display()

#command AT <row>, <col> GET <var>			;
			[<clauses,...>]						;
			COLOR <color>							;
			[<moreClauses,...>]					;
													=> @ <row>, <col> GET <var>					;
														[<clauses>]										;
														SEND colorDisp(<color>)						;
														[<moreClauses>]

#command AT <row>, <col> GET <var>	;
			[<clauses,...>]				;
			RANGE <lo>, <hi>				;
			[<moreClauses,...>]			;
												=> @ <row>, <col> GET <var>							;
													[<clauses>]												;
													VALID {|_1| RangeCheck(_1,, <lo>, <hi>)}		;
													[<moreClauses>]

#command AT <row>, <col> PROMPT <prompt> [MESSAGE <msg>]	;
																			=> __AtPrompt( <row>, <col>, <prompt> , <msg> )

#command AT <row>, <col> SAY <xpr>	;
			[PICTURE <pic>]				;
			[COLOR <color>]				;
												=> DevPos( <row>, <col> )							;
													; DevOutPict( <xpr>, <pic> [, <color>] )

#command AT <row>, <col> SAY <xpr>	;
			[COLOR <color>]				;
												=> DevPos( <row>, <col> )						;
													; DevOut( <xpr> [, <color>] )

#command AT <row>, <col> SAY <sayxpr>	;
			[<sayClauses,...>]				;
			GET <var>							;
			[<getClauses,...>]				;
													=> @ <row>, <col> SAY <sayxpr> [<sayClauses>]			;
														; @ Row(), Col()+1 GET <var> [<getClauses>]

#command AT <row>, <col>		;
									=> Scroll( <row>, <col>, <row> )											;
										; SetPos( <row>, <col> )

#command AT [<clauses,...>] COLOUR [<moreClauses,...>]	;
																		=> @ [<clauses>] COLOR [<moreClauses>]

#command ACCEPT [<c>] TO <var>  	=> <var> := __Accept( <c> )

#command AVERAGE [ <x1> [, <xn>]  TO  <v1> [, <vn>] ]	;
			[FOR <for>]												;
			[WHILE <while>]										;
			[NEXT <next>]											;
			[RECORD <rec>]											;
			[<rest:REST>]											;
			[<noopt:NOOPTIMIZE>]									;
			[ALL]														;
										=> M->__Avg := <v1> := [ <vn> := ] 0								;
											; if ( set(_SET_OPTIMIZE) )										;
											;		DBEvalOptimize(												;
											{|| local( <#v1#>:=@<v1> [, <#vn#>:=@<vn>]),					;
											M->__Avg := M->__Avg + 1,											;
											<#v1#> := <#v1#> + <x1> [, <#vn#> := <#vn#> + <xn>] },	;
											[codestr( <for> )], <{while}>, <next>, <rec>, <.rest.>	;
											)																			;
											; else																	;
											;		DBEval(															;
											{|| local( <#v1#>:=@<v1> [, <#vn#>:=@<vn>]),					;
											M->__Avg := M->__Avg + 1,											;
											<#v1#> := <#v1#> + <x1> [, <#vn#> := <#vn#> + <xn>] },	;
											<{for}>, <{while}>, <next>, <rec>, <.rest.>					;
											)																			;
											; end																		;
											; <v1> := <v1> / M->__Avg [; <vn> := <vn> / M->__Avg ]

#command AVERAGE [ <x1> [, <xn>]  TO  <v1> [, <vn>] ]	;
			[FOR <for>]												;
			[WHILE <while>]										;
			[NEXT <next>]											;
			[RECORD <rec>]											;
			[<rest:REST>]											;
			[ALL]														;
										=> M->__Avg := <v1> := [ <vn> := ] 0								;
											; DBEval(																;
											{|| M->__Avg := M->__Avg + 1,										;
											<v1> := <v1> + <x1> [, <vn> := <vn> + <xn>] },				;
											<{for}>, <{while}>, <next>, <rec>, <.rest.>					;
											)																			;
											; <v1> := <v1> / M->__Avg [; <vn> := <vn> / M->__Avg ]



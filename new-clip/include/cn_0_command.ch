#command ! <*cmd*>								=> RUN <cmd>

#command ? [ <list,...> ]						=> QOut( <list> )
#command ?? [ <list,...> ]						=> QQOut( <list> )

#command @ <top>, <left> CLEAR TO <bottom>, <right>						;
							COLOR <color>												;
														=> DispBox(																		;
															<top>, <left>, <bottom>, <right>, " "								;
															, <color> )																	;
															; SetPos( <top>, <left> )

#command @ <top>, <left> CLEAR TO <bottom>, <right>						;
																	=> Scroll( <top>, <left>, <bottom>, <right> )						;
																		; SetPos( <top>, <left> )

#command @ <top>, <left> CLEAR		;
												=> Scroll( <top>, <left> )													;
													; SetPos( <top>, <left> )

#command @ <row>, <col> GET <var>			;
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

#command @ <top>, <left>, <bottom>, <right> BOX <string>	;
							[COLOR <color>]							;
																			=> DispBox(																		;
																				<top>, <left>, <bottom>, <right>, <string>						;
																				[, <color> ]																;
																				)

#command @ <top>, <left> TO <bottom>, <right> [DOUBLE]	;
							[COLOR <color>]							;
																			=> DispBox(																		;
																				<top>, <left>, <bottom>, <right>, 2									;
																				[, <color> ]																;
																				)

#command @ <top>, <left> TO <bottom>, <right> [COLOR <color>]	;
																					=> DispBox(																		;
																						<top>, <left>, <bottom>, <right>, 1									;
																						[, <color> ]																;
																						)

#command @ <top>, <left>, <bottom>, <right> GET <var>	;
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

#command @ <top>, <left>, <bottom>, <right> GET <var>	;
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

#command @ <top>, <left>, <bottom>, <right> GET <var>		;
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

#command @ <top>, <left>, <bottom>, <right> GET <var>	;
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

#command @ <top>, <left>, <bottom>, <right> GET <var>	;
			TEXT														;
			[VALID <valid>]										;
			[WHEN <when>]											;
			[SEND <msg>]											;
												=> aadd(GetList,textgetnew(<top>,<left>,<bottom>,<right>, 	;
													{|_1| local(_p:=@<var>), iif(_1==NIL,_p,_p:=_1 ) },		;
													<(var)>,setcolor(),<var>,<{valid}>,<{when}>) )				;
													[; ATail(GetList):<msg>]

#command @ <row>, <col> GET <var>		;
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

#command @ <row>, <col> GET <var>				;
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

#command @ <row>, <col> GET <var>		;
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

#command @ <row>, <col> GET <var>			;
			[<clauses,...>]						;
			COLOR <color>							;
			[<moreClauses,...>]					;
													=> @ <row>, <col> GET <var>					;
														[<clauses>]										;
														SEND colorDisp(<color>)						;
														[<moreClauses>]

#command @ <row>, <col> GET <var>	;
			[<clauses,...>]				;
			RANGE <lo>, <hi>				;
			[<moreClauses,...>]			;
												=> @ <row>, <col> GET <var>							;
													[<clauses>]												;
													VALID {|_1| RangeCheck(_1,, <lo>, <hi>)}		;
													[<moreClauses>]

#command @ <row>, <col> PROMPT <prompt> [MESSAGE <msg>]	;
																			=> __AtPrompt( <row>, <col>, <prompt> , <msg> )

#command @ <row>, <col> SAY <xpr>	;
			[PICTURE <pic>]				;
			[COLOR <color>]				;
												=> DevPos( <row>, <col> )							;
													; DevOutPict( <xpr>, <pic> [, <color>] )

#command @ <row>, <col> SAY <xpr>	;
			[COLOR <color>]				;
												=> DevPos( <row>, <col> )						;
													; DevOut( <xpr> [, <color>] )

#command @ <row>, <col> SAY <sayxpr>	;
			[<sayClauses,...>]				;
			GET <var>							;
			[<getClauses,...>]				;
													=> @ <row>, <col> SAY <sayxpr> [<sayClauses>]			;
														; @ Row(), Col()+1 GET <var> [<getClauses>]

#command @ <row>, <col>		;
									=> Scroll( <row>, <col>, <row> )											;
										; SetPos( <row>, <col> )

#command @ [<clauses,...>] COLOUR [<moreClauses,...>]	;
																		=> @ [<clauses>] COLOR [<moreClauses>]


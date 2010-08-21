function hbrowse(nTop, nLeft, nBottom, nRight, url, color)
static __STORY__
local obj
	obj := map()
	obj:className	:= "BROWSER"
	obj:nTop 	:= iif(nTop==NIL, 0, nTop)
	obj:nLeft 	:= iif(nLeft==NIL, 0, nLeft)
	obj:nBottom 	:= iif(nBottom==NIL, maxrow(), nBottom)
	obj:nRight 	:= iif(nRight==NIL, maxcol(), nRight)
	obj:colorSpec	:= iif(color==NIL, COLOR_BROWSER, color)
	obj:width	:= obj:nRight - obj:nLeft - 2
	obj:rows	:= obj:nBottom-obj:nTop-1
	obj:frames	:= {}
	obj:state	:= {}
	obj:doc		:= ""
	obj:url		:= 0
	obj:urlName	:= url
	obj:center	:= .f.
	obj:clrHist	:= {}
	obj:clr		:= 0
	obj:lists	:= {}
	obj:pre		:= .f.
	obj:parser	:= 0
	obj:startHtml	:= .f.
	obj:cnt_t	:= 0
	obj:Story	:= {}//iif(__STORY__!=NIL, aclone(__STORY__), {})
	obj:indStory	:= iif(empty(obj:Story), 0, 1)
	obj:mapKeys	:= HK_get("htmlBrowser")
	obj:str		:= ""
	obj:newStr	:= .f.
	obj:form	:= {}
	obj:ndxForm	:= 0
	obj:isForm	:= .f.
	obj:isDt	:= 0
	obj:numTable	:= 0
	obj:tbl		:= {}
	obj:tblCol	:= {}
	obj:tblRow	:= {}
	obj:tblNewLine	:= .f.
	obj:isRefer	:= .f.
	obj:hRefer	:= ""
	obj:nRefer	:= ""
	obj:iRefer	:= ""
	obj:charset	:= __CHARSET
	obj:charsetOrig	:= __CHARSET

	obj:open	:= @openBrowser()
	obj:runing	:= @runing()
	obj:getUrl	:= @br_getUrl()
	obj:parseTag	:= @parseTag()
	obj:parseNewUrl	:= @parseNewUrl()
	obj:newLine	:= @newLine()
	obj:goBack	:= @goBack()
	obj:goNext	:= @goNext()
	obj:addObject	:= @br_addObject()
	obj:SetCodePage	:= @br_SetCodePage()
	obj:reload	:= @br_reload()

	obj:parse_H	:= @parse_H()
	obj:parse_Head	:= @parse_Head()
	obj:newRefer	:= @newRefer()
	obj:parse_A	:= @parse_A()
	obj:parse_Font	:= @parse_Font()
	obj:parse_Title	:= @parse_Title()
	obj:parse_Hr	:= @parse_Hr()
	obj:parse_Ol	:= @parse_Ol()
	obj:parse_Ul	:= @parse_Ul()
	obj:parse_Li	:= @parse_Li()
	obj:parse_Dt	:= @parse_Dt()
	obj:parse_Dd	:= @parse_Dd()
	obj:parse_Image	:= @parse_Image()
	obj:parse_Table	:= @parse_Table()
	obj:parse_Col_Table := @parse_Col_Table()
	obj:parse_Tbl_Caption := @parse_Tbl_Caption()
	obj:add_Table	:= @add_Table()
	obj:parse_Form	:= @parse_Form()
	obj:parse_Input	:= @parse_Input()
	obj:parse_TextArea:= @parse_TextArea()

	obj:parse_Meta	:= @parse_Meta()

	obj:Browse_File := @Browse_File()

	obj:indTabCol	:= stackNew()
	obj:indTabRow	:= stackNew()

//	obj:close	:= @closeBrowser()
//	obj:runing	:= @runBrowser()
	obj:htmlColors	:= map()
	obj:getHcolor	:= @getHcolor()
	obj:new_clr	:= @new_clr()
	obj:rest_clr	:= @rest_clr()
	obj:__colors 	:= {}      // ������� ������
	obj:__setColor 	:= @__setcolor()
	obj:__setcolor()

	obj:menu	:= br_menu_create()
	obj:open()
return 0

function html_tagNew(tagname, other, title, style, class, event, id, lang, language)

	local mlang	:={"JAVASCRIPT","JSCRIPT","VBSCRIPT"}
	local i
	local obj	:=map()

	tagname=upper(tagName)
	obj:classname	:= "HTML_TAG"
	obj:tagName     := tagname	//
	obj:closed 	:= .f.

	obj:fields	:= map()           // list of fields this tag
	obj:fieldsOrder	:= {}           // list of fields this tag
	obj:hashes	:= map()
	obj:entities	:= {{"&amp;","&"},{"&gt;",">"},{"&lt;","<"}}

	obj:toString	:= @tag_toString()
	obj:closeString	:= @tag_closeString()
	obj:addField	:= @tag_addField()
	obj:hashname	:= @tag_hashname()
	*********
	if title!=NIL
		obj:addField("TITLE",title) // TITLE=string
	endif
	if style!=NIL
		obj:addField("STYLE",style) // STYLE=string
	endif
	if class!=NIL
		obj:addField("CLASS",class) // CLASS=string
	endif
	if event!=NIL
		obj:addField("EVENT",event) // <название_события>=название_сценария
	endif
	if id!=NIL
		obj:addField("ID",id)       // ID=string
	endif
	if valtype(language)=="N"
		if language<1 .or. language>len(mlang)
			language:=1
		endif
		language:=mlang[language]
	endif
	if language!=NIL
		obj:addField("LANGUAGE",language)
					   // LANGUAGE=JAVASCRIPT,JSCRIPT,VBSCRIPT
	endif

	if valtype(other)=="A"
		for i=1 to len(other)
			if len(other[i])==2
				obj:addField(other[i][1],other[i][2])
			endif
		next
		obj:other	:= ""
	else
		obj:other	:= other // прочие поля
	endif


return obj

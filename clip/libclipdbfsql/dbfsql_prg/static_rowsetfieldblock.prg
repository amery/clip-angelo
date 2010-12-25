STATIC FUNCTION rowsetFieldBlock(self,field)
	LOCAL b,toclip,fromclip
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)

	toclip := "_FromRDBMSToClip("+ALLTRIM(STR(self:rowset))+","+;
		ALLTRIM(STR(self:conn))+",'"+self:RDBMS+"',"+;
		ALLTRIM(STR(fieldno))+")"

	fromclip := "SQLSetValue("+ALLTRIM(STR(self:rowset))+","+;
		ALLTRIM(STR(fieldno))+",_FromClipToRDBMS("+;
		ALLTRIM(STR(self:rowset))+","+ALLTRIM(STR(self:conn))+",'"+;
		self:RDBMS+"',"+ALLTRIM(STR(fieldno))+",x,.F.))"
RETURN &("{|x| IF(x==NIL,"+toclip+","+fromclip+")}")

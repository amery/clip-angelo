STATIC FUNCTION rowsetFromClip(self,fieldno,value,totext)
RETURN _FromClipToRDBMS(self:rowset,self:conn,self:RDBMS,fieldno,value,totext)

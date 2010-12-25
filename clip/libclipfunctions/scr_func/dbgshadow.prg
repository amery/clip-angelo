function dbgshadow(x1,y1,x2,y2,attrib)
	attrib := iif(attrib==NIL,7,attrib)
	dispattr(x1+1,y2+1,x2+1,y2+2,attrib)
	dispattr(x2+1,y1+2,x2+1,y2+2,attrib)
return

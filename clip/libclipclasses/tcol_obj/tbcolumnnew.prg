func TbColumnNew(head,bsearch,fieldName)
       local obj
       obj:=map()
       obj:classname:="TBCOLUMN"
       obj:block:=bsearch
       obj:fieldName:=fieldName
       obj:cargo:=NIL
       obj:colorBlock:=NIL
       obj:colSep:=NIL
       obj:colSepH:=NIL
       obj:defColor:={1,2,1,1}
       obj:footing:=NIL
       obj:colorFooting:=NIL
       obj:footSep:=NIL
       obj:heading:=head
       obj:colorHeading:=NIL
       obj:headSep:=NIL
       obj:picture:=NIL
       obj:postBlock:=NIL
       obj:preBlock:=NIL
       obj:width:=NIL
       /* recomended type of data*/
       obj:type:=NIL
       /* display len for varchar() SQL data and other unfixed data len */
       obj:len:=NIL
return obj

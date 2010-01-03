static function rddobj_browse_StockBrowseNew(self,browse, fields, headers, footers, picts, hsep, csep, fsep )
    LOCAL n, column, cType, strhead, strfoot, col_block, astru, i
    local strpict, strhsep, strcsep, strfsep

    if empty( fields )
      astru := self:struct()
      fields:=array(len(astru))
      i:=1; aeval(astru,{|x|fields[i++]:=x[1]})
    endif
    if empty(headers); headers:={}; endif
    if empty(footers); footers:={}; endif
    if empty(picts); picts:=array(len(fields)); endif
    if empty(hsep); hsep:=array(len(fields)); endif
    if empty(csep); csep:=array(len(fields)); endif
    if empty(fsep); fsep:=array(len(fields)); endif
    if valtype(picts)=="C"
       strpict=picts
       picts:={}
       asize(picts,len(fields))
       afill(picts,strpict)
    endif
    if valtype(picts)=="A"
       for n = len(picts)+1 to len(fields)
           aadd(picts,nil)
       next
    endif
    if valtype(headers)=="C"
       strhead=headers
       headers:={}
       asize(headers,len(fields))
       afill(headers,strhead)
    endif
    if valtype(headers)=="A"
       for n = len(headers)+1 to len(fields)
           aadd(headers,fields[n])
       next
    endif
    if valtype(footers)=="C"
       strfoot=footers
       footers:={}
       asize(footers,len(fields))
       afill(footers,strfoot)
    endif
    if valtype(footers)=="A"
       for n = len(footers)+1 to len(fields)
           aadd(footers,"")
       next
    endif
    if valtype(hsep)=="C"
       strhsep=hsep; hsep:=array(len(fields)); afill(hsep,strhsep)
    endif
    if valtype(csep)=="C"
       strcsep=csep; csep:=array(len(fields)); afill(csep,strcsep)
    endif
    if valtype(fsep)=="C"
       strhsep=fsep; fsep:=array(len(fields)); afill(fsep,strfsep)
    endif
    FOR n := 1 TO len(fields)
        if valtype(fields[n])=="C"
          col_block:="{|| __rdd__ "+alltrim(str(self:handle))+" "+fields[n]+" __rdd__}"
          col_block:=&col_block
        else
          col_block:=fields[n]
        endif
        //column := TBColumnNew(Field(n), FieldWBlock(Field(n), Select()))
        column := TBColumnNew(fields[n], col_block)
        column:heading:=headers[n]
        column:footing:=footers[n]
        column:picture:=picts[n]
        column:headSep:=hsep[n]
        column:colSep:=csep[n]
        column:footSep:=fsep[n]
        browse:addColumn(column)
    next
RETURN NIL

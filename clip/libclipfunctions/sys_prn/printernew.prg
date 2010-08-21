*************************************
/* class Printer */
function printerNew(sName,sType,sDescription,sCharset,sScript,xBlock)
	local obj := map(), tmpErr, _err
	obj:classname := "PRINTER"
	obj:name      := upper(sName)
	obj:type      := iif( valtype(sType) !="C", "epson", sType)
	obj:description := sDescription
	obj:charset   := iif(sCharset==NIL, host_charset(), sCharset)
	obj:script    := sScript
	obj:block     := NIL
	if valtype(xBlock) == "B"
		obj:block     := xBlock
	elseif valType(xBlock) == "C"
	       if "{" $ xBlock .and. "|" $ xBlock
	       else
			xBlock := "{|fname|"+xBlock+"}"
	       endif
	       tmpErr:=errorBlock({|err|break(err)})
	       begin sequence
			xBlock:=&xBlock
	       recover using _err
	       end sequence
	       errorBlock(tmpErr)
	       if !empty(_err)
			outlog(3,"Error compile:",xBlock,_err)
	       else
			obj:block := xBlock
	       endif
	endif
return obj

****************************************
/* class Print Queue */
function printQueueNew(sPrinter,nCopies,sPaperSize,sOrientation,nBpage,nEpage)
	local obj := map()
	obj:classname := "PRINTQUEUE"
	obj:name      := upper(sPrinter)
	obj:copies    := iif(valtype(nCopies)=="N",nCopies,1)
	obj:psize     := iif(sPaperSize==NIL,"A4",upper(sPaperSize))
	obj:sOrientation := iif(sOrientation == NIL, "PARTRAIT",upper(sOrientation))
	obj:begPage   := iif(valtype(nBpage) != "N", 1, nBpage)
	obj:endPage   := iif(valtype(nEpage) != "N", 9999, nEpage)
return obj

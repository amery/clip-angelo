function __typeFile(fname,print)
	local sPrint:=set(_SET_PRINTER)
	local sDevice:=set(_SET_DEVICE)
	if print
		set device to printer
		set printer on
	endif

	? memoread(fname)

	set(_SET_PRINTER,sPrint)
	set(_SET_DEVICE,sDevice)
return

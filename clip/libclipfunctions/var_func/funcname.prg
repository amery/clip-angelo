****************************************************************************
function funcname(pn)
	local str,i := 2 // i>1 �������� �� � �������, ������� �������� FUNCNAME()
		   // � � ����������
	pn := upper(pn)
	do while .t.
		str := procname(i)
		if empty(str)
			exit
		endif
		if str == pn
			return .t.
		endif
		i++
	enddo
return .f.

/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

/*****************************************
����� ������ ����
     "<t1><t2><t3>"
     ���
     "t1 t2 t3"
�� ������������ ����
� ���������� ������ �����
*****************************************/
function html_tagSplit(str)
	LOCAL temp:={}, arr:={}, i:=1 ,s
	do while search("[<> ]+",str, arr)
        	s=left(str, arr[i][1]-1)
                if !empty(s)
			aadd(temp, s)
                endif
		str := substr(str, arr[i][2])
		i++
	enddo
        if !empty(str)
		aadd(temp, str)
        endif
return temp

/*****************************************
Делит строку типа
     "<t1><t2><t3>"
     или
     "t1 t2 t3"
на составляющие теги
и возвращает массив тегов
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

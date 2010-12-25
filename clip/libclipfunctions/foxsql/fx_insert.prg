**************************************************
* INSERT INTO data VALUES valData VIA ddriver
******
function fx_insert(data,valData,ddriver)
	local i,j,d,fields:={},values,filename
	i=at(" ",data)
	i:=iif(i==0,len(data)+1,i)
	filename:=alltrim(substr(data,1,i-1))
	data:=alltrim(substr(data,i+1))
	ddriver:=iif(ddriver==NIL,rddSetDefault(),ddriver)
	if (j:=getarea(filename)) > 0
		d:=rddHandle(j)
	else
		d:=rddUseArea(substr(ddriver,1,3),filename,.t.)
		if file(filename+memoext())
			rddSetMemo(d,,filename)
		endif
		if file(filename+"."+lower(substr(ddriver,4,3)))
			rddSetIndex(d,,filename)
		endif
	endif
	//d:=rddUseArea(substr(ddriver,1,3),filename,.t.) //shared
	if empty(data) // fields list is empty
		for i=1 to rddfcount(d)
			aadd(fields,rddFieldName(d,i))
		next
	else
		if !(  left(data,1)=="(" .and. right(data,1)==")"  )
			use
			i:=errorNew()
			i:description:="Sql syntax error:"+data
			i:genCode:=EG_SYNTAX
			i:operation:="INSERT INTO"
			break(i)
		endif
		data:=substr(data,2,len(data)-2)
		fields:=split(data,",")
	endif
	if valtype(valData) == "A"
		values:=valData
	else
	if valtype(valData)=="U"
		values:={}
		for i=1 to len(fields)
			j:=memvarBlock(fields[i])
			if j!=NIL
				aadd(values,eval(j) )
			else
				aadd(values, NIL )
			endif
		next
	else
		if !(  left(valData,1)=="(" .and. right(valData,1)==")"  )
			use
			i:=errorNew()
			i:description:="Sql syntax error:"+valData
			i:genCode:=EG_SYNTAX
			i:operation:="INSERT INTO... VALUES"
			break(i)
		endif
		valData:=substr(valData,2,len(valData)-2)
		values:=split(valData,",")
		for i=1 to len(values)
			if left(values[i],1)=="{"
				values[i]=substr(values[i],2,len(values[i])-2)
				values[i]=ctod(values[i])
			else
				values[i]=&(values[i])
			endif
		next
	endif
	endif
	rddAppend(d)
	for i=1 to len(fields)
		j:=rddFieldPos(d,fields[i])
		if j>0 .and. i<=len(values) .and. values[i]!=NIL
			rddSetValue(d,j,values[i])
		endif
	next
	rddCloseArea(d)
return

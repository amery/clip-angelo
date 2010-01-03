**************************************************
* create table as SQL query "f1 t1(l1,d1), f2 t2 ,...."
******
function fx_dbcreate(data,ddriver)
	local i,ch,m:={},filename
	local flag:=1,f:=.f.
	local fname:="",ftype:="",flen:="",fdec:=""
	i=at(" ",data)
	filename:=alltrim(substr(data,1,i-1))
	data:=alltrim(substr(data,i+1))
	if !(  left(data,1)=="(" .and. right(data,1)==")"  )
		i:=errorNew()
		i:description:="Sql syntax error:"+data
		i:genCode:=EG_SYNTAX
		i:operation:="CREATE TABLE"
		break(i)
	endif
	data:=alltrim(substr(data,2,len(data)-2))
	do while !empty(data)
		 ch:=left(data,1)
		 data:=substr(data,2)
		 if ch=="," .and. !f //end of description field
			aadd(m,{alltrim(fname), alltrim(ftype),val(flen),val(fdec)})
			flag:=1; f:=.f.
			fname:="";ftype:="";flen:="";fdec:=""
			data:=alltrim(data)
			loop
		 endif
		 if ch=="," .and. f
			flag++
			data:=alltrim(data)
			loop
		 endif
		 if ch==" "
			flag++
			data:=alltrim(data)
			loop
		 endif
		 if ch=="("
			flag:=3
			f:=.t.
			data:=alltrim(data)
			loop
		 endif
		 if ch==")"
			flag:=5
			f:=.f.
			data:=alltrim(data)
			loop
		 endif
		 do case
			case flag==1
				fname+=ch
			case flag==2
				ftype+=ch
			case flag==3
				flen+=ch
			case flag==4
				fdec+=ch
		 endcase
	enddo
	aadd(m,{alltrim(fname), alltrim(ftype),val(flen),val(fdec)})
	dbcreate(filename,m,ddriver)
	use (filename) VIA (ddriver) NEW
return

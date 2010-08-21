static function tag_addField(fname,_fdata)
	local i,j,k,fff,s,fdata := ""
	if valtype(fdata)=="C"
		while .t.
			i := atl("&",_fdata)
			if i<=0
				fdata += _fdata
				exit
			endif
			fdata+=left(_fdata,i-1)
			_fdata:=substr(_fdata,i)
			j := atl(";",_fdata)
			if j<=0
				fdata += _fdata
				exit
			endif
			s := left(_fdata,j)
			_fdata:=substr(_fdata,j+1)
			k := ascan(::entities,{|x|lower(s)==x[1]})
			if k<=0
				fdata+=s
				loop
			endif
			fdata += ::entities[k][2]
		end
	endif
	if valtype(fname)=="C"
		fff:=alltrim(upper(fname))
		if fff $ ::fields
		else
			aadd(::fieldsOrder,fff)
		endif
		::fields[fff]:=fdata
		::hashes[fff]:=fff
		return len(::fields)
	endif
return 0

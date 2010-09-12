// return structure with info for all aliases
function aliasesInfo
	local i,j,k,x,y, ret:=map(), sel:=select()
		local __dbi_names:=__dbiInfo(1)
		for i=0 to MAX_ALIASES
			select(i)
				if !used() .and. i!=0
					loop
				endif
		ret[i]:=map()
				k:=mapkeys(__dbi_names)
				for j=1 to len(k)
					x:=k[j]
						if used()
							y:=dbinfo(x)
							ret[i][x]:=y
						else
							ret[i][x]:= NIL
						endif
				next
				ret[i][DBI_RECNO]:=recno()
				ret[i][DBI_LASTREC]:=lastrec()
		next
		select(sel)
return ret

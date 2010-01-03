static function tdb_addField(f)
	local n
	if ValType( F ) == "C"
        	n:=aScan( ::oDict:aFields,{ |e| e:cName == Upper( F ) } )
                if n>0
   			AAdd( ::aFields, aClone( ::oDict:aFields[ n ] ) )
                else
                	msgAlert([Field not found in dictonary])
                endif
	else
		AAdd( ::aFields, F )
	endif
return nil

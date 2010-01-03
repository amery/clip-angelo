static function tdb_AddIndex( xIndex )
	local n
   	if ValType( xIndex ) == "C"
      		n:=aScan( ::oDict:aIndexes,{ |e| e:cName == Upper( xIndex ) } )
                if n>0
      			AAdd( ::aIndexes, aClone( ::oDict:aIndexes[ n ] ) )
                else
                	msgAlert([Index not found in dictonary])
                endif
   	else
      		AAdd( ::aIndexes, xIndex )
   	endif
return nil

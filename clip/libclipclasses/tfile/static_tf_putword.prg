static functio tf_PutWord( nWord )
   local cWord := I2Bin( nWord )
return FWrite( ::hFile, @cWord, 2 )

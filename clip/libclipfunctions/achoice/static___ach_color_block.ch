static function  __ach_color_block(x)
  local t, nrow
  nrow:=iif(x==NIL,__ac_data:nRowMass,x)
  if nrow<1 .or. nrow>__ac_data:lenmass
     return .f.
  endif
  t:=valtype(__ac_data:_lMass[nRow])
  if t=="C"
     return &(__ac_data:_lMass[nRow])
  endif
  if t=="L"
     return __ac_data:_lMass[nRow]
  endif
  if t=="B"
     return eval(__ac_data:_lMass[__ac_data:nRowMass])
  endif
return .f.

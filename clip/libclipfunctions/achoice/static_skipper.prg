static func Skipper(n)
    local i:=0
    i=__ac_data:nRowMass
    __ac_data:lenMass:=min(__ac_data:lenMass,len(__ac_data:massp))
    __ac_data:nRowMass:=max(1,__ac_data:nRowMass)
    __ac_data:nRowMass:=min(__ac_data:lenMass,__ac_data:nRowMass)
    __ac_data:nRowMass+=n
    __ac_data:nRowMass:=max(1,__ac_data:nRowMass)
    __ac_data:nRowMass:=min(__ac_data:lenMass,__ac_data:nRowMass)
return __ac_data:nRowmass-i

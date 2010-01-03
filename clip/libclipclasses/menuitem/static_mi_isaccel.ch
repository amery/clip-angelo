static func mi_isAccel(nkey)
    local s,ret:=.f.
    s=lower("&"+chr(nkey))
    ret=s $ lower(::caption)
return ret

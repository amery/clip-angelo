static function fx_winNum(cName)
        cName := upper(cName)
return ascan(fx_windows,{|x|x[1]==cname})

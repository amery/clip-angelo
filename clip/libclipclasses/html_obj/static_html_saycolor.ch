static function html_sayColor(str,color)
	local ret
        ::__setFont(.t.,,,color)

        ret:=::put(str)

        ::endFont()
return ret

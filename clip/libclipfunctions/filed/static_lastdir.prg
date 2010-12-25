static func lastdir(str)
	str := substr(str, 1, rat("/", str)-1)
	str := substr(str, rat("/", str)+1)
return str

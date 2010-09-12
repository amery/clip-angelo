function run_blank(file,stdin)
	local s

	private vk:=CRLF

	s:=memoread(file)
	s:=strtran(s,"&\r","")

return _run_blank(s,stdin)

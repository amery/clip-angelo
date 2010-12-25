function smtpNew(host)
	local obj:=map()

	obj:classname   := "SMTP"
	obj:host        := iif(host==NIL,"localhost",host)
	obj:timeout     := 6000
	obj:EOL     	:= "&\n"
	obj:LF     	:= CHR(10)
	obj:handle      := NIL
	obj:error       := ""
	obj:fields      := {}
	obj:attachments := {}

	obj:new      := @smtp_new()
	obj:open     := @smtp_connect()
	obj:connect  := @smtp_connect()
	obj:noop     := @smtp_noop()
	obj:hello    := @smtp_hello()
	obj:helo     := @smtp_hello()
	obj:addField := @smtp_addfield()
	obj:attach   := @smtp_attach()
	obj:send     := @smtp_send()
	obj:sendMail := @smtp_send()
	obj:reset    := @smtp_reset()
	obj:rset     := @smtp_reset()
	obj:close    := @smtp_destroy()
	obj:destroy  := @smtp_destroy()
return obj

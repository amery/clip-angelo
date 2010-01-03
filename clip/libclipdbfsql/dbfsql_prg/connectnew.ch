FUNCTION ConnectNew(rdbms)
	LOCAL o := map()

	o:classname := "TCONNECT"

	o:conn := SQLCreateConn(rdbms,PARAM(2),PARAM(3),PARAM(4),PARAM(5),PARAM(6),;
		PARAM(7),PARAM(8),PARAM(9),PARAM(10))

	o:RDBMS := rdbms
	o:Rowsets := ARRAY(0)
	o:Command 	:= @connCommand()
	o:TestParser	:= @connTestParser()
	o:Destroy	:= @connDestroy()
	o:CreateRowset	:= @connCreateRowset()
	o:Start		:= @connStart()
	o:Commit	:= @connCommit()
	o:Rollback	:= @connRollback()

	o:blobCreate	:= @BlobCreate()
	o:blobOpen  	:= @BlobOpen()
	o:blobImport	:= @BlobImport()
	o:blobExport	:= @BlobExport()
	o:blobWrite 	:= @BlobWrite()
	o:blobRead  	:= @BlobRead()
	o:blobSeek  	:= @BlobSeek()
	o:blobTell  	:= @BlobTell()
	o:blobClose 	:= @BlobClose()
	o:blobUnlink	:= @BlobUnlink()
RETURN o

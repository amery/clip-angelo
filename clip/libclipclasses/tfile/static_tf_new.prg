static function tf_new(self, cFileName, nMode )

   self:cName := cFileName
   if File( cFileName )
      self:nMode := iif(nMode==NIL, FO_READWRITE, nMode)
      self:hFile = FOpen( cFileName, self:nMode )
      self:nLen  = FSeek( self:hFile, 0, FS_END )
      FSeek( self:hFile, 0, FS_SET )
   else
      self:nMode := iif(nMode==NIL, FC_NORMAL, nMode)
      self:hFile = FCreate( cFileName, nMode )
      self:nLen  = 0
   endif
   self:nBufferLen := 2048

   self:create	:= @tf_create()
   self:open	:= @tf_open()
   self:close	:= @tf_close()
   self:end	:= @tf_close()
   self:skip	:= @tf_skip()
   self:goto	:= @tf_goto()
   self:goTop	:= @tf_goTop()
   self:goBottom:= @tf_goBottom()
   self:lEof	:= @tf_eof()
   self:Eof	:= @tf_eof()
   self:lBof	:= @tf_bof()
   self:Bof	:= @tf_bof()
   self:nRecno	:= @tf_recno()
   self:Recno	:= @tf_recno()
   self:nGetByte:= @tf_getByte()
   self:GetByte	:= @tf_getByte()
   self:nGetWord:= @tf_getWord()
   self:GetWord	:= @tf_getWord()
   self:cGetStr	:= @tf_getStr()
   self:GetStr	:= @tf_getStr()
   self:putByte	:= @tf_putByte()
   self:putWord	:= @tf_putWord()
   self:putStr	:= @tf_putStr()
   self:appByte	:= @tf_appByte()
   self:appWord	:= @tf_appWord()
   self:appStr	:= @tf_appStr()
   self:count	:= @tf_count()
   self:seek	:= @tf_seek()
   self:delBytes:= @tf_delBytes()
   self:insBytes:= @tf_insBytes()

return self

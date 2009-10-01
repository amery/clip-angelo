
? 'bzip2ped file write/read test: file asdf.bz2'
? m->h:=bzip2Open("asdf.bz2", "wb")

m->msg:='Hello, World!'
? bzip2Write(m->h, m->msg)
? bzip2Close(m->h)

? m->h:=bzip2Open("asdf.bz2", "rb")
m->msg1:=space(100)
? bzip2Read(m->h, @(m->msg1))
? bzip2Close(m->h)

? 'before:', m->msg
? 'after:', m->msg1
?
? 'bzip2/bunzip2 string test'
? 'msg:', m->msg
? 'msg1:=bzip2(msg):', m->msg1:=bzip2(m->msg)
? 'bunzip2(msg1):', bunzip2(m->msg1)
?

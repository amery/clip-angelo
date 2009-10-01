m->port:=1

? 'found', COM_NUM(), 'ports'

IF ! COM_OPEN(m->port)
     ? "cannot opened port !", FERRORSTR()
     break
ENDIF

? 'init', COM_INIT(m->port, 9600, 'N', 8, 1)

? COM_SOFT(m->port, .f.)
? COM_HARD(m->port, .f.)

? COM_TIMEOUT(m->port, 20)

m->msg:='Hello, World!'
? 'send', COM_SEND(m->port, m->msg)

? 'count', COM_COUNT(m->port)
? 'read', COM_READ(m->port, 10)

? COM_CLOSE(m->port)

?

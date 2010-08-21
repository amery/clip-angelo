static function mkblkerr(e,i)
myalert('Error in expression in line '+alltrim(str(i)),{'Stop'})
BREAK(e)
return e

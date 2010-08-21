static function te_centerLine()
   ::__check_line(::line)
   ::edbuffer[::line]=center(alltrim(::edbuffer[::line]),::marginRight,32,.f.)
   ::updated:=.t.
   ::refresh()
return

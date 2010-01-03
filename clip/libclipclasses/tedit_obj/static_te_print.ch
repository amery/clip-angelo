static function te_print()

  printBegin()

  aeval(::edbuffer, {|item| qout(item)})

  printEnd()
return .t.

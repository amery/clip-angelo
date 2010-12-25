*********** initialisation
static function __te_nullInit()
  asize(::__undobuffer, ::lenundo)
  afill(::__undobuffer, NIL)
  asize(::edbuffer,0)
  afill(::koordblock, NIL)
  asize(::__findR, 3)
  afill(::__findR, NIL)
  ::strblock := .f.
  ::rectblock := .f.
  ::mkblock := .f.
return

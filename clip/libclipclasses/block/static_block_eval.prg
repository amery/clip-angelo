static function block_eval(p1,p2,p3,p4,p5,p6,p7,p8,p9)
	local ret:=NIL
	If ::bBlock != NIL
		ret:=Eval( ::bBlock,p1,p2,p3,p4,p5,p6,p7,p8,p9 )
	endif
return ret

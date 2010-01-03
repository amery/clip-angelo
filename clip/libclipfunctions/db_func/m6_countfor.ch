function m6_CountFor(cfor,bfor)
	local var := 0

	DBEvalOptimize({|| local(_count:=@var),_count := _count + 1},cfor)
return var

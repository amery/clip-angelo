static function te_addToClipboard(Clipboard)
    local i, j:=0, cb[2], nT, nL, nB, nR
    if ::strblock .or. ::rectblock
	cb[1] := iif(::strblock, .t., .f.)
	cb[2] := {}
	nT := min(::koordblock[1], ::koordblock[3])
	nB := max(::koordblock[1], ::koordblock[3])
	nL := min(::koordblock[2], ::koordblock[4])
	nR := max(::koordblock[2], ::koordblock[4])
	nB := min(nB, ::lines)
	asize(cb[2], nB-nT+1)
	for i=nT to nB
		j++
		ains(cb[2], j)
		cb[2][j] := iif(::strblock, ::edbuffer[i], substr(::edbuffer[i], nL, nR-nL+1))
	next
	aadd(Clipboard, aclone(cb))
    endif
RETURN

static proc in_key(nm,otv)
do case
case (nm=1.and.otv) .or. (nm=2.and.!otv)
	 keyboard chr(13)
case nm=1.and.!otv
	 keyboard chr(4)+chr(13)
case nm=2.and.otv
	 keyboard chr(19)+chr(13)
endcase
return

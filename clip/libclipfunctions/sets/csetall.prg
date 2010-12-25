function cSetAll(newVal)
	local ret:=""
/*
1       CSETALTE()     SET ALTERNATE  18
2       CSETBELL()     SET BELL	      26
3       CSETCARR()     SET CARRY      ??
4       CSETCENT()     SET CENTURY    __setCentury
5       CSETCONF()     SET CONFIRM    27
6       CSETCONS()     SET CONSOLE    17
7       CSETCURS()     SET CURSOR     16
8       CSETDELE()     SET DELETED    11
9       CSETDELI()     SET DELIMITERS 33
10      CSETDEVI()     SET DEVICE     20
11      CSETESCA()     SET ESCAPE     28
12      CSETEXAC()     SET EXACT      1
13      CSETEXCL()     SET EXCLUSIVE  8
14      CSETFIXE()     SET FIXED      2
15      READINSERT()   INSERT Mode    29
16      CSETINTE()     SET INTENSITY  31
17      CSETPRIN()     SET PRINT      23
18      CSETSCOR()     SET SCOREBOARD 32
19      CSETSOFT()     SET SOFTSEEK   9
20      CSETUNIQ()     SET UNIQUE     10
21      CSETWRAP()     SET WRAP	      35
*/
	local nums := {18,26,99,-1,27,17,16,11,33,20,28,1,8,2,29,31,23,32,9,10,35}

	local i,n,val
	for i=1 to len(nums)
		n := nums[i]
		if n==99
			val = .f.
		elseif n == -1
			val := __setCentury()
		else
			val := set(n)
		endif
		if valtype(val) == "L" .and. val
			ret += "1"
		else
			ret += "0"
		endif
	next
	if valtype(newVal) != "C" .or. len(newVal) != len(nums)
		return ret
	endif
	for i=1 to len(nums)
		n := nums[i]
		val := substr(newVal,i,1)
		val := ( val=="1" )
		if n == 99

		elseif n == -1
			__setCentury(val)
		else
			set(n,val)
		endif
	next
return ret

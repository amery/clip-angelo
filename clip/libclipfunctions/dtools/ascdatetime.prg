function ascDateTime(d,t,type)
	local dt,ret:=""
	local m := {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}
	local w := {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"}
	if valtype(type) !="N"
		type := 1
	endif
	if valtype(d) != "D"
		d := date()
	endif
	if valtype(t) != "C"
		t := time()
	endif
	dt:=dt_create(d,t)
	dt:=dt_add(dt,dt_create(0,0,0,0,timezone(),0,0))
	dt:=dt_normalize(dt)
	if type == 2
		ret += " "+ntoc(dt_day(dt),10,2,"0")
		ret += " "+m[dt_month(dt)]
		ret += " "+str(dt_year(dt),4,0)
		ret += " "+ntoc(dt_hour(dt),10,2,"0")
		ret += ":"+ntoc(dt_min(dt),10,2,"0")
		ret += ":"+ntoc(dt_sec(dt),10,2,"0")
	else
		ret += w[dow(dt_date(dt))]+","
		ret += " "+ntoc(dt_day(dt),10,2,"0")
		ret += " "+m[dt_month(dt)]
		ret += " "+str(dt_year(dt),4,0)
		ret += " "+ntoc(dt_hour(dt),10,2,"0")
		ret += ":"+ntoc(dt_min(dt),10,2,"0")
		ret += ":"+ntoc(dt_sec(dt),10,2,"0")
		ret += " GMT"
	endif
return ret

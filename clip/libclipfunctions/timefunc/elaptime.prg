*********************************************************
 /* from clipper S`87 */
function elaptime(time1,time2)
	local t1,t2

        t1:=iif(valtype(time1)=="C", ssec(time1),time1)
        t2:=iif(valtype(time2)=="C", ssec(time2),time2)

return tstring(iif(t2<t1,86400,0)+t2-t1)

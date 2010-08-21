*********************************************************
 /* convert seconds value to time format "hh:mm:ss" */
function tstring(sec)
return  ntoc((sec/3600)%24,10,2,"0")+":"+;
	ntoc((sec/60)%60,10,2,"0")+":"+;
	ntoc(sec%60,10,2,"0")

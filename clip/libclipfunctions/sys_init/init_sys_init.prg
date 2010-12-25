init procedure sys_init
  local l,x,i,c,cliproot:=cliproot()
  local file

  public GETLIST:={}
  errorSys()

#ifndef OS_CYGWIN
	setTxlat(16, "><")
#endif

  // englisg dow() return 1 for Saturday
  // russian dow() return 1 for Monday
  // for english - chr(1), for russin - chr(0)
  set("FIRST_DAY_WEEK",chr(1))

  set date format to _DDATEFMT()
  set century off
  set(_SET_EVENTMASK, INKEY_ALL)
  set("PRINTER_EOL",chr(13)+chr(10))
  SET KEY K_F1 TO Help

#ifdef CLIPPER50_COMPATIBLE
  // set autopen off
#else
  set autopen on
#endif

  if !dbf_charset_init(cliproot)
	l := [cp437]
	set("DBF_CHARSET",l)
	outlog(2,"set DBF_CHARSET:",l,__FILE__,__LINE__)
	if "ru" $ lower(getenv("LANG")) .or. "ru" $ lower(getenv("CLIP_LANG")) .or. empty(getenv("LANG"))
		// it`s Russian DOS codepage
		set("DBF_CHARSET","cp866")
		outlog(2,"set DBF_CHARSET:","cp866",__FILE__,__LINE__)
	endif
  endif

#ifdef OS_CYGWIN
  for i=asc("A") to asc("Z")
		set(chr(i)+":","/cygdrive/"+chr(i))
		outlog(2,"add drive "+chr(i)+": as "+"/cygdrive/"+lower(chr(i)),__FILE__,__LINE__)
  next
  drives_init(cliproot)
#else
  if !drives_init(cliproot)
	outlog(2,"add drive C: as /",__FILE__,__LINE__)
	set("C:","/")
	//outlog(2,"add drive D: as /usr")
	//set("D:","/usr")
	//outlog(2,"add drive E: as /home")
	//set("E:","/home")
  endif
#endif

  file:=cliproot+PATH_DELIM+"etc"+PATH_DELIM+"def_sets"
  if file(file)
	loadsets(file)
  endif
  file:=PATH_DELIM+"home"+PATH_DELIM+getenv("USER")+PATH_DELIM+".clip"+PATH_DELIM+"def_sets"
  if file(file)
	loadsets(file)
  endif

//  diskchange("C:")
//  INI__CTOOLS_DISKFUNC()

  randomize() // start random functions

  //? ulimit(ULIMIT_CORE,0) // max size core files == 0
  l:=ulimit(ULIMIT_AS)
  x:=0x7fffffff
  for i=1 to 10
	ulimit(ULIMIT_AS,x)
	l:=ulimit(ULIMIT_AS)
		if l>=x
			exit
		endif
		x=int(x/2)
  next
  outlog(2,"set ulimit -v =",l,__FILE__,__LINE__)

  l:=ulimit(ULIMIT_DATA)
  x:=0x7fffffff
  for i=1 to 10
	ulimit(ULIMIT_DATA,x)
	l:=ulimit(ULIMIT_DATA)
	if l>=x
		exit
	endif
	x=int(x/2)
  next
  outlog(2,"set ulimit -d =",l,__FILE__,__LINE__)

  HK_init()           // default hot keys for editor, debugger, ... more
  HK_load()           // load users hot keys

return

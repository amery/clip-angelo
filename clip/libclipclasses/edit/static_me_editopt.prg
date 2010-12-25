static function me_editopt()
local get_color, color, dl, fl, getlist:={}, fullScr := .t., i, y
local cbak, tab, asave, tsave, lang, ssave, hyp, autom, mleft, mright, aindent
local tabpack, maxstrings, autowrap, nopen, buttons := {}
local oWin

i := setcursor()
get_color := "11/1, 14/1, 11/1, 11/1, 11/1, 0/7"
color := setcolor()
setcolor(set("edit_colors_dialog"))
fl := 25
dl := fl+6
/* get locales */

dispbegin()
oWin := wselect()
wopen(int((maxrow())/2)-5, int((maxcol())/2)-(2*fl+17)/2, int((maxrow())/2)+11, int((maxcol())/2)+(2*fl+17)/2)
wbox()
clear screen
cbak := ::Opt:OCREATEBAK
tab := ::Opt:OTABSIZE
lang := ::Opt:OLANGUAGE
asave := ::Opt:OAUTOSAVE
tsave := ::Opt:OTIMEAUTOSAVE
ssave := ::Opt:OSAVESTATUS
hyp := ::Opt:OHYPHEN
autom := ::Opt:OAUTOMARGIN
autowrap := ::Opt:OAUTOWRAP
mleft := ::Opt:OMARGINLEFT
mright := ::Opt:OMARGINRIGHT
aindent := ::Opt:OAUTOINDENT
maxstrings := ::Opt:OMAXSTRINGS
nopen      := ::Opt:OOPENFILES
tabpack := iif(lower(set("edit_tabpack"))=="yes", .t., .f.)

@ -1, maxcol()/2-len([ Options ]) say [ Options ]
y := 1
@ y, 1 say padr([Create .bak files], fl) //color say_color
@ y, fl+2 get cbak CHECKBOX color get_color
y++
@ y, 1 say padr([Tabulation size], fl)
@ y, fl get tab picture "9999"  color get_color
y++
@ y, 1 say padr([Tabulation packing], fl)
@ y, fl+1 get tabpack  picture "XXX" color get_color
y++
@ y, 1 say padr([Open files in ], fl)
y++
aadd(buttons, RadioButton(y+1, 4, [current directory]))
aadd(buttons, RadioButton(y+2, 4, [last directory]))
@ y, 1, y+3, fl+4 get nopen RADIOGROUP buttons //color get_color
y+=4
@ y, 1 say replicate("_", maxcol(fullScr)-2)
y++
@ y, 1 say padr([Autosave files], fl)
@ y, fl+2 get asave CHECKBOX  color get_color
y++
@ y, 1 say padr([Autosave time], fl)
@ y, fl get tsave picture "9999" color get_color
y++
y++
@ y, 1 say replicate("_", maxcol(fullScr)-2)
y++
//@ 7, 1 say padr([Language], fl)
//@ 7, fl get lang  picture "@KS10A" color get_color
//@ 7, 1 say padr("Length string", fl)
//@ 7, fl get Opt:OLENGTH picture "9999"  color get_color
y++
@ y, 1 say padr([Save status], fl)
@ y, fl+2 get ssave CHECKBOX color get_color
y := 1
@ y, dl say padr([Hypen in word], fl)
@ y, dl+fl+7 get hyp  CHECKBOX color get_color
y++
@ y, dl say padr([Auto wrap], fl)
@ y, dl+fl+7 get autowrap  CHECKBOX color get_color
y++
@ y, dl say padr([Auto margin left], fl)
@ y, dl+fl+7 get autom  CHECKBOX color get_color
y+=6
y++
@ y, dl say padr([Margin left], fl)
@ y, dl+fl+5 get mleft  picture "9999" color get_color
y++
@ y, dl say padr([Margin right], fl)
@ y, dl+fl+5 get mright  picture "9999" color get_color
y++
@ y, dl say padr([Max strings], fl)
@ y, dl+fl+3 get maxstrings  picture "999999" color get_color
y++
y++
y++
@ y, dl say padr("Auto indent", fl)
@ y, dl+fl+7 get aindent CHECKBOX color get_color
//@ 4, dl say padr("Page length", fl)
//@ 4, dl+fl+3 get Opt:OPAGELEN  picture "9999" color get_color
//@ 7, dl say padr("Language", fl)
//@ 7, dl+fl get Opt:OLANGUAGE  picture "@KS10A" color get_color
dispend()
read

::Opt:OCREATEBAK    := cbak
::Opt:OTABSIZE      := tab
::Opt:OLANGUAGE     := lang
::Opt:OAUTOSAVE     := asave
::Opt:OTIMEAUTOSAVE := tsave
::Opt:OSAVESTATUS   := ssave
::Opt:OHYPHEN       := hyp
::Opt:OAUTOMARGIN   := autom
::Opt:OAUTOWRAP     := autowrap
::Opt:OMARGINLEFT   := mleft
::Opt:OMARGINRIGHT  := mright
::Opt:OAUTOINDENT   := aindent
::Opt:OMAXSTRINGS   := maxstrings
::Opt:OOPENFILES    := nopen

::autoIndent   := aindent
set("edit_tabpack", iif(tabpack, "yes", "no"))
::Opt:OTABPACK   := set("edit_tabpack")

putenv("LANG="+alltrim(lang))

wclose()
wselect(oWin)
setcolor(color)
::save_options()
for i=1 to ::nWins
	::tobj[i]:TabPack := tabpack
next
setcursor(i)
return .t.

***********************************************
* О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫ О©╫ О©╫О©╫О©╫-О©╫О©╫О©╫О©╫
* О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫
***********************************************
function HK_get_key(keymap,hashKey)
   local ret:=0,i,m
   m=mapkeys(keymap)
   for i=1 to len(m)
	if keymap[m[i]]==hashKey
	   ret=m[i]
	   exit
	endif
   next
return ret

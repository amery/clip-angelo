/********************************************
* func HK_load()
* О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫
* О©╫О©╫О©╫О©╫ "О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫", "О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫", .........
* О©╫О©╫ О©╫О©╫О©╫О©╫О©╫ keymaps, О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫ О©╫:
* 1. О©╫ /usr/local/lib/clip/
* 2. О©╫О©╫ О©╫О©╫О©╫О©╫, О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫ setenv("RTCLIP_PATH")
* 3. О©╫ $HOME/.clip
*/
function HK_load()
local nfile
	nfile := getenv("HOME")+"/.clip/keymaps.po"
	if file(nfile)
		load(nfile)
		if isFunction("init_user_key")
			key_maps := clip("init_user_key",key_maps)
//			key_maps := eval(init_user_key, key_maps)
		endif
	endif
return

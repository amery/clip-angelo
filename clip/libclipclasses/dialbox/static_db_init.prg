static function db_init()
local str
	//save screen to ::__oldscr
	::__oldcolor := setcolor()
	::__oldcursor := setcursor()
	::leninfo := maxcol()-maxcol()*3/4 - 1
	setcolor(::colorSpec)
	set("dbox_colors_list","15/3,15/7,0/7,0/3,")   // ���� list ��������
	set("dbox_colors_dialog","15/7,14/1,w+/b,g/rb,15/7,0/7")   // ���� ��������
	set("dbox_colors_menu","0/7,0/2,r/7,g/rb,15/7,0/7") // ���� ����
	::item:hasFocus := .f.
	asize(::item_area, 0)
	::Find := findOptions(,,set("dbox_colors_dialog"), set("dbox_colors_list"))
	asize(::findR, 3)
return

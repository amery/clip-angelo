static function Browse_File(numb)
local str := ""
	str := fileDialog()
	::doc:setValue(numb, iif(len(str)>256, str, padr(str, 256)))
return
***********
static func __setcolor()
local i, s
       s:=::colorSpec
       while len(s)>0
	   i:=at(",",s)
	   i=iif(i==0,len(s)+1,i)
	   aadd(::__colors,substr(s,1,i-1) )
	   s:=substr(s,i+1)
       enddo
       ::new_clr(::__colors[1])
       ::htmlColors:LIGHT_YELLOW := ::htmlColors:LIGHTYELLOW  := ::htmlColors["#FFFFFC0"] := "GR+"
       ::htmlColors:DARK_YELLOW  := ::htmlColors:DARKYELLOW   := ::htmlColors["#FFFFFCC"] := "GR+"
       ::htmlColors:DARKER_YELLOW:= ::htmlColors:DARKERYELLOW := ::htmlColors["#FFFFF80"] := "GR+"
       ::htmlColors:LIGHT_BLUE   := ::htmlColors:LIGHTBLUE    := ::htmlColors["#DEEFEF"] := "BG+"
       ::htmlColors:LIGHT_GRAY   := ::htmlColors:LIGHTGRAY    := ::htmlColors["#F0F0F0"] := "W"

	::htmlColors:ALICEBLUE           := ::htmlColors["#A0CF00"] := "BG+"
	::htmlColors:ANTIQUEWHITE        := ::htmlColors["#FFE8D0"] := "W"
	::htmlColors:AQUA                := ::htmlColors["#00FFFF"] :=	"BG+"
	::htmlColors:AQUAMARINE          := ::htmlColors["#7FFFD0"] :=	"G+"
	::htmlColors:AZURE               := ::htmlColors["#F0FFFF"] := "W+"
	::htmlColors:BEIGE               := ::htmlColors["#F0F7DF"] := "W"
	::htmlColors:BISQUE              := ::htmlColors["#FFE7C0"] := "GR"
	::htmlColors:BLACK               := ::htmlColors["#000000"] := "N"
	::htmlColors:BLANCHEDALMOND      := ::htmlColors["#FFE8CF"] := "GR"
	::htmlColors:BLUE                := ::htmlColors["#0000FF"] := "B"
	::htmlColors:BLUEVIOLET          := ::htmlColors["#8F28E0"] := "B"
	::htmlColors:BROWN               := ::htmlColors["#A0282F"] := "GR"
	::htmlColors:BURLYWOOD           := ::htmlColors["#DFB880"] := "GR"
	::htmlColors:CADETBLUE           := ::htmlColors["#5F9FA0"] := "G"
	::htmlColors:CHARTREUSE          := ::htmlColors["#7FFF00"] := "G+"
	::htmlColors:CHOCOLATE           := ::htmlColors["#D0861F"] := "GR"
	::htmlColors:CORAL               := ::htmlColors["#FF7F50"] := "R+"
	::htmlColors:CORNFLOWERBLUE      := ::htmlColors["#6097EF"] := "B+"
	::htmlColors:CORNSILK            := ::htmlColors["#FFF8DF"] := "W"
	::htmlColors:CRIMSON             := ::htmlColors["#DF173F"] := "R"
	::htmlColors:CYAN                := ::htmlColors["#00FFFF"] := "BG"
	::htmlColors:DARKBLUE            := ::htmlColors["#00008F"] := "B"
	::htmlColors:DARKCYAN            := ::htmlColors["#00888F"] := "BG"
	::htmlColors:DARKGOLDENROD       := ::htmlColors["#BF87F0"] := "B"
	::htmlColors:DARKGRAY            := ::htmlColors["#AFA8AF"] := "N"
	::htmlColors:DARKGREEN           := ::htmlColors["#006700"] := "G"
	::htmlColors:DARKKHAKI           := ::htmlColors["#BFB76F"] := "G"
	::htmlColors:DARKMAGENTA         := ::htmlColors["#8F008F"] := "RB"
	::htmlColors:DARKOLIVEGREEN      := ::htmlColors["#50682F"] := "G"
	::htmlColors:DARKORANGE          := ::htmlColors["#FF8F00"] := "GR"
	::htmlColors:DARKORCHID          := ::htmlColors["#9F30CF"] := "RB"
	::htmlColors:DARKRED             := ::htmlColors["#8F0000"] :=	"R"
	::htmlColors:DARKSALMON          := ::htmlColors["#EF977F"] := "R+"
	::htmlColors:DARKSEAGREEN        := ::htmlColors["#8FBF8F"] := "G+"
	::htmlColors:DARKSLATEBLUE       := ::htmlColors["#4F3F8F"] := "W"
	::htmlColors:DARKSLATEGRAY       := ::htmlColors["#2F4F4F"] := "W"
	::htmlColors:DARKTURQUOISE       := ::htmlColors["#00CFD0"] := "G+"
	::htmlColors:DARKVIOLET          := ::htmlColors["#9000D0"] := "B"
	::htmlColors:DEEPPINK            := ::htmlColors["#FF1790"] := "RB"
	::htmlColors:DEEPSKYBLUE         := ::htmlColors["#00BFFF"] := "B+"
	::htmlColors:DIMGRAY             := ::htmlColors["#6F686F"] := "W"
	::htmlColors:DODGERBLUE          := ::htmlColors["#1F90FF"] := "B+"
	::htmlColors:FIREBRICK           := ::htmlColors["#B02020"] := "R"
	::htmlColors:FLORALWHITE         := ::htmlColors["#FFF8F0"] := "W+"
	::htmlColors:FORESTGREEN         := ::htmlColors["#208820"] := "G"
	::htmlColors:FUCHSIA             := ::htmlColors["#FF00FF"] := "RB"
	::htmlColors:GAINSBORO           := ::htmlColors["#DFDFDF"] := "W"
	::htmlColors:GHOSTWHITE          := ::htmlColors["#FFF8FF"] := "W"
	::htmlColors:GOLD                := ::htmlColors["#FFD700"] := "GR+"
	::htmlColors:GOLDENROD           := ::htmlColors["#DFA720"] := "GR+"
	::htmlColors:GRAY                := ::htmlColors["#808080"] := "W"
	::htmlColors:GREEN               := ::htmlColors["#008000"] := "G"
	::htmlColors:GREENYELLOW         := ::htmlColors["#AFFF2F"] := "G+"
	::htmlColors:HONEYDEW            := ::htmlColors["#F0FFF0"] := "W"
	::htmlColors:HOTPINK             := ::htmlColors["#FF68B0"] := "R+"
	::htmlColors:INDIANRED           := ::htmlColors["#CF5F5F"] := "R"
	::htmlColors:INDIGO              := ::htmlColors["#4F0080"] := "B"
	::htmlColors:IVORY               := ::htmlColors["#FFFFF0"] := "W+"
	::htmlColors:KHAKI               := ::htmlColors["#F0E78F"] := "G"
	::htmlColors:LAVENDER            := ::htmlColors["#E0E7FF"] := "W"
	::htmlColors:LAVENDERBLUSH       := ::htmlColors["#FFF0F0"] := "W+"
	::htmlColors:LAWNGREEN           := ::htmlColors["#7FFF00"] := "G+"
	::htmlColors:LEMONCHIFFON        := ::htmlColors["#FFF8CF"] := "GR+"
	::htmlColors:LIGHTBLUE           := ::htmlColors["#AFD8E0"] := "B+"
	::htmlColors:LIGHTCORAL          := ::htmlColors["#F08080"] := "R+"
	::htmlColors:LIGHTCYAN           := ::htmlColors["#E0FFFF"] := "BG+"
	::htmlColors:LIGHTGOLDENRODYELLOW:= ::htmlColors["#FFF8D0"] := "W"
	::htmlColors:LIGHTGREEN          := ::htmlColors["#90EF90"] := "G+"
	::htmlColors:LIGHTGREY           := ::htmlColors["#D0D0D0"] := "W+"
	::htmlColors:LIGHTPINK           := ::htmlColors["#FFB7C0"] := "R+"
	::htmlColors:LIGHTSALMON         := ::htmlColors["#FFA07F"] := "R+"
	::htmlColors:LIGHTSEAGREEN       := ::htmlColors["#20B0AF"] := "BG+"
	::htmlColors:LIGHTSKYBLUE        := ::htmlColors["#80CFFF"] := "B+"
	::htmlColors:LIGHTSLATEGRAY      := ::htmlColors["#70889F"] := "W+"
	::htmlColors:LIGHTSTEELBLUE      := ::htmlColors["#B0C7DF"] := "B+"
	::htmlColors:LIGHTYELLOW         := ::htmlColors["#FFFFE0"] := "GR+"
	::htmlColors:LIME                := ::htmlColors["#00FF00"] := "G+"
	::htmlColors:LIMEGREEN           := ::htmlColors["#30CF30"] := "G+"
	::htmlColors:LINEN               := ::htmlColors["#FFF0E0"] := "W"
	::htmlColors:MAGENTA		 := ::htmlColors["#FF00FF"] := "RB"
	::htmlColors:MAROON              := ::htmlColors["#800000"] := "GR"
	::htmlColors:MEDIUMAQUAMARINE    := ::htmlColors["#60CFAF"] := "G+"
	::htmlColors:MEDIUMBLUE          := ::htmlColors["#0000CF"] := "B+"
	::htmlColors:MEDIUMORCHID        := ::htmlColors["#BF57D0"] := "R+"
	::htmlColors:MEDIUMPURPLE        := ::htmlColors["#9070DF"] := "R+"
	::htmlColors:MEDIUMSEAGREEN      := ::htmlColors["#3FB070"] := "G+"
	::htmlColors:MEDIUMSLATEBLUE     := ::htmlColors["#7F68EF"] := "B+"
	::htmlColors:MEDIUMSPRINGGREEN   := ::htmlColors["#00F89F"] := "G+"
	::htmlColors:MEDIUMTURQUOISE     := ::htmlColors["#4FD0CF"] := "G+"
	::htmlColors:MEDIUMVIOLETRED     := ::htmlColors["#C01780"] := "R"
	::htmlColors:MIDNIGHTBLUE        := ::htmlColors["#1F1870"] := "B"
	::htmlColors:MINTCREAM           := ::htmlColors["#F0FFFF"] := "W"
	::htmlColors:MISTYROSE           := ::htmlColors["#FFE7E0"] := "W"
	::htmlColors:MOCCASIN            := ::htmlColors["#FFE7B0"] := "GR+"
	::htmlColors:NAVAJOWHITE         := ::htmlColors["#FFDFAF"] := "GR+"
	::htmlColors:NAVY                := ::htmlColors["#000080"] := "B"
	::htmlColors:OLDLACE             := ::htmlColors["#FFF7E0"] := "W"
	::htmlColors:OLIVE               := ::htmlColors["#808000"] := "G"
	::htmlColors:OLIVEDRAB           := ::htmlColors["#6F8F20"] := "G"
	::htmlColors:ORANGE              := ::htmlColors["#FFA700"] := "GR"
	::htmlColors:ORANGERED           := ::htmlColors["#FF4700"] := "GR"
	::htmlColors:ORCHID 		 := ::htmlColors["#DF70D0"] := "R+"
	::htmlColors:PALEGOLDENROD       := ::htmlColors["#EFE8AF"] := "GR+"
	::htmlColors:PALEGREEN           := ::htmlColors["#9FF89F"] := "G+"
	::htmlColors:PALETURQUOISE       := ::htmlColors["#AFEFEF"] := "G+"
	::htmlColors:PALEVIOLETRED       := ::htmlColors["#DF7090"] := "R+"
	::htmlColors:PAPAYAWHIP          := ::htmlColors["#FFEFD0"] := "GR"
	::htmlColors:PEACHPUFF           := ::htmlColors["#FFD8BF"] := "GR"
	::htmlColors:PERU                := ::htmlColors["#CF873F"] := "GR"
	::htmlColors:PINK                := ::htmlColors["#FFC0CF"] := "R+"
	::htmlColors:PLUM                := ::htmlColors["#DFA1DF"] := "B"
	::htmlColors:POWDERBLUE          := ::htmlColors["#B0E0E0"] := "BG+"
	::htmlColors:PURPLE              := ::htmlColors["#800080"] := "RB"
	::htmlColors:RED                 := ::htmlColors["#FF0000"] := "R"
	::htmlColors:ROSYBROWN           := ::htmlColors["#BF8F8F"] := "GR"
	::htmlColors:ROYALBLUE           := ::htmlColors["#4068E0"] := "B"
	::htmlColors:SADDLEBROWN         := ::htmlColors["#8F4710"] := "GR"
	::htmlColors:SALMON              := ::htmlColors["#FF8070"] := "R+"
	::htmlColors:SANDYBROWN          := ::htmlColors["#F0A760"] := "GR+"
	::htmlColors:SEAGREEN            := ::htmlColors["#2F8850"] := "G"
	::htmlColors:SEASHELL            := ::htmlColors["#FFF7EF"] := "W"
	::htmlColors:SIENNA              := ::htmlColors["#A0502F"] := "GR"
	::htmlColors:SILVER              := ::htmlColors["#C0C0C0"] := "W+"
	::htmlColors:SKYBLUE             := ::htmlColors["#80CFEF"] := "B+"
	::htmlColors:SLATEBLUE           := ::htmlColors["#6F58CF"] := "B"
	::htmlColors:SLATEGRAY           := ::htmlColors["#708090"] := "W"
	::htmlColors:SNOW                := ::htmlColors["#FFF8FF"] := "W+"
	::htmlColors:SPRINGGREEN         := ::htmlColors["#00FF7F"] := "G+"
	::htmlColors:STEELBLUE           := ::htmlColors["#4080B0"] := "B+"
	::htmlColors:TAN                 := ::htmlColors["#D0B78F"] := "GR"
	::htmlColors:TEAL                := ::htmlColors["#008080"] := "G"
	::htmlColors:THISTLE             := ::htmlColors["#DFBFDF"] := "RB+"
	::htmlColors:TOMATO              := ::htmlColors["#FF6040"] := "R"
	::htmlColors:TURQUOISE           := ::htmlColors["#40E0D0"] := "BG+"
	::htmlColors:VIOLET              := ::htmlColors["#EF80EF"] := "RB+"
	::htmlColors:WHEAT               := ::htmlColors["#F0DFB0"] := "GR+"
	::htmlColors:WHITE               := ::htmlColors["#FFFFFF"] := "W+"
	::htmlColors:WHITESMOKE          := ::htmlColors["#F0F7F0"] := "W+"
	::htmlColors:YELLOW              := ::htmlColors["#FFFF00"] := "GR+"
	::htmlColors:YELLOWGREEN         := ::htmlColors["#9FCF30"] := "G+"
return

*
*****************************
*  getDrawColor( chrColor ) - Меняет в строке цветов первый элемент на chrColor
* * * * * * *
*  chrColor  - цвет символа
*  color
*****************************
FUNC getDrawColor( chrColor, color )
local ret:="", pos
IIF ( color==NIL, color:=setColor(), )
IIF (type("chrColor")=="N", chrColor:=alltrim(str(chrColor)), )
if (pos:=at("/", color))!=0
    color=chrColor+substr(color, pos)
endif
return color

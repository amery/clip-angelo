*****************************
*  getSayColor( number, color ) - Возвращает нужный цветвой набор из
*                                 цветовой строки
*  number - номер нужного цвета ( по умолчанию 1 )
*  color  - из какой строки брать ( по умолчанию SetColor() )
*****************************
FUNC getSayColor( number,color )
local mass[0], pos
IIF ( number==NIL, number:=1,)
IIF ( color==NIL, color:=setColor(), )
pos=1
do while !(color=="")
   if (pos:=at( "," ,color))!=0
      aadd( mass, substr( color,1, pos-1 ) )
      color=substr ( color,pos+1 )
   else
      aadd( mass, substr( color,1) )
      color=""
   endif
enddo
return IIF ( number > len( mass ), "", mass[number] )

function browse ( top, left, down, right )

    LOCAL bSaveHandler, error
    LOCAL cScreen

    top  := iif( top  == NIL, 1, top )
    left := iif( left == NIL, 0, left )
    down  := iif( down  == NIL, maxrow()-1, down )
    right:= iif( right== NIL, maxcol() , right )
    top=max(top,0)
    left=max(left,0)
    down=min(down,maxrow())
    right=min(right,maxcol())

    cScreen := SAVESCREEN()
    @ top, left TO down, right DOUBLE
    MyBrowse(top+2, left+1 , down-1, right-1 )

    RESTSCREEN(,,,,cScreen)

return NIL

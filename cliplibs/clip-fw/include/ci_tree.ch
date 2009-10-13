// Trees for FiveWin !!!  Incredible powerfull !!!

#ifndef CI__TREE_CH
#define CI__TREE_CH

#xcommand TREE [<oTree>] ;
             [ BITMAPS <cBmpOpen>, <cBmpClose> ] ;
       => ;
          [<oTree>:=] TreeBegin( <cBmpOpen>, <cBmpClose> )

#xcommand TREEITEM [<oItem> PROMPT ] <cLabel> ;
             [ RESOURCE <cResName1> [, <cResName2> ] ] ;
             [ FILENAME <cBmpOpen> [, <cBmpClose> ] ] ;
             [ <lOpened: OPENED, OPEN> ] ;
       => ;
          [<oItem> :=] _TreeItem( <cLabel>, <cResName1>, <cResName2>,;
                                  <cBmpOpen>, <cBmpClose>, <.lOpened.> )

#xcommand ENDTREE => TreeEnd()

#endif

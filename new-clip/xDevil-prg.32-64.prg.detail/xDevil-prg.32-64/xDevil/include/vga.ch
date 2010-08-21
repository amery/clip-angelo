
#IfNDef __VGA
//
// creates compile error for command like
// @ 24 , ..
// @ 10, 79 ..
// ..
// to modify with appropriate MaxRow() or MaxCol() Function
//
#Xtranslate 24 , => #Error
#Xtranslate 23 , => #Error
#Xtranslate 22 , => #Error
#Xtranslate 21 , => #Error
#Xtranslate , 79 => #Error
#Xtranslate , 78 => #Error
#Xtranslate , 77 => #Error
#Xtranslate , 76 => #Error

#Endif

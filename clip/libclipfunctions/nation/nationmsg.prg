/*******************************/
function nationmsg(num)
local ret:=[Invalid argument]
	do case
        	case num==1
        	     ret=[Database Files    # Records    Last Update     Size]
         	case num==2
         	     ret=[Do you want more samples?]
         	case num=3
         	     ret=[Page No.]
         	case num==4
         	     ret=[** Subtotal **]
         	case num==5
         	     ret=[* Subsubtotal *]
         	case num==6
         	     ret=[*** Total ***]
		case num==7
                     ret:=[Ins]
		case num==8
                     ret:="   "
		case num==9
                     ret:=[Invalid date]
		case num==10
                     ret:=[Range: ]
		case num==11
                     ret:=" - "
		case num==12
                     ret:="Y/N"
        endcase
return ret

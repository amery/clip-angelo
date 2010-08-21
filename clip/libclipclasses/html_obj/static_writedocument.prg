static function writeDocument()
	local i, t
        for i=1 to len(::tags)
                t=valtype(::tags[i])
                do case
                	case t=="O" // .and. "HTML_TAG"$::tags[i]:className
                        	?? ::tags[i]:toString()
                        case t=="C"
                        	?? ::tags[i]
                        case t=="N"
                        	if ::tags[i]>0 .and. ::tags[i]<=len(::tags)
                        		?? ::tags[::tags[i]]:closeString()
                                else
                                	? "Invalid index of closed TAG"
                                endif
                        case t=="U"
                        	?
                        otherwise
	                        ? "Invalid data",toString(::tags[i])
                endcase
        next
        ? "</BODY></HTML>"
return .f.


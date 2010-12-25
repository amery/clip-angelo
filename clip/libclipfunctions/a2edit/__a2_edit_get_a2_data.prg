function __a2_edit_get_a2_data(n, new_data)
       local ret:=NIL
       if n<=len(__a2_data) .and. curr_record<=len(__a2_data[n])
	  ret=__a2_data[n][curr_record]
	  if new_data!=NIL
	     __a2_data[n][curr_record]:=new_data
	  endif
       endif
return ret

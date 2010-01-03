static function commparse(aCode,nTok,s)
local r,k,n,i
do case
  case nTok==x_if
    /*
       if --> {x_if,{if_blk,else_pc,endif_pc,elif_pc}}
    */
    r:={mkblk(s,len(aCode)),0,0,0}
    apush(aCommStack,{x_if,len(aCode)})
  case nTok==x_elif
    /*
      elif --> {x_elif,{if_blk,else_pc,endif_pc,elif_pc,if_pc}}
    */
    k:=apop(aCommStack)
    if k<>nil .and. (k[1]==x_if .or. k[1]==x_elif)
      aCode[k[2]][2][2]:=len(aCode)
      r:={mkblk(s,len(aCode)),0,0,0,IF(k[1]==x_if,k[2],aCode[k[2]][2][5])}
    else
      myalert('ELIF without IF in line '+alltrim(str(len(aCode))),{'Ok'})
      r:=nil
    endif
    apush(aCommStack,{x_elif,len(aCode)})
  case nTok==x_else
    /*
       else --> {x_else,if_pc}
    */
    k:=apop(aCommStack)
    if k<>nil .and. (k[1]==x_if .or. k[1]==x_elif)
      apush(aCommStack,k)
      r:=k[2]
      aCode[r][2][2]:=len(aCode)
      if k[1]==x_elif
	n:=aCode[k[2]][2][5]
	aCode[n][2][3]:=len(aCode)
      endif
    else
      myalert('ELSE without IF in line '+alltrim(str(len(aCode))),{'Ok'})
      r:=nil
    endif
  case nTok==x_endif
    /*
       endif --> {x_endif,if_pc}
    */
    k:=apop(aCommStack)
    if k<>nil .and. (k[1]==x_if .or. k[1]==x_elif)
      r:=k[2]
      aCode[r][2][3]:=len(aCode)
      if k[1]==x_elif
	n:=aCode[k[2]][2][5]
	aCode[n][2][3]:=len(aCode)
      endif
    else
      myalert('ENDIF without IF in line '+alltrim(str(len(aCode))),{'Ok'})
      r:=nil
    endif
  case nTok==x_while
    /*
      while --> {x_while,{while_blk,endw_pc}}
    */
    r:={mkblk(s,len(aCode)),0}
    apush(aCommStack,{x_while,len(aCode)})
  case nTok==x_endw
    /*
      endw --> {x_endw,while_pc}
    */
    k:=apop(aCommStack)
    if k<>nil .and. k[1]==x_while
      r:=k[2]
      aCode[r][2][2]:=len(aCode)
    else
      myalert('ENDW without WHILE in line '+alltrim(str(len(aCode))),{'Ok'})
      r:=nil
    endif
  case nTok==x_for
    /*
      for --> {x_for,{init_blk,if_blk,inc_blk,next_pc}}
    */
    apush(aCommStack,{x_for,len(aCode)})
    n:=lineparse(s,';')
    if len(n)==3 .or. len(n)==2
      r:={mkblk(n[1],len(aCode)),mkblk(n[2],len(aCode)),IF(len(n)==3,mkblk(n[3],len(aCode)),mkblk('',len(aCode))),0}
    else
      myalert('FOR must have 3 arguments!',{'Ok'})
      r:=nil
    endif
  case nTok==x_next
    /*
      next --> {x_next,for_pc}
    */
    k:=apop(aCommStack)
    if k<>nil .and. k[1]==x_for
      r:=k[2]
      aCode[r][2][4]:=len(aCode)
    else
      myalert('NEXT without FOR in line '+alltrim(str(len(aCode))),{'Ok'})
      r:=nil
    endif
  case nTok==x_private
    /*
      private --> {x_private,array_of_var_names}
    */
    r:=lineparse(s,',')
  case nTok==x_public
    /*
      public --> {x_public,array_of_var_names}
    */
    r:=lineparse(s,',')
  case nTok==x_release
    /*
      release --> {x_release,array_of_var_names}
    */
    r:=lineparse(s,',')
  case nTok==x_exit .or. nTok==x_loop
    /*
      exit --> {x_exit,closest_loop_pc}
      loop --> {x_loop,closest_loop_pc}
    */
    n:=scantok(aCommStack,{x_while,x_for})
    if n<>0
      r:=aCommStack[n][2]
    else
      myalert(IF(nTok==x_exit,'EXIT','LOOP')+' not in loop in line '+alltrim(str(len(aCode))),{'Ok'})
      r:=nil
    endif
  case nTok==x_proc
    /*
       proc --> {x_proc,{proc_name,endp_pc,ret_pc=0}
    */
    apush(aCommStack,{x_proc,len(aCode)})
    apush(blprocs,{lower(alltrim(s)),len(aCode)})
    r:={lower(alltrim(s)),0,0}
  case nTok==x_endp
    /*
       endp --> {x_endp,proc_pc}
    */
    k:=apop(aCommStack)
    if k<>nil .and. k[1]==x_proc
      r:=k[2]
      aCode[r][2][2]:=len(aCode)
    else
      myalert('ENDP without PROC in line '+alltrim(str(len(aCode))),{'Ok'})
      r:=nil
    endif
  case nTok==x_call
    /*
       call --> {x_call,proc_name}
    */
    r:=lower(alltrim(s))
  case nTok==x_write
    r:=mkblk(s,len(aCode))
  case nTok==x_return
    /*
       return --> {ret_pc,ret_expr}
    */
    n:=scantok(aCommStack,{x_proc})
    if n<>0
      if empty(alltrim(s))
	r:={aCommStack[n][2]}
      else
	r:={aCommStack[n][2],mkblk(s,len(aCode))}
      endif
    else
      //myalert('RETURN not within PROC in line '+alltrim(str(len(aCode))),{'Ok'})
      r:={nil}
    endif
  case nTok==x_push
    r:=mkblk("apush(blStack,"+s+")",len(aCode))
  case nTok==x_pop
    r:=mkblk(s+":=apop(blStack)",len(aCode))
  case nTok==x_param
    r:={}
    k:=lineparse(s,',')
    for i:=1 to len(k)
      apush(r,{alltrim(k[i]),mkblk(alltrim(k[i])+":=apop(blStack)",len(aCode))})
    next
  case nTok==x_block
    r:=mkblk(s,len(aCode),.t.)
end case
return r

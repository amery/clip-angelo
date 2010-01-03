static function realrun(aCode,ipc)
local pc,key,op,p,i,v,k
if ipc==nil
  pc:=1
else
  pc:=ipc
endif
while pc<=len(aCode) .and. pc<>0
  m->gpc:=pc
  op:=aCode[pc][1]
  p:=aCode[pc][2]
  do case
    case op==x_eval .or. op==x_pop .or. op==x_push
      pc:=e_eval(p,pc)
    case op==x_if
      pc:=if_eval(p,pc,aCode)
    case op==x_elif
      pc:=elif_eval(p,pc,aCode)
    case op==x_while
      pc:=while_eval(p,pc)
    case op==x_for
      pc:=for_eval(p,pc)
    case op==x_endif
      pc:=endif_eval(p,pc)
    case op==x_endw
      pc:=endw_eval(p,pc)
    case op==x_next
      pc:=next_eval(p,pc,aCode)
    case op==x_else
      pc:=el_eval(p,pc,aCode)
    case op==x_exit
      pc:=exit_eval(p,aCode)
    case op==x_loop
      pc:=loop_eval(p,pc)
    case op==x_private
      for i:=1 to len(p)
	v:=alltrim(p[i])
	private &v
      next
      pc:=pc+1
    case op==x_public
      pc:=pub_eval(p,pc)
    case op==x_release
      pc:=rel_eval(p,pc)
    case op==x_write
      pc++
    case op==x_proc
      pc:=0
    case op==x_call
      i:=ascan(blprocs,{|x| x[1]==p})
      if i==0
	pc++
      else
	realrun(aCode,blprocs[i][2]+1)
	pc++
      endif
    case op==x_endp
      pc:=0
    case op==x_return
      pc:=0
      if len(p)>1
	apush(blstack,eval(p[2]))
      endif
    case op==x_param
      for i:=len(p) to 1 step -1
	v:=p[i][1]
	private &v
	eval(p[i][2])
      next
      pc++
    case op==x_block
      apush(blstack,p)
      pc++
    otherwise
      pc++
  end case

  /*
     Script can be stopped by Ctrl-BackSpace but only if it is
     non recursive :-( Anyone can help?
  */
  key:=inkey()
  if key==K_CTRL_BS
    exit
  endif
end
return nil

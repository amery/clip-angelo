static function _run_blank2(_s1,stdin)
local str,_s2,f1xx,f2xx,scr,obr:=.f.
local promstr ,nstr,buffer
local oldError, _error
private ___s3,_usl
olderror=ERRORBLOCK({|x| _error:=x, mybreak(_error)})
do while !(_s1=="")
   aaaaaaaaaa()
   Begin Sequence
       obr:=.f.
       promstr=get_end(@_s1,"#if","#endif")
       if !empty(promstr)
	  aaaaaaaaaa()
	  f1xx=at(memvar->vk,promstr)
	  _usl=substr(promstr,1,f1xx-1)
	  promstr=substr(promstr,f1xx+len(vk))
	  if &_usl
	     _run_blank2(promstr,stdin)
	  endif
	  obr:=.t.
	  //break
       endif
       promstr=get_end(@_s1,"#while","#endwhile")
       if !empty(promstr)
	  aaaaaaaaaa()
	  f1xx=at(memvar->vk,promstr)
	  _usl=substr(promstr,1,f1xx-1)
	  promstr=substr(promstr,f1xx+len(vk))
	  nstr=1
	  do while (&_usl)
	     if nstr==1
		_run_blank2(promstr,stdin)
	     else
		aaaaaaaaaa(,,,mlcount(promstr,250))
		_run_blank2(promstr,stdin)
	     endif
	     nstr++
	     if !stdin
		inkey()
	     endif
	     if lastkey()==27
		_nesc++
	     elseif lastkey()!=0
		_nesc=0
	     endif
	     if _nesc=2
	       buffer=" ���� ��������� �� ������� <ESC>! "+vk
	       fwrite(_pnumber,buffer,len(buffer))
	       exit
	     endif
	  enddo
	  obr:=.t.
	  //break
       endif
       if !obr
	  f1xx=at(memvar->vk,_s1)
	  _s2=iif(f1xx!=0,substr(_s1,1,f1xx-1),_s1)
	  _s1=iif(f1xx!=0,substr(_s1,f1xx+len(vk)),"")
	  if at(":=",_s2)!=0
	     ___s3:=alltrim(_s2)
	     _s2:=&___s3
	     _s2:=""
	     obr:=.t.
	  endif
       else
	  _s2 := ""
       endif
       do while !(_s2=="")
	   f1xx=at("#",_s2)
	   if f1xx!=0
	      ___s3=substr(_s2,f1xx)
	      do while substr(___s3,1,1)="#"
		 ___s3=substr(___s3,2)
	      enddo
	      buffer=substr(_s2,1,f1xx-1)
	      //? "a1",buffer
	      fwrite(_pnumber,buffer,len(buffer))
	      f2xx=atalpha(___s3)
	      _s2=iif(f2xx=0,"",substr(___s3,f2xx))
	      ___s3=iif(f2xx=0,___s3,substr(___s3,1,f2xx-1))
	   else
	      buffer=_s2
	      //? "a2",buffer
	      fwrite(_pnumber,_s2,len(_s2))
	      exit
	   endif
	   if !isdigit(___s3)
	      if select("blank")!=0
		 if blank->what==0
		    str=alltrim(sstr(&___s3))
		    fwrite(_pnumber,str,len(str))
		 elseif blank->what<0          //KP,PL...,Q_OBOR
		    str=sstr(&___s3)
		    fwrite(_pnumber,str,len(str))
		 else                          //BALANS-15,EKON1-11
		    str=sstr(&___s3,blank->what,val(blank->decim))
		    fwrite(_pnumber,str,len(str))
		 endif
	      else
		    str=sstr(&___s3)
		    fwrite(_pnumber,str,len(str))
	      endif
	      buffer=str
	   endif
       enddo
       if !obr
	  fwrite(_pnumber,vk,len(vk))
       endif
    End Sequence
    if !stdin
       inkey()
    endif
    if _nesc=2
      buffer=" ���������� ������ �������� �������� <ESC>! "+vk
      fwrite(_pnumber,buffer,len(buffer))
      exit
    endif
enddo
ERRORBLOCK(olderror)
return

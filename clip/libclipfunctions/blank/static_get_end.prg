static function  get_end (str,begstr,endstr)
* str ���������� �� ���������, ��� ����� ��������
* ���������� ������ ��� ��������� #if � �������� #endif
local promstr,begpos,endpos,nlspace
local countv,begz,i,retstr,a,b
promstr=upper(ltrim(str))
nlspace=len(str)-len(promstr)
begstr=upper(begstr)
endstr=upper(endstr)
if !(substr(promstr,1,len(begstr))==begstr)
   return ""
endif
begpos=at(begstr,promstr)+len(begstr)
promstr=strtran(promstr,begstr,space(len(begstr)),1,1)
countv=1
do while countv>0
  a=at(begstr,promstr)
  b=at(endstr,promstr)
  do case
    case (a!=0 .and. a<b) .or. (a!=0 .and. b=0)
     promstr=strtran(promstr,begstr,space(len(begstr)),1,1)
     countv++
    case (b!=0 .and. b<a) .or. (a=0 .and. b!=0)
     promstr=strtran(promstr,endstr,space(len(endstr)),1,1)
     countv--
    case a=0 .and. b=0
     exit
  endcase
enddo
endpos=b
endpos=iif(endpos==0,len(promstr)+1,endpos)
retstr=rtrim(substr(str,begpos+nlspace,endpos-begpos))
str=substr(str,endpos+nlspace+len(endstr))
i=at(vk,str)
i=iif(i=0,0,i+len(vk))
str:=substr(str,i)
return retstr

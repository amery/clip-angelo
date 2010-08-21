/*
����� ������ �� ��������� �������� �����,
���������� ��������� � arr � ���������� ���������� ������
str -  �������� ������
len -  ����� ���������
delim - �����������
*/
FUNCTION splitLen(str, len, delim)
local m, nstr, i, arr := {}
  str := alltrim(str)
  delim := iif(delim==NIL, " ", delim)
  if len(str)<=len
	aadd(arr, str)
	RETURN arr
  endif
  m := split(str, delim)
  nstr := ''
  for i in m
    if len(nstr)+len(i)<len
      nstr += i+delim
    else
      aadd(arr, nstr)
      nstr := i+delim
    endif
  next
  aadd(arr, nstr)
RETURN arr

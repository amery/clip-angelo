/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**************************************************************************
#include <ci_common.ch>
**************************************************************************
**************************************************************************
**************************************************************************
**************************************************************************
**************************************************************************
#include <str_func/AllT.prg>
#include <str_func/atc.prg>
#include <str_func/cgi_split.prg>
#include <str_func/low_alt.prg>
#include <str_func/memotran.prg>
#include <str_func/parse_rules_text.prg>
#include <str_func/splitLen.prg>
#include <str_func/upp_alt.prg>
**************************************************************************
/*******************
see SPLIT in _regex.c
* s1 - string
* s2 - delimiter
FUNCTION Split(s1,s2)
LOCAL temp:={}, arr:={}, i:=1, p

do while search(s2, s1, arr)
	aadd(temp, left(s1, arr[i][1]-1))
	s1 := substr(s1, arr[i][2])
	i++
enddo
aadd(temp, s1)
RETURN temp
*/

/****************************************************************
* simple SPLIT without regex
* s1 - string
* s2 - simple delimiter
FUNCTION ChSplit(s1,s2)
LOCAL temp:={}, arr:={}, i:=len(s2), k

do while (k := at(s2, s1))>0
	aadd(temp, left(s1, k-1))
	s1 := substr(s1, k+i)
enddo
aadd(temp, s1)
RETURN temp*/
****************************************************************

/*
    Copyright (C) 1998-2003 Yevgen Bondar <elb@lg.bank.gov.ua>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

/***
	DAYS( <nSeconds> )

 �����頥� 楫�� �᫮ ���� ��室� �� ��������� �᫠ ᥪ㭤

 ����砭��: ���⮪ �� ������� �� 24 �� �����頥��� �㭪樥� TSTRING().
*/

FUNC DAYS(cl_secs)
RETURN INT( cl_secs / 86400 )

/***
	LENNUM( <nNum> )
 �����頥� ����� �� �८�ࠧ������ �᫠ � ��ப�
*/

FUNC LENNUM(cl_number)
RETURN	LEN(LTRIM(STR(cl_number)))


/***
 SECS( <��ப� �६���>)

 �����頥� �᫮ ᥪ㭤, ᮤ�ঠ���� � ��।����� ��ࠬ���

	��᫮ ᥪ㭤 � ��ਮ��
	-------    -----------
	     60    1 �����
	   3600    1 ��
	  86400    1 ����
*/

FUNC SECS(cl_time)

RETURN	VAL( cl_time ) * 3600 +;
	VAL(SUBSTR(cl_time,4)) * 60 +;
	VAL(SUBSTR(cl_time,7))


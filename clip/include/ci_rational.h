/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/

/*
   note russian
   ����� rational - ����� � ��������� ������, ��������������
   � ���� ������������ ����� �� ���� �������� integer x/y
   � �������������� ���-��� ��������,
   ������� �� ���� - ���� ���� !!!
   ������� ��������� �������� �� ������� !!!

   (c) itk 2000, www.itk.ru
   autor - uri, uri@itk.ru

*/

#include "ci_integer.h"

#ifndef CN__ITK_RATIONAL_H

#define CN__ITK_RATIONAL_H

#include "ci_rational/typedef_struct_rational.h"

rational *rational_init();

		/* ������� ������ ������ */

rational *rational_fromString(char *s);

		/* ������� ������ � ��������� ��� ��������� ��
		   ��������� ���������� ��������� ������� � ��������,
		   ������������, ���������� � ����������������� ���� */

rational *rational_long_init(long i);

		/* ������� ������ � ��������� ��� ��������� ��
		   ��������� long ������� */

rational *rational_double_init(double i);

		/* ������� ������ � ��������� ��� ��������� ��
		   double ������� */

rational *rational_long2_init(long n, long d);

		/* ������� ������ � ��������� ��� ��������� �� long ��������
		   � ���� ����� n/d */

rational *rational_integer_init(integer * i);

		/*������� ������ � ��������� ��� ��������� �� integer ������� */

rational *rational_integer2_init(integer * n, integer * d);

		/* ������� ������ � ��������� ��� ��������� �� integer
		   �������� � ���� ������������ ����� n/d */

void rational_destroy(rational * data);

		/* ���������� ������ */

rational *rational_copy(rational * y);

		/*������� ������ � ��������� ��� ��������� �� rational ������� */

rational *rational_assign(rational * data, rational * y);

		/* ��������� data ��������� �� rational ������� */

rational *rational_inv(rational * data);

		/* ���������� �������� ������, �.�. x=(-y); */

rational *rational_inverse(rational * data);

		/* �������� ���� �������� ������� �� �������� */

void rational_normalize(rational * data);

		/* ���������� ���� �� ����� �������� */

int rational_empty(rational * data);

		/* ��������� ������ �� ������, ��� int �����==0 */

int rational_sign(rational * data);

		/* ���������� 0 - ���� ������ ����� 0,
		   -1 - ���� ������ ����, 1-���� ������ */

int rational_cmp(rational * data, rational * y);

		/* ���������� ��� rational, ����������:
		   -1 ���� ������ ������ ���������
		   0 ���� �����
		   1 ���� ������ ������ ���������
		 */

int rational_abscmp(rational * data, rational * y);

		/* ���������� ��� rational �� ���������� ���������, ����������:
		   -1 ���� ������ ������ ���������
		   0 ���� �����
		   1 ���� ������ ������ ���������
		 */

rational *rational_abs(rational * data);

		/* ���������� ���������� �������� */

rational *rational_pow(rational * data, long y);

		/* return pow(data,y) */

rational *rational_powa(rational * data, long y);

		/* data=pow(data,y) */

rational *rational_sqr(rational * data);

		/* ���������� ������� ������� */

rational *rational_sqrt(rational * data);

		/* ���������� ���������� ������ �������
		   �������� �� "����������", �� ������ ����������� */

integer *rational_toInteger(rational * data);

		/* ���������� "���������" � ������� ����� �������� */

rational *rational_double_adda(rational * data, double y);

rational *rational_adda(rational * data, rational * y);

		 /* ����������� ���� �� �������� y */

rational *rational_double_add(rational * data, double y);

rational *rational_add(rational * data, rational * y);

		/* ���������� �������� y */

rational *rational_double_suba(rational * data, double y);

rational *rational_suba(rational * data, rational * y);

		/* ��������� ���� �� �������� y */

rational *rational_double_sub(rational * data, double y);

rational *rational_sub(rational * data, rational * y);

		/* �������� �������� y */

rational *rational_double_mula(rational * data, double y);

rational *rational_mula(rational * data, rational * y);

		/* �������� ���� �� y */

rational *rational_double_mul(rational * data, double y);

rational *rational_mul(rational * data, rational * y);

		/* ��������  �� y */

rational *rational_double_diva(rational * data, double y);

rational *rational_diva(rational * data, rational * y);

		/* ����� ���� �� y */

rational *rational_double_div(rational * data, double y);

rational *rational_div(rational * data, rational * y);

		/* �����  �� y */

rational *rational_fmod(rational * data, rational * y);

		/* ���������� ������� �� ������� */

char *rational_toString2(rational * data);

		/* ��������� �������� � ��������� �������������
		   � ���� 999999/999999 */

char *rational_toString(rational * data, int base, int dec, int group);

		/* ��������� �������� � ��������� �������������
		   base - � ����� ����������, �� �� ����� 16 */

double rational_toDouble(rational * data);

		/* ��������� �������� � double */

#endif

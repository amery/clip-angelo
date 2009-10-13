/*
$Log: integer.h,v $
Revision 1.1  2006/06/22 19:35:16  itk
uri: init sf.net repository

Revision 1.2  2000/09/12 08:50:41  clip
DMALLOC disabled
from uri:

Revision 1.1  2000/09/12 07:28:31  clip
add integer & rational source
from uri:

*/
/*
   note russian
   ����� integer - ����� � �������������� ���-��� ��������
   ������� �� ���� - ���� ���� !!!
   ������� ��������� �������� �� ������� !!!

   (c) itk 2000, www.itk.ru
   autor - uri, uri@itk.ru

*/

#ifndef CI___ITK_INTEGER_H

#define CI___ITK_INTEGER_H

                /* malloc debiging */
/* #define DMALLOC */

               /* ����������� �� 32 �������� ��� �� 64 �������� */
#define INTEGER_SHORTB
               /*  ��� ������� 0 */
#define INTEGER_SYM_NULL  48
#define INTEGER_SYM_A     65
#define INTEGER_SYM_MINUS 45
#define INTEGER_SYM_PLUS  43

#ifdef INTEGER_SHORTB
                        /* ���������� �������� - short */
   #define  INTEGER_HALF       16
                        /* ������ �������� - int */
   #define  INTEGER_FULL       32
                        /* ������������ ����� ��� 16 ������� */
   #define  INTEGER_MAXNUM     0xffffL

   typedef  unsigned short *   INTEGER_VECTOR     ;
   typedef  unsigned short     INTEGER_HALF_TYPE  ;
   typedef  unsigned int       INTEGER_FULL_TYPE  ;
#else
                        /* ���������� �������� - int */
   #define  INTEGER_HALF       32
                        /* ������ �������� -long */
   #define  INTEGER_FULL       64
                        /* ������������ ����� ��� 32 ������� */
   #define  INTEGER_MAXNUM     0xffffffffL

   typedef  unsigned int *     INTEGER_VECTOR    ;
   typedef  unsigned int       INTEGER_HALF_TYPE ;
   typedef  unsigned long      INTEGER_FULL_TYPE ;
#endif

#define  EXTRACT(x) ( x & INTEGER_MAXNUM )
#define  UP(x)      ( x << INTEGER_HALF )
#define  DOWN(x)    ( ( x >> INTEGER_HALF ) & INTEGER_MAXNUM )

typedef struct
{
   int	sign;		/* sign, nonzero is negative */
   int	len;		/* number of values in array */
   INTEGER_VECTOR vec;	/* pointer to array of values */
}
integer;

char *      integer_toString(integer * data, int base);
		/*
		��������� �������� � ��������� �������������
		base - � ����� ����������, �� �� ����� 16
		*/

char *      integer_toString2(integer *data);
		/*
		��������� �������� � ��������� �������������
		� ���� 999999 999999 999999 ..... � ���-�� ������
		����������� �������
		*/

long        integer_toLong(integer * data);
		/*���������� "���������" � 64 ���� �������� �������� ������ */

void        integer_destroy(integer *data);
		/* ���������� ������ */

integer *   integer_init ( );
		/* ������� ������ ������  */

integer *   integer_long_init ( long i);
		/* ������� ������ � ��������� ��� ���������
		�� ��������� long ������� */

integer *   integer_long2_init ( long low,long high);
		/* ������� ������ � ��������� ��� ���������
		�� long ��������
		������� ��������� ����������,
		������� �������� */

integer *   integer_fromString(const char * s);
		/* ������� ������ � ��������� ��� ���������
		�� ���������� ��������� */

integer *   integer_mul(integer * d1, integer * d2);
		/* d1 * d2 */

integer *   integer_mula(integer *d1, integer * d2);
                /* d1 *= d2 */

integer *   integer_long_mul(integer * data, long y);
		/* return (data*y) */

integer *   integer_long_mula(integer * data, long y);
		/* data*=y */

integer *   integer_pow(integer * data, long y );
		/* return ( pow(data,y) */

integer *   integer_powa(integer * data, long y );
		/* data=pow(data,y) */

integer *  integer_gcd(integer * data, integer * y);
		/* ���������� ���������� ����� �������� ��������
		������� � integer y */

integer *   integer_unscale(integer *data, INTEGER_HALF_TYPE y, INTEGER_HALF_TYPE * ost);
integer *   integer_sunscale(integer *data, INTEGER_HALF_TYPE y, INTEGER_HALF_TYPE * ost);
                /* ����� ���� �� y, ������� ���������� � ost */

integer *   integer_sdivide(integer * data, integer * y, integer ** div);
                /* ����� ���� �� y, ������� ���������� � div */

integer *   integer_div(integer * data, integer * y);
		/*  return (data/y)  */

integer *   integer_diva(integer * data, integer * y);
		/*  (data/=y)  */

integer *   integer_mod(integer * data, integer * y);
		/*  return (data%y) */

int         integer_lg(integer * data);
		/* ���������� �������� ������ */

integer *   integer_sqrt(integer * data);
		/* ���������� ���������� ������ �����,
		������ �� �������������� ����� ���� 0 */

integer *   integer_sadd(integer * d1, integer * d2);
		/*  d1 += d2   ��� ����� ����� ����� */

integer *   integer_long_add(integer * data, long y);
		/* return (data+y) */

integer *   integer_long_adda(integer * data, long y);
		/* data+=y */

integer *   integer_add(integer * d1, integer * d2);
		/* return (d1+d2) */

integer *   integer_adda(integer * d1, integer * d2);
		/* d1+=d2 */

integer *   integer_inv(integer * data);
		/* ���������� �������� ������, �.�. x=(-y); */

integer *   integer_inverse(integer * data);
	        /* �������� ���� �������� ������� */

integer *   integer_ssub(integer * d1, integer * d2);
		/* d1-=d2 ��� ����� ����� ����� */

integer *   integer_long_sub(integer * data, long y);
		/* return (data-y) */

integer *   integer_long_suba(integer * data, long y);
		/* data-=y */

integer *   integer_sub(integer * d1, integer * d2);
		/* return (d1-d2) */

integer *   integer_suba(integer *d1, integer * d2);
		/* d1-=d2 */

integer *   integer_lshift(integer * data, int y);
		/* return (data<<y) */

integer *   integer_lshifta(integer * data, int y);
		/* data<<=y */

integer *   integer_rshift(integer * data, int y);
		/* return (data>>y) */

integer *   integer_rshifta(integer * data, int y);
		/* data>>=y */

int         integer_getBit(integer * data, int ind);
		/*���������� �������� ���� � ������� ind ( 0 || 1 )*/

integer *   integer_setBit (integer * data, int ind, int value);
integer *   integer_setBita(integer * data, int ind, int value);
		/* ������������� ��� � ������� ind �
		�������� value ( 0 || 1 ) */

integer *   integer_or(integer * d1, integer * d2);
integer *   integer_long_or(integer * d1, long y);
integer *   integer_ora(integer * d1, integer * d2);
		/* ��������� OR */

integer *   integer_and(integer * d1, integer * d2);
integer *   integer_long_and(integer * d1, long y);
integer *   integer_anda(integer * d1, integer * d2);
		/* ��������� AND */

integer *   integer_xor(integer * d1, integer * d2);
integer *   integer_long_xor(integer * d1, long y);
integer *   integer_xora(integer * d1, integer * d2);
		/* ��������� XOR */

int         integer_abscmp(integer * d1, integer * d2);
		/* ���������� ��� integer �� ���������� ���������, ����������:
		-1 ���� ������ ������ ���������
		0 ���� �����
		1 ���� ������ ������ ��������� */

int         integer_cmp(integer * d1, integer * d2);
		/*���������� ��� integer, ����������:
		-1 ���� ������ ������ ���������
		0 ���� �����
		1 ���� ������ ������ ���������*/

integer *   integer_abs(integer * data);
		/*���������� ���������� ��������*/

int         integer_even(integer * data);
		/* ���������� 1 - ���� ������ ����� */

integer *   integer_clear(integer * data);
		/* ������� ��� ������������ ����� � 0,
		�� ����������� � �� ���������������� ������ */

integer *   integer_copy (integer * data);
		/* ������������ ������ */

integer *   integer_assign (integer * dst, integer * src);
		/* ������������ src to dst */

int	    integer_empty(integer * data);
		/* ��������� ������ �� ������, ��� �����==0 */

int         integer_len(integer * data);
		/* ���������� ����� ������� */

INTEGER_VECTOR integer_vec(integer * data);
		/* vec() ���������� ������ ��������� */

int         integer_realLen(integer * data);
		/*  ���������� �������� ����� �.�.
		������ �������� ��������  */

int         integer_sign(integer * data);
		/* ���������� 0- ���� ������ ����� 0,
		-1 -���� ������ ����, 1 -���� ������ */

integer *   integer_resize(integer * data, int newlen);
		/* �������� ������ �������  */

#endif

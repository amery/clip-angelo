#include "ci_config.ch"

#ifndef CI_AC_DEFINED
#define CI_AC_DEFINED

#define AC_NONE_STRINGS [ None selected strings ]

/* code of user fuction messages */
#define AC_IDLE            0          /* �������� ������� ������          */
#define AC_HITTOP          1          /* ������� �������� ����� �� ������ */
#define AC_HITBOTTOM       2          /* ������� �������� ���� �� ������  */
#define AC_EXCEPT          3          /* ���������������� �������         */
#define AC_NOITEM          4          /* ��� ��������� �������            */

/* return code from user function */
#define AC_ABORT            0     /* �������� ACHOICE(), ������� 0              */
#define AC_SELECT           1     /* ������� ������� �������, ������� �� �����  */
#define AC_CONT             2     /* ���������� ACHOICE()                       */
#define AC_GOTO             3     /* ������� � ��������� ������� � ������       */
				  /* ������, ��������������� ������� �������    */
#define AC_REDRAW           4     /* Re-Draw the ACHOICE() window. */

#endif


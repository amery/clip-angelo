#ifndef _DBEDIT_DEFINED
#define _DBEDIT_DEFINED
/*
#define DE_HEADSEP      translate_charset(__CHARSET__,host_charset(),"���")
#define DE_COLSEP       translate_charset(__CHARSET__,host_charset()," � ")
#define DE_FOOTSEP      translate_charset(__CHARSET__,host_charset(),"���")
*/
#define DE_HEADSEP      "|"
#define DE_COLSEP       "-"
#define DE_FOOTSEP      "="

#define DE_COLOR        "N/W, N/BG, B/W, B/BG, B/W, B/BG, R/W, B/R"

#define DE_IDLE             0          // �������� ������� �������
#define DE_HITTOP           1          // ������� ����� �� ������� �����
#define DE_HITBOTTOM        2          // ������� ����� �� ������� ����
#define DE_EMPTY            3          // ��� ������� � ������� �������
#define DE_EXCEPT           4          // ������� �� �������������� DBEDIT()

#define DE_ABORT            0           // ��������� DBEDIT()
#define DE_CONT             1           // ���������� DBEDIT()
#define DE_REFRESH          2           // ���������/������� ��� ������ �� ������
#define DE_APPEND           3           // undocumented return value,
					// EOF as new record

#endif


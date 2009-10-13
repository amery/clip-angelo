
/*
   $Log: list.h,v $
   Revision 1.1  2006/06/22 19:35:17  itk
   uri: init sf.net repository

   Revision 1.2  2000/05/24 18:34:35  clip
   _clip_push_area
   indents all
   Paul Lasarev <paul@itk.ru>

   Revision 1.1  1999/10/25 16:39:28  paul
   first entry

   * Revision 1.1  1997/03/12  14:51:41  paul
   * initial
   *
 */

#ifndef CI_LIST_H
#define CI_LIST_H

typedef struct
{
	void *head;
	void *current;
}
List;

typedef struct
{
	void *prev;
	void *next;
}
ListEl;

void init_List(List * list);
int empty_List(List * list);

int first_List(List * list);
int last_List(List * list);

int next_List(List * list);
int Next_List(List * list);	/* ring next */

int prev_List(List * list);
int Prev_List(List * list);	/* ring prev */
int seek_List(List * list, void *item);		/* ������������� ������� */

void remove_List(List * list);	/* ������� �������; ������� ���������� ��������� */
void removeIt_List(List * list, void *item);

void insert_List(List * list, void *item);	/* ��������� ����� �������� */
void insertBefore_List(List * list, void *item);	/* ��������� ����� �������  */
void append_List(List * list, void *item);	/* ��������� � �����        */
void prepend_List(List * list, void *item);	/* ��������� � ������       */

void removeAll_List(List * list);

void put_List(List * list, void *item);		/* �������� ������� */

#endif

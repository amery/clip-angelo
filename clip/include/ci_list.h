
/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
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
int seek_List(List * list, void *item);	/* ������������� ������� */

void remove_List(List * list);	/* ������� �������; ������� ���������� ��������� */
void removeIt_List(List * list, void *item);

void insert_List(List * list, void *item);	/* ��������� ����� �������� */
void insertBefore_List(List * list, void *item);	/* ��������� ����� �������  */
void append_List(List * list, void *item);	/* ��������� � �����        */
void prepend_List(List * list, void *item);	/* ��������� � ������       */

void removeAll_List(List * list);

void put_List(List * list, void *item);	/* �������� ������� */

#endif

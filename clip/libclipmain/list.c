
/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

   * Revision 1.1  1997/03/12  14:51:41  paul
   * initial
   *
 */

/*#include <ci_clip.h>*/
#include <ci_clip.h>
#include <ci_list.h>

#define PREV(el) ((ListEl *)(el))->prev_of_ListEl
#define NEXT(el) ((ListEl *)(el))->next_of_ListEl

#include <list/init_List.c>
#include <list/empty_List.c>
#include <list/first_List.c>
#include <list/last_List.c>
#include <list/next_List.c>
#include <list/Next_List.c>
#include <list/prev_List.c>
#include <list/Prev_List.c>
#include <list/seek_List.c>
#include <list/remove_List.c>
#include <list/removeIt_List.c>
#include <list/insert_List.c>
#include <list/insertBefore_List.c>
#include <list/append_List.c>
#include <list/prepend_List.c>
#include <list/removeAll_List.c>
#include <list/put_List.c>

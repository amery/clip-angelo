/*
Start total new system v. 0.0
with hard coded long name variables to have clear system
Angelo GIRARDI
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <ci_po_util.h>

#define COMPAT_DELIM '\177'

/*
#include <po_util/typedef_enum_ExtractState.h>
*/
typedef enum
{
	E_Norm_of_ExtractState,
	   E_Start_of_ExtractState,
	      E_Msg_of_ExtractState,
	         E_Stop_of_ExtractState,
}
ExtractState;
/*
#include <po_util/typedef_struct_Buf.h>
*/
typedef struct
{
	char *ptr_of_Buf;
	int end_of_Buf;
	int size_of_Buf;
}
Buf;

/*
#include <po_util/typedef_enum_StrState.h>
*/
typedef enum
{
	S_norm_of_StrState,
			   S_bs_of_StrState,
			   		   	      S_oct1_of_StrState,
			   		   	      		   	      	         S_oct2_of_StrState,
			   		   	      		   	      	         		   	      	         	            S_stop_of_StrState,
}
StrState;
static void put_Buf(Buf * bp, int ch);

static int max_num = 96;

#include <po_util/static_put_Buf.c>
#include <po_util/cmp_PoEntry.c>
#include <po_util/new_PoEntry.c>
#include <po_util/delete_PoEntry.c>
#include <po_util/static_raw_put_str.c>
#include <po_util/static_put_char.c>
#include <po_util/static_put_str.c>
#include <po_util/po_parse_template.c>

#define TM_YEAR_ORIGIN 1900

#include <po_util/static_difftm.c>

/* According to Sun's Uniforum proposal the default message domain is
   named `messages'.  */
#define MESSAGE_DOMAIN_DEFAULT "messages"

#include <po_util/po_write_header.c>
#include <po_util/static_write_po_header.c>
#include <po_util/po_write_entry.c>
#include <po_util/static_end_pl_index.c>
#include <po_util/po_read_entry.c>
#include <po_util/po_read_entry_compat.c>
#include <po_util/po_write_entry_compat.c>

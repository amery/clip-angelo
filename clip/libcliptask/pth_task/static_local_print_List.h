static void
local_print_List(List * list)
{
   int       r;

   Task     *tp;

   for (r = first_List(list); r; r = next_List(list))
    {
       tp = (Task *) list->current;
#if defined(_LDEBUG) || defined(_MDEBUG)
       printf("list-print,task=%p,task->id=%ld,namename=%s,%p,data=%s#\n",
	      (void *) tp, tp->id, tp->name, tp->name, (char *) tp->data);
#endif
    }

}

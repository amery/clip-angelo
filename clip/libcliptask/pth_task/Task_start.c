TASK_DLLEXPORT int
Task_start(Task * tp)
{
/* not need under PTH*/
#ifdef _LDEBUG
   printf("F=%s,L=%d,name=%s,%p\n", __FILE__, __LINE__, tp->name, tp->name);
#endif
   return 1;
}

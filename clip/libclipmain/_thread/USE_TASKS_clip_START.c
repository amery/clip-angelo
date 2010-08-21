/*
 *    taskid := start(main_func)
 */
int
clip_START(ClipMachine * ClipMachineMemory)
{
   Task *tp;

   char *name, *tname = "block";

   start_data *sp;

   int argc = ClipMachineMemory->argc - 1;

   int i;

   ClipVar *stack;

   static int first = 1;

   int type;

   type = _clip_parinfo(ClipMachineMemory, 1);
   if (type != CHARACTER_type_of_ClipVarType && type != CCODE_type_of_ClipVarType && type != PCODE_type_of_ClipVarType)
      return EG_ARG;

   name = _clip_parc(ClipMachineMemory, 1);

   stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
   memset(stack, 0, sizeof(ClipVar) * (argc + 1));

   for (i = 0; i < argc; ++i)
      _clip_clone(ClipMachineMemory, stack + i, ARGPTR(ClipMachineMemory, i + 2));

   sp = (start_data *) calloc(sizeof(start_data), 1);
   sp->stack = stack;
   sp->argc = argc;
   if (name)
      tname = sp->name = strdup(name);
   else
      _clip_clone(ClipMachineMemory, &sp->block, _clip_spar(ClipMachineMemory, 1));
   sp->ClipMachineMemory = ClipMachineMemory;

   tp = Task_new(tname, 4096 * 32, sp, task_run, task_destroy);

   if (tp == NULL)
      _clip_retnl(ClipMachineMemory, -1);

   _clip_retnl(ClipMachineMemory, Task_get_id(tp));

   Task_start(tp);
   if (first)
   {
      Task_start_sheduler();
      first = 0;
   }
   Task_yield();

   return 0;
}

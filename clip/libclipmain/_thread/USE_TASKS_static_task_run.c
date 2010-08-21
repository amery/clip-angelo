static void *
task_run(void *data)
{
   start_data *sp = (start_data *) data;

   ClipMachine *ClipMachineMemory = sp->ClipMachineMemory;

   int r;

   void *ret;

   sp->ClipMachineMemory = new_ClipMachine(ClipMachineMemory->screen);

   if (sp->name)
      ret = _clip_clip(sp->ClipMachineMemory, sp->name, sp->argc, sp->stack, 0);
   else
      ret = _clip_eval(sp->ClipMachineMemory, &sp->block, sp->argc, sp->stack, 0);

   if (r)
      _clip_logg(0, "task_run: cannot start function '%s'", sp->name);

   /*printf("\ntask_run done\n"); */
   /*       return r; */
   return ret;
}

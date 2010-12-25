static void
task_destroy(void *data)
{
   start_data *sp = (start_data *) data;

   int i;

   /*printf("\ntask_destroy\n"); */

   for (i = 0; i < sp->argc + 1; i++)
      _clip_destroy(sp->ClipMachineMemory, sp->stack + i);

   if (sp->name)
      free(sp->name);
   else
      _clip_destroy(sp->ClipMachineMemory, &sp->block);

#if 1
   delete_ClipMachine(sp->ClipMachineMemory);
#endif
   free(sp->stack);
   free(sp);
}

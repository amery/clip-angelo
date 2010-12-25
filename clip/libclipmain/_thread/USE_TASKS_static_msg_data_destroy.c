static void
msg_data_destroy(void *data)
{
   msg_data *m = (msg_data *) data;

   _clip_destroy(m->ClipMachineMemory, &m->var);
   free(m);
}

static msg_data *
msg_data_new(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   msg_data *rp;

   rp = (msg_data *) calloc(sizeof(msg_data), 1);
   rp->ClipMachineMemory = ClipMachineMemory;
   _clip_clone(ClipMachineMemory, &rp->var, vp);

   return rp;
}

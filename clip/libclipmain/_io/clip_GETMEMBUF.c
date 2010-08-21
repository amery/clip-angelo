int
clip_GETMEMBUF(ClipMachine * ClipMachineMemory)
{
   ClipVar *rp = RETPTR(ClipMachineMemory);

   if (ClipMachineMemory->pbuf)
      _clip_var_str(ClipMachineMemory->pbuf->buf_of_OutBuf, ClipMachineMemory->pbuf->ptr_of_OutBuf - ClipMachineMemory->pbuf->buf_of_OutBuf, rp);
   return 0;
}

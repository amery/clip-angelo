/*
   OUTWORD(<nPort|cHexPort>,<nOutByte|cHexOutByte>) --> lSuccessful
 */
int
clip_OUTWORD(ClipMachine * ClipMachineMemory)
{
   int port, val, r;

   char *portc, *valc;

   if (ClipMachineMemory->argc < 2)
      return EG_ARG;

   portc = _clip_parc(ClipMachineMemory, 1);
   valc = _clip_parc(ClipMachineMemory, 2);

   if (portc)
      port = strtol(portc, 0, 16);
   else
      port = _clip_parni(ClipMachineMemory, 1);
   if (valc)
      val = strtol(valc, 0, 16);
   else
      val = _clip_parni(ClipMachineMemory, 2);

   r = m_out_portw(ClipMachineMemory, port, val);

   _clip_retl(ClipMachineMemory, !r);

   return 0;
}

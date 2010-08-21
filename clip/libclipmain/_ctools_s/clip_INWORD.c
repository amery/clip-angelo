/*
   INWORD(<nPort|cHexPort>) --> nByte
 */
int
clip_INWORD(ClipMachine * ClipMachineMemory)
{
   int port, r;

   char *portc;

   if (ClipMachineMemory->argc < 1)
      return EG_ARG;

   portc = _clip_parc(ClipMachineMemory, 1);

   if (portc)
      port = strtol(portc, 0, 16);
   else
      port = _clip_parni(ClipMachineMemory, 1);

   r = m_in_portw(ClipMachineMemory, port);

   _clip_retni(ClipMachineMemory, r);

   return 0;
}

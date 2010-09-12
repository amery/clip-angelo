int
clip_GETCLIPBOARDDATA(ClipMachine * ClipMachineMemory)
{
   int len;

   char *data = (char *) from_clipboard(_clip_parni(ClipMachineMemory, 1), &len);

   _clip_retcn_m(ClipMachineMemory, data, len);
   return 0;
}

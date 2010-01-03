int
clip_SETCLIPBOARDDATA(ClipMachine * ClipMachineMemory)
{
   int       len;

   char     *data = _clip_parcl(ClipMachineMemory, 1, &len);

   if (data == NULL)
      _clip_retl(ClipMachineMemory, 0);
   else
      _clip_retl(ClipMachineMemory, to_clipboard((const unsigned char *) data, _clip_parni(ClipMachineMemory, 2), len));

   return 0;
}

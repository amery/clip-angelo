int
clip_QOUT(ClipMachine * ClipMachineMemory)
{
   char *pEol = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("PRINTER_EOL"));

   if (pEol == NULL || *pEol == 0)
      out_any(ClipMachineMemory, "\n", 1, _clip_colorSelect(ClipMachineMemory), DEV_DEVS);
   else
      out_any(ClipMachineMemory, pEol, strlen(pEol), _clip_colorSelect(ClipMachineMemory), DEV_DEVS);
   clip_QQOUT(ClipMachineMemory);
   sync_mp(ClipMachineMemory);
   return 0;
}

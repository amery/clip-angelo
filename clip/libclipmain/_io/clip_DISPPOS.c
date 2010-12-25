/* disppos(nRow,nCol) */
int
clip_DISPPOS(ClipMachine * ClipMachineMemory)
{
   int y = _clip_parnl(ClipMachineMemory, 1);

   int x = _clip_parnl(ClipMachineMemory, 2);

   _clip_fullscreen(ClipMachineMemory);

   clip_region(ClipMachineMemory, &y, &x, 0, 0, 1, -1);

   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;
   sync_mp(ClipMachineMemory);

   return 0;
}

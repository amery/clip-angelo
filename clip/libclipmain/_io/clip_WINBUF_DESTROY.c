int
clip_WINBUF_DESTROY(ClipMachine * ClipMachineMemory)
{
   int no;

   WinBuf *wp;

   if (ClipMachineMemory->argc < 1)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);

   if (!wp)
      return EG_ARG;

   destroy_WinBuf(wp);
	winbufs->items_of_Coll[no] = 0;

   return 0;
}

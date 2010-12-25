/*
   buf = winbuf_create(height,width) - ����������� height x width
	   (����� ��� ����� ���� ���).
*/
int
clip_WINBUF_CREATE(ClipMachine * ClipMachineMemory)
{
   int height, width, i;

   WinBuf *wb;

   if (ClipMachineMemory->argc < 2)
      return EG_ARG;

   height = _clip_parni(ClipMachineMemory, 1);
   width = _clip_parni(ClipMachineMemory, 2);

   wb = new_WinBuf(height, width);

   if (!winbufs)
      winbufs = new_Coll(destroy_WinBuf, 0);

	for (i = 0; i < winbufs->count_of_Coll; i++)
   {
		if (!winbufs->items_of_Coll[i])
      {
			winbufs->items_of_Coll[i] = wb;
	 break;
      }
   }

   if (i == winbufs->count_of_Coll)
      append_Coll(winbufs, wb);

   _clip_retni(ClipMachineMemory, i);

   return 0;
}

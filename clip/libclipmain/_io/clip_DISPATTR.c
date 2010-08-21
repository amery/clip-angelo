/*
   dispattr(nTop,nLeft,nBottom,nRight,cColor|nColor)
 */
int
clip_DISPATTR(ClipMachine * ClipMachineMemory)
{
   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int bottom = _clip_parni(ClipMachineMemory, 3);

   int right = _clip_parni(ClipMachineMemory, 4);

   int i, j, attr;

   Screen *sp;

   ClipVar *vp;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   vp = _clip_par(ClipMachineMemory, 5);
   if (vp)
   {
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	 attr = _clip_parni(ClipMachineMemory, 5);
      else
	 attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 5));
   }
   else
      attr = _clip_colorSelect(ClipMachineMemory);

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);
   for (i = top; i <= bottom; ++i)
   {
      sp->touched[i] = 1;
      for (j = left; j <= right; ++j)
      {
	 /*int pg = sp->colors[i][j] & PG_ATTR; */

	 sp->colors[i][j] = attr /*| pg */ ;
      }
   }

   sync_mp(ClipMachineMemory);
   return 0;
}

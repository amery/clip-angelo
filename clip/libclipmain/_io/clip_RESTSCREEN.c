int
clip_RESTSCREEN(ClipMachine * ClipMachineMemory)
{
   int l;

   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int bottom = _clip_parni(ClipMachineMemory, 3);

   int right = _clip_parni(ClipMachineMemory, 4);

   ClipVar *vp = _clip_par(ClipMachineMemory, 5);

   char *s;

   if (!vp || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      return 0;

   _clip_fullscreen(ClipMachineMemory);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      top = 0;
   if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
      left = 0;
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
   {
      ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

      bottom = wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
   }
   if (_clip_parinfo(ClipMachineMemory, 4) != NUMERIC_type_of_ClipVarType)
   {
      ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

      right = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
   }
   s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
   l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);
   rest_region(ClipMachineMemory, top, left, bottom, right, s, l);

   sync_mp(ClipMachineMemory);
   return 0;
}

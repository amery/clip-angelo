int
clip_SAVESCREEN(ClipMachine * ClipMachineMemory)
{
   int l;

   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int bottom = _clip_parni(ClipMachineMemory, 3);

   int right = _clip_parni(ClipMachineMemory, 4);

   int par = _clip_parinfo(ClipMachineMemory, 0);

   int mainscreen = (_clip_parl(ClipMachineMemory, 5) != 0 ? 0 : ClipMachineMemory->wnum);

   int modescreen = (_clip_parl(ClipMachineMemory, 5) == 0 ? 0 : 2);

   ClipVar *rp;

   char *s;

   ClipWindow *wp;

   wp = ClipMachineMemory->windows + mainscreen;

   _clip_fullscreen(ClipMachineMemory);

   if (par < 3 || _clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      bottom = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;
   if (par < 4 || _clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      right = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, modescreen, mainscreen);

   s = save_region(ClipMachineMemory, 0, top, left, bottom, right, &l);

   rp = RETPTR(ClipMachineMemory);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;

   sync_mp(ClipMachineMemory);

   return 0;
}

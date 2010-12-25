/*
  winbuf_out_trans(buf,top,left,str,color) - output "transparent" text
  2003/08/21, Mauricio Abre
*/
int
clip_WINBUF_OUT_TRANS(ClipMachine * ClipMachineMemory)
{
   int no;

   WinBuf *wp;

   int top, left;

   char *str;

   int slen;

   ClipVar *vp;

   int attr;

   if (ClipMachineMemory->argc < 5)
      return EG_ARG;

   no = _clip_parni(ClipMachineMemory, 1);
   wp = find_WinBuf(no);

   if (!wp)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 2);
   left = _clip_parni(ClipMachineMemory, 3);
   str = _clip_parcl(ClipMachineMemory, 4, &slen);

   if (!str)
      return EG_ARG;

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

   if (top >= 0 && top < wp->hi)
   {
      int y = top;

      int x, i;

      for (x = left, i = 0; x < wp->wi && i < slen; i++, x++)
      {
	 int ch;

	 ch = ClipMachineMemory->term_xlat[((unsigned char *) str)[i]];

	 if (ch < 32 && !is_pgch(ch))
	 {
	    wp->chars[y][x] = ch + 64;
	    wp->colors[y][x] = attr | 0x8;
	    wp->attrs[y][x] = 0;
	 }
	 else
	 {
	    wp->chars[y][x] = ch;
	    wp->colors[y][x] = (wp->colors[y][x] & 0xf0) | (attr & 0x0f);
	    wp->attrs[y][x] = 0;
	 }
      }
   }

   return 0;
}

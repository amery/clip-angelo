int
clip_SCROLL(ClipMachine * ClipMachineMemory)
{
   int i, j;

   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   int bottom = _clip_parni(ClipMachineMemory, 3);

   int right = _clip_parni(ClipMachineMemory, 4);

   int vert = _clip_parni(ClipMachineMemory, 5);

   int hor = _clip_parni(ClipMachineMemory, 6);

   int attr = _clip_colorSelect(ClipMachineMemory);

   char fill = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb));

   Screen *sp;

   int fs = ClipMachineMemory->fullscreen;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      top = 0;
   if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
      left = 0;
   if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType)
      bottom = sp->base->Lines - 1;
   if (_clip_parinfo(ClipMachineMemory, 4) != NUMERIC_type_of_ClipVarType)
      right = sp->base->Columns - 1;

   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);

   if (!vert && !hor)
   {
      if (!fs)
	 clear_Screen(ClipMachineMemory->screen);

      for (i = top; i <= bottom; ++i)
      {
	 sp->touched[i] = 1;
	 for (j = left; j <= right; ++j)
	 {
	    sp->chars[i][j] = fill;
	    sp->colors[i][j] = attr;
	    sp->attrs[i][j] = 0;
	 }
      }

      return 0;
   }

   if (vert)
   {
      char *ls = 0, *rs = 0;

      int ll, rl;

      if ((right - left) < (sp->base->Columns - /* / */ 2))
      {
	 for (i = top; i <= bottom; ++i)
	    sp->touched[i] = 1;

	 if (vert > 0)
	 {
	    for (i = left; i <= right; ++i)
	    {
	       for (j = top; j <= bottom - vert; ++j)
	       {
		  sp->chars[j][i] = sp->chars[j + vert][i];
		  sp->colors[j][i] = sp->colors[j + vert][i];
		  sp->attrs[j][i] = sp->attrs[j + vert][i];
	       }
	       if (vert > bottom)
		  vert = bottom;
	       for (j = bottom - vert + 1; j <= bottom; ++j)
	       {
		  sp->chars[j][i] = fill;
		  sp->colors[j][i] = attr;
		  sp->attrs[j][i] = 0;
	       }
	    }
	 }
	 else
	 {
	    for (i = left; i <= right; ++i)
	    {
	       for (j = bottom; j >= top - vert; --j)
	       {
		  sp->chars[j][i] = sp->chars[j + vert][i];
		  sp->colors[j][i] = sp->colors[j + vert][i];
		  sp->attrs[j][i] = sp->attrs[j + vert][i];
	       }
	       if ((0 - vert) > top)
		  vert = 0 - top;
	       for (j = top - vert - 1; j >= top; --j)
	       {
		  sp->chars[j][i] = fill;
		  sp->colors[j][i] = attr;
		  sp->attrs[j][i] = 0;
	       }
	    }
	 }
      }
      else
      {
	 if (left > 0)
	    ls = save_region(ClipMachineMemory, 0, top, 0, bottom, left - 1, &ll);
	 if (right < sp->base->Columns - 1)
	    rs = save_region(ClipMachineMemory, 0, top, right + 1, bottom, sp->base->Columns - 1, &rl);

	 scrollw_Screen(sp, top, left, bottom, right, vert, attr);

	 if (ls)
	    rest_region(ClipMachineMemory, top, 0, bottom, left - 1, ls, ll);
	 if (rs)
	    rest_region(ClipMachineMemory, top, right + 1, bottom, sp->base->Columns - 1, rs, rl);
      }
   }

   if (hor > 0)
   {
      for (i = top; i <= bottom; ++i)
      {
	 sp->touched[i] = 1;
	 for (j = left; j <= right - hor; ++j)
	 {
	    sp->chars[i][j] = sp->chars[i][j + hor];
	    sp->colors[i][j] = sp->colors[i][j + hor];
	    sp->attrs[i][j] = sp->attrs[i][j + hor];
	 }
	 for (j = right - hor + 1; j <= right; ++j)
	 {
	    sp->chars[i][j] = fill;
	    sp->colors[i][j] = attr;
	    sp->attrs[i][j] = attr;
	 }
      }
   }
   else if (hor < 0)
   {
      for (i = top; i <= bottom; ++i)
      {
	 sp->touched[i] = 1;
	 for (j = right; j >= left - hor; --j)
	 {
	    sp->chars[i][j] = sp->chars[i][j + hor];
	    sp->colors[i][j] = sp->colors[i][j + hor];
	    sp->attrs[i][j] = sp->attrs[i][j + hor];
	 }
	 for (j = left - hor - 1; j >= left; --j)
	 {
	    sp->chars[i][j] = fill;
	    sp->colors[i][j] = attr;
	    sp->attrs[i][j] = 0;
	 }
      }
   }

   sync_mp(ClipMachineMemory);

   return 0;
}

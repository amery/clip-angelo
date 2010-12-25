static void
termcap_set_color(ScreenData * dp, int color)
{
   int fg, bg;

   /*color &= 0xff; */

   if (dp->oldcolor == color)
      return;

   if (dp->lineDrawMode)
   {
      if (dp->termcap_GE)
	 termcap_put_raw_str(dp, dp->termcap_GE);
      else if (dp->termcap_AE)
	 termcap_put_raw_str(dp, dp->termcap_AE);
      dp->lineDrawMode = 0;
   }

   if (dp->termcap_Visuals & VisualColors)
   {
      termcap_set_fgbg(dp, color & 0xf, (color >> 4) & 0xf);
      dp->oldcolor = color;
      return;
   }

   /* Set dim/bold/inverse. */
   fg = color & 0xf;
   bg = color & 0xf0;
   switch (dp->termcap_Visuals & (VisualBold | VisualBlink | VisualDim | VisualInverse))
   {
   case VisualBold | VisualDim | VisualInverse:
      if (bg)
      {
	 if (fg > 7)
	    goto bold_inverse;
	 else if (fg < 7)
	    goto dim_inverse;
	 else
	    goto inverse;
      }
      if (fg > 7)
	 goto bold;
      if (fg < 7)
	 goto dim;
      goto normal;
   case VisualDim | VisualInverse:
      if (!bg)
      {
	 if (fg >= 7)
	    goto normal;
	 else
	    goto dim;
      }
      if (fg >= 7)
	 goto inverse;
    dim_inverse:
      termcap_put_raw_str(dp, dp->termcap_MH);
      termcap_put_raw_str(dp, dp->termcap_SO);
      break;
   case VisualBold | VisualInverse:
      if (!bg)
      {
	 if (fg <= 7)
	    goto normal;
	 else
	    goto bold;
      }
      if (fg <= 7)
	 goto inverse;
    bold_inverse:
      termcap_put_raw_str(dp, dp->termcap_MD);
      termcap_put_raw_str(dp, dp->termcap_SO);
      break;
   case VisualBold | VisualDim:
      if (fg > 7)
	 goto bold;
      if (fg < 7)
	 goto dim;
      if (bg & 8)
	 goto blink;
      goto normal;
   case VisualInverse:
      if (!bg)
	 goto normal;
    inverse:
      termcap_put_raw_str(dp, dp->termcap_ME);
      termcap_put_raw_str(dp, dp->termcap_SO);
      break;
   case VisualBold:
      if (fg <= 7)
	 goto normal;
    bold:
      termcap_put_raw_str(dp, dp->termcap_ME ? dp->termcap_ME : dp->termcap_SE);
      termcap_put_raw_str(dp, dp->termcap_MD);
      break;
   case VisualBlink:
      if (bg <= 7)
	 goto normal;
    blink:
      termcap_put_raw_str(dp, dp->termcap_ME ? dp->termcap_ME : dp->termcap_SE);
      termcap_put_raw_str(dp, dp->termcap_mb);
      break;
   case VisualDim:
      if (fg >= 7)
	 goto normal;
    dim:
      termcap_put_raw_str(dp, dp->termcap_ME ? dp->termcap_ME : dp->termcap_SE);
      termcap_put_raw_str(dp, dp->termcap_MH);
      break;
   case 0:
    normal:
      termcap_put_raw_str(dp, dp->termcap_ME ? dp->termcap_ME : dp->termcap_SE);
      break;
   }
   dp->oldcolor = color;
}

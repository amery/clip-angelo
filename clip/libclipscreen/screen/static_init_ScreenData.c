static void
init_ScreenData(ScreenData * dp)
{
   char *bools = dp->terminfo.bools;

   int *nums = dp->terminfo.nums;

   char *s;

   dp->termcap_TI = get_str(dp, NO_smcup);
   dp->termcap_TE = get_str(dp, NO_rmcup);

   dp->termcap_KS = get_str(dp, NO_smkx);
   dp->termcap_KE = get_str(dp, NO_rmkx);

   dp->termcap_CM = get_str(dp, NO_cup);

   dp->termcap_GS = get_str(dp, NO_smacs);
   dp->termcap_GE = get_str(dp, NO_rmacs);
   dp->termcap_AS = get_str(dp, NO_smacs);
   dp->termcap_AE = get_str(dp, NO_rmacs);

   dp->termcap_G1 = get_str(dp, NO_g1);
   dp->termcap_G2 = get_str(dp, NO_g2);
   dp->termcap_AC = get_key(dp, NO_acsc);

   dp->termcap_AL = get_str(dp, NO_il1);
   dp->termcap_DL = get_str(dp, NO_dl1);

   dp->termcap_SF = get_str(dp, NO_indn);
   dp->termcap_SR = get_str(dp, NO_rin);
   dp->termcap_CS = get_str(dp, NO_csr);

   dp->termcap_CL = get_str(dp, NO_clear);
   dp->termcap_CE = get_str(dp, NO_el);

   dp->termcap_TS = get_str(dp, NO_tsl);
   dp->termcap_FS = get_str(dp, NO_fsl);

   dp->termcap_NF = nums[NO_Nf];
   dp->termcap_NB = nums[NO_Nb];

   dp->termcap_lines = nums[NO_lines];
   dp->termcap_columns = nums[NO_cols];

   dp->termcap_CF = get_str(dp, NO_Cf);
   dp->termcap_CB = get_str(dp, NO_Cb);

   dp->termcap_MF = get_str(dp, NO_Mf);
   dp->termcap_MB = get_str(dp, NO_Mb);

   dp->termcap_MD = get_str(dp, NO_bold);
   dp->termcap_mb = get_str(dp, NO_blink);
   dp->termcap_MH = get_str(dp, NO_dim);
   dp->termcap_MR = get_str(dp, NO_rev);
   dp->termcap_ME = get_str(dp, NO_sgr0);

   dp->termcap_SE = get_str(dp, NO_rmso);
   dp->termcap_SO = get_str(dp, NO_smso);

   dp->termcap_AF = get_str(dp, NO_setaf);
   dp->termcap_AB = get_str(dp, NO_setab);

   dp->termcap_SETF = get_str(dp, NO_setf);
   dp->termcap_SETB = get_str(dp, NO_setb);

   dp->termcap_PO = get_str(dp, NO_mc5);
   dp->termcap_POO = get_str(dp, NO_mc5p);
   dp->termcap_PF = get_str(dp, NO_mc4);

   dp->termcap_Km = get_str(dp, NO_kmous);
   dp->termcap_Mi = get_str(dp, NO_minfo);

   dp->termcap_Ct = get_str(dp, NO_Ct);
   dp->termcap_C2 = bools[NO_C2];
   dp->termcap_MS = bools[NO_msgr];

   dp->termcap_xo = bools[NO_xon];

   s = get_str(dp, NO_xonc);
   if (s)
      dp->termcap_XN = s[0];
   s = get_str(dp, NO_xoffc);
   if (s)
      dp->termcap_XF = s[0];

   dp->termcap_HS = bools[NO_hs];

   dp->termcap_BL = get_str(dp, NO_bel);

   dp->termcap_IS = get_str(dp, NO_is2);
   dp->termcap_EA = get_str(dp, NO_enacs);

   dp->termcap_VS = get_str(dp, NO_cvvis);
   dp->termcap_VE = get_str(dp, NO_cnorm);
   dp->termcap_VI = get_str(dp, NO_civis);
#if 0
   printf("%s\n", dp->termcap_VS);
   printf("%s\n", dp->termcap_VE);
   printf("%s\n", dp->termcap_VI);
   sleep(10);
#endif

   dp->next_key = 0;
   dp->outptr = dp->outbuf;
   dp->lineDrawMode = 0;
   dp->boldMode = 0;
   dp->blinkMode = 0;
   dp->useBlinkAttr = 1;
   dp->useBoldAttr = 1;
   dp->oldcolor = 0;
   dp->oldcursor = 0;
   dp->matchpos = 0;
   dp->matchno = 0;
   dp->lomatch = 255;
   dp->himatch = 0;
   dp->national_mode = 0;
   dp->has_meta1 = 0;
   dp->has_meta2 = 0;
   dp->prev_national_mode = 0;
   dp->prev_has_meta1 = 0;
   dp->prev_has_meta2 = 0;
   dp->key_count = 0;
   dp->scrool = 0;
   dp->rscrool = 0;
   dp->pg_mode = 1;
   dp->utf8_mode = 0;

   strncpy(dp->meta_key, "\033", sizeof(dp->meta_key));
   strncpy(dp->meta1_key, "\012", sizeof(dp->meta1_key));
   strncpy(dp->meta2_key, "\013", sizeof(dp->meta2_key));
#ifdef USE_NATIONAL_KEY
   strncpy(dp->national_key, "\016", sizeof(dp->national_key));
#else
   memset(dp->national_key, 0, sizeof(dp->national_key));
#endif

   memcpy(dp->nationalTable, koi8_nationalTable, 128);

   initKey(dp);
}

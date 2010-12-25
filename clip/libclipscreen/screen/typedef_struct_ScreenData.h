typedef struct
{
   ScreenBase *base;

   int termcap_Visuals;
   int fd;
   Screen *realScreen;

   char *termcap_TI, *termcap_TE;
   char *termcap_KS, *termcap_KE;
   char *termcap_CM;

   char *termcap_GS, *termcap_GE;
   char *termcap_G1, *termcap_G2;
   char *termcap_AS, *termcap_AE;

   char *termcap_AC;
   char *termcap_AL, *termcap_DL;
   char *termcap_SF, *termcap_SR, *termcap_CS;
   char *termcap_CL, *termcap_CE;
   char *termcap_TS, *termcap_FS;
   int termcap_NF, termcap_NB;

   char *termcap_CF, *termcap_CB;
   char *termcap_MF, *termcap_MB;
   char *termcap_MD, *termcap_mb, *termcap_MH, *termcap_MR, *termcap_ME;
   char *termcap_SE, *termcap_SO;

   char *termcap_AF, *termcap_AB;
   char *termcap_SETF, *termcap_SETB;

   char *termcap_PO, *termcap_PF, *termcap_POO;
   char *termcap_Km, *termcap_Mi;
   char *termcap_Ct;

   char termcap_C2;
   char termcap_MS;
   int termcap_HS;

   char termcap_xo;
   int termcap_XN, termcap_XF;

   char *termcap_BL;

   char *termcap_IS;
   char *termcap_EA;

   char *termcap_VE;		/* normal visible */
   char *termcap_VS;		/* very visible */
   char *termcap_VI;		/* invisible */

   int termcap_lines;
   int termcap_columns;

   unsigned long next_key;

   unsigned char outbuf[1024];
   unsigned char *outptr;

   int lineDrawMode;
   int boldMode;
   int blinkMode;
   int useBlinkAttr, useBoldAttr;
   int oldcolor;
   int oldcursor;
   int matchpos, matchno, lomatch, himatch;
   int national_mode, has_meta1, has_meta2;
   int prev_national_mode, prev_has_meta1, prev_has_meta2;
   int key_count;

   short ctab[16], btab[16];
   int scrool, rscrool;

   char meta_key[16];
   char meta1_key[16];
   char meta2_key[16];
   char national_key[16];

   unsigned char nationalTable[128];
   unsigned char inputTable[256];
   unsigned char outputTable[256];
   unsigned char pgTable[256];
   unsigned long uniTable[256];

   struct termios start_mode;
   struct termios work_mode;

   int pg_mode;
   int utf8_mode;

   Keytab keytab[KEYTAB_SIZE];

   Terminfo terminfo;
}
ScreenData;

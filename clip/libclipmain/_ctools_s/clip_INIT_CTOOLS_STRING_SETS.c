
int
clip_INIT_CTOOLS_STRING_SETS(ClipMachine * ClipMachineMemory)
{
   char *status, *setref, *atlike, *atsep, *delims, *dayweek, *numcount;

   int *charsort, *clearb, *cleara;

   clearb = calloc(sizeof(int), 1);
   cleara = calloc(sizeof(int), 1);

   *((char *) clearb) = ' ';
   _clip_store_item(ClipMachineMemory, HASH_setclearb, clearb);
   *cleara = 7;
   _clip_store_item(ClipMachineMemory, HASH_setcleara, cleara);

   numcount = calloc(sizeof(long), 1);

   _clip_store_item(ClipMachineMemory, HASH_numcount, numcount);

   status = calloc(2, 1);
   status[0] = 't';
   _clip_store_item(ClipMachineMemory, HASH_csetatmupa, status);

   charsort = calloc(4, sizeof(int));

   _clip_store_item(ClipMachineMemory, HASH_charsort_param, charsort);

   setref = calloc(2, 1);
   setref[0] = 't';
   _clip_store_item(ClipMachineMemory, HASH_csetref, setref);

   atlike = calloc(2, 1);
   atlike[1] = '?';
   _clip_store_item(ClipMachineMemory, HASH_setatlike, atlike);

   /* 5th int value and 2 strins with len==1 plus 0 character */
   /*
      1 int value for current position tokennext(),
      2 int value for tokenat(), tokenat(.t.),
      2 string value tokensep()+chr(0), tokensep(.t.)+chr(0),
      int value string length for first parameter tokeninit(),
      int value for 3th parameter tokeninit()
    */
   atsep = calloc(5 * sizeof(int) + 4, 1);

   _clip_store_item(ClipMachineMemory, HASH_token_atsep, atsep);
   /* string for 1th parameter tokeninit() */
   _clip_store_item(ClipMachineMemory, HASH_token_string, NULL);
   /* 256 flags of delimiters characters for 2th parameter tokeninit() */
   delims = calloc(256, 1);
   _clip_store_item(ClipMachineMemory, HASH_token_delimiters, delims);
   dayweek = calloc(2, 1);
   dayweek[0] = '1';
   _clip_store_item(ClipMachineMemory, HASH_first_day_week, dayweek);
   return 0;
}

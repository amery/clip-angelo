void
init_Terminfo(Terminfo * info)
{
   int i;

   info->name = 0;

   for (i = 0; i < MAX_BOOL; ++i)
      info->bools[i] = 0;
   for (i = 0; i < MAX_NUM; ++i)
      info->nums[i] = -1;
   for (i = 0; i < MAX_STR; ++i)
      info->strings[i] = -1;
   for (i = 0; i < MAX_KEY; ++i)
      info->keys[i] = -1;

   info->buf = 0;
   info->bufsize = 0;
   info->bufpos = 0;
}

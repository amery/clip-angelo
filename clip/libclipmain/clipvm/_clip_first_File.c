int
_clip_first_File(struct ClipFile *file, ClipCodeVar * dest, long *hash)
{
   long funcOffs = GETLONG(M_OFFS(file->body_of_ClipFile, 7, 4));

   char *modbeg = M_OFFS(file->body_of_ClipFile, 2, 0);

   int pubnum = GETLONG(M_OFFS(file->body_of_ClipFile, 4, 2));

   /*int initnum = GETLONG( M_OFFS(file->body_of_ClipFile, 7, 5)); */
   /*int exitnum = GETLONG( M_OFFS(file->body_of_ClipFile, 7, 6)); */
   int num;

   num = pubnum;

   file->pos_of_ClipFile = (long *) (modbeg + funcOffs);
   file->end_of_ClipFile = file->pos_of_ClipFile + 2 * num;
   if (num == 0)
      return 0;
   make_func(file, dest, hash);
   file->pos_of_ClipFile += 2;

   return 1;
}

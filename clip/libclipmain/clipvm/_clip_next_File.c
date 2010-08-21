int
_clip_next_File(struct ClipFile *file, ClipCodeVar * dest, long *hash)
{
   if (file->pos_of_ClipFile == file->end_of_ClipFile)
      return 0;
   make_func(file, dest, hash);
   file->pos_of_ClipFile += 2;
   return 1;
}

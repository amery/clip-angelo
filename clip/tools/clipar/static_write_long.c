static void
write_long(FILE * file, long s)
{
   if (fwrite(&s, sizeof(s), 1, file) != 1)
      fwerr(file);
}

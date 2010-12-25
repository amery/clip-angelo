static void
write_short(FILE * file, short s)
{
   if (fwrite(&s, sizeof(s), 1, file) != 1)
      fwerr(file);
}

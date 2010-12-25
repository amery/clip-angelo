static void
write_str(FILE * file, char *str)
{
   int len = strlen(str);

   if (fwrite(str, len + 1, 1, file) != 1)
      fwerr(file);
}

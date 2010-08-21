static void
write_buf(FILE * file, char *buf, int len)
{
   if (fwrite(buf, len, 1, file) != 1)
      fwerr(file);
}

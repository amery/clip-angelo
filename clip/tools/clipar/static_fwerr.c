static void
fwerr(FILE * file)
{
   fclose(file);
   error("write error: %s", strerror(errno));
}

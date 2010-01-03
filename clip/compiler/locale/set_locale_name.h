int
set_locale_name(char *name)
{
   if (out)
    {
       fclose(out);
       out = 0;
    }
   file_name = strdup(name);
   return 0;
}

int
resume_locale(void)
{
   if (out)
   {
      fclose(out);
      out = 0;
   }
   return 0;
}

static void
cleanup(void)
{
   if (fin)
   {
      fclose(fin);
      fin = 0;
   }
   if (fout)
   {
      fclose(fout);
      fout = 0;
   }
   if (in_path[0])
   {
      remove(in_path);
      in_path[0] = 0;
   }
   if (out_path[0])
   {
      remove(out_path);
      out_path[0] = 0;
   }
   if (tpid != 0)
   {
      int status;

      kill(tpid, SIGTERM);
      waitpid(tpid, &status, 0);
      tpid = 0;
   }
}

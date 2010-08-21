static void
cleanup(int code)
{
   if (fin)
      fclose(fin);
   if (fout >= 0)
      close(fout);
   if (in_path[0])
      remove(in_path);
   if (out_path[0])
      remove(out_path);
#ifndef OS_MINGW
   if (tpid != 0)
   {
      int status;

      kill(tpid, SIGTERM);
      waitpid(tpid, &status, 0);
   }
#endif

   tcsetattr(0, TCSANOW, &oterm);
   write(1, "\n", 1);

   exit(code);
}

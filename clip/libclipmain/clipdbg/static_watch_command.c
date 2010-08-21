static int
watch_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   int i;

   if (argc < 1)
   {
		for (i = 0; i < watchs.count_of_Coll; i++)
			fprintf(_clip_dbg_out, "watch %d: %s\n", i, (char *) watchs.items_of_Coll[i]);
   }
   else if (argc == 1 && !strcmp(argv[0], "-clear"))
   {
      freeAll_Coll(&watchs);
      freeAll_Coll(&swatchs);
   }
   else
   {
      OutBuf buf;

      init_Buf(&buf);

      for (i = 0; i < argc; i++)
      {
	 putBuf_Buf(&buf, argv[i], strlen(argv[i]));
	 putBuf_Buf(&buf, " ", 1);
      }
      putByte_Buf(&buf, 0);

      if (search_Coll(&swatchs, buf.buf_of_OutBuf, 0))
      {
	 fprintf(_clip_dbg_out, "watch already exist: %s\n", buf.buf_of_OutBuf);
	 destroy_Buf(&buf);
      }
      else
      {
	 insert_Coll(&swatchs, buf.buf_of_OutBuf);
	 append_Coll(&watchs, buf.buf_of_OutBuf);
      }
   }

   return 0;
}

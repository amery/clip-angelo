static int
ls_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   ClipFrame *fp = get_frame(ClipMachineMemory);

   int       nstatics = fp->ClipVarDef_statics_of_ClipFrame ? fp->ClipVarDef_statics_of_ClipFrame[0].name_of_ClipVarDef : 0;

   int       nlocals =
    fp->ClipVarFrame_localvars_of_ClipFrame ? fp->ClipVarFrame_localvars_of_ClipFrame->size_of_ClipVarFrame : 0;
   int       nprivates = HashTable_len(ClipMachineMemory->privates);

   int       npublics = HashTable_len(ClipMachineMemory->publics);

   int       show_local = 0, show_static = 0, show_private = 0, show_public = 0;

   int       i, r;

   char      buf[96];

   for (i = 0; i < argc; i++)
    {
       if (!strcasecmp(argv[i], "local"))
	  show_local = 1;
       else if (!strcasecmp(argv[i], "static"))
	  show_static = 1;
       else if (!strcasecmp(argv[i], "private"))
	  show_private = 1;
       else if (!strcasecmp(argv[i], "public"))
	  show_public = 1;
    }

   if (!show_local && !show_static && !show_private && !show_public)
    {
       show_local = 1;
       show_static = 1;
       show_private = 1;
       show_public = 1;
    }

   if (show_local)
    {
       fprintf(_clip_dbg_out, "%d locals:", nlocals);
       if (fp->ClipVarFrame_localvars_of_ClipFrame)
	{
	   char     *s = (char *) fp->ClipVarFrame_localvars_of_ClipFrame->names_of_ClipVarFrame;

	   for (i = 0; i < nlocals; i++)
	    {
	       fprintf(_clip_dbg_out, " %s", s);
	       s += strlen(s) + 1;
	    }
	}
       fprintf(_clip_dbg_out, "\n");
    }
   if (show_static)
    {
       fprintf(_clip_dbg_out, "%d statics:", nstatics);
       for (i = 0; i < nstatics; i++)
	{
	   ClipVarDef *dp = fp->ClipVarDef_statics_of_ClipFrame + i + 1;

	   _clip_hash_name(ClipMachineMemory, dp->name_of_ClipVarDef, buf, sizeof(buf));
	   fprintf(_clip_dbg_out, " %s", buf);
	}
       fprintf(_clip_dbg_out, "\n");
    }
   if (show_private)
    {
       fprintf(_clip_dbg_out, "%d privates:", nprivates);
       for (r = HashTable_first(ClipMachineMemory->privates); r; r = HashTable_next(ClipMachineMemory->privates))
	{
	   VarEntry *vp = (VarEntry *) HashTable_current(ClipMachineMemory->privates);

	   _clip_hash_name(ClipMachineMemory, vp->hash_of_VarEntry, buf, sizeof(buf));
	   fprintf(_clip_dbg_out, " %s", buf);

	   for (vp = vp->VarEntry_next_of_VarEntry; vp; vp = vp->VarEntry_next_of_VarEntry)
	    {
	       _clip_hash_name(ClipMachineMemory, vp->hash_of_VarEntry, buf, sizeof(buf));
	       fprintf(_clip_dbg_out, " %s", buf);
	    }
	}
       fprintf(_clip_dbg_out, "\n");
    }
   if (show_public)
    {
       fprintf(_clip_dbg_out, "%d publics:", npublics);
       for (r = HashTable_first(ClipMachineMemory->publics); r; r = HashTable_next(ClipMachineMemory->publics))
	{
	   VarEntry *vp = (VarEntry *) HashTable_current(ClipMachineMemory->publics);

	   _clip_hash_name(ClipMachineMemory, vp->hash_of_VarEntry, buf, sizeof(buf));
	   fprintf(_clip_dbg_out, " %s", buf);

	   for (vp = vp->VarEntry_next_of_VarEntry; vp; vp = vp->VarEntry_next_of_VarEntry)
	    {
	       _clip_hash_name(ClipMachineMemory, vp->hash_of_VarEntry, buf, sizeof(buf));
	       fprintf(_clip_dbg_out, " %s", buf);
	    }
	}
       fprintf(_clip_dbg_out, "\n");
    }

   return 0;
}

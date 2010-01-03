static int
print_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   int       show_local = 0, show_static = 0, show_private = 0, show_public =
    0, show_memvar = 0, show_field = 0, show_first = 0;
   int       i, cnt, binary = 0;


   for (i = 0; i < argc; i++)
    {
       if (!strcasecmp(argv[i], "-local"))
	  show_local = 1;
       else if (!strcasecmp(argv[i], "-static"))
	  show_static = 1;
       else if (!strcasecmp(argv[i], "-private"))
	  show_private = 1;
       else if (!strcasecmp(argv[i], "-public"))
	  show_public = 1;
       else if (!strcasecmp(argv[i], "-memvar"))
	  show_memvar = 1;
       else if (!strcasecmp(argv[i], "-field"))
	  show_field = 1;
       else if (!strcasecmp(argv[i], "-binary"))
	  binary = 1;
       else
	  break;
    }

   if (!show_local && !show_static && !show_private && !show_public && !show_memvar && !show_field)
    {
       show_local = 1;
       show_static = 1;
       show_field = 1;
       show_public = 1;
       show_private = 1;
       show_first = 1;
    }

   for (cnt = 0; i < argc; i++, cnt++)
    {
       long      hash;

       long     *dim = 0;

       int       ndim = 0;

       int       found = 0;

       char     *name = argv[i];

       parse_name(name, &hash, &dim, &ndim);

       if (show_local)
	{
	   int       j = 0;

	   ClipFrame *fp = get_frame(ClipMachineMemory);

	   int       nlocals =
	    fp->ClipVarFrame_localvars_of_ClipFrame ? fp->ClipVarFrame_localvars_of_ClipFrame->size_of_ClipVarFrame : 0;
	   char     *s;

	   if (fp->ClipVarFrame_localvars_of_ClipFrame)
	    {
	       s = (char *) fp->ClipVarFrame_localvars_of_ClipFrame->names_of_ClipVarFrame;
	       for (j = 0; j < nlocals; j++)
		{
		   if (j)
		      s += strlen(s) + 1;

		   if (_clip_casehashstr(s) == hash)
		    {
		       fprintf(_clip_dbg_out, "local; %s; ", s);
		       print_var(ClipMachineMemory,
				 fp->ClipVarFrame_localvars_of_ClipFrame->ClipVar_vars_of_ClipVarFrame + nlocals - 1 - j, dim,
				 ndim, binary);
		       found++;
		       if (show_first)
			  goto cont;
		       else
			  break;
		    }
		}
	    }
	   if (j == nlocals)
	      if (!show_first)
		 fprintf(_clip_dbg_out, "No local variable '%s'", name);
	}

       if (show_static)
	{
	   int       j;

	   ClipFrame *fp = get_frame(ClipMachineMemory);

	   int       nstatics =
	    fp->ClipVarDef_statics_of_ClipFrame ? fp->ClipVarDef_statics_of_ClipFrame[0].name_of_ClipVarDef : 0;

	   for (j = 0; j < nstatics; j++)
	    {
	       ClipVarDef *dp = fp->ClipVarDef_statics_of_ClipFrame + j + 1;

	       if (dp->name_of_ClipVarDef == hash)
		{
		   fprintf(_clip_dbg_out, "static; %s; ", name);
		   print_var(ClipMachineMemory, dp->ClipVar_vp_of_ClipVarDef, dim, ndim, binary);
		   found++;
		   if (show_first)
		      goto cont;
		   else
		      break;
		}
	    }
	   if (j == nstatics)
	      if (!show_first)
		 fprintf(_clip_dbg_out, "No static variable '%s'", name);
	}
       if (show_private)
	{
	   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->privates, hash);

	   if (vp)
	    {
	       fprintf(_clip_dbg_out, "private; %s; ", name);
	       print_var(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry, dim, ndim, binary);
	       found++;
	       if (show_first)
		  goto cont;
	    }
	   else
	    {
	       if (!show_first)
		  fprintf(_clip_dbg_out, "No private variable '%s'", name);
	    }
	}
       if (show_public)
	{
	   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->publics, hash);

	   if (vp)
	    {
	       fprintf(_clip_dbg_out, "public; %s; ", name);
	       print_var(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry, dim, ndim, binary);
	       found++;
	       if (show_first)
		  goto cont;
	    }
	   else
	    {
	       if (!show_first)
		  fprintf(_clip_dbg_out, "No public variable '%s'", name);
	    }
	}
       if (show_memvar)
	{
	   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->privates, hash);

	   if (!vp)
	      vp = (VarEntry *) HashTable_fetch(ClipMachineMemory->publics, hash);

	   if (vp)
	    {
	       fprintf(_clip_dbg_out, "memvar; %s; ", name);
	       print_var(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry, dim, ndim, binary);
	       found++;
	       if (show_first)
		  goto cont;
	    }
	   else
	    {
	       if (!show_first)
		  fprintf(_clip_dbg_out, "No memvar variable '%s'", name);
	    }
	}
       if (!found && show_first)
	  fprintf(_clip_dbg_out, "No variable '%s'", name);
     cont:
       fprintf(_clip_dbg_out, "\n");
       free(dim);
    }

   if (!cnt)
      fprintf(_clip_dbg_out, "\n");
   return 0;
}

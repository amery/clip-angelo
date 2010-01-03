/*
	return 1 if memvar, 2 if field name, 3 if array/map selector
		, 0 if unspecified (field/memvar)
	*fnamep name of field or memvar
	*anamep alias (if exist)
	*dimp - realloc'able array index vector
	*ndimp - it's length
*/
CLIP_DLLEXPORT int
_clip_parse_name(ClipMachine * ClipMachineMemory, char *name, int l, char **anamep,
		 int *alp, char **fnamep, int *flp, long **dimp, int *ndimp)
{
   char     *s, *e, *p, *ep;

   int       ret = 0;

   char     *aname = 0, *fname = 0;

   int       al = 0, fl = 0;

   long      val;

   int       ll;


   for (e = name + l; e > name && isspace(e[-1]); e--, l--)
      ;
   fname = name;
   fl = l;

   for (s = name; s < e; s++)
    {
       int       el, c = 0;

       for (; s < e && isspace(*s); s++)
	  ;
       p = s;
       for (ep = p; ep < e && is_id((c = *ep)); ep++)
	  ;
       if (ep == e)
	  break;
       if (c != ' ' && c != '\t' && c != '-' && c != '[' && c != ']' && c != ':')
	  continue;

       el = ep - p;
       if ((el == 5 && !strncasecmp(p, "field", 5))
	   || (el == 6 && !strncasecmp(p, "_field", 6)) || (el == 1 && !strncasecmp(p, "f", 1)))
	{
	   for (; ep < e && (c = *ep) != '-'; ep++)
	      ;
	   if (c == '-' && ep < e && ep[1] == '>')
	    {
	       for (p = ep + 2; (p < e && *p == ' ') || *p == '\n'; p++)
		  ;
	       fname = p;
	       fl = e - p;
	       ret = 2;
	    }
	}
       else if ((el == 6 && !strncasecmp(p, "memvar", 6)) || (el == 1 && !strncasecmp(p, "m", 1)))
	{
	   for (; ep < e && (c = *ep) != '-'; ep++)
	      ;
	   if (c == '-' && ep < e && ep[1] == '>')
	    {
	       for (p = ep + 2; (p < e && *p == ' ') || *p == '\n'; p++)
		  ;
	       fname = p;
	       fl = e - p;
	       ret = 1;
	    }
	}
       else if (!strncasecmp(ep, "::", 2))
	{
	   fname = "__SELF__";
	   fl = 8;
	   ep += 2;
	}

       if (!ret)
	{
	   fname = p;
	   fl = ep - p;
	   ll = strspn(ep, " \t\n\r");
	   ep += ll;
	   ll = strspn(ep, "[]:->");
	   if (ll == 2 && ep[0] == '-' && ep[1] == '>')
	    {
	       al = el;
	       aname = p;
	       p = ep + 2;
	       ll = strspn(p, " \t\n\r");
	       p += ll;
	       fname = p;
	       fl = strcspn(p, " \t\r\n[]:-");
	       ret = 2;
	    }
	}

       if (!ret)
	{
	   while (ep < e)
	    {
	       ll = strspn(ep, " \t\n\r[]:");
	       ep += ll;
	       ll = strcspn(ep, " \t\n\r[]:");
	       if (!ll)
		  break;

	       ret = 3;

	       if (dimp && ndimp)
		  *dimp = (long *) realloc(*dimp, sizeof(long) * (1 + *ndimp));

	       if (is_dig(ep, ll))
		  val = atoi(ep) - 1;
	       else
		{
		   ClipVar  *var;

		   var = _clip_ref_memvar_noadd(ClipMachineMemory, _clip_casehashbytes(0, ep, ll));
		   if (var)
		      val = _clip_long(var) - 1;
		   else
		      val = _clip_casehashbytes(0, ep, ll);
		}
	       if (dimp && ndimp)
		{
		   (*dimp)[*ndimp] = val;
		   (*ndimp)++;
		}
	       ep += ll;
	    }
	}
    }

   if (anamep && alp)
    {
       *anamep = aname;
       *alp = al;
    }
   if (fnamep && flp)
    {
       *fnamep = fname;
       *flp = fl;
    }

   return ret;
}

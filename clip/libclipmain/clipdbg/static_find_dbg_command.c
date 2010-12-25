static DbgCommandBucket *
find_dbg_command(char *cmd)
{
   DbgCommandBucket *dcp, *dbeg, *dend, *fp = 0;

   for (dcp = dbg_buckets; dcp->name; dcp++)
   {
      int l;

      int found = 0;

      if (toupper(dcp->name[0]) != toupper(cmd[0]))
	 continue;

      for (dbeg = dcp; dcp->name && toupper(dcp->name[0]) == toupper(cmd[0]); dcp++)
      {
	 if (!strcasecmp(dcp->name, cmd))
	    return dcp;
      }

      dend = dcp;
      l = strlen(cmd);
      for (dcp = dbeg; dcp < dend; dcp++)
      {
	 if (!strncasecmp(dcp->name, cmd, l))
	 {
	    found++;
	    fp = dcp;
	 }
      }
      if (found == 1)
	 return fp;

      return 0;
   }

   return 0;
}

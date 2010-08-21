static int
entry_handler(void *par, char *filename, int mline, char *txt)
{
   EntryData *epar = (EntryData *) par;

   PoEntry e, *ep, *pp;

   int ind;

   e.msg = txt;

	if (!search_Coll(&epar->ids_of_EntryData, &e, &ind))
   {
      ep = new_PoEntry(txt, filename, mline, 0);
		atInsert_Coll(&epar->ids_of_EntryData, ep, ind);
		append_Coll(&epar->uids_of_EntryData, ep);
      debug(2, "found entry: '%s' %s:%d", txt, filename, mline);
   }
   else
   {
		pp = (PoEntry *) epar->ids_of_EntryData.items_of_Coll[ind];
      ep = new_PoEntry(0, filename, mline, pp);
      debug(2, "found duplicated entry: '%s' %s:%d", txt, filename, mline);
   }

   return 0;
}

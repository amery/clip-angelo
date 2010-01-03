static void
ins_name(char *name)
{
   int       no;

   if (!name)
      return;
   if (!search_Coll(&curFile->Coll_names_of_File, name, &no))
      atInsert_Coll(&curFile->Coll_names_of_File, strdup(name), no);
}

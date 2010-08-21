static VarEntry *
new_VarEntry(long hash)
{
   NEWVAR(VarEntry, ret);
   ret->hash_of_VarEntry = hash;
   return ret;
}

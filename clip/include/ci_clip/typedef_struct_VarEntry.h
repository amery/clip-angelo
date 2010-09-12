typedef struct VarEntry
{
   long hash_of_VarEntry;	/* hash */
   ClipVar ClipVar_var_of_VarEntry;	/* v */
   struct VarEntry *VarEntry_next_of_VarEntry;	/* next */
}
VarEntry;

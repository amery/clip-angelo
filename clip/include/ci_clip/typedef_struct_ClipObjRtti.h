typedef struct ClipObjRtti
{
   const char *name_of_ClipObjRtti;
   long      hashId_of_ClipObjRtti;
   void      (*destroy_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
					struct ClipObjRtti * rtti_of_ClipObjRtti);
   void     *(*clone_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, union ClipVar * dest_of_ClipObjRtti,
				      union ClipVar * self_of_ClipObjRtti);
   int       (*print_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				      struct ClipObjRtti * rtti_of_ClipObjRtti, char **buf, int *buflen);
   int       (*parse_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				      struct ClipObjRtti * rtti_of_ClipObjRtti, char *buf_of_ClipObjRtti,
				      int buflen_of_ClipObjRtti);
   int       (*save_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				     struct ClipObjRtti * rtti_of_ClipObjRtti, int recsize, char ***vectp_of_ClipObjRtti,
				     int *num_of_ClipObjRtti);
   int       (*load_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void **obj_of_ClipObjRtti,
				     struct ClipObjRtti * rtti_of_ClipObjRtti, int recsize_of_ClipObjRtti,
				     char **vect_of_ClipObjRtti, int num_of_ClipObjRtti);
   int       (*call_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				     struct ClipObjRtti * rtti_of_ClipObjRtti, long hash_of_ClipObjRtti);
   int       (*get_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				    struct ClipObjRtti * rtti_of_ClipObjRtti, long hash_of_ClipObjRtti);
   int       (*set_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				    struct ClipObjRtti * rtti_of_ClipObjRtti, long hash_of_ClipObjRtti);
   long      (*hash_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				     struct ClipObjRtti * rtti_of_ClipObjRtti);
   int       (*cmp_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				    struct ClipObjRtti * rtti_of_ClipObjRtti, union ClipVar * robj_of_ClipObjRtti);
   int       (*fetch_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				      struct ClipObjRtti * rtti_of_ClipObjRtti, int dim_of_ClipObjRtti);
   int       (*store_of_ClipObjRtti) (struct ClipMachine * ClipMachineMemory, void *obj_of_ClipObjRtti,
				      struct ClipObjRtti * rtti_of_ClipObjRtti, int dim_of_ClipObjRtti);
   void     *data_of_ClipObjRtti;
}
ClipObjRtti;

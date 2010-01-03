typedef struct ClipRefVar
{
   ClipType  ClipType_type_of_ClipRefVar;	/* t */
   union ClipVar *ClipVar_of_ClipRefVar;	/* vp */
   ClipFieldDef *ClipFieldDef_of_ClipRefVar;	/* fp */
}
ClipRefVar;

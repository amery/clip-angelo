typedef struct ClipCodeVar
{
   ClipType  ClipType_type_of_ClipCodeVar;
   union
   {
      ClipFunction *ClipFunction_function_of_ClipCodeVar;
      ClipBlock *ClipBlock_block_of_ClipCodeVar;
   }
   u;
   struct ClipVarFrame *ClipVarFrame_uplocals_of_ClipCodeVar;
}
ClipCodeVar;

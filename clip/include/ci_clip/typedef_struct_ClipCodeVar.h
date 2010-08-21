typedef struct ClipCodeVar
{
   ClipType ClipType_type_of_ClipCodeVar;
   union
   {
      ClipFunction *ClipFunction_function_of_ClipCodeVar;
      ClipBlock *ClipBlock_block_of_ClipCodeVar;
   }
   union_of_ClipCodeVar;
   struct ClipVarFrame *ClipVarFrame_uplocals_of_ClipCodeVar;
}
ClipCodeVar;

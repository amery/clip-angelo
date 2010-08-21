int
_clip_colorSelect(ClipMachine * ClipMachineMemory)
{
   int ret;

   switch (ClipMachineMemory->colorSelect)
   {
   case 0:
      ret = ClipMachineMemory->attr.standard_of_ClipAttr;
      break;
   case 1:
      ret = ClipMachineMemory->attr.enhanced_of_ClipAttr;
      break;
   case 2:
      ret = ClipMachineMemory->attr.border_of_ClipAttr;
      break;
   case 3:
      ret = ClipMachineMemory->attr.background_of_ClipAttr;
      break;
   case 4:
      ret = ClipMachineMemory->attr.unselected_of_ClipAttr;
      break;
   case 5:
      ret = ClipMachineMemory->attr.u1_of_ClipAttr;
      break;
   case 6:
      ret = ClipMachineMemory->attr.u2_of_ClipAttr;
      break;
   case 7:
      ret = ClipMachineMemory->attr.u3_of_ClipAttr;
      break;
   case 8:
      ret = ClipMachineMemory->attr.u4_of_ClipAttr;
      break;
   case 9:
      ret = ClipMachineMemory->attr.u5_of_ClipAttr;
      break;
   default:
      ret = ClipMachineMemory->attr.standard_of_ClipAttr;
      break;
   }
   return ret;
}

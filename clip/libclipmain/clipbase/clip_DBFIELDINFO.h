int
clip_DBFIELDINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBFIELDINFO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       cmd = _clip_parni(ClipMachineMemory, 1);

   int       fno = _clip_parni(ClipMachineMemory, 2) - 1;

   int       er;


   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   switch (cmd)
    {
    case DBS_BLOB_LEN:
    case DBS_BLOB_OFFSET:
    case DBS_BLOB_POINTER:
    case DBS_BLOB_TYPE:
       {
/*			if(wa->rd->memo && wa->rd->fields[fno].type=='M')
				if(wa->rd->memo->vtbl->fieldinfo(ClipMachineMemory,wa->rd,wa->rd->memo,cmd,
					__PROC__)) return 1;*/
	  break;
       }
    case DBS_DEC:
       {
	  _clip_retni(ClipMachineMemory, wa->rd->fields[fno].dec);
	  break;
       }
    case DBS_LEN:
       {
	  _clip_retni(ClipMachineMemory, wa->rd->fields[fno].len);
	  break;
       }
    case DBS_NAME:
       {
	  _clip_retc(ClipMachineMemory, wa->rd->fields[fno].name);
	  break;
       }
    case DBS_TYPE:
       {
	  char      t[2];

	  t[0] = wa->rd->fields[fno].type;
	  t[1] = 0;
	  _clip_retc(ClipMachineMemory, t);
	  break;
       }
    }
   return 0;
 err:
   return er;
}

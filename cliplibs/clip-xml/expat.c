/*
    Copyright (C) 2005  ITK
    Author  : Kornilova E.V. <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

#include <string.h>
#include <errno.h>
#include <expat.h>

#include "ci_clip.h"
#include "ci_charset.h"

#include "ci_hashcode.h"
#include "ci_clip-expat.h"
#include "ci_clip-expat.ch"

static ClipVar _xml_list;
static ClipVar *xml_list = &_xml_list;

CLIP_DLLEXPORT C_parser *
_list_get_cparser(ClipMachine * ClipMachineMemory, void *pointer)
{
   double d;
   if (pointer && xml_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      if (_clip_mgetn(ClipMachineMemory, xml_list, (long) pointer, &d) == 0)
	 return (C_parser *) ((long) d);
   return NULL;
}

CLIP_DLLEXPORT void
_list_put_cparser(ClipMachine * ClipMachineMemory, void *pointer, C_parser * cpar)
{
   if (xml_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      _clip_map(ClipMachineMemory, xml_list);
   if (pointer)
      _clip_mputn(ClipMachineMemory, xml_list, (long) pointer, (long) cpar);
}

CLIP_DLLEXPORT void
_list_remove_cparser(ClipMachine * ClipMachineMemory, void *pointer)
{
   if (pointer && xml_list->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      _clip_mdel(ClipMachineMemory, xml_list, (long) pointer);
}

CLIP_DLLEXPORT void
_destroy_c_parser(void *obj)
{
   C_parser *cpar = (C_parser *) obj;

   _list_remove_cparser(cpar->cmachine, cpar->parser);

   _clip_destroy(cpar->cmachine, cpar->userData);

   _clip_destroy(cpar->cmachine, &cpar->characterDataHandler);
   _clip_destroy(cpar->cmachine, &cpar->startElementHandler);
   _clip_destroy(cpar->cmachine, &cpar->endElementHandler);
   _clip_destroy(cpar->cmachine, &cpar->startCdataSectionHandler);
   _clip_destroy(cpar->cmachine, &cpar->endCdataSectionHandler);
   _clip_destroy(cpar->cmachine, &cpar->commentHandler);
   _clip_destroy(cpar->cmachine, &cpar->defaultHandler);

   _clip_destroy_c_item(cpar->cmachine, cpar->handle, _C_ITEM_TYPE_XML_PARSER);
   free(cpar);

}

CLIP_DLLEXPORT C_parser *
_register_parser(ClipMachine * ClipMachineMemory, XML_Parser parser)
{
   int handle = -1;
   C_parser *cpar = (C_parser *) calloc(1, sizeof(C_parser));
   cpar->cmachine = ClipMachineMemory;
   cpar->parser = parser;

   _clip_map(ClipMachineMemory, &cpar->obj);

  /* Saving widget info into CLIP state machine
   * and it`s handle to a map HANDLE property */
   handle = _clip_store_c_item(ClipMachineMemory, cpar, _C_ITEM_TYPE_XML_PARSER, NULL);
   cpar->handle = handle;
   _clip_mputn(ClipMachineMemory, &cpar->obj, HASH_HANDLE, handle);

  /* Store C_parser pointer in list of parsers */
   _list_put_cparser(ClipMachineMemory, parser, cpar);

   return cpar;
}

CLIP_DLLEXPORT C_parser *
_fetch_c_arg(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar;

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      {
	 cpar = (C_parser *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1), _C_ITEM_TYPE_XML_PARSER);
      }
   else
      {
	 if (_clip_parinfo(ClipMachineMemory, 1) == MAP_type_of_ClipVarType)
	    {
	       double h;
	       _clip_mgetn(ClipMachineMemory, _clip_spar(ClipMachineMemory, 1), HASH_HANDLE, &h);
	       cpar = (C_parser *) _clip_fetch_c_item(ClipMachineMemory, (int) h, _C_ITEM_TYPE_XML_PARSER);
	    }
	 else
	    {
	       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_EXPAT", EG_ARG, "Bad parser descriptor");
	       return NULL;
	    }
      }
   if (!cpar)
      {
	 _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, "CLIP_EXPAT", EG_ARG, "Bad parser descriptor");
	 return NULL;
      }
   return cpar;
}

static int XMLCALL
unknownEncoding(void *encodingHandlerData, const XML_Char * name, XML_Encoding * info)
{
   int i, len1 = 0;
   cons_CharsetEntry *cs1 = 0;

   if (load_charset_name((char *) name, &cs1, &len1))
      {
	 _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", (char *) name, strerror(errno));
	 return XML_STATUS_ERROR;
      }

   for (i = 0; i < 256; i++)
      info->map[i] = i;

   for (i = 0; i < len1; i++)
      {
	 int ch;
	 unsigned long unich;
	 cons_CharsetEntry *cp;

	 cp = cs1 + i;
	 ch = cp->ch;
	 unich = cp->unich;

	 if (ch >= 256 || ch < 0x80)
	    continue;

	 info->map[i] = unich;
      }
   free(cs1);

   for (i = 0; i < 32; i++)
      info->map[i] = i;

   info->data = 0;
   info->convert = 0;
   info->release = 0;

   return XML_STATUS_OK;
}

int
clip_XML_PARSERCREATE(ClipMachine * ClipMachineMemory)
{
   char *encoding = _clip_parc(ClipMachineMemory, 1);
   XML_Parser parser = NULL;
   C_parser *cpar;

   CHECKOPT(1, CHARACTER_type_of_ClipVarType);

   parser = XML_ParserCreate((encoding ? encoding : NULL));
   XML_SetUnknownEncodingHandler(parser, unknownEncoding, NULL);
   XML_SetParamEntityParsing(parser, XML_PARAM_ENTITY_PARSING_NEVER);
   cpar = _register_parser(ClipMachineMemory, parser);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpar->obj);

   return 0;
 err:
   return 1;
}

int
clip_XML_PARSERFREE(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   XML_Parser parser = cpar->parser;

   CHECKCPARSER(cpar);

   _destroy_c_parser(cpar);
   XML_ParserFree(parser);

   return 0;
 err:
   return 1;
}

int
clip_XML_SETUSERDATA(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *userData = _clip_par(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);

   cpar->userData = userData;

   XML_SetUserData(cpar->parser, cpar);

   return 0;
 err:
   return 1;
}

int
clip_XML_SETPARAMENTITYPARSING(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   int flag = _clip_parni(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);

   XML_SetParamEntityParsing(cpar->parser, flag);
   return 0;
 err:
   return 1;
}

int
clip_XML_PARSE(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   const char *str = _clip_parc(ClipMachineMemory, 2);
   int len = _clip_parni(ClipMachineMemory, 3);
   int isFinal = _clip_parl(ClipMachineMemory, 4);

   CHECKCPARSER(cpar);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, LOGICAL_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, (int) XML_Parse(cpar->parser, str, len, isFinal));

   return 0;
 err:
   return 1;
}

int
clip_XML_GETERRORCODE(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);

   CHECKCPARSER(cpar);

   _clip_retni(ClipMachineMemory, XML_GetErrorCode(cpar->parser));

   return 0;
 err:
   return 1;
}

int
clip_XML_ERRORSTRING(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   CHECKCPARSER(cpar);

   _clip_retc(ClipMachineMemory, (char *) XML_ErrorString(XML_GetErrorCode(cpar->parser)));
   return 0;
 err:
   return 1;
}

int
clip_XML_GETCURRENTBYTEINDEX(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   CHECKCPARSER(cpar);

   _clip_retni(ClipMachineMemory, XML_GetCurrentByteIndex(cpar->parser));
   return 0;
 err:
   return 1;
}

int
clip_XML_GETCURRENTLINENUMBER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   CHECKCPARSER(cpar);

   _clip_retni(ClipMachineMemory, XML_GetCurrentLineNumber(cpar->parser));
   return 0;
 err:
   return 1;
}

int
clip_XML_GETCURRENTCOLUMNNUMBER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   CHECKCPARSER(cpar);

   _clip_retni(ClipMachineMemory, XML_GetCurrentColumnNumber(cpar->parser));
   return 0;
 err:
   return 1;
}

static int
_character_data_handler(void *userData, const XML_Char * s, int len)
{
   C_parser *cud = (C_parser *) userData;
   ClipVar str;
   ClipVar n;
   ClipVar stack[3], *app, *nv = 0;
   ClipVar res;
   int ret = 1;
   unsigned l = 0;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   memset(&res, 0, sizeof(ClipVar));
   memset(&str, 0, sizeof(ClipVar));
   memset(&n, 0, sizeof(ClipVar));
   _clip_var_str(s, len, &str);

   n.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	n.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = len;

   app = cud->userData;
   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
      {
	 l = 1;
	 nv = NEW(ClipVar);

	 *nv = *app;

	 nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	 app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
	 app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	 app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

      }

   stack[0] = *app;
   _clip_mclone(cud->cmachine, &stack[1], &str);
   _clip_mclone(cud->cmachine, &stack[2], &n);

   if (_clip_eval(cud->cmachine, &(cud->characterDataHandler), 3, stack, &res) == 0)
      ret = res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(cud->cmachine, &res);

   if (l)
      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

   _clip_destroy(cud->cmachine, &stack[1]);
   _clip_destroy(cud->cmachine, &stack[2]);

   _clip_destroy(cud->cmachine, &str);
   _clip_destroy(cud->cmachine, &n);

   return ret;
}

int
clip_XML_SETCHARACTERDATAHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *func = _clip_spar(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->characterDataHandler, func);
   XML_SetCharacterDataHandler(cpar->parser, (XML_CharacterDataHandler) _character_data_handler);

   return 0;
 err:
   return 1;
}

static int
_start_element_handler(void *userData, const XML_Char * name, const XML_Char ** attrs)
{
   C_parser *cud = (C_parser *) userData;
   ClipVar str;
   ClipVar *eattr, *item;
   ClipVar stack[3], *app, *nv = 0;
   ClipVar res;
   int ret = 1, i, j;
   unsigned l = 0;
   long vect[2];

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   memset(&str, 0, sizeof(ClipVar));
   eattr = malloc(sizeof(ClipVar));
   _clip_var_str(name, strlen(name), &str);

   vect[0] = 0;
   vect[1] = 0;
   _clip_array(cud->cmachine, eattr, 1, vect);
   for (i = 0, j = 0; attrs[i]; i += 2, j++)
      {
	 ClipVar var;
	 item = malloc(sizeof(ClipVar));

	 vect[0] = 2;
	 _clip_array(cud->cmachine, item, 1, vect);
	 vect[0] = j + 1;
	 _clip_asize(cud->cmachine, eattr, 1, vect);
	 vect[0] = j;
	 _clip_aset(cud->cmachine, eattr, item, 1, vect);

	 memset(&var, 0, sizeof(var));
	 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) attrs[i];
	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(attrs[i]);
	 vect[1] = 0;
	 _clip_aset(cud->cmachine, eattr, &var, 2, vect);

	 memset(&var, 0, sizeof(var));
	 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) attrs[i + 1];
	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(attrs[i + 1]);
	 vect[1] = 1;
	 _clip_aset(cud->cmachine, eattr, &var, 2, vect);

	 _clip_destroy(cud->cmachine, item);
	 free(item);

      }

   app = cud->userData;
   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
      {
	 l = 1;
	 nv = NEW(ClipVar);

	 *nv = *app;

	 nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	 app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	 app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	 app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

      }
   stack[0] = *app;
   _clip_mclone(cud->cmachine, &stack[1], &str);
   _clip_mclone(cud->cmachine, &stack[2], eattr);

   if (_clip_eval(cud->cmachine, &(cud->startElementHandler), 3, stack, &res) == 0)
      ret = res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(cud->cmachine, &res);

   if (l)
      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

   _clip_destroy(cud->cmachine, &stack[1]);
   _clip_destroy(cud->cmachine, &stack[2]);

   _clip_destroy(cud->cmachine, &str);
   _clip_destroy(cud->cmachine, eattr);
   free(eattr);
   return ret;
}

int
clip_XML_SETSTARTELEMENTHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *func = _clip_spar(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->startElementHandler, func);
   XML_SetStartElementHandler(cpar->parser, (XML_StartElementHandler) _start_element_handler);

   return 0;
 err:
   return 1;
}

static int
_end_element_handler(void *userData, const XML_Char * name)
{
   C_parser *cud = (C_parser *) userData;
   ClipVar str;
   ClipVar stack[2], *app, *nv = 0;
   ClipVar res;
   int ret = 1;
   unsigned l = 0;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   memset(&str, 0, sizeof(ClipVar));
   _clip_var_str(name, strlen(name), &str);

   app = cud->userData;
   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
      {
	 l = 1;
	 nv = NEW(ClipVar);

	 *nv = *app;

	 nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	 app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	 app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	 app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

      }
   stack[0] = *app;
   _clip_mclone(cud->cmachine, &stack[1], &str);

   if (_clip_eval(cud->cmachine, &(cud->endElementHandler), 2, stack, &res) == 0)
      ret = res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(cud->cmachine, &res);

   if (l)
      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

   _clip_destroy(cud->cmachine, &stack[1]);

   _clip_destroy(cud->cmachine, &str);
   return ret;
}

int
clip_XML_SETENDELEMENTHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *func = _clip_spar(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->endElementHandler, func);
   XML_SetEndElementHandler(cpar->parser, (XML_EndElementHandler) _end_element_handler);

   return 0;
 err:
   return 1;
}

int
clip_XML_SETELEMENTHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *sfunc = _clip_spar(ClipMachineMemory, 2);
   ClipVar *efunc = _clip_spar(ClipMachineMemory, 3);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   CHECKARG2(3, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->startElementHandler, sfunc);
   _clip_mclone(ClipMachineMemory, &cpar->endElementHandler, efunc);
   XML_SetElementHandler(cpar->parser, (XML_StartElementHandler) _start_element_handler, (XML_EndElementHandler) _end_element_handler);

   return 0;
 err:
   return 1;
}

static int
_comment_handler(void *userData, const XML_Char * data)
{
   C_parser *cud = (C_parser *) userData;
   ClipVar str;
   ClipVar stack[2], *app, *nv = 0;
   ClipVar res;
   int ret = 1;
   unsigned l = 0;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   memset(&str, 0, sizeof(ClipVar));
   _clip_var_str(data, strlen(data), &str);

   app = cud->userData;
   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
      {
	 l = 1;
	 nv = NEW(ClipVar);

	 *nv = *app;

	 nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	 app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	 app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	 app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

      }
   stack[0] = *app;
   _clip_mclone(cud->cmachine, &stack[1], &str);

   if (_clip_eval(cud->cmachine, &(cud->commentHandler), 2, stack, &res) == 0)
      ret = res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(cud->cmachine, &res);

   if (l)
      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

   _clip_destroy(cud->cmachine, &stack[1]);

   _clip_destroy(cud->cmachine, &str);
   return ret;
}

int
clip_XML_SETCOMMENTHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *func = _clip_spar(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->commentHandler, func);
   XML_SetCommentHandler(cpar->parser, (XML_CommentHandler) _comment_handler);

   return 0;
 err:
   return 1;
}

static int
_start_cdata_section_handler(void *userData)
{
   C_parser *cud = (C_parser *) userData;
   ClipVar stack[1], *app, *nv = 0;
   ClipVar res;
   int ret = 1;
   unsigned l = 0;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));

   app = cud->userData;
   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
      {
	 l = 1;
	 nv = NEW(ClipVar);

	 *nv = *app;

	 nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	 app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	 app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	 app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

      }
   stack[0] = *app;

   if (_clip_eval(cud->cmachine, &(cud->startCdataSectionHandler), 1, stack, &res) == 0)
      ret = res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(cud->cmachine, &res);

   if (l)
      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

   return ret;
}

int
clip_XML_SETSTARTCDATASECTIONHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *func = _clip_spar(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->startCdataSectionHandler, func);
   XML_SetStartCdataSectionHandler(cpar->parser, (XML_StartCdataSectionHandler) _start_cdata_section_handler);

   return 0;
 err:
   return 1;
}

static int
_end_cdata_section_handler(void *userData)
{
   C_parser *cud = (C_parser *) userData;
   ClipVar stack[1], *app, *nv = 0;
   ClipVar res;
   int ret = 1;
   unsigned l = 0;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));

   app = cud->userData;
   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
      {
	 l = 1;
	 nv = NEW(ClipVar);

	 *nv = *app;

	 nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	 app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	 app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	 app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

      }
   stack[0] = *app;

   if (_clip_eval(cud->cmachine, &(cud->endCdataSectionHandler), 1, stack, &res) == 0)
      ret = res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(cud->cmachine, &res);

   if (l)
      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

   return ret;
}

int
clip_XML_SETENDCDATASECTIONHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *func = _clip_spar(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->endCdataSectionHandler, func);
   XML_SetEndCdataSectionHandler(cpar->parser, (XML_EndCdataSectionHandler) _end_cdata_section_handler);

   return 0;
 err:
   return 1;
}

int
clip_XML_SETCDATASECTIONHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *sfunc = _clip_spar(ClipMachineMemory, 2);
   ClipVar *efunc = _clip_spar(ClipMachineMemory, 3);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);
   CHECKARG2(3, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->startCdataSectionHandler, sfunc);
   _clip_mclone(ClipMachineMemory, &cpar->endCdataSectionHandler, efunc);
   XML_SetCdataSectionHandler(cpar->parser, (XML_StartCdataSectionHandler) _start_cdata_section_handler, (XML_EndCdataSectionHandler) _end_cdata_section_handler);

   return 0;
 err:
   return 1;
}

static int
_default_handler(void *userData, const XML_Char * s, int len)
{
   C_parser *cud = (C_parser *) userData;
   ClipVar str;
   ClipVar n;
   ClipVar stack[3], *app, *nv = 0;
   ClipVar res;
   int ret = 1;
   unsigned l = 0;

   memset(&stack, 0, sizeof(stack));
   memset(&res, 0, sizeof(ClipVar));
   memset(&str, 0, sizeof(ClipVar));
   memset(&n, 0, sizeof(ClipVar));
   _clip_var_str(s, len, &str);

   n.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	n.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = len;

   app = cud->userData;
   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
      {
	 l = 1;
	 nv = NEW(ClipVar);

	 *nv = *app;

	 nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	 app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	 app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	 app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

      }
   stack[0] = *app;
   _clip_mclone(cud->cmachine, &stack[1], &str);
   _clip_mclone(cud->cmachine, &stack[2], &n);

   if (_clip_eval(cud->cmachine, &(cud->defaultHandler), 3, stack, &res) == 0)
      ret = res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType ? res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar : ret;

   _clip_destroy(cud->cmachine, &res);

   if (l)
      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

   _clip_destroy(cud->cmachine, &stack[1]);
   _clip_destroy(cud->cmachine, &stack[2]);

   _clip_destroy(cud->cmachine, &str);
   _clip_destroy(cud->cmachine, &n);
   return ret;
}

int
clip_XML_SETDEFAULTHANDLER(ClipMachine * ClipMachineMemory)
{
   C_parser *cpar = _fetch_c_arg(ClipMachineMemory);
   ClipVar *func = _clip_spar(ClipMachineMemory, 2);

   CHECKCPARSER(cpar);
   CHECKARG2(2, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   _clip_mclone(ClipMachineMemory, &cpar->defaultHandler, func);
   XML_SetDefaultHandler(cpar->parser, (XML_DefaultHandler) _default_handler);

   return 0;
 err:
   return 1;
}

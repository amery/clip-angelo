
 /*
    Start total new system v. 0.0
    with hard coded long name variables to have clear system
    Angelo GIRARDI
  */

#include "ci_clip.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "ci_clic.h"
#include "ci_nodepriv.h"
#include "ci_clipvm.h"
#include "ci_clipcfg.h"

static void flush_codestr(CodestrNode * np, void *nod);

static int loopNo, loopPart = 0;

static clip_hash_t
n_hashstr(char *str)
{
   char *s;

   int is_dig = 1;

   for (s = str; *s; s++)
      {
	 if (!isdigit(*s))
	    {
	       is_dig = 0;
	       break;
	    }
      }

   if (is_dig)
      return (clip_hash_t) atol(str);
   else
      return hashstr(str);
}

/* [ utils */

#define SETSHORT(bp, offs, val) setshort(np->node.line,bp,offs,val)

#ifdef FORCEALIGN

static void
setshort(int line, StrBuf * bp, long offs, long lval)
{
   short val;

   if (lval > 32767 || lval < -32767)
      {
	 yyerror("po code too large at line %d", line);
      }
   val = lval;
   memcpy(bp->buf + offs, (char *) &val, sizeof(short));
}

#else

/*#define SETSHORT(bp, offs, val) (*(short*)((bp)->buf+(offs))=(val))*/

static void
setshort(int line, StrBuf * bp, long offs, long val)
{
   if (val > 32767 || val < -32767 /*val & 0xffff0000 */ )
      {
	 yyerror("po code too large at line %d", line);
      }
   (*(short *) ((bp)->buf_of_StrBuf + (offs)) = (val & 0xffff));
}

#endif

static void
ins_name(char *name)
{
   int no;

   if (!name)
      return;
   if (!search_Coll(&curFile->Coll_names_of_File, name, &no))
      atInsert_Coll(&curFile->Coll_names_of_File, strdup(name), no);
}

void
fprintfOffs(FILE * file, int offs, const char *format, ...)
{
   va_list ap;

   while (offs-- > 0)
      fputc('\t', file);
   va_start(ap, format);
   vfprintf(file, format, ap);
   va_end(ap);
}

static void
addDeep(Function * fp, int no)
{
   fp->deep_of_Function += no;
   if (fp->deep_of_Function > fp->maxdeep_of_Function)
      fp->maxdeep_of_Function = fp->deep_of_Function;
}

static void
print_line(void *v, int level, FILE * out)
{
   VAR(Node, np, v);
   if (clic_line != np->line)
      {
	 fprintfOffs(out, level, "_frame.line = %d;\n", np->line);
	 if (genDebug)
	    fprintfOffs(out, level, "_clip_line(_mp);\n");
      }
   clic_line = np->line;
}

static void
put_line(void *v, int level, StrBuf * out)
{
   VAR(Node, np, v);
   if (clic_line != np->line)
      {
	 putByte_StrBuf(out, CLIP_LINE);
	 putShort_StrBuf(out, np->line);
      }
   clic_line = np->line;
}

static int
add_to_undeclared(char *name, Function ** fp)
{
   int no, fno;

   Function buf;

   *fp = 0;
   buf.name_of_Function = name;
   buf.isPublic_of_Function = 1;
   if (search_Coll(&curFile->Coll_functions_of_File, &buf, &fno))
      {
	 if (!search_Coll(&curFile->Coll_externFunctions_of_File, name, &no))
	    {
	       *fp = (Function *) curFile->Coll_functions_of_File.items[fno];
	       if ((*fp)->isPublic_of_Function)
		  return 1;
	       else
		  return 2;
	    }
      }
   else
      {
	 if (!search_Coll(&curFile->Coll_undeclExternFunctions_of_File, name, &no))
	    {
	       char *s = strdup(name);

	       char *p;

	       for (p = s; *p; p++)
		  *p = toupper(*p);
	       insert_Coll(&curFile->Coll_undeclExternFunctions_of_File, s);
	    }
      }

   return 1;
}

/* ] utils */
/* [ Node */

void
init_Node(void *node, int (*pass) (void *, Pass, int, void *), const char *name)
{
   VAR(Node, np, node);
   memset(np, 0, sizeof(Node));
   if (pass)
      np->pass = pass;
   else
      np->pass = pass_Node;
   init_List(&np->list);
   np->line = cl_line;
   np->file = currentFile();
   np->isExec = 1;
   np->seqNo = (curFunction ? curFunction->seqNo_of_Function : 0);
   np->name = name;
}

int
pass_Node(void *self, Pass pass, int level, void *par)
{
   int r;

   VAR(Node, np, self);
   for (r = first_List(&np->list); r; r = next_List(&np->list))
      {
	 VAR(Node, p, np->list.current);
	 if (p->pass(p, pass, level + 1, par))
	    break;
      }

   return 0;
}

Node *
new_Node()
{
   NEWVAR(Node, ret);
   init_Node(ret, pass_Node, "node");
   return ret;
}

void
delete_Node(Node * node)
{
   if (!node)
      return;
   node->pass(node, Destroy, 0, node);
   free(node);
}

int
getCount_Node(void *node)
{
   VAR(Node, np, node);
   int r, ret = 0;

   for (r = first_List(&np->list); r; r = next_List(&np->list))
      ret++;
   return ret;
}

void
append_Node(Node * list, Node * node)
{
   if (!list || !node)
      return;
   append_List(&list->list, node);
}

void
join_Node(Node * list, Node * np)
{
   Node *p, *pp;

   if (!list || !np)
      return;
   for (p = (Node *) np->list.head; p; p = pp)
      {
	 pp = (Node *) p->listel.next;
	 append_List(&list->list, p);
	 if (pp == (Node *) np->list.head)
	    break;
      }

   np->list.head = np->list.current = NULL;
}

int
traverse_Node(Node * node, int (*func) (Node * np, void *par), void *par)
{
#if 0
   Node *p, *p0;

   for (p0 = p = (Node *) node->list.head; p;)
      {
	 int r;

	 r = func(p, par);
	 if (r)
	    return r;

	 p = (Node *) p->listel.next;
	 if (p == p0)
	    break;
      }

   return 0;
#endif
   TraversePar tpar;

   if (!node)
      return 1;
   tpar.func = func;
   tpar.par = par;

   return node->pass(node, Traverse, 0, &tpar);

}

void
prepend_Node(Node * list, Node * node)
{
   if (!list || !node)
      return;
   prepend_List(&list->list, node);
}

/* ] Node */
/* [ OperListNode */

static int
pass_OperListNode(void *self, Pass pass, int level, void *par)
{
   VAR(Node, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "operlist node:\n");
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   return pass_Node(np, pass, level, par);
}

Node *
new_OperListNode()
{
   NEWVAR(Node, ret);
   init_Node(ret, pass_OperListNode, "operlist");
   ret->isExec = 0;
   return ret;
}

/* ] OperListNode */
/* [ OperExprNode */

static int
pass_OperExprNode(void *self, Pass pass, int level, void *par)
{
   VAR(OperExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "operexpr node:\n");
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    fp->deep_of_Function = 0;
	    np->expr->isTop = 1;
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    int ret;

	    print_line(self, level, out);
	    ret = np->expr->pass(np->expr, pass, level, par);
	    if (!np->expr->isAssign)
	       fprintfOffs(out, level, "_clip_pop(_mp);\n");
	    return ret;
	 }
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int ret;

	    put_line(self, level, out);
	    ret = np->expr->pass(np->expr, pass, level, par);
	    if (!np->expr->isAssign)
	       putByte_StrBuf(out, CLIP_POP);
	    return ret;
      case Traverse:
	    {
	       VAR(TraversePar, tp, par);
	       tp->func((Node *) self, tp->par);
	    }
	    break;
      default:
	    break;
	 }
      }
   return np->expr->pass(np->expr, pass, level + 1, par);
}

Node *
new_OperExprNode(Node * node)
{
   NEWVAR(OperExprNode, ret);
   init_Node(ret, pass_OperExprNode, "operexpr");
   ret->expr = node;
   return (Node *) ret;
}

/* ] OperExprNode */
/* [ ConstNode */

static int
pass_ConstNode(void *self, Pass pass, int level, void *par)
{
   VAR(ConstNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "const node (%d) isInit=%d:\n", np->no, np->node.isInit);
	 switch (np->type)
	    {
	    case CONST_NUMBER:
	       fprintfOffs(stdout, level, "NUMBER=%s%s, rval='%s'\n", np->node.isMinus ? "-" : "", np->val, np->rval ? np->rval : "");
	       break;
	    case CONST_STRING:
	       fprintfOffs(stdout, level, "STRING='%s'\n", np->val);
	       break;
	    case CONST_NIL:
	       fprintfOffs(stdout, level, "NIL\n");
	       break;
	    case CONST_LOGIC:
	       fprintfOffs(stdout, level, "LOGIC=%s %d\n", np->no ? ".T." : ".F.", np->node.isMinus);
	       break;
	    }
	 break;
      case CTextInit:
	 {
	    VAR(FILE, out, par);
	    switch (np->type)
	       {
	       case CONST_NUMBER:
		  {
		     char *s = strchr(np->val, '.');

		     if (!s)
			s = strchr(np->val, 'e');
		     if (!s)
			s = strchr(np->val, 'E');
		     fprintfOffs(out, level, "{ { { NUMERIC_type_of_ClipVarType,0,0,0,0,0}, %s%s%s } }", np->node.isMinus ? "-" : "", np->val, s ? "" : ".0");
		  }
		  break;
	       case CONST_STRING:
		  fprintfOffs(out, level, "{ { { CHARACTER_type_of_ClipVarType,%d,0,F_MSTAT_ClipFlags,0 }, {", haveMacro(np->val));
		  printString(out, np->val);
		  fprintf(out, ", %ld } } }", (long) strlen(np->val));
		  break;
	       case CONST_NIL:
		  fprintfOffs(out, level, "{ { UNDEF_type_of_ClipVarType,0,0,0,0 } }");
		  break;
	       case CONST_LOGIC:
		  {
		     int val = np->no;

		     if (np->node.isMinus)
			val = !val;
		     fprintfOffs(out, level, "{ { { LOGICAL_type_of_ClipVarType, 0,0,0,0 }, %s } }", val ? "1" : "0");
		  }
		  break;
	       }
	 }
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);

	    switch (np->type)
	       {
	       case CONST_NUMBER:
		  putString_StrBuf(out, np->val);
		  break;
	       case CONST_STRING:
#if 0
		  {
		     int lq = '"', rq = '"';

		     choose_quot(np->val, &lq, &rq, 1);
		     if (lq)
			putByte_StrBuf(out, lq);
		     putString_StrBuf(out, np->val);
		     if (rq)
			putByte_StrBuf(out, rq);
		  }
#endif
		  {
		     char *s = quot_str(np->val, 1);

		     putString_StrBuf(out, s);
		     free(s);
		  }
		  break;
	       case CONST_NIL:
		  putString_StrBuf(out, "NIL");
		  break;
	       case CONST_LOGIC:
		  if (np->no)
		     putString_StrBuf(out, ".T.");
		  else
		     putString_StrBuf(out, ".F.");
		  break;
	       }
	 }
	 return 0;
      case CText:
	 {
	    VAR(FILE, out, par);
	    switch (np->type)
	       {
	       case CONST_NUMBER:
		  fprintfOffs(out, level, "_clip_push( _mp, &_num_%d );\n", np->no);
		  break;
	       case CONST_STRING:
		  fprintfOffs(out, level, "if ( (_ret=_clip_push(_mp, &_str_%d) )) goto _trap_%d;\n", np->no, np->node.seqNo);
		  break;
	       case CONST_NIL:
		  fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
		  break;
	       case CONST_LOGIC:
		  if (np->no)
		     fprintfOffs(out, level, "_clip_push_true( _mp );\n");
		  else
		     fprintfOffs(out, level, "_clip_push_false( _mp );\n");
		  break;
	       }
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    switch (np->type)
	       {
	       case CONST_NUMBER:
		  putByte_StrBuf(out, CLIP_PUSH_NUM);
		  putShort_StrBuf(out, np->no);
		  break;
	       case CONST_STRING:
		  putByte_StrBuf(out, CLIP_PUSH_STR);
		  putShort_StrBuf(out, np->no);
		  break;
	       case CONST_NIL:
		  putByte_StrBuf(out, CLIP_PUSH_NIL);
		  break;
	       case CONST_LOGIC:
		  if (np->no)
		     putByte_StrBuf(out, CLIP_PUSH_TRUE);
		  else
		     putByte_StrBuf(out, CLIP_PUSH_FALSE);
		  break;
	       }
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   return 0;
}

int
compareConstNode(void *n1, void *n2)
{
   VAR(ConstNode, np1, n1);
   VAR(ConstNode, np2, n2);
   if (np1->type != np2->type)
      return np1->type < np2->type ? -1 : 1;
   switch (np1->type)
      {
      case CONST_NUMBER:
	 {
	    double d1 = strtod(np1->val, NULL);

	    double d2 = strtod(np2->val, NULL);

	    if (d1 < d2)
	       return -1;
	    else if (d1 > d2)
	       return 1;
	    else
	       return 0;
	 }
      case CONST_STRING:
	 return strcmp(np1->val, np2->val);
      }
   return 0;
}

Node *
new_NumberConstNode(char *val, char *rval)
{
   int no;

   NEWVAR(ConstNode, ret);

   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_NUMBER;
   ret->val = val;
   ret->rval = rval;
   if (search_Coll(&curFile->Coll_numbers_of_File, ret, &no))
      {
	 VAR(ConstNode, np, curFile->Coll_numbers_of_File.items[no]);
	 ret->no = np->no;
      }
   else
      {
	 ret->no = curFile->Coll_numbers_of_File.count;
	 insert_Coll(&curFile->Coll_numbers_of_File, ret);
	 insert_Coll(&curFile->Coll_unsortedNumbers_of_File, ret);
      }
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isNum = 1;
   curFunction->goto_trap_of_Function++;
   return (Node *) ret;
}

Node *
new_HashConstNode(char *name)
{
   char buf[32];

   snprintf(buf, sizeof(buf), "%ld", hashstr(name));
   curFunction->goto_trap_of_Function++;
   ins_name(name);
   return new_NumberConstNode(strdup(buf), name);
}

Node *
new_StringConstNode(char *val)
{
   int no;

//#define NEWVAR(type,var) type *var=(type*)calloc(sizeof(type),1)
//      NEWVAR(ConstNode, ret);

   ConstNode *ret = (ConstNode *) calloc(sizeof(ConstNode), 1);

   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_STRING;
   ret->val = val;
   ret->len = strlen(val);
   if (search_Coll(&curFile->Coll_strings_of_File, ret, &no))
      {
	 VAR(ConstNode, np, curFile->Coll_strings_of_File.items[no]);
	 ret->no = np->no;
      }
   else
      {
	 ret->no = curFile->Coll_strings_of_File.count;
	 insert_Coll(&curFile->Coll_strings_of_File, ret);
	 insert_Coll(&curFile->Coll_unsortedStrings_of_File, ret);
      }
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isStr = 1;
   curFunction->goto_trap_of_Function++;
   return (Node *) ret;
}

Node *
new_LogicConstNode(int val)
{
   NEWVAR(ConstNode, ret);
   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_LOGIC;
   ret->no = val;
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isLog = 1;
   ret->node.isMinus = 0;
   if (val)
      curFile->haveTrue_of_File++;
   else
      curFile->haveFalse_of_File++;
   curFunction->goto_trap_of_Function++;
   return (Node *) ret;
}

Node *
new_NilConstNode()
{
   NEWVAR(ConstNode, ret);
   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_NIL;
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isNil = 1;
   curFunction->goto_trap_of_Function++;
   curFile->haveNil_of_File++;
   return (Node *) ret;
}

/* ] ConstNode */
/* [ LocalDefNode */

void
print_Var(Var * vp, int level)
{
   fprintfOffs(stdout, level, "");
   if (vp->alias)
      printf("%s->", vp->alias);
   printf("%s as=%s isRef=%d refvar=%d isLocalRef=%d", vp->name, vp->type ? vp->type : "undef", vp->isRef, vp->refvar ? vp->refvar->no : -1, vp->isLocalRef);
   if (vp->init)
      {
	 printf(":=\n");
	 vp->init->pass(vp->init, Print, level + 1, NULL);
      }
   else if (vp->arr)
      {
	 int i;

	 for (i = 0; i < vp->arr->count; i++)
	    printf("[%s]", (char *) vp->arr->items[i]);
	 printf("\n");
      }
   else
      printf("\n");
}

Node *
new_LocalDefNode1(VarColl * cp)
{
   return new_LocalDefNode(cp, 1, 0);
}

Node *
new_LocalDefNode(VarColl * cp, int err, int localref)
{
   int i;

   if (curFunction->lastExec_of_Function && err)
      yyerror("LOCAL after executable statements");

   for (i = 0; i < cp->unsorted.count; i++)
      {
	 VAR(Var, vp, cp->unsorted.items[i]);
	 Node *ip = 0;

	 vp->isLocalRef = localref;
	 if (vp->macro)
	    {
	       yyerror("macro variable name not allowed here");
	       continue;
	    }
	 ins_name(vp->name);
	 if (undeclaredName(vp->name, 0))
	    {
	       add_VarColl(curFunction->VarColl_locals_of_Function, vp);
	    }

	 if (!err)
	    {
	       vp->isRef = 0;
	       curFunction->uplocals_of_Function++;
	       continue;
	    }

	 if (vp->init && vp->init->isConst)
	    vp->init->isInit = 1;
#if 1
	 if (vp->init
	    /*&& (vp->isRef || pcode_flag || !vp->init->isConst || vp->init->isArray) */
	    )
	    {
	       ip = vp->init;
	    }
	 else if (vp->arr)
	    {
	       ip = new_NewArrayNode(vp->arr);
	    }

	 if (ip)
	    {
	       Node *lp = new_LocalNode(vp);

	       Node *np = new_OperExprNode(new_AssignNode(lp, ip, '='));

	       np->line = vp->line;
	       append_Node(curFunction->Node_body_of_Function, np);
	    }
#endif
      }

   return (Node *) NULL;
}

/* ] LocalDefNode */
/* [ StaticDefNode */

Node *
new_StaticDefNode(VarColl * cp)
{
   int i;

   if (curFunction->lastExec_of_Function)
      yyerror("STATIC after executable statements");

   for (i = 0; i < cp->coll.count; i++)
      {
	 VAR(Var, vp, cp->coll.items[i]);
	 if (vp->macro)
	    {
	       yyerror("macro variable name not allowed here");
	       continue;
	    }
	 ins_name(vp->name);
	 if (undeclaredName(vp->name, 0))
	    add_VarColl(curFunction->VarColl_statics_of_Function, vp);
	 if (vp->init && !vp->init->isConst && !vp->init->isCode)
	    {
	       int is_const = 0;

	       vp->init->pass(vp->init, TestConst, 0, &is_const);
#if 0
	       if (!is_const)
		  yyerror("only constant expression allowed for static variable initialisation");
#endif
	    }
	 if (vp->init)
	    {
	       vp->init->isInit = 1;
	       curFunction->goto_trap_of_Function++;
	    }
	 vp->no = curFile->staticNo_of_File;
	 ++curFile->staticNo_of_File;
      }

   return (Node *) NULL;
}

/* ] StaticDefNode */
/* [ MemvarDefNode */

Node *
new_MemvarDefNode(VarColl * cp)
{
   int i;

   if (curFunction->lastExec_of_Function)
      yyerror("MEMVAR after executable statements");

   for (i = 0; i < cp->coll.count; i++)
      {
	 VAR(Var, vp, cp->coll.items[i]);
	 if (vp->macro)
	    {
	       yyerror("macro variable name not allowed here");
	       continue;
	    }
	 ins_name(vp->name);
	 if (undeclaredName(vp->name, 1))
	    add_VarColl(curFunction->VarColl_memvars_of_Function, vp);
      }

   return (Node *) NULL;
}

/* ] MemvarDefNode */
/* [ FieldDefNode */

Node *
new_FieldDefNode(VarColl * cp, char *alias)
{
   int i;

   if (curFunction->lastExec_of_Function)
      yyerror("FIELD after executable statements");

   for (i = 0; i < cp->coll.count; i++)
      {
	 VAR(Var, vp, cp->coll.items[i]);
	 if (vp->macro)
	    {
	       yyerror("macro variable name not allowed here");
	       continue;
	    }
	 ins_name(vp->name);
	 if (undeclaredName(vp->name, 0))
	    add_VarColl(curFunction->VarColl_fields_of_Function, vp);
	 if (alias)
	    vp->alias = strdup(alias);
      }

   free(alias);

   return (Node *) NULL;
}

/* ] FieldDefNode */
/* [ PublicDefNode */

static int
pass_PublicDefNode(void *self, Pass pass, int level, void *par)
{
   VAR(DefNode, np, self);
   switch (pass)
      {
      case Print:
	 {
	    int i, count = np->cp->unsorted.count;

	    fprintfOffs(stdout, level, "public def node: %d vars\n", count);
	    for (i = 0; i < count; i++)
	       {
		  VAR(Var, vp, np->cp->unsorted.items[i]);
		  print_Var(vp, level + 1);
	       }
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   return pass_Node(np, pass, level, par);
}

Node *
new_PublicDefNode(VarColl * cp)
{
   int i, no;

   NEWVAR(DefNode, ret);

   init_Node(ret, pass_PublicDefNode, "publicdef");

   for (i = 0; i < cp->coll.count; i++)
      {
	 VAR(Var, vp, cp->coll.items[i]);
	 Node *vap, *np = 0;

	 if (!vp->macro)
	    {
	       ins_name(vp->name);
	       if (!undeclaredName(vp->name, 1))
		  rm_VarColl(curFunction->VarColl_memvars_of_Function, vp);
	       if (search_VarColl(curFunction->VarColl_publics_of_Function, vp->name, &no))
		  {
		    /*vp = (Var *) curFunction->VarColl_publics->coll.items[no]; */
		  }
	       else
		  {
		     add_VarColl(curFunction->VarColl_publics_of_Function, vp);
		  }

	       append_Node(&ret->node, new_CreateVarNode(vp, 1, strdup("public")));

	       vap = new_PublicNode(vp);
	    }
	 else
	    {
	       vap = vp->macro;
	       append_Node(&ret->node, new_CreateVarNode(vp, 1, strdup("public")));
	    }

	 if (vp->init)
	    {
	       np = new_OperExprNode(new_AssignNode(vap, vp->init, '='));
	       np->line = vp->init->line;
	    }
	 else if (vp->arr)
	    {
	       np = new_OperExprNode(new_AssignNode(vap, new_NewArrayNode(vp->arr), '='));
	       np->line = vp->line;
	    }
	 else if (vp->macro)
	    {
	      /*np = new_OperExprNode(vp->macro); */
	    }
	 if (np)
	    append_Node(&ret->node, np);
      }

   ret->cp = cp;

   return (Node *) ret;
}

/* ] PublicDefNode */
/* [ CreateVarNode */

static int
pass_CreateVarNode(void *self, Pass pass, int level, void *par)
{
   VAR(CreateVarNode, np, self);
   switch (pass)
      {
      case Print:
	 {
	    fprintfOffs(stdout, level, "create var node: space=%s vars\n", np->spacename);
	    print_Var(np->vp, level + 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    print_line(self, level, out);
	    if (np->vp->macro)
	       {
		  np->vp->macro->pass(np->vp->macro, /*pass, */ CTextLval,
				      level + 1, par);
		  if (np->space == 1)
		     fprintfOffs(out, level, "_clip_memvar_public( _mp, _clip_pop_hash( _mp ) );\n");
		  else if (np->space == 0)
		     fprintfOffs(out, level, "_clip_add_private( _mp, _clip_pop_hash( _mp ) );\n");
		  else
		     fprintfOffs(out, level, "_clip_memvar_space( _mp, _clip_space( _mp, %ld /* %s */) ,  _clip_pop_hash( _mp ), 1 );\n", (long) np->space, np->spacename);
	       }
	    else
	       {
		  if (np->space == 1)
		     fprintfOffs(out, level, "_clip_memvar_public( _mp, %ld /* %s */ );\n", (long) hashstr(np->vp->name), np->vp->name);
		  else if (np->space == 0)
		     fprintfOffs(out, level, "_clip_memvar_private( _mp, %ld /* %s */ );\n", (long) hashstr(np->vp->name), np->vp->name);
		  else
		     fprintfOffs(out, level, "_clip_memvar_space( _mp, _clip_space( _mp, %ld /* %s */) , %ld /* %s */, 1 );\n", (long) np->space, np->spacename, (long) hashstr(np->vp->name),
				 np->vp->name);
	       }
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    put_line(self, level, out);
	    if (np->vp->macro)
	       {
		  np->vp->macro->pass(np->vp->macro, /*pass, */ OTextLval,
				      level + 1, par);
		  if (np->space == 1)
		     putByte_StrBuf(out, CLIP_MEMVAR_PUBLIC_POP);
		  else if (np->space == 0)
		     putByte_StrBuf(out, CLIP_MEMVAR_PRIVATE_POP);
		  else
		     {
			putByte_StrBuf(out, CLIP_MEMVAR_SPACE_POP);
			putLong_StrBuf(out, np->space);
		     }
	       }
	    else
	       {
		  if (np->space == 1)
		     putByte_StrBuf(out, CLIP_MEMVAR_PUBLIC);
		  else if (np->space == 0)
		     putByte_StrBuf(out, CLIP_MEMVAR_PRIVATE);
		  else
		     {
			putByte_StrBuf(out, CLIP_MEMVAR_SPACE);
			putLong_StrBuf(out, np->space);
		     }
		  putLong_StrBuf(out, hashstr(np->vp->name));
	       }
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;

      }

   return 0;
}

Node *
new_CreateVarNode(Var * vp, long space, char *spacename)
{
   NEWVAR(CreateVarNode, ret);

   ins_name(spacename);
   ins_name(vp->name);
   init_Node(ret, pass_CreateVarNode, "createvar");
   ret->vp = vp;
   ret->space = space;
   ret->spacename = spacename;

   return (Node *) ret;
}

/* ] CreateVarNode */
/* [ PrivateDefNode */

static int
pass_PrivateDefNode(void *self, Pass pass, int level, void *par)
{
   VAR(DefNode, np, self);
   switch (pass)
      {
      case Print:
	 {
	    int i, count = np->cp->unsorted.count;

	    fprintfOffs(stdout, level, "private def node: %d vars\n", count);
	    for (i = 0; i < count; i++)
	       {
		  VAR(Var, vp, np->cp->unsorted.items[i]);
		  print_Var(vp, level + 1);
	       }
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return pass_Node(np, pass, level, par);
}

Node *
new_PrivateDefNode(VarColl * cp)
{
   int i;

   NEWVAR(DefNode, ret);

   init_Node(ret, pass_PrivateDefNode, "privatedef");

   for (i = 0; i < cp->coll.count; i++)
      {
	 int no;

	 VAR(Var, vp, cp->coll.items[i]);
	 Node *vap, *np = 0;

	 if (!vp->macro)
	    {
	       ins_name(vp->name);
	       if (!undeclaredName(vp->name, 1))
		  rm_VarColl(curFunction->VarColl_memvars_of_Function, vp);
	       if (search_VarColl(curFunction->VarColl_privates_of_Function, vp->name, &no))
		  ;		/* vp = (Var*) curFunction->VarColl_privates->coll.items[no]; */
	       else
		  {
#if 0
		     add_VarColl(curFunction->privates, vp);
		     prepend_Node(curFunction->body, new_CreateVarNode(vp, 0, strdup("private")));
#else
		     append_Node(&ret->node, new_CreateVarNode(vp, 0, strdup("private")));
#endif
		  }

	       vap = new_PrivateNode(vp);
	    }
	 else
	    {
	       vap = vp->macro;
	       append_Node(&ret->node, new_CreateVarNode(vp, 0, strdup("private")));
	      /*append_Node(curFunction->body, new_CreateVarNode(vp, 0, strdup("private"))); */
	    }

	 if (vp->init)
	    {
	       np = new_OperExprNode(new_AssignNode(vap, vp->init, '='));
	       np->line = vp->init->line;
	    }
	 else if (vp->arr)
	    {
	       np = new_OperExprNode(new_AssignNode(vap, new_NewArrayNode(vp->arr), '='));
	       np->line = vp->line;
	    }
	 else if (vp->macro)
	    {
	      /*np = new_OperExprNode(vp->macro); */
	    }

	 if (np)
	    append_Node(&ret->node, np);
      }

   ret->cp = cp;

   return (Node *) ret;
}

/* ] PrivateDefNode */
/* [ ParametersDefNode */

static int
pass_ParametersDefNode(void *self, Pass pass, int level, void *par)
{
   VAR(DefNode, np, self);
   switch (pass)
      {
      case Print:
	 {
	    int i, count = np->cp->unsorted.count;

	    fprintfOffs(stdout, level, "parameters def node: %d vars\n", count);
	    for (i = 0; i < count; i++)
	       {
		  VAR(Var, vp, np->cp->unsorted.items[i]);
		  print_Var(vp, level + 1);
	       }
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    int i, count = np->cp->unsorted.count;

	    print_line(self, level, out);
	    for (i = count - 1; i >= 0; --i)
	       {
		  VAR(Var, vp, np->cp->unsorted.items[i]);
		  fprintfOffs(out, level, "_clip_memvar_param( _mp, %ld /* %s */, %d );\n", (long) hashstr(vp->name), vp->name, vp->no);
	       }
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int i, count = np->cp->unsorted.count;

	    put_line(self, level, out);
	    for (i = count - 1; i >= 0; --i)
	       {
		  VAR(Var, vp, np->cp->unsorted.items[i]);
		  putByte_StrBuf(out, CLIP_MEMVAR_PARAM);
		  putLong_StrBuf(out, hashstr(vp->name));
		  putByte_StrBuf(out, vp->no);
	       }
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_ParametersDefNode(VarColl * cp)
{
   int i;

   NEWVAR(DefNode, ret);

   if (curFunction->VarColl_params_of_Function->coll.count)
      yyerror("PARAMETERS and '(' <paramlist> ')' are incompatible");

   init_Node(ret, pass_ParametersDefNode, "parametersdef");

   ret->cp = cp;
   ret->node.isExec = 0;

   for (i = 0; i < cp->unsorted.count; i++)
      {
	 VAR(Var, vp, cp->unsorted.items[i]);
	 if (vp->macro)
	    {
	       yyerror("macro variable name not allowed here");
	       continue;
	    }
	 if (vp->init)
	    {
	       yyerror("init expression not allowed here");
	       continue;
	    }
	 if (vp->arr)
	    {
	       yyerror("array declaration not allowed here");
	       continue;
	    }
	 ins_name(vp->name);
#if 0
	 if (undeclaredName(vp->name))
	    if (!undeclaredName(vp->name))
	       {
		  rm_VarColl(curFunction->memvars, vp);
	       }
#endif
	 add_VarColl(curFunction->VarColl_parameters_of_Function, vp);
#if 0
	 vp->isParameter = 1;
	 add_VarColl(curFunction->plocals, vp);
	 vp->init = (Node *) ret;
#endif
      }

#if 1
   return (Node *) ret;
#else
   return 0;
#endif
}

/* ] ParametersDefNode */
/* [ ParamNode */

static int
pass_ParamNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Param node: %s (%d, %d)\n", np->vp->name, np->vp->no, np->vp->pno);
	 break;
      case CalcDeep:
	 {
	   /*VAR(Function, fp, par); */
	   /*addDeep(fp,1); */
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintf(out, "\t_clip_param(_mp, %d, %d);\n", np->vp->pno, np->vp->no);
	 }
	 break;

      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_PARAM);
	    putByte_StrBuf(out, np->vp->pno);
	    putShort_StrBuf(out, np->vp->no);
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_ParamNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_ParamNode, "param");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->fp = curFunction;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

/* ] ParamNode */
/* [ LocalNode */

static int
pass_LocalNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level,
		     "Local node: %s (%d), isParam=%d(%d), isCodeParam=%d, isRef=%d, level=%d\n",
		     np->vp->name, np->vp->no, np->vp->isParam, np->vp->pno, np->vp->isCodeParam, np->vp->isRef, np->vp->level);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    if (np->vp->isCodeParam)
	       {
		  VAR(StrBuf, out, cp->buf);
		  putString_StrBuf(out, np->vp->name);
	       }
	    else
	       {
		  flush_codestr(cp, new_QuotNode(&np->node));
	       }
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_push_local( _mp, %d))) goto _trap_%d;\n", np->vp->no, np->node.seqNo);
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_LOCAL);
	    putShort_StrBuf(out, np->vp->no);
	 }
	 break;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    fprintf(out, "_clip_ref_local( _mp, %d)", np->vp->no);
	 }
	 break;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_REF_LOCAL);
	    putShort_StrBuf(out, np->vp->no);
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_LocalNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_LocalNode, "local");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isLocal = 1;
   ret->fp = curFunction;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

/* ] LocalNode */
/* [ StaticNode */

static int
pass_StaticNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Static node: %s %s (%d)\n", np->vp->func->name_of_Function, np->vp->name, np->vp->no);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    flush_codestr(cp, new_QuotNode(&np->node));
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_push_static( _mp, &s_%s_%s))) goto _trap_%d;\n", np->vp->func->name_of_Function, np->vp->name, np->node.seqNo);
	 }
	 break;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    fprintf(out, "&s_%s_%s", np->vp->func->name_of_Function, np->vp->name);
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_STATIC);
	    putShort_StrBuf(out, np->vp->no);
	 }
	 break;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_REF_STATIC);
	    putShort_StrBuf(out, np->vp->no);
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_StaticNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_StaticNode, "static");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

/* ] StaticNode */
/* [ MemvarNode */

static int
pass_MemvarNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Memvar node: %s (%d)\n", np->vp->name, np->vp->no);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "m->");
	    putString_StrBuf(out, np->vp->name);
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_memvar( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	 }
	 break;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    if (np->node.isAssignLval)
	       fprintf(out, "_clip_ref_memvar( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	    else
	       fprintf(out, "_clip_ref_memvar_noadd( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_MEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    if (np->node.isAssignLval)
	       putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR);
	    else
	       putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR_NOADD);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_MemvarNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_MemvarNode, "memvar");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isMemvar = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

/* ] MemvarNode */
/* [ FMemvarNode */

static int
pass_FMemvarNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "FMemvar node: %s (%d) isArg=%d firstMemvar=%d\n", np->vp->name, np->vp->no, np->isArg, np->p1);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, np->vp->name);
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (np->p1)
	       fprintfOffs(out, level, "if ( (_ret=_clip_memvarf( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	    else
	       fprintfOffs(out, level, "if ( (_ret=_clip_fmemvar( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	 }
	 break;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    fprintf(out, "_clip_ref_memvar( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	 }
	 break;
      case CTextRef:
	 {
	    VAR(FILE, out, par);
	    if (np->isArg)
	       {
		  fprintfOffs(out, level, "if ((_ret=_clip_ref( _mp, ");
		  fprintf(out, "_clip_ref_memvar_noadd( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
		  fprintf(out, ", 0 ))) goto _trap_%d;\n", np->node.seqNo);
	       }
	    else
	       {
		  fprintfOffs(out, level, "if ( (_ret=_clip_ref_fmemvar( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	       }
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (np->p1)
	       putByte_StrBuf(out, CLIP_PUSH_MEMVARF);
	    else
	       putByte_StrBuf(out, CLIP_PUSH_FMEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case OTextRef:
	 {
	    VAR(StrBuf, out, par);
	    if (np->isArg)
	       {
		  putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR_NOADD);
		  putLong_StrBuf(out, hashstr(np->vp->name));
		  putByte_StrBuf(out, CLIP_MAKE_REF);
		  putByte_StrBuf(out, 0);
	       }
	    else
	       {
		  putByte_StrBuf(out, CLIP_REF_FMEMVAR);
		  putLong_StrBuf(out, hashstr(np->vp->name));
	       }
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_FMemvarNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_FMemvarNode, "fmemvar");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isFMemvar = 1;
   ret->node.isMemvar = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

Node *
new_MemvarFNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_FMemvarNode, "fmemvar");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isFMemvar = 1;
   ret->node.isMemvar = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);
   ret->p1 = 1;

   return (Node *) ret;
}

/* ] FMemvarNode */
/* [ FieldNode */

static int
pass_FieldNode(void *self, Pass pass, int level, void *par)
{
   VAR(FieldNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Field node:\n");
	 if (np->area)
	    fprintfOffs(stdout, level + 1, "area: %s\n", np->area);
	 if (np->name)
	    fprintfOffs(stdout, level + 1, "name: %s\n", np->name);
	 if (np->areaExpr)
	    {
	       fprintfOffs(stdout, level + 1, "areaExpr:\n");
	       np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
	    }
	 if (np->nameExpr)
	    {
	       fprintfOffs(stdout, level + 1, "nameExpr:\n");
	       np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    }
	 return 0;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    if (np->area)
	       {
		  putString_StrBuf(out, np->area);
		  putString_StrBuf(out, "->");
	       }
	    else if (np->areaExpr)
	       {
		  if (np->areaExpr->isMacro)
		     {
			np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
			putString_StrBuf(out, "->");
		     }
		  else
		     {
			putString_StrBuf(out, "(");
			np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
			putString_StrBuf(out, ")->");
		     }
	       }
	    else
	       {
		  putString_StrBuf(out, "FIELD->");
	       }

	    if (np->name)
	       {
		  putString_StrBuf(out, np->name);
	       }
	    else if (np->nameExpr)
	       {
		  if (!np->nameExpr->isMacro)
		     putString_StrBuf(out, "(");
		  np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
		  if (!np->nameExpr->isMacro)
		     putString_StrBuf(out, ")");
	       }
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 3);
	    if (np->areaExpr)
	       np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
	    if (np->nameExpr)
	       np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	 }
	 return 0;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (np->name && !np->area && !np->areaExpr)
	       fprintfOffs(out, level, "if ( (_ret=_clip_field( _mp, %ld /* %s */, -1 ))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, np->node.seqNo);
	    else if (np->name && np->area)
	       fprintfOffs(out, level,
			   "if ( (_ret=_clip_field( _mp, %ld /* %s */, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, (long) n_hashstr(np->area), np->area, np->node.seqNo);
	    else if (np->name && np->areaExpr)
	       {
		  if (np->areaExpr->isMacro)
		     np->areaExpr->pass(np->areaExpr, CTextLval, level, par);
		  else
		     np->areaExpr->pass(np->areaExpr, pass, level, par);
		  fprintfOffs(out, level, "if ( (_ret=_clip_field( _mp, %ld /* %s */, _clip_pop_hash(_mp) ))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, np->node.seqNo);
	       }
	    else if (np->area && np->nameExpr)
	       {
		  fprintfOffs(out, level, "if ( (_ret=_clip_push_area( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) n_hashstr(np->area), np->area, np->node.seqNo);
		  np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
		  fprintfOffs(out, level, "if ( (_ret=_clip_pop_area( _mp ) )) goto _trap_%d;\n", np->node.seqNo);
	       }
	    else if (np->areaExpr && np->nameExpr)
	       {
		  if (np->areaExpr->isMacro)
		     np->areaExpr->pass(np->areaExpr, CTextLval, level, par);
		  else
		     np->areaExpr->pass(np->areaExpr, pass, level, par);
		  fprintfOffs(out, level, "if ( (_ret=_clip_push_area( _mp, _clip_pop_hash(_mp) ) )) goto _trap_%d;\n", np->node.seqNo);
		  np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
		  fprintfOffs(out, level, "if ( (_ret=_clip_pop_area( _mp ) )) goto _trap_%d;\n", np->node.seqNo);
	       }
	    else if (np->nameExpr)
	       {
		  if (np->nameExpr->isMacro)
		     np->nameExpr->pass(np->nameExpr, CTextLval, level + 1, par);
		  else
		     np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
		  fprintfOffs(out, level, "{ long _a, _f; _clip_pop_fieldhash(_mp, &_a, &_f); if ( (_ret=_clip_field( _mp, _f, _a )) ) goto _trap_%d; }\n", np->node.seqNo);
	       }
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (np->name && !np->area && !np->areaExpr)
	       {
		  putByte_StrBuf(out, CLIP_FIELD);
		  putLong_StrBuf(out, hashstr(np->name));
		  putLong_StrBuf(out, -1);
	       }
	    else if (np->name && np->area)
	       {
		  putByte_StrBuf(out, CLIP_FIELD);
		  putLong_StrBuf(out, hashstr(np->name));
		  putLong_StrBuf(out, n_hashstr(np->area));
	       }
	    else if (np->name && np->areaExpr)
	       {
		  if (np->areaExpr->isMacro)
		     np->areaExpr->pass(np->areaExpr, OTextLval, level, par);
		  else
		     np->areaExpr->pass(np->areaExpr, pass, level, par);
		  putByte_StrBuf(out, CLIP_FIELD_POP);
		  putLong_StrBuf(out, hashstr(np->name));
	       }
	    else if (np->area && np->nameExpr)
	       {
		  putByte_StrBuf(out, CLIP_PUSH_AREA);
		  putLong_StrBuf(out, n_hashstr(np->area));
		  np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
		  putByte_StrBuf(out, CLIP_POP_AREA);
	       }
	    else if (np->areaExpr && np->nameExpr)
	       {
		  if (np->areaExpr->isMacro)
		     np->areaExpr->pass(np->areaExpr, OTextLval, level, par);
		  else
		     np->areaExpr->pass(np->areaExpr, pass, level, par);
		  putByte_StrBuf(out, CLIP_PUSH_AREA_POP);
		  np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
		  putByte_StrBuf(out, CLIP_POP_AREA);
	       }
	    else if (np->nameExpr)
	       {
		  if (np->nameExpr->isMacro)
		     np->nameExpr->pass(np->nameExpr, OTextLval, level, par);
		  else
		     np->nameExpr->pass(np->nameExpr, pass, level, par);
		  putByte_StrBuf(out, CLIP_FIELD_POP_NAME);
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   if (np->areaExpr)
      np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
   if (np->nameExpr)
      np->nameExpr->pass(np->nameExpr, pass, level + 1, par);

   return 0;
}

Node *
new_FieldNode(Var * vp)
{
   NEWVAR(FieldNode, ret);

   init_Node(ret, pass_FieldNode, "field");
   ret->node.isExpr = 1;
  /*ret->node.isLval=1; */
   ret->node.isField = 1;
   ins_name(vp->name);

   ret->name = vp->name;
   ret->area = vp->alias;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

Node *
new2_FieldNode(char *area, Node * areaExpr, char *name, Node * nameExpr)
{
   NEWVAR(FieldNode, ret);

   init_Node(ret, pass_FieldNode, "field");
   ret->node.isExpr = 1;
  /*ret->node.isLval = 1; */
   ret->node.isField = 1;
  /*ret->node.isMemvar = 1; */
   ins_name(area);
   ins_name(name);

   ret->name = name;
   ret->area = area;
   ret->nameExpr = nameExpr;
   ret->areaExpr = areaExpr;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

/* ] FieldNode */
/* [ PublicNode */

static int
pass_PublicNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Public node: %s (%d)\n", np->vp->name, np->vp->no);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    flush_codestr(cp, new_QuotNode(&np->node));
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_public( _mp, %ld /* %s */ ) )) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	 }
	 break;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    fprintf(out, "_clip_ref_public( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_PUBLIC);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_REF_PUBLIC);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_PublicNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_PublicNode, "public");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isMemvar = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

/* ] PublicNode */
/* [ PrivateNode */

static int
pass_PrivateNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Private node: %s (%d)\n", np->vp->name, np->vp->no);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    flush_codestr(cp, new_QuotNode(&np->node));
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_memvar( _mp, %ld /* %s */ ) )) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	 }
	 break;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    fprintf(out, "_clip_ref_memvar( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_MEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_PrivateNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_PrivateNode, "private");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isMemvar = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

/* ] PrivateNode */
/* [ ParametersNode */

static int
pass_ParameterNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Parameter node: %s (%d)\n", np->vp->name, np->vp->no);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    flush_codestr(cp, new_QuotNode(&np->node));
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_memvar( _mp, %ld /* %s */ ) )) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	 }
	 break;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    fprintf(out, "_clip_ref_memvar( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_MEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_ParameterNode(Var * vp)
{
   NEWVAR(VarNode, ret);

   init_Node(ret, pass_ParameterNode, "parameter");
   ret->vp = vp;
   ret->node.isExpr = 1;
   ret->node.isLval = 1;
   ret->node.isMemvar = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(vp->name);

   return (Node *) ret;
}

/* ] ParametersNode */
/* [ ArgNode */

static int
pass_ArgNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Arg node: byRef==%d\n", np->i);
	 break;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (np->i)
	       {
		  fprintfOffs(out, level, "if ((_ret=_clip_ref( _mp, ");
		 /*if (np->expr->isConst) */
		  fprintf(out, "(ClipVar*)");
		  np->expr->pass(np->expr, CTextLval, level, par);
		  fprintf(out, ", 0 ))) goto _trap_%d;\n", np->node.seqNo);
	       }
	    else
	       np->expr->pass(np->expr, pass, level, par);
	   /*
	      if (np->i)
	      fprintfOffs(out, level, "_clip_unref_arr( _mp );\n");
	    */
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (np->i)
	       {
		  np->expr->pass(np->expr, OTextLval, level, par);
		  putByte_StrBuf(out, CLIP_MAKE_REF);
		  putByte_StrBuf(out, 0);
	       }
	    else
	       np->expr->pass(np->expr, pass, level, par);
	   /*
	      if (np->i)
	      putByte_StrBuf(out, CLIP_UNREF_ARR);
	    */
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_ArgNode(Node * expr, int byRef)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ArgNode, "arg");
   ret->expr = expr;
   ret->node.isExpr = 1;
   ret->node.isLval = expr->isLval;
   ret->node.isConst = expr->isConst;
   ret->i = byRef;
   if (byRef && expr->isConst)
      yyerror("reference to constant");
   if (byRef && !expr->isLval)
      yyerror("reference to temporary object");

   return (Node *) ret;
}

/* ] ArgNode */
/* [ RefNode */

static int
pass_RefNode(void *self, Pass pass, int level, void *par)
{
   VAR(RefNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Ref node: %s selfref=%d\n", np->name ? np->name : "", np->selfref);
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 2);
	    if (np->name)
	       {
		  Function buf;

		  int no;

		  buf.name_of_Function = np->name;
		  buf.isPublic_of_Function = 1;
		  if (!search_Coll(&curFile->Coll_functions_of_File, &buf, &no))
		     {
			if (!search_Coll(&curFile->Coll_externFunctions_of_File, np->name, &no))
			   {
			      yyerror("reference to undeclared function '%s' near line %d", np->name, np->node.line);
			      return 0;
			   }
		     }
		  else
		     {
			np->fp = (Function *) curFile->Coll_functions_of_File.items[no];
			np->name = np->fp->name_of_Function;
		     }
	       }
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (np->expr)
	       {
		  if (np->expr->isArrEl)
		     {
			VAR(ArrElNode, ap, np->expr);
			if (!strcmp(ap->oarr->name, "call"))
			   np->expr->pass(np->expr, CText, level, par);
			else
			   {
			      np->expr->pass(np->expr, CTextLval, level, par);
			      fprintfOffs(out, level, "if ((_ret=_clip_ref( _mp, _clip_pop_ref(_mp), 0 ))) goto _trap_%d;\n", np->node.seqNo);
			   }
		     }
		  else if (np->expr->isMacro)
		     {
			np->expr->pass(np->expr, CTextLval, level, par);
			fprintfOffs(out, level, "if ((_ret=_clip_refmacro(_mp) )) goto _trap_%d;\n", np->node.seqNo);
		     }
		  else if (np->expr->isField)
		     {
			np->expr->pass(np->expr, CText, level, par);
		     }
		  else if (np->expr->isFMemvar)
		     {
			np->expr->pass(np->expr, CTextRef, level, par);
		     }
		  else
		     {
			fprintfOffs(out, level, "if ((_ret=_clip_ref( _mp, ");
			np->expr->pass(np->expr, CTextLval, level, par);
			fprintf(out, ", %d ))) goto _trap_%d;\n", (np->selfref && np->expr->isLocal) ? 1 : 0, np->node.seqNo);
		     }
	       }
	    else
	       {
		  fprintfOffs(out, level, "_clip_push_func( _mp, clip_%s, 0, 1 );\n", np->name);
	       }
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (np->expr)
	       {
		  if (np->expr->isMacro)
		     {
			np->expr->pass(np->expr, OTextLval, level, par);
			putByte_StrBuf(out, CLIP_REFMACRO);
		     }
		  else if (np->expr->isField)
		     {
			np->expr->pass(np->expr, OText, level, par);
		     }
		  else if (np->expr->isFMemvar)
		     {
			np->expr->pass(np->expr, OTextRef, level, par);
		     }
		  else
		     {
			int done = 0;

			if (np->expr->isArrEl)
			   {
			      VAR(ArrElNode, ap, np->expr);
			      if (!strcmp(ap->oarr->name, "call"))
				 {
				    np->expr->pass(np->expr, OText, level, par);
				    done = 1;
				 }
			   }

			if (!done)
			   {
			      np->expr->pass(np->expr, OTextLval, level, par);
			      putByte_StrBuf(out, CLIP_MAKE_REF);
			      putByte_StrBuf(out, (np->selfref && np->expr->isLocal) ? 1 : 0);
			   }
		     }
	       }
	    else
	       {
		  putByte_StrBuf(out, CLIP_PUSH_CODE);
		  putShort_StrBuf(out, np->fp->mno_of_Function);
		  putByte_StrBuf(out, 0);
		  putByte_StrBuf(out, 1);
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   if (np->expr)
      np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_RefNode(Node * expr)
{
   RefNode *ret;

   if (expr->isNil)
      {
	 Node *r = new_Node();

	 r->isNil = 1;
	 r->isRef = 1;
	 return r;
      }

   ret = NEW(RefNode);
   init_Node(ret, pass_RefNode, "ref");
   ret->node.isExpr = 1;
   ret->node.isLval = 0;
   ret->node.isConst = 0;
   if (expr->isConst)
      yyerror("reference to constant");

#if 0
   if (!strcmp(expr->name, "macro"))
      {
	 yyerror("reference to macro not allowed");
      }
#endif

   if (strcmp(expr->name, "call"))
      {
	 if (!expr->isLval && !expr->isField)
	    yyerror("reference to non-lvalue object");
	 ret->expr = expr;
	 ret->node.isRef = 1;
      }
   else
      {
	 CallNode *np = (CallNode *) expr;

	 if (np->argc)
	    yyerror("reference to function with args not allowed");
	 else
	    ret->name = strdup(np->name);
      }

   return (Node *) ret;
}

/* ] RefNode */
/* [ CallNode */

static const char *
ref_name(CallNode * np)
{
#if 1
   return "_localvars";
#else
   if (!np || !np->cfunc || !np->cfunc->reflocals)
      return "0";
   else
      return "_reflocals";
#endif
}

static int
pass_CallNode(void *self, Pass pass, int level, void *par)
{
   VAR(CallNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "CallNode: %s (%d) byref=%d, isC=%d, rest=%d\n", np->name, np->argc, np->byref, np->isC, np->rest);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    VAR(Node, npp, self);
	    int r, n = 0;

	    putString_StrBuf(out, np->name);
	    putString_StrBuf(out, "(");
	    for (r = first_List(&npp->list); r; r = next_List(&npp->list))
	       {
		  VAR(Node, p, npp->list.current);
		  if (n)
		     putString_StrBuf(out, ", ");
		  n++;
		  if (p->pass(p, pass, level + 1, par))
		     break;
	       }
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case Codegen1:
	 {
	    Node *npp = (Node *) np;

	    int r;

	    for (r = first_List(&npp->list); r; r = next_List(&npp->list))
	       {
		  VAR(Node, p, npp->list.current);
		  if (!strcmp(p->name, "ref"))
		     {
			Node *np1 = ((RefNode *) p)->expr;

			if (np1 && !strcmp(np1->name, "fmemvar"))
			   {
			      ((VarNode *) np1)->isArg = 1;
			   }
		     }
	       }
	 }
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    int deep, ret;

	    np->isC = add_to_undeclared(np->name, &np->func);
	    addDeep(fp, 1);
	    deep = fp->deep_of_Function;
	    ret = pass_Node(self, pass, level, par);
	    fp->deep_of_Function = deep;
	    if (np->func && !np->func->isPublic_of_Function)
	       np->name = np->func->name_of_Function;
	    return ret;
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	    pass_Node(self, pass, level - 1, par);
	    switch (np->isC)
	       {
	       case 2:
		  fprintfOffs(out, level, "if ( (_ret=_clip_%s(_mp, clip_%s, %d, %d, %s))) goto _trap_%d;\n", np->node.isTop ? "proc" : "func", np->name, np->argc, np->rest, ref_name(np),
			      np->node.seqNo);
		  break;
	       case 1:
	       case 0:
		  fprintfOffs(out, level,
			      "if ( (_ret=_clip_%s_hash(_mp, %ld /* %s */, %d, %d, %s))) goto _trap_%d;\n",
			      np->node.isTop ? "proc" : "func", (long) hashstr(np->name), np->name, np->argc, np->rest, ref_name(np), np->node.seqNo);
		  break;
	       }
	    return 0;
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_NIL);
	    pass_Node(self, pass, level - 1, par);
	    switch (np->isC)
	       {
	       case 2:
		  if (np->node.isTop)
		     putByte_StrBuf(out, np->rest ? CLIP_SPROCR : CLIP_SPROC);
		  else
		     putByte_StrBuf(out, np->rest ? CLIP_SFUNCR : CLIP_SFUNC);
		  putByte_StrBuf(out, np->argc);
		  putShort_StrBuf(out, np->func->mno_of_Function);
		  break;
	       case 1:
	       case 0:
		  if (np->node.isTop)
		     putByte_StrBuf(out, np->rest ? CLIP_PROCR : CLIP_PROC);
		  else
		     putByte_StrBuf(out, np->rest ? CLIP_FUNCR : CLIP_FUNC);
		  putByte_StrBuf(out, np->argc);
		  putLong_StrBuf(out, hashstr(np->name));
		  break;
	       }
	    return 0;
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return pass_Node(self, pass, level, par);
}

static void
gettext_CallNode(CallNode * np)
{
   if (!strcasecmp(np->name, "_clic_gettext"))
      {
	 Node *npp = (Node *) np;

	 int r;

	 char *msg = 0;

	 Node *module = 0;

	 int no = 0;

	 for (r = first_List(&npp->list); r; r = next_List(&npp->list), no++)
	    {
	       VAR(Node, p, npp->list.current);
	       if (!strcmp(p->name, "const"))
		  {
		     ConstNode *cp = (ConstNode *) p;

		     if (cp->type == CONST_STRING)
			{
			   if (no == 0)
			      msg = cp->val;
			}
		  }
	       if (no == 1)
		  module = p;
	    }
	 if (msg && !module)
	    put_locale_string(msg);
	 if (!module && no == 1)
	    {
	       append_Node(&np->node, new_StringConstNode(strdup(CLIP_MODULE)));
	       np->argc++;
	    }
      }
   else if (!strcasecmp(np->name, "_clic_ngettext"))
      {
	 Node *npp = (Node *) np;

	 int r;

	 int no = 0;

	 char *singular = 0;

	 char *plural = 0;

	 Node *module = 0;

	 for (r = first_List(&npp->list); r; r = next_List(&npp->list), no++)
	    {
	       VAR(Node, p, npp->list.current);
	       if (!strcmp(p->name, "const"))
		  {
		     ConstNode *cp = (ConstNode *) p;

		     if (cp->type == CONST_STRING)
			{
			   if (no == 0)
			      singular = cp->val;
			   else if (no == 1)
			      plural = cp->val;
			}
		  }
	       if (no == 3)
		  module = p;
	    }
	 if (singular && plural /*&& !module */ )
	    put_locale_string_plural(singular, plural);
	 if (!module && no == 3)
	    {
	       append_Node(&np->node, new_StringConstNode(strdup(CLIP_MODULE)));
	       np->argc++;
	    }
      }
}

Node *
new_CallNode(char *name, Coll * argv, int rest)
{
   int i;

   char *s;

   NEWVAR(CallNode, ret);
   init_Node(ret, pass_CallNode, "call");
   if (search_Coll(&reffuncs, name, 0))
      ret->byref = 1;
   for (i = 0; i < argv->count; i++)
      {
	 VAR(Node, np, argv->items[i]);
	 if (ret->byref && !np->isRef && !np->isConst && !np->isField && !np->isFMemvar && np->isLval && !np->isMacro)
	    {
	       np = new_RefNode(np);
	    }
	 append_Node(&ret->node, np);
      }
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = argv->count;
   ret->isC = 1;
   ret->rest = rest;
   ret->cfunc = curFunction;
   curFunction->goto_trap_of_Function++;

   ret->node.isAssign = 1;
   ins_name(name);

   delete_Coll(argv);
   gettext_CallNode(ret);
   return (Node *) ret;
}

Node *
newList_CallNode(char *name, Node * exprlist)
{
   char *s;

   NEWVAR(CallNode, ret);

   init_Node(ret, pass_CallNode, "call");
   join_Node((Node *) ret, exprlist);
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = getCount_Node((Node *) ret);
   ret->isC = 1;
   ret->cfunc = curFunction;
   ins_name(name);
   curFunction->goto_trap_of_Function++;

   gettext_CallNode(ret);
   return (Node *) ret;
}

Node *
new2_CallNode(const char *name, Node * expr1, Node * expr2)
{
   return new3_CallNode(name, expr1, expr2, 0);
}

Node *
new3_CallNode(const char *name, Node * expr1, Node * expr2, Node * expr3)
{
   char *s;

   int argc = 0;

   NEWVAR(CallNode, ret);

   init_Node(ret, pass_CallNode, "call");
   if (expr1)
      {
	 append_Node(&ret->node, expr1);
	 argc++;
      }
   if (expr2)
      {
	 append_Node(&ret->node, expr2);
	 argc++;
      }
   if (expr3)
      {
	 append_Node(&ret->node, expr3);
	 argc++;
      }
   ret->name = strdup(name);
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ins_name((char *) name);
   ret->argc = argc;
  /*ret->isC = 2; */
   ret->isC = 1;
   ret->cfunc = curFunction;
   curFunction->goto_trap_of_Function++;

   gettext_CallNode(ret);
   return (Node *) ret;
}

/*static int local_get_no = 0;*/

Node *
new_CGetNode(Coll * argv)
{
   Node *np, *ip, *sp, *ap;

   if (argv->count < 1)
      goto ret;

   np = (Node *) argv->items[0];

   if (np->isConst)
      {
	 yyerror("cannot GET constant");
	 goto ret;
      }

   if (np->isArrEl)
      {
	 int r, n;

	 Node *npp, *ep;

	 ap = npp = ((ArrElNode *) np)->arr;
	 ep = new_ExprArrNode();

	 for (n = 0, r = first_List(&npp->list); r; r = next_List(&npp->list), n++)
	    {
	       VAR(Node, p, npp->list.current);
	       if (!n)
		  ap = p;
	       else
		  append_Coll(&(((ExprArrNode *) ep)->coll), p);
	    }

	 ip = new_ArrayInitNodeN(ep, ((ExprArrNode *) ep)->coll.count);
      }
   else
      {
	 ap = np;
	 ip = new_NilConstNode();
      }

   if (ap->isMemvar)
      sp = new_StringConstNode(strdup(((VarNode *) ap)->vp->name));
   else if (ap->isMacro)
      {
	 ExprNode *ep = (ExprNode *) ap;

	 sp = ep->expr;
      }
   else if (ap->isField)
      {
#if 1
	 sp = new_CodestrNode(ap, 0, 0);
#else
	 StrBuf *bp;

	 CodestrNode cn;

	 bp = new_StrBuf();
	 memset(&cn, 0, sizeof(cn));
	 cn.buf = bp;
	 init_Coll(&cn.coll, 0, 0);
	 ap->pass(ap, PrintSrc, 0, &cn);
	 flush_codestr(&cn, 0);
	 putByte_StrBuf(bp, 0);
	 sp = new_StringConstNode(strdup(bp->buf));
	 destroy_Coll(&cn.coll);
	 delete_StrBuf(bp);
#endif
      }
   else
      sp = new_NilConstNode();

#if 1
   {
      Var *vp, *vp2;

      VarColl *vc, *vc2;

      Node *cp, *el, *ldp;

      Function *fp;

      Coll *icp;

      vp = new_Var(strdup("_1"));
      vc = new_VarColl();
      add_VarColl(vc, vp);

      if (!np->isRef)
	 np = new_RefNode(np);
      vp2 = newInit_Var(strdup("_P"), np);

      fp = curFunction;
      cp = new_CodeNode(vc);

      vc2 = new_VarColl();
      add_VarColl(vc2, vp2);
      ldp = new_LocalDefNode(vc2, 0, 0);
      vp2->isLocalRef = 1;
     /*prepend_Node(curFunction->body, ldp); */

      icp = new_Coll(NULL, NULL);
      append_Coll(icp, new_OpNode(new_LocalNode(vp), new_NilConstNode(), 'E'));
      append_Coll(icp, new_LocalNode(vp2));
      el = new_IfNode(icp, new_AssignNode(new_LocalNode(vp2), new_LocalNode(vp), '='));

      fin_Function(curFunction);
      append_Node(curFunction->Node_body_of_Function, new_ReturnNode(el, 0 /*1 */ ));

      curFunction = fp;
      np = cp;
   }
#else
   if (!np->isRef && !np->isConst	/*&& !np->isField
					   && np->isLval && !np->isMacro */ )
      {
	 np = new_RefNode(np);
      }

   {
      Var *vp;

      char vname[64];

      snprintf(vname, sizeof(vname), "__local_get_%d", local_get_no++);
      vp = new_Var(strdup(vname));
      ins_name(vp->name);
      add_VarColl(curFunction->locals, vp);
      np = new_AssignNode(new_LocalNode(vp), np, '=');
   }
#endif

   argv->items[0] = np;

   atInsert_Coll(argv, sp, 1);
   atInsert_Coll(argv, ip, 1);

 ret:
   return new_CallNode(strdup("__CGET__"), argv, 0);
}

/* ] CallNode */
/* [ ExprListNode */

static int
pass_ExprListNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprListNode, npp, self);

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ExprListNode: isLast %d\n", npp->last);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    VAR(Node, np, self);
	    int r, n = 0;

	    for (r = first_List(&np->list); r; r = next_List(&np->list))
	       {
		  VAR(Node, p, np->list.current);
		  if (n)
		     putString_StrBuf(out, ", ");
		  n++;
		  if (p->pass(p, pass, level + 1, par))
		     break;
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      case CText:
	 {
	    int r;

	    VAR(FILE, out, par);
	    VAR(Node, np, self);
	    for (r = first_List(&np->list); r; r = next_List(&np->list))
	       {
		  VAR(Node, p, np->list.current);
		  p->pass(p, pass, level + 1, par);
		  if (p->listel.next != np->list.head && npp->last)
		     fprintfOffs(out, level + 1, "_clip_pop(_mp);\n");
	       }

	 }
	 return 0;
      case OText:
	 {
	    int r;

	    VAR(StrBuf, out, par);
	    VAR(Node, np, self);
	    for (r = first_List(&np->list); r; r = next_List(&np->list))
	       {
		  VAR(Node, p, np->list.current);
		  p->pass(p, pass, level + 1, par);
		  if (p->listel.next != np->list.head && npp->last)
		     putByte_StrBuf(out, CLIP_POP);
	       }

	 }
	 return 0;
      default:
	 break;
      }
   return pass_Node(self, pass, level - 1, par);
}

Node *
new_ExprListNode()
{
   NEWVAR(ExprListNode, ret);

   init_Node(ret, pass_ExprListNode, "exprlist");
   ret->node.isExpr = 1;
   ret->node.isExprList = 1;

   return (Node *) ret;
}

/* ] ExprListNode */
/* [ ExprArrNode */

static int
pass_ExprArrNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprArrNode, npp, self);

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ExprArrNode: %d, islast=%d\n", npp->coll.count, npp->last);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    int n;

	    for (n = 0; n < npp->coll.count; n++)
	       {
		  VAR(Node, p, npp->coll.items[n]);
		  if (n)
		     putString_StrBuf(out, ", ");
		  if (p->pass(p, pass, level + 1, par))
		     break;
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      case CText:
	 {
	    int n, e;

	    VAR(FILE, out, par);
	    for (n = 0, e = npp->coll.count; n < e; n++)
	       {
		  VAR(Node, p, npp->coll.items[n]);
		  p->pass(p, pass, level + 1, par);
		  if (n == (e - 1) && npp->last)
		     fprintfOffs(out, level + 1, "_clip_pop(_mp);\n");
	       }

	 }
	 return 0;
      case OText:
	 {
	    int n, e;

	    VAR(StrBuf, out, par);
	    for (n = 0, e = npp->coll.count; n < e; n++)
	       {
		  VAR(Node, p, npp->coll.items[n]);
		  p->pass(p, pass, level + 1, par);
		  if (n == (e - 1) && npp->last)
		     putByte_StrBuf(out, CLIP_POP);
	       }

	 }
	 return 0;
      default:
	 break;
      }

   {
      int n, e;

      for (n = 0, e = npp->coll.count; n < e; n++)
	 {
	    VAR(Node, p, npp->coll.items[n]);
	    p->pass(p, pass, level + 1, par);
	 }
   }
   return 0;
}

Node *
new_ExprArrNode()
{
   NEWVAR(ExprArrNode, ret);

   init_Node(ret, pass_ExprArrNode, "exprarr");
   ret->node.isExpr = 1;
   ret->node.isExprList = 1;
   init_Coll(&(ret->coll), 0, 0);

   return (Node *) ret;
}

/* ] ExprArrNode */
/* [ AssignNode */

static int
pass_AssignNode(void *self, Pass pass, int level, void *par)
{
   VAR(AssignNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "AssignNode: %c\n", np->op ? np->op : ' ');
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    np->var->pass(np->var, pass, level + 1, par);
	    switch (np->op)
	       {
	       case 0:
	       case '=':
	       default:
		  putString_StrBuf(out, ":=");
		  break;
	       case '+':
		  putString_StrBuf(out, "+=");
		  break;
	       case '-':
		  putString_StrBuf(out, "-=");
		  break;
	       case '*':
		  putString_StrBuf(out, "*=");
		  break;
	       case '/':
		  putString_StrBuf(out, "/=");
		  break;
	       }
	    np->expr->pass(np->expr, pass, level + 1, par);
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);

	    if (np->expr->isRef && np->expr->isNil)
	       {
		  fprintfOffs(out, level, "if ((_ret=_clip_unref( _mp, ");
		  np->var->pass(np->var, CTextLval, level, par);
		  fprintf(out, " ))) goto _trap_%d;\n", np->node.seqNo);
		  return 0;
	       }

	    np->expr->pass(np->expr, pass, level, par);

	    if (np->expr->isRef)
	       {
		  fprintfOffs(out, level, "if ((_ret=_clip_refassign( _mp, ");
		  np->var->pass(np->var, CTextLval, level, par);
		  fprintf(out, ", %d ))) goto _trap_%d;\n", np->node.isTop ? 1 : 0, np->node.seqNo);
	       }
	    else if (np->var->isMacro)
	       {
		  np->var->pass(np->var, CTextLval, level, par);
		  fprintfOffs(out, level, "if ((_ret=_clip_macroassign( _mp, '%c', %d, %d))) goto _trap_%d;\n", np->op, np->node.isTop, np->field_flag, np->node.seqNo);

	       }
	    else
	       {
		  int islocal = 0;

		  if (np->var->isLocal)
		     {
			VAR(VarNode, lp, np->var);
			if (!lp->vp->isParam && np->op == '='
			   /*&& !(lp->vp->init && lp->vp->init->isRef) */
			    && !lp->vp->isLocalRef)
			   islocal = 1;
		     }
		  if (np->node.isTop)
		     {
			if (np->op != '=')
			   {
			      fprintfOffs(out, level, "if ((_ret=_clip_opassign( _mp, ");
			      np->var->pass(np->var, CTextLval, level, par);
			      fprintf(out, ", %d ))) goto _trap_%d;\n", np->op, np->node.seqNo);
			   }
			else
			   {
			      fprintfOffs(out, level, "if ((_ret=_clip_assign( _mp, ");
#define D20061219
/*
bug in:
ndata := 0
@0,0 get ndata
readmodal(getlist)
ndata := 0   // this line crashed reference in get-object
readmodal(getlist)
*/
#ifndef D20061219
			      if (islocal)
				 fprintf(out, "_clip_ref_destroy(_mp ,");
#endif
			      np->var->pass(np->var, CTextLval, level, par);
#ifndef D20061219
			      if (islocal)
				 fprintf(out, ")");
#endif
			      fprintf(out, " ))) goto _trap_%d;\n", np->node.seqNo);
			   }
		     }
		  else
		     {
			if (np->op != '=')
			   {
			      fprintfOffs(out, level, "if ((_ret=_clip_opiassign( _mp, ");
			      np->var->pass(np->var, CTextLval, level, par);
			      fprintf(out, ", %d ))) goto _trap_%d;\n", np->op, np->node.seqNo);
			   }
			else
			   {
			      fprintfOffs(out, level, "if ((_ret=_clip_iassign( _mp, ");
			      if (islocal)
				 fprintf(out, "_clip_ref_destroy(_mp ,");
			      np->var->pass(np->var, CTextLval, level, par);
			      if (islocal)
				 fprintf(out, ")");
			      fprintf(out, " ))) goto _trap_%d;\n", np->node.seqNo);
			   }
		     }
	       }
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (np->expr->isRef && np->expr->isNil)
	       {
		  np->var->pass(np->var, OTextLval, level, par);
		  putByte_StrBuf(out, CLIP_UNREF);
		  return 0;
	       }
	    np->expr->pass(np->expr, pass, level, par);
	    if (np->expr->isRef)
	       {
		  np->var->pass(np->var, OTextLval, level, par);
		  putByte_StrBuf(out, CLIP_REFASSIGN);
		  putByte_StrBuf(out, np->node.isTop ? 1 : 0);
	       }
	    else if (np->var->isMacro)
	       {
		  unsigned char b;

		  np->var->pass(np->var, OTextLval, level, par);
		  putByte_StrBuf(out, CLIP_MACROASSIGN);
		  putByte_StrBuf(out, np->op);
		  b = (np->node.isTop ? 1 : 0) | (np->field_flag ? 2 : 0);
		  putByte_StrBuf(out, b);
	       }
	    else
	       {
		  np->var->pass(np->var, OTextLval, level, par);
		  if (np->var->isLocal)
		     {
			VAR(VarNode, lp, np->var);
			if (!lp->vp->isParam && np->op == '='
			   /*&& !(lp->vp->init && lp->vp->init->isRef) */
			    && !lp->vp->isLocalRef)
			   {
			      putByte_StrBuf(out, CLIP_REF_DESTROY);
			   }
		     }

		  if (np->node.isTop)
		     {
			if (np->op == '=')
			   putByte_StrBuf(out, CLIP_ASSIGN);
			else
			   {
			      putByte_StrBuf(out, CLIP_OPASSIGN);
			      putByte_StrBuf(out, np->op);
			   }
		     }
		  else
		     {
			if (np->op == '=')
			   putByte_StrBuf(out, CLIP_IASSIGN);
			else
			   {
			      putByte_StrBuf(out, CLIP_OPIASSIGN);
			      putByte_StrBuf(out, np->op);
			   }
		     }
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->var->pass(np->var, pass, level + 1, par);
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_AssignNode(Node * var, Node * expr, int op)
{
   AssignNode *ret;

   if (var->isField)
      {
	 FieldNode *fp = (FieldNode *) var;

	 if (op != '=')
	    {
	       return new_AssignFieldNode(fp->area, fp->areaExpr, fp->name, fp->nameExpr, new_OpNode(var, expr, op));
	    }
	 else
	    {
	       return new_AssignFieldNode(fp->area, fp->areaExpr, fp->name, fp->nameExpr, expr);
	    }
      }

   if (!var->isLval)
      yyerror("assign to non-lvalue");

   if (var->isMacro && expr->isRef)
      yyerror("reference to macro assign not allowed");

   if (op == '=' && field_flag && !strcmp(var->name, "fmemvar"))
      {
	 return new_AssignFmemvarNode(var, expr);
      }
   var->isAssignLval = 1;

   ret = NEW(AssignNode);
   init_Node(ret, pass_AssignNode, "assign");
   ret->node.isExpr = 1;

   ret->var = var;
   ret->expr = expr;
   ret->op = op;
   ret->node.isAssign = 1;
   ret->field_flag = field_flag;

   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

/* ] AssignNode */
/* [ AssignFmemvarNode */

static int
pass_AssignFmemvarNode(void *self, Pass pass, int level, void *par)
{
   VAR(AssignNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "AssignFmemvarNode:\n");
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, ((VarNode *) np->var)->vp->name);
	    putString_StrBuf(out, ":=");
	    np->expr->pass(np->expr, pass, level, par);
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    VAR(VarNode, vp, np->var);

	    np->expr->pass(np->expr, pass, level, par);
	    if (np->node.isTop)
	       {
		  fprintfOffs(out, level, "if ((_ret=_clip_fm_assign( _mp, " "%ld /* %s */  ))) goto _trap_%d;\n", (long) hashstr(vp->vp->name), vp->vp->name, np->node.seqNo);
	       }
	    else
	       {
		  fprintfOffs(out, level, "if ((_ret=_clip_fm_iassign( _mp, " "%ld /* %s */  ))) goto _trap_%d;\n", (long) hashstr(vp->vp->name), vp->vp->name, np->node.seqNo);
	       }
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    VAR(VarNode, vp, np->var);

	    np->expr->pass(np->expr, pass, level, par);
	    if (np->node.isTop)
	       {
		  putByte_StrBuf(out, CLIP_FM_ASSIGN);
		  putLong_StrBuf(out, hashstr(vp->vp->name));
	       }
	    else
	       {
		  putByte_StrBuf(out, CLIP_FM_IASSIGN);
		  putLong_StrBuf(out, hashstr(vp->vp->name));
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->var->pass(np->var, pass, level + 1, par);
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_AssignFmemvarNode(Node * var, Node * expr)
{
   AssignNode *ret;

   ret = NEW(AssignNode);
   init_Node(ret, pass_AssignFmemvarNode, "assignfmemvar");
   ret->node.isExpr = 1;

   ret->var = var;
   ret->expr = expr;
   ret->node.isAssign = 1;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

/* ] AssignFmemvarNode */
/* [ AssignFieldNode */

static int
pass_AssignFieldNode(void *self, Pass pass, int level, void *par)
{
   VAR(AssignFieldNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "AssignFieldNode: %s\n", np->name ? np->name : "");
	 fprintfOffs(stdout, level, "area: %s\n", np->area ? np->area : "");
	 if (np->areaExpr)
	    np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
	 if (np->nameExpr)
	    {
	       fprintfOffs(stdout, level, "field:\n");
	       np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    }
	 else
	    fprintfOffs(stdout, level, "field: %s\n", np->name);
	 fprintfOffs(stdout, level, "expr:\n");
	 np->expr->pass(np->expr, pass, level + 1, par);
	 return 0;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    if (np->area)
	       {
		  putString_StrBuf(out, np->area);
		  putString_StrBuf(out, "->");
	       }
	    else if (np->areaExpr)
	       {
		  putString_StrBuf(out, "(");
		  np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
		  putString_StrBuf(out, ")");
	       }
	    else
	       {
		  putString_StrBuf(out, "FIELD->");
	       }

	    if (np->name)
	       {
		  putString_StrBuf(out, np->name);
	       }
	    else if (np->nameExpr)
	       {
		  if (!np->nameExpr->isMacro)
		     putString_StrBuf(out, "(");
		  np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
		  if (!np->nameExpr->isMacro)
		     putString_StrBuf(out, ")");
	       }
	    putString_StrBuf(out, ":=");
	    np->expr->pass(np->expr, pass, level + 1, par);
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);

	    np->expr->pass(np->expr, pass, level, par);
	    if (np->nameExpr)
	       {
		  if (np->nameExpr->isMacro)
		     {
			np->nameExpr->pass(np->nameExpr, CTextLval, level, par);
			if (np->areaExpr || np->area)
			   fprintfOffs(out, level, "_clip_calc_hash(_mp);\n");
			else
			   fprintfOffs(out, level, "_clip_calc_hash2(_mp, 0);\n");
		     }
		  else
		     np->nameExpr->pass(np->nameExpr, pass, level, par);
	       }
	    if (np->areaExpr)
	       {
		  if (np->areaExpr->isMacro)
		     np->areaExpr->pass(np->areaExpr, CTextLval, level, par);
		  else
		     np->areaExpr->pass(np->areaExpr, pass, level, par);
	       }

	    if (np->node.isTop)
	       fprintfOffs(out, level, "if ( (_ret=_clip_assign_field");
	    else
	       fprintfOffs(out, level, "if ( (_ret=_clip_iassign_field");

	    if (np->nameExpr)
	       {
		  fprintf(out, "( _mp, _clip_pop_hash(_mp), ");
		  if (np->nameExpr->isMacro && !np->areaExpr && !np->area)
		     fprintf(out, "_clip_pop_hash(_mp) ))) goto _trap_%d;\n", np->node.seqNo);
	       }
	    else
	       fprintf(out, "( _mp, %ld /* %s */, ", (long) hashstr(np->name), np->name);

	    if (np->areaExpr)
	       {
		  fprintf(out, "_clip_pop_hash(_mp)))) goto _trap_%d;\n", np->node.seqNo);
	       }
	    else
	       {
		  if (np->area)
		     fprintf(out, "%ld /* %s */))) goto _trap_%d;\n", (long) n_hashstr(np->area), np->area, np->node.seqNo);
		  else if (np->nameExpr && np->nameExpr->isMacro)
		     ;
		  else
		     fprintf(out, "-1))) goto _trap_%d;\n", np->node.seqNo);
	       }
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);

	    np->expr->pass(np->expr, pass, level, par);
	    if (np->areaExpr)
	       {
		  if (np->areaExpr->isMacro)
		     np->areaExpr->pass(np->areaExpr, OTextLval, level, par);
		  else
		     np->areaExpr->pass(np->areaExpr, pass, level, par);
	       }

	    if (np->nameExpr)
	       {
		  if (np->nameExpr->isMacro)
		     {
			np->nameExpr->pass(np->nameExpr, OTextLval, level, par);
			if (np->areaExpr || np->area)
			   putByte_StrBuf(out, CLIP_CALC_HASH);
			else
			   putByte_StrBuf(out, CLIP_CALC_HASH2);
		     }
		  else
		     np->nameExpr->pass(np->nameExpr, pass, level, par);
	       }
	    if (np->areaExpr)
	       {
		  if (np->nameExpr)
		     {
			if (np->node.isTop)
			   putByte_StrBuf(out, CLIP_ASSIGN_PFIELD_POP);
			else
			   putByte_StrBuf(out, CLIP_IASSIGN_PFIELD_POP);
		     }
		  else
		     {
			if (np->node.isTop)
			   putByte_StrBuf(out, CLIP_ASSIGN_FIELD_POP);
			else
			   putByte_StrBuf(out, CLIP_IASSIGN_FIELD_POP);
			putLong_StrBuf(out, hashstr(np->name));
		     }
	       }
	    else
	       {
		  if (np->nameExpr)
		     {
			if (np->node.isTop)
			   {
			      if (np->nameExpr->isMacro && !np->areaExpr && !np->area)
				 {
				    putByte_StrBuf(out, CLIP_ASSIGN_PFIELD_POP);
				 }
			      else
				 {
				    putByte_StrBuf(out, CLIP_ASSIGN_PFIELD);
				    if (np->area)
				       putLong_StrBuf(out, n_hashstr(np->area));
				    else
				       putLong_StrBuf(out, -1);
				 }
			   }
			else
			   {
			      if (np->nameExpr->isMacro && !np->areaExpr && !np->area)
				 {
				    putByte_StrBuf(out, CLIP_IASSIGN_PFIELD_POP);
				 }
			      else
				 {
				    putByte_StrBuf(out, CLIP_IASSIGN_PFIELD);
				    if (np->area)
				       putLong_StrBuf(out, n_hashstr(np->area));
				    else
				       putLong_StrBuf(out, -1);
				 }
			   }
		     }
		  else
		     {
			if (np->node.isTop)
			   {
			      putByte_StrBuf(out, CLIP_ASSIGN_FIELD);
			      putLong_StrBuf(out, hashstr(np->name));
			      if (np->area)
				 putLong_StrBuf(out, n_hashstr(np->area));
			      else
				 putLong_StrBuf(out, -1);
			   }
			else
			   {
			      putByte_StrBuf(out, CLIP_IASSIGN_FIELD);
			      putLong_StrBuf(out, hashstr(np->name));
			      if (np->area)
				 putLong_StrBuf(out, n_hashstr(np->area));
			      else
				 putLong_StrBuf(out, -1);
			   }
		     }
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   if (np->nameExpr)
      np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
   if (np->areaExpr)
      np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_AssignFieldNode(char *area, Node * areaExpr, char *name, Node * nameExpr, Node * expr)
{
   NEWVAR(AssignFieldNode, ret);

   init_Node(ret, pass_AssignFieldNode, "assignfield");
   ret->node.isExpr = 1;

   ret->area = area;
   ret->areaExpr = areaExpr;
   ret->name = name;
   ret->nameExpr = nameExpr;
   ret->expr = expr;
   ret->node.isAssign = 1;
   curFunction->goto_trap_of_Function++;
   ins_name(name);
   ins_name(area);

   return (Node *) ret;
}

/* ] AssignFieldNode */
/* [ ArrayInitNode */

static int
pass_ArrayInitNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ArrayInitNode (%d):\n", np->i);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "{");
	    np->expr->pass(np->expr, pass, level + 1, par);
	    putString_StrBuf(out, "}");
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, np->i + 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);

	    np->expr->pass(np->expr, CText, level, par);
	    fprintfOffs(out, level, "_clip_sarray( _mp, %d );\n", np->i);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, OText, level, par);
	    putByte_StrBuf(out, CLIP_ARRAY);
	    putLong_StrBuf(out, np->i);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_ArrayInitNode(Node * exprlist)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ArrayInitNode, "arrayinit");
   ret->node.isExpr = 1;

   ret->i = getCount_Node(exprlist);
   ret->expr = exprlist;
   ret->node.isConst = exprlist->isConst;
   ret->node.isArray = 1;

   return (Node *) ret;
}

Node *
new_ArrayInitNodeN(Node * exprlist, int n)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ArrayInitNode, "arrayinit");
   ret->node.isExpr = 1;

   ret->i = n;
   ret->expr = exprlist;
   ret->node.isConst = exprlist->isConst;
   ret->node.isArray = 1;

   return (Node *) ret;
}

/* ] ArrayInitNode */
/* [ NewArrayNode */

static int
pass_NewArrayNode(void *self, Pass pass, int level, void *par)
{
   VAR(NewArrayNode, np, self);
   int i;

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "NewArrayNode (%d):\n", np->cp->count);
	 for (i = 0; i < np->cp->count; ++i)
	    {
	       VAR(char, s, np->cp->items[i]);

	       fprintfOffs(stdout, level + 1, "%ldL\n", atol(s));
	    }
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    int i;

	    fprintfOffs(out, level, "_clip_newarray( _mp, %d ", np->cp->count);
	    for (i = 0; i < np->cp->count; ++i)
	       {
		  VAR(char, s, np->cp->items[i]);

		  fprintf(out, ", %ldL", atol(s));
	       }
	    fprintf(out, " );\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int i;

	    putByte_StrBuf(out, CLIP_NEWARRAY);
	    putLong_StrBuf(out, np->cp->count);
	    for (i = 0; i < np->cp->count; ++i)
	       {
		  VAR(char, s, np->cp->items[i]);

		  putLong_StrBuf(out, atol(s));
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   return 0;
}

Node *
new_NewArrayNode(Coll * cp)
{
   NEWVAR(NewArrayNode, ret);

   init_Node(ret, pass_NewArrayNode, "newarray");
   ret->node.isExpr = 1;

   ret->cp = cp;
   ret->node.isArray = 1;

   return (Node *) ret;
}

/* ] NewArrayNode */
/* [ MethodNode */
int
pass_MethodNode(void *self, Pass pass, int level, void *par)
{
   VAR(ObjNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "MethodNode: %s (%d)\n", np->name, np->argc);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    VAR(Node, npp, self);
	    int r, n = 0;

	    np->obj->pass(np->obj, pass, level, par);
	    putString_StrBuf(out, ":");
	    putString_StrBuf(out, np->name);
	    putString_StrBuf(out, "(");
	    for (r = first_List(&npp->list); r; r = next_List(&npp->list))
	       {
		  VAR(Node, p, npp->list.current);
		  if (n)
		     putString_StrBuf(out, ", ");
		  n++;
		  if (p->pass(p, pass, level + 1, par))
		     break;
	       }
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    int deep, ret;

	    addDeep(fp, 1);
	    deep = fp->deep_of_Function;
	    np->obj->pass(np->obj, pass, level, par);
	    ret = pass_Node(self, pass, level, par);
	    fp->deep_of_Function = deep;
	    return ret;
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	    np->obj->pass(np->obj, pass, level, par);
	    pass_Node(self, pass, level - 1, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_call(_mp, %d, %ld /* %s */))) goto _trap_%d;\n", np->argc, (long) hashstr(np->name), np->name, np->node.seqNo);
	    return 0;
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_NIL);
	    np->obj->pass(np->obj, pass, level, par);
	    pass_Node(self, pass, level - 1, par);
	    putByte_StrBuf(out, CLIP_CALL);
	    putByte_StrBuf(out, np->argc);
	    putLong_StrBuf(out, hashstr(np->name));
	    return 0;
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   np->obj->pass(np->obj, pass, level, par);
   return pass_Node(self, pass, level, par);
}

Node *
new_MethodNode(Node * obj, char *name, Coll * argv, int rest)
{
   int i;

   char *s;

   NEWVAR(ObjNode, ret);

   init_Node(ret, pass_MethodNode, "method");
   for (i = 0; i < argv->count; i++)
      {
	 VAR(Node, np, argv->items[i]);
	 append_Node(&ret->node, np);
      }
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = argv->count + 1;
   ret->obj = obj;
   ret->rest = rest;
   ins_name(name);

   delete_Coll(argv);
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

/* ] MethodNode */
/* [ GetNode */
int
pass_GetNode(void *self, Pass pass, int level, void *par)
{
   VAR(ObjNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "GetNode: %s\n", np->name);
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    int deep, ret;

	    addDeep(fp, 1);
	    deep = fp->deep_of_Function;
	    ret = np->obj->pass(np->obj, pass, level, par);
	    fp->deep_of_Function = deep;
	    return ret;
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	    np->obj->pass(np->obj, pass, level, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_get(_mp, %ld /* %s */))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, np->node.seqNo);
	    return 0;
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_NIL);
	    np->obj->pass(np->obj, pass, level, par);
	    putByte_StrBuf(out, CLIP_GET);
	    putLong_StrBuf(out, hashstr(np->name));
	    return 0;
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return np->obj->pass(np->obj, pass, level + 1, par);
}

Node *
new_GetNode(Node * obj, char *name)
{
   char *s;

   NEWVAR(ObjNode, ret);

   init_Node(ret, pass_GetNode, "get");
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = 0;
   ret->obj = obj;
   curFunction->goto_trap_of_Function++;
   ins_name(name);

   return (Node *) ret;
}

/* ] GetNode */
/* [ SetNode */
int
pass_SetNode(void *self, Pass pass, int level, void *par)
{
   VAR(ObjNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "SetNode: %s\n", np->name);
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    int deep, ret;

	    addDeep(fp, 1);
	    deep = fp->deep_of_Function;
	    np->obj->pass(np->obj, pass, level, par);
	    ret = pass_Node(self, pass, level, par);
	    fp->deep_of_Function = deep;
	    return ret;
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	    pass_Node(self, pass, level - 1, par);
	    np->obj->pass(np->obj, pass, level, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_set(_mp, %ld /* %s */))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, np->node.seqNo);
	    return 0;
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PUSH_NIL);
	    np->obj->pass(np->obj, pass, level, par);
	    pass_Node(self, pass, level - 1, par);
	    putByte_StrBuf(out, CLIP_SET);
	    putLong_StrBuf(out, hashstr(np->name));
	    return 0;
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   np->obj->pass(np->obj, pass, level, par);
   return pass_Node(self, pass, level, par);
}

Node *
new_SetNode(Node * obj, char *name, Node * expr)
{
   char *s;

   NEWVAR(ObjNode, ret);

   init_Node(ret, pass_SetNode, "set");
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = 0;
   ret->obj = obj;
   append_Node(&ret->node, expr);
   curFunction->goto_trap_of_Function++;
   ins_name(name);

   return (Node *) ret;
}

/* ] SetNode */
/*[ ReturnNode */

static int
pass_ReturnNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ReturnNode: isExprList=%d\n", np->i);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "return ");
	    if (np->expr)
	       np->expr->pass(np->expr, pass, level, par);
	 }
	 return 0;
      case CalcDeep:
	 {
	    if (np->expr)
	       np->expr->pass(np->expr, pass, level, par);
	    return 0;
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (!np->i)
	       {
		  if (np->expr)
		     {
			np->expr->pass(np->expr, pass, level, par);
			fprintfOffs(out, level, "_clip_return( _mp );\n");
		     }
	       }
	    else
	       {
		  int count = getCount_Node(np->expr);

		  if (count > 0)
		     {
			np->expr->pass(np->expr, pass, level, par);
			fprintfOffs(out, level, "_clip_return( _mp );\n");
		     }
	       }
	    fprintfOffs(out, level, "goto _return;\n");
	    return 0;
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (!np->i)
	       {
		  if (np->expr)
		     {
			np->expr->pass(np->expr, pass, level, par);
			putByte_StrBuf(out, CLIP_RETURN_POP);
		     }
		  else
		     putByte_StrBuf(out, CLIP_RETURN);
	       }
	    else
	       {
		  int count = getCount_Node(np->expr);

		  if (count > 0)
		     {
			np->expr->pass(np->expr, pass, level, par);
			putByte_StrBuf(out, CLIP_RETURN_POP);
		     }
		  else
		     putByte_StrBuf(out, CLIP_RETURN);
	       }
	    return 0;
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   if (np->expr)
      np->expr->pass(np->expr, pass, level, par);
   return 0;
}

Node *
new_ReturnNode(Node * expr, int isExprList)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ReturnNode, "return");
   ret->expr = expr;
   ret->i = isExprList;

   return (Node *) ret;
}

/* ] ReturnNode */
/* [ CodeNode */

static int
pass_CodeNode(void *self, Pass pass, int level, void *par)
{
   VAR(CodeNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "CodeNode: %s: %d locals\n", np->func->name_of_Function, np->func->VarColl_locals_of_Function->unsorted.count);
	 break;
      case TestConst:
	 {
	    VAR(int, ip, par);

	    np->func->Node_body_of_Function->pass(np->func->Node_body_of_Function, TestConst, 0, ip);
	   /*(*ip)++; */
	 }
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 3);
	 }
	 break;

      case Codegen1:
	 {
	   /*VAR(Function, fp, par); */
	    int i;

	    for (i = 0; i < np->func->VarColl_locals_of_Function->unsorted.count; i++)
	       {
		  VAR(Var, vp, np->func->VarColl_locals_of_Function->unsorted.items[i]);
		  Node *ip;

		  if (vp->init)
		     ip = vp->init;
		  else if (vp->arr)
		     ip = new_NewArrayNode(vp->arr);
		  else
		     ip = new_NilConstNode();

		  if (ip->isRef)
		     {
		       /*((RefNode*)ip) -> selfref = 1; */
		     }

		  prepend_Node((Node *) self, ip);
	       }

	 }
	 break;

      case CText:
	 {
	    VAR(FILE, out, par);
	    pass_Node(self, pass, level + 1, par);
	    fprintfOffs(out, level, "_clip_push_func( _mp, &clip_%s, %d, 0 );\n", np->func->name_of_Function, np->func->VarColl_locals_of_Function->unsorted.count);
	    return 0;
	 }

	 break;
      case CTextInit:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "{ { {CCODE_type_of_ClipVarType,0,0,0,0,0}, clip_%s} }", np->func->name_of_Function);
	    return 0;
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    pass_Node(self, pass, level + 1, par);
	    putByte_StrBuf(out, CLIP_PUSH_CODE);
	    putShort_StrBuf(out, np->func->mno_of_Function);
	    putByte_StrBuf(out, np->func->VarColl_locals_of_Function->unsorted.count);
	    putByte_StrBuf(out, 0);
	    return 0;
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   pass_Node(self, pass, level + 1, par);
   return 0;
}

Node *
new_CodeNode(VarColl * params)
{
   char buf[32], *s;

   int i;

   NEWVAR(CodeNode, ret);

   init_Node(ret, pass_CodeNode, "code");
   sprintf(buf, "code_%d", curFile->Coll_codeblocks_of_File.count);
   ret->fp = curFunction;
   curFunction = ret->func = new_Function(strdup(buf), params, 0, 0, curFunction, 1);
   for (s = ret->func->name_of_Function; *s; ++s)
      *s = tolower(*s);
   insert_Coll(&curFile->Coll_codeblocks_of_File, ret->func);
   init_Coll(&ret->inits, 0, 0);
   ret->node.isCode = 1;

   if (params)
      for (i = 0; i < params->unsorted.count; i++)
	 {
	    VAR(Var, vpp, params->unsorted.items[i]);
	    vpp->isCodeParam = 1;
	    vpp->isParam = 1;
	 }

   return (Node *) ret;
}

/* ] CodeNode */
/* [ LangNode */

static int
pass_LangNode(void *self, Pass pass, int level, void *par)
{
   VAR(LangNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "LangNode: '%s'\n", np->ctext);
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    char *s;

	    fprintfOffs(out, level, "");
	    for (s = np->ctext; *s; ++s)
	       {
		  fputc(*s, out);
		  if (*s == '\n' && s[1])
		     fprintfOffs(out, level, "");
	       }
	 }
	 break;
      case OText:
	 {
	    yyerror("CTEXT in pcode module");
	    break;
	 }
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_LangNode(char *ctext)
{
   NEWVAR(LangNode, ret);

   init_Node(ret, pass_LangNode, "lang");
   ret->ctext = ctext;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

/* ] LangNode */
/* [ ArrElNode */

static int
pass_ArrElNode(void *self, Pass pass, int level, void *par)
{
   VAR(ArrElNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ArrElNode: op='%c'\n", np->op);
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 2);
	 }
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    VAR(Node, npp, np->arr);
	    int r, n = 0, loc = 0;

	    putString_StrBuf(out, "(");
	    if (np->oarr->isLocal)
	       {
		  if (!np->expr)
		     flush_codestr(cp, new_QuotNode(&np->node));
		  else
		     yyerror("assign to element of local array conflicts with macro");
	       }
	    else
	       {
		  for (r = first_List(&npp->list); r; r = next_List(&npp->list))
		     {
			VAR(Node, p, npp->list.current);
			if (!n && p->isLocal)
			   loc = 1;

			if (n)
			   putString_StrBuf(out, "[");
			if (p->pass(p, pass, level + 1, par))
			   break;
			if (n)
			   putString_StrBuf(out, "]");
			n++;
		     }
		  if (np->expr)
		     {
			putByte_StrBuf(out, np->op);
			np->expr->pass(np->expr, pass, level + 1, par);
		     }
	       }
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (np->expr)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  np->arr->pass(np->arr, pass, level, par);
		  if (np->node.isTop)
		     {
			if (np->op == '=')
			   fprintfOffs(out, level, "if ((_ret=_clip_store( _mp, %d, %d ))) goto _trap_%d;\n", np->dim, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0, np->node.seqNo);
			else
			   fprintfOffs(out, level, "if ((_ret=_clip_opstore( _mp, %d, '%c' ))) goto _trap_%d;\n", np->dim, np->op, np->node.seqNo);
		     }
		  else
		     {
			if (np->op == '=')
			   fprintfOffs(out, level, "if ((_ret=_clip_istore( _mp, %d, %d ))) goto _trap_%d;\n", np->dim, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0, np->node.seqNo);
			else
			   fprintfOffs(out, level, "if ((_ret=_clip_opistore( _mp, %d, '%c' ))) goto _trap_%d;\n", np->dim, np->op, np->node.seqNo);
		     }
	       }
	    else
	       {
		  np->arr->pass(np->arr, pass, level, par);
		 /*if (np->node.isTop)
		    fprintfOffs(out, level, "if ((_ret=_clip_fetch( _mp, %d ))) goto _trap_%d;\n", np->dim, np->node.seqNo);
		    else */
		  fprintfOffs(out, level, "if ((_ret=_clip_ifetch( _mp, %d ))) goto _trap_%d;\n", np->dim, np->node.seqNo);
	       }
	 }
	 return 0;
      case CTextLval:
	 {
	    VAR(FILE, out, par);
	    np->arr->pass(np->arr, CText, level, par);
	    fprintfOffs(out, level, "{ClipVar *vp = _clip_fetchref( _mp, %d ); _mp->fp->sp->p.vp = vp;}\n", np->dim);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (np->expr)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  np->arr->pass(np->arr, pass, level, par);
		  if (np->node.isTop)
		     {
			if (np->op == '=')
			   {
			      putByte_StrBuf(out, CLIP_STORE);
			      putByte_StrBuf(out, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0);
			   }
			else
			   {
			      putByte_StrBuf(out, CLIP_OPSTORE);
			      putByte_StrBuf(out, np->op);
			   }
		     }
		  else
		     {
			if (np->op == '=')
			   {
			      putByte_StrBuf(out, CLIP_ISTORE);
			      putByte_StrBuf(out, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0);
			   }
			else
			   {
			      putByte_StrBuf(out, CLIP_OPISTORE);
			      putByte_StrBuf(out, np->op);
			   }
		     }
		  putByte_StrBuf(out, np->dim);
	       }
	    else
	       {
		  np->arr->pass(np->arr, pass, level, par);
		 /*if (np->node.isTop)
		    putByte_StrBuf(out, CLIP_FETCH);
		    else */
		  putByte_StrBuf(out, CLIP_IFETCH);
		  putByte_StrBuf(out, np->dim);
	       }
	 }
	 return 0;
      case OTextLval:
	 {
	    VAR(StrBuf, out, par);
	    np->arr->pass(np->arr, OText, level, par);
	    putByte_StrBuf(out, CLIP_FETCHREF);
	    putByte_StrBuf(out, np->dim);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->arr->pass(np->arr, pass, level + 1, par);
   if (np->expr)
      np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_ArrElNode(Node * arr, Node * expr, int op)
{
   NEWVAR(ArrElNode, ret);
   Node *oarr;

   init_Node(ret, pass_ArrElNode, "arrel");

   first_List(&arr->list);
   oarr = (Node *) arr->list.current;
   if (!strcmp(oarr->name, "fmemvar"))
      {
	 Node *narr = new_MemvarNode(((VarNode *) oarr)->vp);

	 narr->line = oarr->line;
	 narr->file = oarr->file;

	 remove_List(&arr->list);
	 prepend_Node(arr, narr);
      }

   ret->oarr = oarr;
   ret->arr = arr;
   ret->expr = expr;
   ret->dim = getCount_Node(arr) - 1;
   ret->op = op;
   curFunction->goto_trap_of_Function++;
   if (ret->expr)
      ret->node.isAssign = 1;
   ret->node.isArrEl = 1;
   if (!expr)
      ret->node.isLval = 1;

   return (Node *) ret;
}

/* ] ArrElNode */
/* [ MacroNode */

static int
find_paramsonly(Node * np, void *par)
{
   VAR(int, ip, par);

   if (!strcmp(np->name, "macro"))
      np = ((ExprNode *) np)->expr;

   if (!strcmp(np->name, "local"))
      {
	 if (((VarNode *) np)->vp->isCodeParam)
	    return 0;
      }
   else if (!strcmp(np->name, "const")	/* bug#55?? */
	    || !strcmp(np->name, "arrel") || !strcmp(np->name, "call"))
      return 0;

   if (!np->isExpr)
      return 0;

   if (!np->isLval)
      return 0;

   *ip = 0;
   return 0;
}

static int
pass_MacroNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "MacroNode :\n");
	 break;
      case SearchMacro:
	 {
	    VAR(int, haveMacro, par);

	    int paramsonly = 1;

	    TraversePar tpar;

	    tpar.func = find_paramsonly;
	    tpar.par = &paramsonly;
	    np->expr->pass(np->expr, Traverse, 0, &tpar);
	    if (!paramsonly && !np->i)
	       (*haveMacro)++;
	 }
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    if (np->expr->isArrEl)
	       {
		  VAR(StrBuf, out, cp->buf);
		  putString_StrBuf(out, "&");
		  np->expr->pass(np->expr, pass, level + 1, par);
	       }
	    else if (!strcmp(np->expr->name, "local") && ((VarNode *) np->expr)->vp->isCodeParam)
	       {
		  VAR(StrBuf, out, cp->buf);
		  putString_StrBuf(out, "&");
		  putString_StrBuf(out, (((VarNode *) np->expr)->vp->name));
	       }
	    else
	       flush_codestr(cp, np->expr);
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, CText, level, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_macro( _mp ))) goto _trap_%d;\n", np->node.seqNo);
	 }
	 return 0;
      case CTextLval:
	 {
	    np->expr->pass(np->expr, CText, level, par);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, OText, level, par);
	    putByte_StrBuf(out, CLIP_MACRO);
	 }
	 return 0;
      case OTextLval:
	 {
	    np->expr->pass(np->expr, OText, level, par);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}

Node *
new_MacroNode(Node * expr, int isexpr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_MacroNode, "macro");
   ret->expr = expr;
   ret->node.isMacro = 1;
   ret->node.isExpr = expr->isExpr;
   ret->node.isLval = 1;
   ret->i = isexpr;
   if (!strcmp(expr->name, "fmemvar"))
      ((VarNode *) expr)->p1 = 1;

   return (Node *) ret;
}

/* ] MacroNode */
/* [ PcountNode */

static int
pass_PcountNode(void *self, Pass pass, int level, void *par)
{
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "PcountNode\n");
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "_clip_pcount( _mp );\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PCOUNT);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   return 0;
}

Node *
new_PcountNode()
{
   NEWVAR(Node, ret);

   init_Node(ret, pass_PcountNode, "pcount");

   return (Node *) ret;
}

/* ] PcountNode */
/* [ PshiftNode */

static int
pass_PshiftNode(void *self, Pass pass, int level, void *par)
{
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "PshiftNode\n");
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    fprintfOffs(out, level, "_clip_pshift( _mp );\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_PSHIFT);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   return 0;
}

Node *
new_PshiftNode(Node * expr)
{
   NEWVAR(Node, ret);

   init_Node(ret, pass_PshiftNode, "pshift");

   return (Node *) ret;
}

/* ] PcountNode */
/* [ ParnNode */

static int
pass_ParnNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ParnNode :\n");
	 break;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, CText, level, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_parn( _mp ))) goto _trap_%d;\n", np->node.seqNo);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, OText, level, par);
	    putByte_StrBuf(out, CLIP_PARN);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level, par);
   return 0;
}

Node *
new_ParnNode(Node * expr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ParnNode, "parn");
   ret->expr = expr;

   return (Node *) ret;
}

/* ] ParnNode */
/* [ CallNameNode */

static int
pass_CallNameNode(void *self, Pass pass, int level, void *par)
{
   VAR(CallNameNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "CallNameNode:(%d)\n", np->argc);
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    int deep;

	    np->name->pass(np->name, pass, level, par);
	    addDeep(fp, 1);
	    deep = fp->deep_of_Function;
	    pass_Node(self, pass, level, par);
	    fp->deep_of_Function = deep;
	    return 0;
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->name->pass(np->name, CTextLval, level, par);
	    fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	    pass_Node(self, pass, level - 1, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_func_name(_mp, %d))) goto _trap_%d;\n", np->argc, np->node.seqNo);
	    return 0;
	 }
	 break;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->name->pass(np->name, OTextLval, level, par);
	    putByte_StrBuf(out, CLIP_PUSH_NIL);
	    pass_Node(self, pass, level - 1, par);
	    putByte_StrBuf(out, CLIP_FUNC_NAME);
	    putByte_StrBuf(out, np->argc);
	    return 0;
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->name->pass(np->name, pass, level, par);

   return pass_Node(self, pass, level, par);
}

Node *
new_CallNameNode(Node * name, Coll * argv)
{
   int i;

   NEWVAR(CallNameNode, ret);
   init_Node(ret, pass_CallNameNode, "callname");
   for (i = 0; i < argv->count; i++)
      {
	 VAR(Node, np, argv->items[i]);
	 append_Node(&ret->node, np);
      }
   ret->name = name;
   ret->argc = argv->count;
   curFunction->goto_trap_of_Function++;
   delete_Coll(argv);
   return (Node *) ret;
}

/* ] CallNameNode */
/* [ IncrNode */

static int
pass_IncrNode(void *self, Pass pass, int level, void *par)
{
   VAR(IncrNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "IncrNode: isIncr=%d isPost=%d:\n", np->isIncr, np->isPost);
	 break;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "(");
	    if (!np->isPost)
	       putString_StrBuf(out, np->isIncr ? "++" : "--");
	    np->expr->pass(np->expr, pass, level, par);
	    if (np->isPost)
	       putString_StrBuf(out, np->isIncr ? "++" : "--");
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (np->expr->isLval)
	       {
		  fprintfOffs(out, level, "if ( (_ret=_clip_incr( _mp, ");
		  np->expr->pass(np->expr, CTextLval, level, par);
		  fprintf(out, ", %d, %d ))) goto _trap_%d;\n", np->isIncr, np->isPost, np->node.seqNo);
	       }
	    else if (np->expr->isArrEl)
	       {
		  np->expr->pass(np->expr, CText, level, par);
	       }
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    if (np->expr->isLval)
	       {
		  np->expr->pass(np->expr, OTextLval, level, par);
		  putByte_StrBuf(out, np->isIncr ? (np->isPost ? CLIP_INCR_POST : CLIP_INCR) : (np->isPost ? CLIP_DECR_POST : CLIP_DECR));
	       }
	    else if (np->expr->isArrEl)
	       {
		  np->expr->pass(np->expr, OText, level, par);
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level, par);
   return 0;
}

Node *
new_IncrNode(Node * expr, int isIncr, int isPost)
{
   IncrNode *ret;

   if (expr->isArrEl)
      {
	 Node *np;

	 ArrElNode *ap = (ArrElNode *) expr;

	/* ap->op = 'I'; */
	 np = new_OpNode(expr, new_NumberConstNode(strdup("1"), 0), isIncr ? '+' : '-');

	 return new_ArrElNode(ap->arr, np, '=');
      }

   if (expr->isField)
      {
	 Node *np;

	 if (isPost)
	    {
	       np = new_OpNode(expr, new_NumberConstNode(strdup("1"), 0), isIncr ? '+' : '-');
	       np = new_AssignNode(expr, np, '=');
	       np = new_SwapNode(expr, np);
	    }
	 else
	    {
	       np = new_OpNode(expr, new_NumberConstNode(strdup("1"), 0), isIncr ? '+' : '-');
	       np = new_AssignNode(expr, np, '=');
	    }
	 return np;
      }

   ret = NEW(IncrNode);
   init_Node(ret, pass_IncrNode, "incr");
   ret->expr = expr;
   ret->isIncr = isIncr;
   ret->isPost = isPost;
   if (!expr->isLval && !expr->isArrEl /*&& !expr->isField */ )
      yyerror("INCR with non-lvalue");

   return (Node *) ret;
}

/* ] IncrNode */
/* [ OpNode */

static int
pass_OpNode(void *self, Pass pass, int level, void *par)
{
   VAR(OpNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "OpNode : op='%c'\n", np->op);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "(");
	    np->expr1->pass(np->expr1, pass, level, par);
	    switch (np->op)
	       {
	       case 'e':
		  putString_StrBuf(out, "=");
		  break;
	       case 'E':
		  putString_StrBuf(out, "==");
		  break;
	       case 'N':
		  putString_StrBuf(out, "!=");
		  break;
	       case 'L':
		  putString_StrBuf(out, "<=");
		  break;
	       case 'G':
		  putString_StrBuf(out, ">=");
		  break;
	       default:
		  putByte_StrBuf(out, np->op);
		  break;
	       }
	    np->expr2->pass(np->expr2, pass, level, par);
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr1->pass(np->expr1, pass, level, par);
	    np->expr2->pass(np->expr2, pass, level, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_op( _mp, '%c' ))) goto _trap_%d;\n", np->op, np->node.seqNo);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr1->pass(np->expr1, pass, level, par);
	    np->expr2->pass(np->expr2, pass, level, par);
	    putByte_StrBuf(out, CLIP_OP);
	    putByte_StrBuf(out, np->op);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr1->pass(np->expr1, pass, level + 1, par);
   np->expr2->pass(np->expr2, pass, level + 1, par);
   return 0;
}

Node *
new_OpNode(Node * expr1, Node * expr2, int op)
{
   NEWVAR(OpNode, ret);

   init_Node(ret, pass_OpNode, "op");
   ret->expr1 = expr1;
   ret->expr2 = expr2;
   ret->op = op;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

/* ] OpNode */
/* [ OrNode */

static int
pass_OrNode(void *self, Pass pass, int level, void *par)
{
   VAR(OpNode, np, self);
   int no = np->op;

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "OrNode label=%d\n", no);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "(");
	    np->expr1->pass(np->expr1, pass, level + 1, par);
	    putString_StrBuf(out, ".OR.");
	    np->expr2->pass(np->expr2, pass, level + 1, par);
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr1->pass(np->expr1, pass, level, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_tcond( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	    fprintfOffs(out, level, "if ( _if ) goto _endor_%d;\n", no);
	    fprintfOffs(out, level, "_clip_pop(_mp);\n");
	    np->expr2->pass(np->expr2, pass, level, par);
	    fprintfOffs(out, level, "_endor_%d:\n", no);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int jmp;

	    np->expr1->pass(np->expr1, pass, level, par);

	    putByte_StrBuf(out, CLIP_ITCOND);
	    jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    putByte_StrBuf(out, CLIP_POP);
	    np->expr2->pass(np->expr2, pass, level, par);
	    SETSHORT(out, jmp, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (jmp + sizeof(short)));

	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr1->pass(np->expr1, pass, level + 1, par);
   np->expr2->pass(np->expr2, pass, level + 1, par);
   return 0;
}

Node *
new_OrNode(Node * expr1, Node * expr2)
{
   NEWVAR(OpNode, ret);

   init_Node(ret, pass_OrNode, "or");
   ret->expr1 = expr1;
   ret->expr2 = expr2;
   curFunction->goto_trap_of_Function++;
   ret->op = curFunction->labelNo_of_Function++;
   curFunction->haveIf_of_Function++;

   return (Node *) ret;
}

/* ] OrNode */
/* [ AndNode */

static int
pass_AndNode(void *self, Pass pass, int level, void *par)
{
   VAR(OpNode, np, self);
   int no = np->op;

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "AndNode label=%d\n", no);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "(");
	    np->expr1->pass(np->expr1, pass, level + 1, par);
	    putString_StrBuf(out, ".AND.");
	    np->expr2->pass(np->expr2, pass, level + 1, par);
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr1->pass(np->expr1, pass, level, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_tcond( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	    fprintfOffs(out, level, "if ( !_if ) goto _endand_%d;\n", no);
	    fprintfOffs(out, level, "_clip_pop(_mp);\n");
	    np->expr2->pass(np->expr2, pass, level, par);
	    fprintfOffs(out, level, "_endand_%d:\n", no);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int jmp;

	    np->expr1->pass(np->expr1, pass, level, par);

	    putByte_StrBuf(out, CLIP_TCOND);
	    jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    putByte_StrBuf(out, CLIP_POP);
	    np->expr2->pass(np->expr2, pass, level, par);
	    SETSHORT(out, jmp, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (jmp + sizeof(short)));

	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr1->pass(np->expr1, pass, level + 1, par);
   np->expr2->pass(np->expr2, pass, level + 1, par);
   return 0;
}

Node *
new_AndNode(Node * expr1, Node * expr2)
{
   NEWVAR(OpNode, ret);

   init_Node(ret, pass_AndNode, "or");
   ret->expr1 = expr1;
   ret->expr2 = expr2;
   curFunction->goto_trap_of_Function++;
   ret->op = curFunction->labelNo_of_Function++;
   curFunction->haveIf_of_Function++;

   return (Node *) ret;
}

/* ] AndNode */
/* [ NotNode */

static int
pass_NotNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "NotNode :\n");
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "(!");
	    np->expr->pass(np->expr, pass, level, par);
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, pass, level, par);
	    fprintfOffs(out, level, "if (  (_ret=_clip_not( _mp ))) goto _trap_%d;\n", np->node.seqNo);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, pass, level, par);
	    putByte_StrBuf(out, CLIP_NOT);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level, par);
   return 0;
}

Node *
new_NotNode(Node * expr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_NotNode, "not");
   ret->expr = expr;
   ret->node.isConst = expr->isConst;
   expr->isMinus = 1;
   ret->node.isNum = expr->isNum;
   ret->node.isStr = expr->isStr;
   ret->node.isLog = expr->isLog;

   return (Node *) ret;
}

/* ] NotNode */
/* [ MinusNode */

static int
pass_MinusNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "MinusNode :\n");
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "(-");
	    np->expr->pass(np->expr, pass, level, par);
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, pass, level, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_minus( _mp ))) goto _trap_%d;\n", np->node.seqNo);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, pass, level, par);
	    putByte_StrBuf(out, CLIP_MINUS);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level, par);
   return 0;
}

Node *
new_MinusNode(Node * expr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_MinusNode, "minus");
   ret->expr = expr;
   ret->node.isConst = expr->isConst;
   expr->isMinus = 1;
   ret->node.isMinus = 1;
   ret->node.isNum = expr->isNum;
   ret->node.isStr = expr->isStr;
   ret->node.isLog = expr->isLog;

   return (Node *) ret;
}

/* ] NotNode */
/* [ IfNode */

static int
pass_IfNode(void *self, Pass pass, int level, void *par)
{
   VAR(IfNode, np, self);
   int i;

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "IfNode :\n");
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "iif(");
	    for (i = 0; i < np->elseifs->count; i += 2)
	       {
		  VAR(Node, p, np->elseifs->items[i]);
		  VAR(Node, lp, np->elseifs->items[i + 1]);
		  p->pass(p, pass, level, par);
		  putString_StrBuf(out, ",");
		  lp->pass(lp, pass, level, par);
	       }
	    putString_StrBuf(out, ",");
	    if (np->elselist)
	       np->elselist->pass(np->elselist, pass, level, par);
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    int no = np->labelNo;

	    int eno = no + np->elseifs->count / 2 + (np->elselist ? 0 : -1);

	    print_line(self, level, out);
	    for (i = 0; i < np->elseifs->count; i += 2)
	       {
		  VAR(Node, p, np->elseifs->items[i]);
		  VAR(Node, lp, np->elseifs->items[i + 1]);
		  p->pass(p, pass, level, par);
		  fprintfOffs(out, level, "if ((_ret=_clip_cond( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
		  fprintfOffs(out, level, "if ( !_if ) goto _endif_%d_%d;\n", no, loopPart);
		  lp->pass(lp, pass, level, par);
		  fprintfOffs(out, level, "goto _endif_%d_%d;\n", eno, loopPart);
		  fprintfOffs(out, level, "_endif_%d_%d:\n", no, loopPart);
		  ++no;
	       }
	    if (np->elselist)
	       {
		  np->elselist->pass(np->elselist, pass, level, par);
		  fprintfOffs(out, level, "_endif_%d_%d:\n", no, loopPart);
	       }
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int no;

	    int count = np->elseifs->count / 2 + (np->elselist ? 0 : -1);

	    int eoffs;

	    int *ejmps = (int *) malloc(sizeof(int) * (count + 1));

	    put_line(self, level, out);
	    for (i = 0, no = 0; i < np->elseifs->count; i += 2, ++no)
	       {
		  VAR(Node, p, np->elseifs->items[i]);
		  VAR(Node, lp, np->elseifs->items[i + 1]);
		  int jmp;

		  p->pass(p, pass, level, par);
		  putByte_StrBuf(out, CLIP_COND);
		  jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
		  putShort_StrBuf(out, 0);
		  lp->pass(lp, pass, level, par);

		  if (no < count)
		     {
			putByte_StrBuf(out, CLIP_GOTO);
			ejmps[no] = out->ptr_of_StrBuf - out->buf_of_StrBuf;
			putShort_StrBuf(out, 0);
		     }

		  SETSHORT(out, jmp, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (jmp + sizeof(short)));
	       }
	    if (np->elselist)
	       {
		  np->elselist->pass(np->elselist, pass, level, par);
	       }
	    eoffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    for (i = 0; i < count; ++i)
	       {
		  SETSHORT(out, ejmps[i], eoffs - (ejmps[i] + sizeof(short)));
	       }
	    free(ejmps);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   for (i = 0; i < np->elseifs->count; i += 2)
      {
	 VAR(Node, p, np->elseifs->items[i]);
	 VAR(Node, lp, np->elseifs->items[i + 1]);
	 p->pass(p, pass, level, par);
	 lp->pass(lp, pass, level, par);
      }
   if (np->elselist)
      np->elselist->pass(np->elselist, pass, level, par);
   return 0;
}

Node *
new_IfNode(Coll * elseifs, Node * elselist)
{
   NEWVAR(IfNode, ret);
   Node *expr = 0;

   if (elseifs && elseifs->count)
      expr = (Node *) elseifs->items[0];

   init_Node(ret, pass_IfNode, "if");
   if (expr)
      {
	 ret->node.line = expr->line;
	 ret->node.file = expr->file;
      }

   ret->elseifs = elseifs;
   ret->elselist = elselist;
   ret->labelNo = curFunction->labelNo_of_Function;
   curFunction->labelNo_of_Function += (elseifs->count / 2 + 1);
   curFunction->goto_trap_of_Function++;
   curFunction->haveIf_of_Function++;

   return (Node *) ret;
}

/* ] IfNode */
/* [ WhileNode */

typedef struct
{
   StrBuf *out;
   int boffs;
   int eoffs;
}
LoopData;

static int
pass_WhileNode(void *self, Pass pass, int level, void *par)
{
   VAR(WhileNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "WhileNode :\n");
	 break;
      case PrintSrc:
	 {
	    yyerror("%s not allowed in CODESTR", np->node.name);
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    int no = np->labelNo;

	    int saveNo = loopNo;

	    print_line(self, level, out);
	    fprintfOffs(out, level, "_loop_%d:\n", no);

	    np->expr->pass(np->expr, pass, level, par);

	    fprintfOffs(out, level, "if ((_ret=_clip_cond( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	    fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);

	    loopNo = no;
	    np->list->pass(np->list, pass, level, par);

	    fprintfOffs(out, level, "goto _loop_%d;\n", no);
	    fprintfOffs(out, level, "_endloop_%d:\n", no);
	    loopNo = saveNo;
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int jmp, eoffs, boffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;

	    LoopData ld;

	    put_line(self, level, out);
	    np->expr->pass(np->expr, pass, level, par);

	    putByte_StrBuf(out, CLIP_COND);
	    jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    np->list->pass(np->list, pass, level, par);

	    putByte_StrBuf(out, CLIP_GOTO);
	    eoffs = (out->ptr_of_StrBuf - out->buf_of_StrBuf) + sizeof(short);

	    putShort_StrBuf(out, boffs - eoffs);
	    SETSHORT(out, jmp, eoffs - (jmp + sizeof(short)));

	    ld.out = out;
	    ld.boffs = boffs;
	    ld.eoffs = eoffs;
	    np->list->pass(np->list, OTextLoop, level, &ld);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level, par);
   np->list->pass(np->list, pass, level, par);
   return 0;
}

Node *
new_WhileNode(Node * expr, Node * list)
{
   NEWVAR(WhileNode, ret);

   init_Node(ret, pass_WhileNode, "while");
   ret->node.line = expr->line;
   ret->node.file = expr->file;
   ret->expr = expr;
   ret->list = list;
   ret->labelNo = curFunction->labelNo_of_Function;
   curFunction->labelNo_of_Function += 1;
   curFunction->haveIf_of_Function++;

   return (Node *) ret;
}

/* ] WhileNode */
/* [ LoopExitNode */

static int
pass_LoopExitNode(void *self, Pass pass, int level, void *par)
{
   VAR(LoopExitNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "%s\n", np->isLoop ? "LOOP" : "EXIT");
	 break;
      case PrintSrc:
	 {
	    yyerror("%s not allowed in CODESTR", np->node.name);
	 }
	 return 0;
      case CText:
	 {
	    VAR(FILE, out, par);
	    if (np->isLoop)
	       fprintfOffs(out, level, "goto _loop_%d;\n", loopNo);
	    else
	       fprintfOffs(out, level, "goto _endloop_%d;\n", loopNo);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    putByte_StrBuf(out, CLIP_GOTO);
	    np->offs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);
	 }
	 return 0;
      case OTextLoop:
	 {
	    VAR(LoopData, ld, par);
	    if (!np->processed)
	       {
		  if (np->isLoop)
		     SETSHORT(ld->out, np->offs, ld->boffs - (np->offs + sizeof(short)));

		  else
		     SETSHORT(ld->out, np->offs, ld->eoffs - (np->offs + sizeof(short)));

		  np->processed++;
	       }
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   return 0;
}

Node *
new_LoopExitNode(int isLoop)
{
   NEWVAR(LoopExitNode, ret);

   init_Node(ret, pass_LoopExitNode, "loopexit");
   ret->isLoop = isLoop;

   return (Node *) ret;
}

/* ] LoopExitNode */
/* [ ForNode */

static int
pass_ForNode(void *self, Pass pass, int level, void *par)
{
   VAR(ForNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ForNode :\n");
	 break;
      case PrintSrc:
	 {
	    yyerror("%s not allowed in CODESTR", np->node.name);
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    int no = np->labelNo;

	    int saveNo = loopNo;

	    print_line(self, level, out);
	    loopPart = 0;
	    np->init->pass(np->init, pass, level, par);
	    np->to->pass(np->to, pass, level, par);
	    np->step->pass(np->step, pass, level, par);

	    fprintfOffs(out, level, "if ((_ret=_clip_forstep( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	    fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);

	    fprintfOffs(out, level, "goto _lbeg_%d;\n", no);

	    fprintfOffs(out, level, "_loop_%d:\n", no);
	    loopPart = 1;
	    np->var->pass(np->var, pass, level, par);
	    np->to->pass(np->to, pass, level, par);
	    np->step->pass(np->step, pass, level, par);
	    loopPart = 0;

	    fprintfOffs(out, level, "if ((_ret=_clip_forstep( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	    fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);

	    fprintfOffs(out, level, "_lbeg_%d:\n", no);

	    loopNo = no;
	    np->list->pass(np->list, pass, level, par);

	    fprintfOffs(out, level, "goto _loop_%d;\n", no);
	    fprintfOffs(out, level, "_endloop_%d:\n", no);
	    loopNo = saveNo;
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int jmp, eoffs, ibeg, lbeg, boffs;

	    LoopData ld;

	    put_line(self, level, out);
	    np->init->pass(np->init, pass, level, par);
	    np->to->pass(np->to, pass, level, par);
	    np->step->pass(np->step, pass, level, par);
	    putByte_StrBuf(out, CLIP_FORSTEP);
	    ibeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    putByte_StrBuf(out, CLIP_GOTO);
	    lbeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    boffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    put_line(self, level, out);
	    np->var->pass(np->var, pass, level, par);
	    np->to->pass(np->to, pass, level, par);
	    np->step->pass(np->step, pass, level, par);
	    putByte_StrBuf(out, CLIP_FORSTEP);
	    jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    SETSHORT(out, lbeg, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (lbeg + sizeof(short)));

	    np->list->pass(np->list, pass, level, par);

	    putByte_StrBuf(out, CLIP_GOTO);
	    eoffs = (out->ptr_of_StrBuf - out->buf_of_StrBuf) + sizeof(short);
	    putShort_StrBuf(out, boffs - eoffs);

	    SETSHORT(out, jmp, eoffs - (jmp + sizeof(short)));
	    SETSHORT(out, ibeg, eoffs - (ibeg + sizeof(short)));

	    ld.out = out;
	    ld.boffs = boffs;
	    ld.eoffs = eoffs;
	    np->list->pass(np->list, OTextLoop, level, &ld);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   np->init->pass(np->init, pass, level, par);
   np->step->pass(np->step, pass, level, par);
   np->var->pass(np->var, pass, level, par);
   np->to->pass(np->to, pass, level, par);
   np->list->pass(np->list, pass, level, par);

   return 0;
}

Node *
new_ForNode(Node * var, Node * from, Node * to, Node * step, Node * list)
{
   NEWVAR(ForNode, ret);

   init_Node(ret, pass_ForNode, "for");
   ret->node.line = from->line;
   ret->node.file = from->file;
   ret->to = to;
   ret->step = step;
   ret->init = new_AssignNode(var, from, '=');
   ret->var = new_AssignNode(var, step, '+');
   ret->list = list;
   ret->labelNo = curFunction->labelNo_of_Function;
   curFunction->labelNo_of_Function += 1;
   curFunction->haveIf_of_Function++;

   return (Node *) ret;
}

/* ] ForNode */
/* [ ForeachNode */

static int
pass_ForeachNode(void *self, Pass pass, int level, void *par)
{
   VAR(ForNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "ForeachNode : keys=%d\n", np->keys);
	 break;
      case PrintSrc:
	 {
	    yyerror("%s not allowed in CODESTR", np->node.name);
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    int no = np->labelNo;

	    int saveNo = loopNo;

	    print_line(self, level, out);

	    np->init->pass(np->init, pass, level, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_map_first( _mp, %d, &_if ))) goto _trap_%d;\n", np->keys, np->node.seqNo);
	    fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);
	    fprintfOffs(out, level, "goto _lbeg_%d;\n", no);

	    fprintfOffs(out, level, "_loop_%d:\n", no);

	    np->init->pass(np->init, pass, level, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_map_next( _mp, %d, &_if ))) goto _trap_%d;\n", np->keys, np->node.seqNo);
	    fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);

	    fprintfOffs(out, level, "_lbeg_%d:\n", no);

	    fprintfOffs(out, level, "if ((_ret=_clip_assign( _mp, ");
	    np->var->pass(np->var, CTextLval, level, par);
	    fprintf(out, " ))) goto _trap_%d;\n", np->node.seqNo);

	    loopNo = no;
	    np->list->pass(np->list, pass, level, par);

	    fprintfOffs(out, level, "goto _loop_%d;\n", no);
	    fprintfOffs(out, level, "_endloop_%d:\n", no);
	    loopNo = saveNo;
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int jmp, eoffs, ibeg, lbeg, boffs;

	    LoopData ld;

	    put_line(self, level, out);
	    np->init->pass(np->init, pass, level, par);
	    putByte_StrBuf(out, CLIP_MAP_FIRST);
	    putByte_StrBuf(out, np->keys);
	    ibeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    putByte_StrBuf(out, CLIP_GOTO);
	    lbeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    boffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    put_line(self, level, out);
	    np->init->pass(np->init, pass, level, par);
	    putByte_StrBuf(out, CLIP_MAP_NEXT);
	    putByte_StrBuf(out, np->keys);
	    jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    SETSHORT(out, lbeg, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (lbeg + sizeof(short)));

	    np->var->pass(np->var, OTextLval, level, par);
	    putByte_StrBuf(out, CLIP_ASSIGN);
	    np->list->pass(np->list, pass, level, par);

	    putByte_StrBuf(out, CLIP_GOTO);
	    eoffs = (out->ptr_of_StrBuf - out->buf_of_StrBuf) + sizeof(short);
	    putShort_StrBuf(out, boffs - eoffs);

	    SETSHORT(out, jmp, eoffs - (jmp + sizeof(short)));
	    SETSHORT(out, ibeg, eoffs - (ibeg + sizeof(short)));

	    ld.out = out;
	    ld.boffs = boffs;
	    ld.eoffs = eoffs;
	    np->list->pass(np->list, OTextLoop, level, &ld);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   np->var->pass(np->var, pass, level, par);
   np->init->pass(np->init, pass, level, par);
   np->list->pass(np->list, pass, level, par);

   return 0;
}

Node *
new_ForeachNode(Node * var, Node * expr, Node * list, int keys)
{
   NEWVAR(ForNode, ret);

   init_Node(ret, pass_ForeachNode, "foreach");
   ret->node.line = var->line;
   ret->node.file = var->file;
   ret->init = expr;
   ret->var = var;
   ret->list = list;
   ret->labelNo = curFunction->labelNo_of_Function;
   ret->keys = keys;
   curFunction->labelNo_of_Function += 1;
   curFunction->haveIf_of_Function++;

   return (Node *) ret;
}

/* ] ForeachNode */
/* [ SeqNode */

typedef struct
{
   StrBuf *out;
   long offs;
}
TrapData;

static int
pass_SeqNode(void *self, Pass pass, int level, void *par)
{
   VAR(SeqNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "SeqNode :\n");
	 break;
      case PrintSrc:
	 {
	    yyerror("%s not allowed in CODESTR", np->node.name);
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    if (np->using)
	       addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    print_line(self, level, out);
	    np->list->pass(np->list, pass, level, par);
	    fprintfOffs(out, level, "goto _end_%d;\n", np->seqNo);
	    fprintfOffs(out, level - 1, "_trap_%d:\n", np->seqNo);
	   /* unrecoverable error */
#if 0
	    fprintfOffs(out, level, "if ( _ret<0 )\n");
	    fprintfOffs(out, level + 1, "goto _trap_0;\n");
#endif
	   /* clean up the stack */
	    fprintfOffs(out, level, "while ( _frame.sp > _frame.stack )\n");
	    fprintfOffs(out, level, "\t_clip_destroy(_mp, --_frame.sp);\n");
	    if (np->using)
	       {
		  fprintfOffs(out, level, "if (_mp->trapVar)\n");
		  fprintfOffs(out, level, "{\n");
		  fprintfOffs(out, level, "\t*_frame.sp++ = *_mp->trapVar;\n");
		  fprintfOffs(out, level, "\tfree(_mp->trapVar);\n");
		  fprintfOffs(out, level, "\t_mp->trapVar = 0;\n");
		  fprintfOffs(out, level, "\t_clip_assign(_mp, ");
		  np->using->pass(np->using, CTextLval, level, par);
		  fprintf(out, ");\n");
		  fprintfOffs(out, level, "}\n");
	       }
	    if (np->recover)
	       np->recover->pass(np->recover, pass, level, par);
	    fprintfOffs(out, level, "_ret = 0;\n");
	    fprintfOffs(out, level - 1, "_end_%d:\n", np->seqNo);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int toffs, moffs, eoffs;

	    TrapData td;

	    put_line(self, level, out);
	    putByte_StrBuf(out, CLIP_SETTRAP);
	    toffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);

	    np->list->pass(np->list, pass, level, par);
	    putByte_StrBuf(out, CLIP_GOTO);
	    eoffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);
	    SETSHORT(out, toffs, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (toffs + sizeof(short)));

	    moffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    if (np->seqLevel > 0)
	       {
		 /* recover previous handler */
		  putByte_StrBuf(out, CLIP_SETTRAP);
		  np->toffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
		  putShort_StrBuf(out, 0);
	       }
	    else
	       putByte_StrBuf(out, CLIP_RESETTRAP);

	    putByte_StrBuf(out, CLIP_RECOVER);
	    if (np->using)
	       {
		  np->using->pass(np->using, OTextLval, level, par);
		  putByte_StrBuf(out, CLIP_USING);
	       }
	    if (np->recover)
	       np->recover->pass(np->recover, pass, level, par);
	    SETSHORT(out, eoffs, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (eoffs + sizeof(short)));

	    td.out = out;
	    td.offs = moffs;
	    np->list->pass(np->list, OTextTrap, level, &td);
	 }
	 return 0;
      case OTextTrap:
	 {
	    VAR(TrapData, tp, par);
	   /* SETSHORT (tp->out,  np->toffs,  tp->offs); */
	    SETSHORT(tp->out, np->toffs, (tp->offs - (np->toffs + sizeof(short))));
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->list->pass(np->list, pass, level, par);
   if (np->using)
      np->using->pass(np->using, pass, level, par);
   if (np->recover)
      np->recover->pass(np->recover, pass, level, par);
   return 0;
}

Node *
new_SeqNode(Node * list, Node * using, Node * recover, int seqNo)
{
   NEWVAR(SeqNode, ret);

   init_Node(ret, pass_SeqNode, "seq");
   ret->list = list;
   ret->using = using;
   ret->recover = recover;
   ret->seqNo = seqNo;
   ret->curSeqNo = curFunction->seqNo_of_Function;
   ret->seqLevel = curFunction->seqLevel_of_Function;

   return (Node *) ret;
}

/* ] SeqNode */
/* [ BreakNode */

static int
pass_BreakNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "Break node:\n");
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "BREAK(");
	    if (np->expr)
	       np->expr->pass(np->expr, pass, level, par);
	    putString_StrBuf(out, ")");
	 }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    print_line(self, level, out);
	    if (np->expr)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  fprintfOffs(out, level, "_clip_trap_pop(_mp);\n");
		  fprintfOffs(out, level, "_ret = -1;\n");
		  fprintfOffs(out, level, "goto _trap_%d;\n", np->node.seqNo);
	       }
	    else
	       {
		  fprintfOffs(out, level, "_clip_trap_var(_mp, _frame.filename, _frame.line, 0);\n");
		  fprintfOffs(out, level, "_ret = -1;\n");
		  fprintfOffs(out, level, "goto _trap_%d;\n", np->node.seqNo);
	       }
	    if (!np->i)
	       fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    put_line(self, level, out);
	    if (np->expr)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  putByte_StrBuf(out, CLIP_BREAK_EXPR);
	       }
	    else
	       putByte_StrBuf(out, CLIP_BREAK);
	    if (!np->i)
	       putByte_StrBuf(out, CLIP_PUSH_NIL);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   if (np->expr)
      np->expr->pass(np->expr, pass, level, par);
   return 0;
}

Node *
new_BreakNode(Node * expr, int isOper)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_BreakNode, "break");
   ret->expr = expr;
   ret->i = isOper;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}

/* ] BreakNode */
/* [ NamespaceDefNode */

static int
pass_NamespaceDefNode(void *self, Pass pass, int level, void *par)
{
   VAR(DefNode, np, self);
   switch (pass)
      {
      case Print:
	 {
	    int i, count = np->cp->unsorted.count;

	    fprintfOffs(stdout, level, "namespace '%s' def node: %d vars\n", np->name, count);
	    for (i = 0; i < count; i++)
	       {
		  VAR(Var, vp, np->cp->unsorted.items[i]);
		  print_Var(vp, level + 1);
	       }
	 }
	 break;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   return 0;
}

Node *
new_NamespaceDefNode(char *name, VarColl * cp)
{
   int i;

   NEWVAR(DefNode, ret);
   long space = hashstr(name);

   ins_name(name);

   init_Node(ret, pass_NamespaceDefNode, "namespacedef");

   ret->node.isExec = 0;
   for (i = 0; i < cp->coll.count; i++)
      {
	 VAR(Var, vp, cp->coll.items[i]);
	 vp->alias = strdup(name);
	 ins_name(vp->name);
	 if (undeclaredName(vp->name, 0))
	    add_VarColl(curFunction->VarColl_fields_of_Function, vp);
	 append_Node(curFunction->Node_body_of_Function, new_CreateVarNode(vp, space, name));
	 if (vp->init)
	    {
	       Node *np = new_OperExprNode(new_AssignNode(new_FieldNode(vp), vp->init, '='));

	       np->line = vp->init->line;
	       append_Node(curFunction->Node_body_of_Function, np);
	    }
	 else if (vp->arr)
	    {
	       Node *np = new_OperExprNode(new_AssignNode(new_FieldNode(vp),
							  new_NewArrayNode(vp->arr),
							  '='));

	       np->line = vp->line;
	       append_Node(curFunction->Node_body_of_Function, np);
	    }

      }

   ret->name = name;
   ret->cp = cp;

   return (Node *) ret;
}

/* ] NamespaceDefNode */
/* [ newArrInit_Var */
Var *
newArrInit_Var(char *name, Node * node)
{
   Node *p, *p0;

   int isNum = 1;

   ins_name(name);
   if (!node->isExprList)
      return newInit_Var(name, node);

   for (p0 = p = (Node *) node->list.head; p;)
      {
	 if (!p->isNum)
	    {
	       isNum = 0;
	       break;
	    }

	 p = (Node *) p->listel.next;
	 if (p == p0)
	    break;
      }

   if (isNum)
      {
	 Coll *arr = new_Coll(free, 0);

	 for (p0 = p = (Node *) node->list.head; p;)
	    {
	       append_Coll(arr, ((ConstNode *) p)->val);
	       p = (Node *) p->listel.next;
	       if (p == p0)
		  break;
	    }
	 return newArr_Var(name, arr);
      }
   else
      return newInit_Var(name, new_NewArrNode(node));
}

Var *
mnewArrInit_Var(Node * macro, Node * node)
{
   Node *p, *p0;

   int isNum = 1;

   if (!node->isExprList)
      return mnewInit_Var(macro, node);

   for (p0 = p = (Node *) node->list.head; p;)
      {
	 if (!p->isNum)
	    {
	       isNum = 0;
	       break;
	    }

	 p = (Node *) p->listel.next;
	 if (p == p0)
	    break;
      }

   if (isNum)
      {
	 Coll *arr = new_Coll(free, 0);

	 for (p0 = p = (Node *) node->list.head; p;)
	    {
	       append_Coll(arr, ((ConstNode *) p)->val);
	       p = (Node *) p->listel.next;
	       if (p == p0)
		  break;
	    }
	 return mnewArr_Var(macro, arr);
      }
   else
      return mnewInit_Var(macro, new_NewArrNode(node));
}

/* ] newArrInit_Var */
/* [ NewArrNode */

static int
pass_NewArrNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "NewArrNode (%d):\n", np->i);
	 break;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, CText, level, par);
	    fprintfOffs(out, level, "_clip_dimarray( _mp, %d );\n", np->i);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, OText, level, par);
	    putByte_StrBuf(out, CLIP_DIMARRAY);
	    putLong_StrBuf(out, np->i);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   pass_Node(np->expr, pass, level, par);
   return 0;
}

Node *
new_NewArrNode(Node * exprlist)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_NewArrNode, "newarr");
   ret->node.isExpr = 1;

   ret->i = getCount_Node(exprlist);
   ret->expr = exprlist;
   ret->node.isConst = exprlist->isConst;
   ret->node.isArray = 1;

   return (Node *) ret;
}

/* ] NewArrNode */
/* [ SwapNode */

static int
pass_SwapNode(void *self, Pass pass, int level, void *par)
{
   VAR(OpNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "SwapNode:\n");
	 break;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr1->pass(np->expr1, CText, level, par);
	    np->expr2->pass(np->expr2, CText, level, par);
	   /*fprintfOffs(out, level, "_clip_swap( _mp, 1 );\n"); */
	    fprintfOffs(out, level, "_clip_pop( _mp );\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr1->pass(np->expr1, OText, level, par);
	    np->expr2->pass(np->expr2, OText, level, par);
	   /*putByte_StrBuf(out, CLIP_SWAP);
	      putByte_StrBuf(out, 1); */
	    putByte_StrBuf(out, CLIP_POP);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr1->pass(np->expr1, pass, level + 1, par);
   np->expr2->pass(np->expr2, pass, level + 1, par);
   return 0;
}

Node *
new_SwapNode(Node * expr1, Node * expr2)
{
   NEWVAR(OpNode, ret);

   init_Node(ret, pass_SwapNode, "swap");
   ret->node.isExpr = 1;

   ret->expr1 = expr1;
   ret->expr2 = expr2;
   ret->node.isConst = expr1->isConst;

   return (Node *) ret;
}

/* ] SwapNode */
/* [ LocaleStringNode */

static int
pass_LocaleStringNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "LocaleStringNode: module %s:\n", CLIP_MODULE);
	 break;
      case PrintSrc:
	 {
	    VAR(CodestrNode, cp, par);
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, "[");
	    np->expr->pass(np->expr, pass, level, par);
	    putString_StrBuf(out, "]");
	 }
	 return 0;
      case CalcDeep:
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, CText, level, par);
	    fprintfOffs(out, level, "_clip_push_locale( _mp );\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, OText, level, par);
	    putByte_StrBuf(out, CLIP_PUSH_LOCALE);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   pass_Node(np->expr, pass, level, par);
   return 0;
}

Node *
new_LocaleStringNode(char *val)
{
   Node *sp;

   int lm, l;

   char *s;

   NEWVAR(ExprNode, ret);

   l = strlen(val) + 1;
   lm = strlen(CLIP_MODULE) + 1;
   s = (char *) malloc(l + lm);
   memcpy(s, CLIP_MODULE, lm);
   memcpy(s + lm, val, l);

   free(val);
   s[lm - 1] = 1;
   sp = new_StringConstNode(s);

   init_Node(ret, pass_LocaleStringNode, "locale");

   ret->expr = sp;
   ret->node.isExpr = 1;

   return (Node *) ret;
}

/* ] LocaleStringNode */

/* [ CodestrNode */
static void
flush_codestr(CodestrNode * np, void *nod)
{
   if (np->buf->ptr_of_StrBuf > np->buf->buf_of_StrBuf)
      {
	 Node *sp;

	 putByte_StrBuf(np->buf, 0);
	 sp = new_StringConstNode(np->buf->buf_of_StrBuf);
	 np->buf->buf_of_StrBuf = np->buf->ptr_of_StrBuf = np->buf->end_of_StrBuf = 0;
	 np->buf->size_of_StrBuf = 0;

	 append_Coll(&np->coll, sp);
      }
   if (nod)
      append_Coll(&np->coll, nod);
}

static int
pass_CodestrNode(void *self, Pass pass, int level, void *par)
{
   VAR(CodestrNode, np, self);
   int i;

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "CodestrNode: %d\n", np->coll.count);
	 if (np->params)
	    {
	       for (i = 0; i < np->params->unsorted.count; i++)
		  {
		     VAR(Var, vpp, np->params->unsorted.items[i]);
		     fprintfOffs(stdout, level + 1, "param %d: %s\n", i, vpp->name);
		  }
	    }
	 for (i = 0; i < np->coll.count; i++)
	    {
	       VAR(Node, npp, np->coll.items[i]);
	       fprintfOffs(stdout, level, "%d:\n", i);
	       npp->pass(npp, pass, level + 1, par);
	    }
	 return 0;
      case CalcDeep:
	 {
	    VAR(Function, fp, par);
	    addDeep(fp, 1);
	 }
	 break;
      case Codegen2:
	 {
	    VAR(StrBuf, out, np->buf);
	    if (np->block || np->params)
	       {
		  putString_StrBuf(out, "{|");
		  if (np->params)
		     {
			for (i = 0; i < np->params->unsorted.count; i++)
			   {
			      VAR(Var, vpp, np->params->unsorted.items[i]);
			      if (i)
				 putString_StrBuf(out, ",");
			      putString_StrBuf(out, vpp->name);
			   }
		     }
		  putString_StrBuf(out, "|");
	       }
	    np->expr->pass(np->expr, PrintSrc, level + 1, np);
	    if (np->block || np->params)
	       putString_StrBuf(out, "}");
	    flush_codestr(np, 0);
	 }
	 return 0;
      case CText:
	 {
	    VAR(FILE, out, par);
	    for (i = 0; i < np->coll.count; i++)
	       {
		  VAR(Node, npp, np->coll.items[i]);
		  npp->pass(npp, pass, level + 1, par);
	       }
	    fprintfOffs(out, level, "_clip_catstr(_mp, %d);\n", np->coll.count);
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    for (i = 0; i < np->coll.count; i++)
	       {
		  VAR(Node, npp, np->coll.items[i]);
		  npp->pass(npp, pass, level + 1, par);
	       }
	    putByte_StrBuf(out, CLIP_CATSTR);
	    putShort_StrBuf(out, np->coll.count);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   for (i = 0; i < np->coll.count; i++)
      {
	 VAR(Node, npp, np->coll.items[i]);
	 npp->pass(npp, pass, level + 1, par);
      }

   return 0;
}

Node *
new_CodestrNode(Node * expr, int block, VarColl * params)
{
   int i;

   NEWVAR(CodestrNode, ret);

   init_Node(ret, pass_CodestrNode, "codestr");

   ret->expr = expr;
   ret->node.isExpr = 1;
   ret->buf = new_StrBuf();
   ret->block = block;
   ret->params = params;
   init_Coll(&ret->coll, 0, 0);

   if (params)
      for (i = 0; i < params->unsorted.count; i++)
	 {
	    VAR(Var, vpp, params->unsorted.items[i]);
	    vpp->isCodeParam = 1;
	    vpp->isParam = 1;
	 }

   return (Node *) ret;
}

/* ] CodestrNode */

/* [ QuotNode */

static int
pass_QuotNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "QuotNode:\n");
	 break;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, CText, level, par);
	    fprintfOffs(out, level, "_clip_quot( _mp );\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    np->expr->pass(np->expr, OText, level, par);
	    putByte_StrBuf(out, CLIP_QUOT);
	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }
   np->expr->pass(np->expr, pass, level, par);
   return 0;
}

Node *
new_QuotNode(Node * expr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_QuotNode, "quot");

   ret->expr = expr;
   ret->node.isExpr = 1;

   return (Node *) ret;
}

/* ] QuotNode */
/* [ SwitchNode */

typedef struct
{
   long hash;
   SwitchEl *ep;
   Node *cp;
}
CaseLabel;

static clip_hash_t
hash_const(Node * npp)
{
   ConstNode *np;

   long r = 0;

   if (strcmp(npp->name, "const"))
      return 0;

   np = (ConstNode *) npp;

   switch (np->type)
      {
      case CONST_NUMBER:
	 r = strtod(np->val, 0);
	 break;
      case CONST_STRING:
	 r = _clip_hashstr(np->val);
	 break;
      case CONST_NIL:
	 r = 0;
	 break;
      case CONST_LOGIC:
	 r = np->no ? 1 : 0;
	 break;
      }

   if (npp->isMinus)
      r = 0 - r;
   return (clip_hash_t) r;
}

static char *
str_const(Node * npp, char *buf, int bufsize)
{
   ConstNode *np;

   buf[0] = 0;
   if (strcmp(npp->name, "const"))
      return buf;

   np = (ConstNode *) npp;

   switch (np->type)
      {
      case CONST_NUMBER:
	 snprintf(buf, bufsize, "%s", np->val);
	 break;
      case CONST_STRING:
	 snprintf(buf, bufsize, "%s", np->val);
	 break;
      case CONST_NIL:
	 snprintf(buf, bufsize, "NIL");
	 break;
      case CONST_LOGIC:
	 snprintf(buf, bufsize, "%s", np->no ? ".T." : ".F.");
	 break;
      }

   return buf;
}

static int
cmp_label(void *p1, void *p2)
{
   long h1 = ((CaseLabel *) p1)->hash;

   long h2 = ((CaseLabel *) p2)->hash;

   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
   else
      return 0;
}

static int
pass_SwitchNode(void *self, Pass pass, int level, void *par)
{
   VAR(SwitchNode, np, self);
   int i;

   switch (pass)
      {
      case Print:
	 fprintfOffs(stdout, level, "SwitchNode: %d cases:\n", np->cases->count);
	 fprintfOffs(stdout, level + 1, "expr:\n");
	 np->expr->pass(np->expr, pass, level + 1, par);
	 for (i = 0; i < np->cases->count; i++)
	    {
	       int j;

	       VAR(SwitchEl, ep, np->cases->items[i]);

	       fprintfOffs(stdout, level + 1, "%d: %d labels\n", i, ep->labels_of_SwitchEl->count);
	       for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
		  {
		     VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
		     fprintfOffs(stdout, level + 2, "hash: %ld\n", (long) hash_const(cp));
		     cp->pass(cp, pass, level + 2, par);
		  }
	       fprintfOffs(stdout, level + 1, "operlist:\n");
	       ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 2, par);
	    }
	 if (np->other)
	    {
	       fprintfOffs(stdout, level + 1, "other:\n");
	       np->other->pass(np->other, pass, level + 2, par);
	    }

	 return 0;
      case CText:
	 {
	    VAR(FILE, out, par);
	    np->expr->pass(np->expr, pass, level + 1, par);
	    fprintfOffs(out, level, "switch( _clip_pop_shash( _mp ) )\n");
	    fprintfOffs(out, level, "{\n");

	    for (i = 0; i < np->cases->count; i++)
	       {
		  int j;

		  VAR(SwitchEl, ep, np->cases->items[i]);

		  for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
		     {
			VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
			fprintfOffs(out, level, "case %ld:\n", (long) hash_const(cp));
		     }
		  ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 1, par);
		  fprintfOffs(out, level + 1, "break;\n");
	       }
	    if (np->other)
	       {
		  fprintfOffs(out, level, "default:\n");
		  np->other->pass(np->other, pass, level + 1, par);
		  fprintfOffs(out, level + 1, "break;\n");
	       }
	    fprintfOffs(out, level, "}\n");
	 }
	 return 0;
      case OText:
	 {
	    VAR(StrBuf, out, par);
	    int labels, ibeg, iend;

	    np->expr->pass(np->expr, pass, level + 1, par);
	    putByte_StrBuf(out, CLIP_SWITCH);
	    putShort_StrBuf(out, np->labels->count);
	    ibeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	   /* other offs */
	    putShort_StrBuf(out, 0);
	   /* label hashs */
	    for (i = 0; i < np->labels->count; i++)
	       {
		  VAR(CaseLabel, lp, np->labels->items[i]);
		  putLong_StrBuf(out, lp->hash);
	       }
	   /* place for offsets */
	    labels = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    for (i = 0; i < np->labels->count; i++)
	       {
		  putShort_StrBuf(out, 0);
	       }
	   /* bodies */
	    for (i = 0; i < np->cases->count; i++)
	       {
		  VAR(SwitchEl, ep, np->cases->items[i]);

		  ep->offs_of_SwitchEl = out->ptr_of_StrBuf - out->buf_of_StrBuf;
		  ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 1, par);
		  putByte_StrBuf(out, CLIP_GOTO);
		  ep->end_of_SwitchEl = out->ptr_of_StrBuf - out->buf_of_StrBuf;
		  putShort_StrBuf(out, 0);

	       }
	    SETSHORT(out, ibeg, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (ibeg + sizeof(short)));
	    if (np->other)
	       {
		  np->other->pass(np->other, pass, level + 1, par);
	       }

	    iend = out->ptr_of_StrBuf - out->buf_of_StrBuf;

	   /* label jumps */
	    for (i = 0; i < np->labels->count; i++)
	       {
		  VAR(CaseLabel, lp, np->labels->items[i]);
		  SETSHORT(out, labels + i * sizeof(short), lp->ep->offs_of_SwitchEl - (ibeg + sizeof(short)));
	       }
	    for (i = 0; i < np->cases->count; i++)
	       {
		  VAR(SwitchEl, ep, np->cases->items[i]);
		  SETSHORT(out, ep->end_of_SwitchEl, iend - (ep->end_of_SwitchEl + sizeof(short)));
	       }

	 }
	 return 0;
      case Traverse:
	 {
	    VAR(TraversePar, tp, par);
	    tp->func((Node *) self, tp->par);
	 }
	 break;
      default:
	 break;
      }

   np->expr->pass(np->expr, pass, level, par);
   for (i = 0; i < np->cases->count; i++)
      {
	 int j;

	 VAR(SwitchEl, ep, np->cases->items[i]);

	 for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
	    {
	       VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
	       cp->pass(cp, pass, level + 2, par);
	    }
	 ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 2, par);
      }
   if (np->other)
      np->other->pass(np->other, pass, level + 2, par);
   return 0;
}

Node *
new_SwitchNode(Node * expr, Coll * cases, Node * other)
{
   NEWVAR(SwitchNode, ret);
   Coll *coll;

   int i;

   init_Node(ret, pass_SwitchNode, "switch");

   ret->expr = expr;
   ret->other = other;

   coll = new_Coll(0, cmp_label);

   for (i = 0; i < cases->count; i++)
      {
	 int j;

	 VAR(SwitchEl, ep, cases->items[i]);

	 for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
	    {
	       VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
	       int ind;

	       NEWVAR(CaseLabel, lp);

	       lp->hash = hash_const(cp);
	       lp->ep = ep;
	       lp->cp = cp;

	       if (search_Coll(coll, lp, &ind))
		  {
		     char buf[64], buf1[64];

		     VAR(CaseLabel, cpp, coll->items[ind]);
		     yyerror("CASE label duplication: '%s' at line %d and '%s' at line %d", str_const(cp, buf, sizeof(buf)), cp->line, str_const(cpp->cp, buf1, sizeof(buf1)), cpp->cp->line);
		     free(lp);
		  }
	       else
		  insert_Coll(coll, lp);
	    }
      }

   ret->cases = cases;
   ret->labels = coll;

   return (Node *) ret;
}

/* ] SwitchNode */

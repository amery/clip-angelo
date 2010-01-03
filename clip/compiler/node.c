
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

#include "node/static_n_hashstr.h"

/* [ utils */

#define SETSHORT(bp, offs, val) setshort(np->node.line,bp,offs,val)

#ifdef FORCEALIGN
#include "node/static_setshort.h"

#else

/*#define SETSHORT(bp, offs, val) (*(short*)((bp)->buf+(offs))=(val))*/

#include "node/static_setshort_2.h"

#endif
#include "node/static_ins_name.h"
#include "node/fprintfOffs.h"
#include "node/static_addDeep.h"
#include "node/static_print_line.h"
#include "node/static_put_line.h"
#include "node/static_add_to_undeclared.h"

/* ] utils */
/* [ Node */
#include "node/init_Node.h"
#include "node/pass_Node.h"
#include "node/new_Node.h"
#include "node/delete_Node.h"
#include "node/getCount_Node.h"
#include "node/append_Node.h"
#include "node/join_Node.h"
#include "node/traverse_Node.h"
#include "node/prepend_Node.h"

/* ] Node */
/* [ OperListNode */
#include "node/static_pass_OperListNode.h"
#include "node/new_OperListNode.h"

/* ] OperListNode */
/* [ OperExprNode */
#include "node/static_pass_OperExprNode.h"
#include "node/new_OperExprNode.h"

/* ] OperExprNode */
/* [ ConstNode */
#include "node/static_pass_ConstNode.h"
#include "node/compareConstNode.h"
#include "node/new_NumberConstNode.h"
#include "node/new_HashConstNode.h"
#include "node/new_StringConstNode.h"
#include "node/new_LogicConstNode.h"
#include "node/new_NilConstNode.h"

/* ] ConstNode */
/* [ LocalDefNode */
#include "node/print_Var.h"

#include "node/new_LocalDefNode1.h"
#include "node/new_LocalDefNode.h"

/* ] LocalDefNode */
/* [ StaticDefNode */
#include "node/new_StaticDefNode.h"

/* ] StaticDefNode */
/* [ MemvarDefNode */
#include "node/new_MemvarDefNode.h"

/* ] MemvarDefNode */
/* [ FieldDefNode */
#include "node/new_FieldDefNode.h"

/* ] FieldDefNode */
/* [ PublicDefNode */
#include "node/static_pass_PublicDefNode.h"
#include "node/new_PublicDefNode.h"

/* ] PublicDefNode */
/* [ CreateVarNode */
#include "node/static_pass_CreateVarNode.h"
#include "node/new_CreateVarNode.h"
/* ] CreateVarNode */
/* [ PrivateDefNode */
#include "node/static_pass_PrivateDefNode.h"
#include "node/new_PrivateDefNode.h"

/* ] PrivateDefNode */
/* [ ParametersDefNode */
#include "node/static_pass_ParametersDefNode.h"
#include "node/new_ParametersDefNode.h"

/* ] ParametersDefNode */
/* [ ParamNode */
#include "node/static_pass_ParamNode.h"
#include "node/new_ParamNode.h"

/* ] ParamNode */
/* [ LocalNode */
#include "node/static_pass_LocalNode.h"
#include "node/new_LocalNode.h"

/* ] LocalNode */
/* [ StaticNode */
#include "node/static_pass_StaticNode.h"
#include "node/new_StaticNode.h"

/* ] StaticNode */
/* [ MemvarNode */
#include "node/static_pass_MemvarNode.h"
#include "node/new_MemvarNode.h"

/* ] MemvarNode */
/* [ FMemvarNode */
#include "node/static_pass_FMemvarNode.h"
#include "node/new_FMemvarNode.h"
#include "node/new_MemvarFNode.h"

/* ] FMemvarNode */
/* [ FieldNode */
#include "node/static_pass_FieldNode.h"
#include "node/new_FieldNode.h"
#include "node/new2_FieldNode.h"

/* ] FieldNode */
/* [ PublicNode */
#include "node/static_pass_PublicNode.h"
#include "node/new_PublicNode.h"

/* ] PublicNode */
/* [ PrivateNode */
#include "node/static_pass_PrivateNode.h"
#include "node/new_PrivateNode.h"

/* ] PrivateNode */
/* [ ParametersNode */
#include "node/static_pass_ParameterNode.h"
#include "node/new_ParameterNode.h"

/* ] ParametersNode */
/* [ ArgNode */
#include "node/static_pass_ArgNode.h"
#include "node/new_ArgNode.h"

/* ] ArgNode */
/* [ RefNode */
#include "node/static_pass_RefNode.h"
#include "node/new_RefNode.h"

/* ] RefNode */
/* [ CallNode */
#include "node/static_ref_name.h"
#include "node/static_pass_CallNode.h"
#include "node/static_gettext_CallNode.h"
#include "node/new_CallNode.h"
#include "node/newList_CallNode.h"
#include "node/new2_CallNode.h"
#include "node/new3_CallNode.h"

/*static int local_get_no = 0;*/
#include "node/new_CGetNode.h"

/* ] CallNode */
/* [ ExprListNode */
#include "node/static_pass_ExprListNode.h"
#include "node/new_ExprListNode.h"

/* ] ExprListNode */
/* [ ExprArrNode */
#include "node/static_pass_ExprArrNode.h"
#include "node/new_ExprArrNode.h"

/* ] ExprArrNode */
/* [ AssignNode */
#include "node/static_pass_AssignNode.h"
#include "node/new_AssignNode.h"

/* ] AssignNode */
/* [ AssignFmemvarNode */
#include "node/static_pass_AssignFmemvarNode.h"
#include "node/new_AssignFmemvarNode.h"

/* ] AssignFmemvarNode */
/* [ AssignFieldNode */
#include "node/static_pass_AssignFieldNode.h"
#include "node/new_AssignFieldNode.h"

/* ] AssignFieldNode */
/* [ ArrayInitNode */
#include "node/static_pass_ArrayInitNode.h"
#include "node/new_ArrayInitNode.h"
#include "node/new_ArrayInitNodeN.h"

/* ] ArrayInitNode */
/* [ NewArrayNode */
#include "node/static_pass_NewArrayNode.h"
#include "node/new_NewArrayNode.h"

/* ] NewArrayNode */
/* [ MethodNode */
#include "node/pass_MethodNode.h"
#include "node/new_MethodNode.h"

/* ] MethodNode */
/* [ GetNode */
#include "node/pass_GetNode.h"
#include "node/new_GetNode.h"

/* ] GetNode */
/* [ SetNode */
#include "node/pass_SetNode.h"
#include "node/new_SetNode.h"

/* ] SetNode */
/*[ ReturnNode */
#include "node/static_pass_ReturnNode.h"
#include "node/new_ReturnNode.h"

/* ] ReturnNode */
/* [ CodeNode */
#include "node/static_pass_CodeNode.h"
#include "node/new_CodeNode.h"

/* ] CodeNode */
/* [ LangNode */
#include "node/static_pass_LangNode.h"
#include "node/new_LangNode.h"

/* ] LangNode */
/* [ ArrElNode */
#include "node/static_pass_ArrElNode.h"
#include "node/new_ArrElNode.h"

/* ] ArrElNode */
/* [ MacroNode */
#include "node/static_find_paramsonly.h"
#include "node/static_pass_MacroNod.h"
#include "node/new_MacroNode.h"

/* ] MacroNode */
/* [ PcountNode */
#include "node/static_pass_PcountNode.h"
#include "node/new_PcountNode.h"

/* ] PcountNode */
/* [ PshiftNode */
#include "node/static_pass_PshiftNode.h"
#include "node/new_PshiftNode.h"

/* ] PcountNode */
/* [ ParnNode */
#include "node/static_pass_ParnNode.h"
#include "node/new_ParnNode.h"

/* ] ParnNode */
/* [ CallNameNode */
#include "node/static_pass_CallNameNode.h"
#include "node/new_CallNameNode.h"

/* ] CallNameNode */
/* [ IncrNode */
#include "node/static_pass_IncrNode.h"
#include "node/new_IncrNode.h"

/* ] IncrNode */
/* [ OpNode */
#include "node/static_pass_OpNode.h"
#include "node/new_OpNode.h"

/* ] OpNode */
/* [ OrNode */
#include "node/static_pass_OrNode.h"
#include "node/new_OrNode.h"

/* ] OrNode */
/* [ AndNode */
#include "node/static_pass_AndNode.h"
#include "node/new_AndNode.h"

/* ] AndNode */
/* [ NotNode */
#include "node/static_pass_NotNode.h"
#include "node/new_NotNode.h"

/* ] NotNode */
/* [ MinusNode */
#include "node/static_pass_MinusNode.h"
#include "node/new_MinusNode.h"

/* ] NotNode */
/* [ IfNode */
#include "node/static_pass_IfNode.h"
#include "node/new_IfNode.h"

/* ] IfNode */
/* [ WhileNode */
#include "node/typedef_struct_LoopData.h"
#include "node/static_pass_WhileNode.h"
#include "node/new_WhileNode.h"

/* ] WhileNode */
/* [ LoopExitNode */
#include "node/static_pass_LoopExitNode.h"
#include "node/new_LoopExitNode.h"

/* ] LoopExitNode */
/* [ ForNode */
#include "node/static_pass_ForNode.h"
#include "node/new_ForNode.h"

/* ] ForNode */
/* [ ForeachNode */
#include "node/static_pass_ForeachNode.h"
#include "node/new_ForeachNode.h"
/* ] ForeachNode */
/* [ SeqNode */
#include "node/typedef_struct_TrapData.h"
#include "node/static_pass_SeqNode.h"
#include "node/new_SeqNode.h"

/* ] SeqNode */
/* [ BreakNode */
#include "node/static_pass_BreakNode.h"
#include "node/new_BreakNode.h"

/* ] BreakNode */
/* [ NamespaceDefNode */
#include "node/static_pass_NamespaceDefNode.h"
#include "node/new_NamespaceDefNode.h"

/* ] NamespaceDefNode */
/* [ newArrInit_Var */
#include "node/newArrInit_Var.h"
#include "node/mnewArrInit_Var.h"

/* ] newArrInit_Var */
/* [ NewArrNode */
#include "node/static_pass_NewArrNode.h"
#include "node/new_NewArrNode.h"

/* ] NewArrNode */
/* [ SwapNode */
#include "node/static_pass_SwapNode.h"
#include "node/new_SwapNode.h"

/* ] SwapNode */
/* [ LocaleStringNode */
#include "node/static_pass_LocaleStringNode.h"
#include "node/new_LocaleStringNode.h"

/* ] LocaleStringNode */

/* [ CodestrNode */
#include "node/static_flush_codestr.h"
#include "node/static_pass_CodestrNode.h"
#include "node/new_CodestrNode.h"

/* ] CodestrNode */

/* [ QuotNode */
#include "node/static_pass_QuotNode.h"
#include "node/new_QuotNode.h"

/* ] QuotNode */
/* [ SwitchNode */
#include "node/typedef_struct_CaseLabel.h"
#include "node/static_hash_const.h"
#include "node/static_str_const.h"
#include "node/static_cmp_label.h"
#include "node/static_pass_SwitchNode.h"
#include "node/new_SwitchNode.h"
/* ] SwitchNode */

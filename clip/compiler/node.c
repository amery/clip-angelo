
 /*
    Start total new system v. 0.0
    with hard coded long name variables to have clear system
    Angelo GIRARDI
  */

#include <ci_clip.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ci_clic.h>
#include <ci_nodepriv.h>
#include <ci_clipvm.h>
#include <ci_clipcfg.h>

static void flush_codestr(CodestrNode * np, void *nod);

static int loopNo, loopPart = 0;

#include <node/static_n_hashstr.c>

/* [ utils */

#define SETSHORT(bp, offs, val) setshort(np->node.line,bp,offs,val)

#ifdef FORCEALIGN
#include <node/static_setshort.c>

#else

/*#define SETSHORT(bp, offs, val) (*(short*)((bp)->buf+(offs))=(val))*/

#include <node/static_setshort_2.c>

#endif
#include <node/static_ins_name.c>
#include <node/fprintfOffs.c>
#include <node/static_addDeep.c>
#include <node/static_print_line.c>
#include <node/static_put_line.c>
#include <node/static_add_to_undeclared.c>

/* ] utils */
/* [ Node */
#include <node/init_Node.c>
#include <node/pass_Node.c>
#include <node/new_Node.c>
#include <node/delete_Node.c>
#include <node/getCount_Node.c>
#include <node/append_Node.c>
#include <node/join_Node.c>
#include <node/traverse_Node.c>
#include <node/prepend_Node.c>

/* ] Node */
/* [ OperListNode */
#include <node/static_pass_OperListNode.c>
#include <node/new_OperListNode.c>

/* ] OperListNode */
/* [ OperExprNode */
#include <node/static_pass_OperExprNode.c>
#include <node/new_OperExprNode.c>

/* ] OperExprNode */
/* [ ConstNode */
#include <node/static_pass_ConstNode.c>
#include <node/compareConstNode.c>
#include <node/new_NumberConstNode.c>
#include <node/new_HashConstNode.c>
#include <node/new_StringConstNode.c>
#include <node/new_LogicConstNode.c>
#include <node/new_NilConstNode.c>

/* ] ConstNode */
/* [ LocalDefNode */
#include <node/print_Var.c>

#include <node/new_LocalDefNode1.c>
#include <node/new_LocalDefNode.c>

/* ] LocalDefNode */
/* [ StaticDefNode */
#include <node/new_StaticDefNode.c>

/* ] StaticDefNode */
/* [ MemvarDefNode */
#include <node/new_MemvarDefNode.c>

/* ] MemvarDefNode */
/* [ FieldDefNode */
#include <node/new_FieldDefNode.c>

/* ] FieldDefNode */
/* [ PublicDefNode */
#include <node/static_pass_PublicDefNode.c>
#include <node/new_PublicDefNode.c>

/* ] PublicDefNode */
/* [ CreateVarNode */
#include <node/static_pass_CreateVarNode.c>
#include <node/new_CreateVarNode.c>
/* ] CreateVarNode */
/* [ PrivateDefNode */
#include <node/static_pass_PrivateDefNode.c>
#include <node/new_PrivateDefNode.c>

/* ] PrivateDefNode */
/* [ ParametersDefNode */
#include <node/static_pass_ParametersDefNode.c>
#include <node/new_ParametersDefNode.c>

/* ] ParametersDefNode */
/* [ ParamNode */
#include <node/static_pass_ParamNode.c>
#include <node/new_ParamNode.c>

/* ] ParamNode */
/* [ LocalNode */
#include <node/static_pass_LocalNode.c>
#include <node/new_LocalNode.c>

/* ] LocalNode */
/* [ StaticNode */
#include <node/static_pass_StaticNode.c>
#include <node/new_StaticNode.c>

/* ] StaticNode */
/* [ MemvarNode */
#include <node/static_pass_MemvarNode.c>
#include <node/new_MemvarNode.c>

/* ] MemvarNode */
/* [ FMemvarNode */
#include <node/static_pass_FMemvarNode.c>
#include <node/new_FMemvarNode.c>
#include <node/new_MemvarFNode.c>

/* ] FMemvarNode */
/* [ FieldNode */
#include <node/static_pass_FieldNode.c>
#include <node/new_FieldNode.c>
#include <node/new2_FieldNode.c>

/* ] FieldNode */
/* [ PublicNode */
#include <node/static_pass_PublicNode.c>
#include <node/new_PublicNode.c>

/* ] PublicNode */
/* [ PrivateNode */
#include <node/static_pass_PrivateNode.c>
#include <node/new_PrivateNode.c>

/* ] PrivateNode */
/* [ ParametersNode */
#include <node/static_pass_ParameterNode.c>
#include <node/new_ParameterNode.c>

/* ] ParametersNode */
/* [ ArgNode */
#include <node/static_pass_ArgNode.c>
#include <node/new_ArgNode.c>

/* ] ArgNode */
/* [ RefNode */
#include <node/static_pass_RefNode.c>
#include <node/new_RefNode.c>

/* ] RefNode */
/* [ CallNode */
#include <node/static_ref_name.c>
#include <node/static_pass_CallNode.c>
#include <node/static_gettext_CallNode.c>
#include <node/new_CallNode.c>
#include <node/newList_CallNode.c>
#include <node/new2_CallNode.c>
#include <node/new3_CallNode.c>

/*static int local_get_no = 0;*/
#include <node/new_CGetNode.c>

/* ] CallNode */
/* [ ExprListNode */
#include <node/static_pass_ExprListNode.c>
#include <node/new_ExprListNode.c>

/* ] ExprListNode */
/* [ ExprArrNode */
#include <node/static_pass_ExprArrNode.c>
#include <node/new_ExprArrNode.c>

/* ] ExprArrNode */
/* [ AssignNode */
#include <node/static_pass_AssignNode.c>
#include <node/new_AssignNode.c>

/* ] AssignNode */
/* [ AssignFmemvarNode */
#include <node/static_pass_AssignFmemvarNode.c>
#include <node/new_AssignFmemvarNode.c>

/* ] AssignFmemvarNode */
/* [ AssignFieldNode */
#include <node/static_pass_AssignFieldNode.c>
#include <node/new_AssignFieldNode.c>

/* ] AssignFieldNode */
/* [ ArrayInitNode */
#include <node/static_pass_ArrayInitNode.c>
#include <node/new_ArrayInitNode.c>
#include <node/new_ArrayInitNodeN.c>

/* ] ArrayInitNode */
/* [ NewArrayNode */
#include <node/static_pass_NewArrayNode.c>
#include <node/new_NewArrayNode.c>

/* ] NewArrayNode */
/* [ MethodNode */
#include <node/pass_MethodNode.c>
#include <node/new_MethodNode.c>

/* ] MethodNode */
/* [ GetNode */
#include <node/pass_GetNode.c>
#include <node/new_GetNode.c>

/* ] GetNode */
/* [ SetNode */
#include <node/pass_SetNode.c>
#include <node/new_SetNode.c>

/* ] SetNode */
/*[ ReturnNode */
#include <node/static_pass_ReturnNode.c>
#include <node/new_ReturnNode.c>

/* ] ReturnNode */
/* [ CodeNode */
#include <node/static_pass_CodeNode.c>
#include <node/new_CodeNode.c>

/* ] CodeNode */
/* [ LangNode */
#include <node/static_pass_LangNode.c>
#include <node/new_LangNode.c>

/* ] LangNode */
/* [ ArrElNode */
#include <node/static_pass_ArrElNode.c>
#include <node/new_ArrElNode.c>

/* ] ArrElNode */
/* [ MacroNode */
#include <node/static_find_paramsonly.c>
#include <node/static_pass_MacroNod.c>
#include <node/new_MacroNode.c>

/* ] MacroNode */
/* [ PcountNode */
#include <node/static_pass_PcountNode.c>
#include <node/new_PcountNode.c>

/* ] PcountNode */
/* [ PshiftNode */
#include <node/static_pass_PshiftNode.c>
#include <node/new_PshiftNode.c>

/* ] PcountNode */
/* [ ParnNode */
#include <node/static_pass_ParnNode.c>
#include <node/new_ParnNode.c>

/* ] ParnNode */
/* [ CallNameNode */
#include <node/static_pass_CallNameNode.c>
#include <node/new_CallNameNode.c>

/* ] CallNameNode */
/* [ IncrNode */
#include <node/static_pass_IncrNode.c>
#include <node/new_IncrNode.c>

/* ] IncrNode */
/* [ OpNode */
#include <node/static_pass_OpNode.c>
#include <node/new_OpNode.c>

/* ] OpNode */
/* [ OrNode */
#include <node/static_pass_OrNode.c>
#include <node/new_OrNode.c>

/* ] OrNode */
/* [ AndNode */
#include <node/static_pass_AndNode.c>
#include <node/new_AndNode.c>

/* ] AndNode */
/* [ NotNode */
#include <node/static_pass_NotNode.c>
#include <node/new_NotNode.c>

/* ] NotNode */
/* [ MinusNode */
#include <node/static_pass_MinusNode.c>
#include <node/new_MinusNode.c>

/* ] NotNode */
/* [ IfNode */
#include <node/static_pass_IfNode.c>
#include <node/new_IfNode.c>

/* ] IfNode */
/* [ WhileNode */
#include <node/typedef_struct_LoopData.h>
#include <node/static_pass_WhileNode.c>
#include <node/new_WhileNode.c>

/* ] WhileNode */
/* [ LoopExitNode */
#include <node/static_pass_LoopExitNode.c>
#include <node/new_LoopExitNode.c>

/* ] LoopExitNode */
/* [ ForNode */
#include <node/static_pass_ForNode.c>
#include <node/new_ForNode.c>

/* ] ForNode */
/* [ ForeachNode */
#include <node/static_pass_ForeachNode.c>
#include <node/new_ForeachNode.c>
/* ] ForeachNode */
/* [ SeqNode */
#include <node/typedef_struct_TrapData.h>
#include <node/static_pass_SeqNode.c>
#include <node/new_SeqNode.c>

/* ] SeqNode */
/* [ BreakNode */
#include <node/static_pass_BreakNode.c>
#include <node/new_BreakNode.c>

/* ] BreakNode */
/* [ NamespaceDefNode */
#include <node/static_pass_NamespaceDefNode.c>
#include <node/new_NamespaceDefNode.c>

/* ] NamespaceDefNode */
/* [ newArrInit_Var */
#include <node/newArrInit_Var.c>
#include <node/mnewArrInit_Var.c>

/* ] newArrInit_Var */
/* [ NewArrNode */
#include <node/static_pass_NewArrNode.c>
#include <node/new_NewArrNode.c>

/* ] NewArrNode */
/* [ SwapNode */
#include <node/static_pass_SwapNode.c>
#include <node/new_SwapNode.c>

/* ] SwapNode */
/* [ LocaleStringNode */
#include <node/static_pass_LocaleStringNode.c>
#include <node/new_LocaleStringNode.c>

/* ] LocaleStringNode */

/* [ CodestrNode */
#include <node/static_flush_codestr.c>
#include <node/static_pass_CodestrNode.c>
#include <node/new_CodestrNode.c>

/* ] CodestrNode */

/* [ QuotNode */
#include <node/static_pass_QuotNode.c>
#include <node/new_QuotNode.c>

/* ] QuotNode */
/* [ SwitchNode */
#include <node/typedef_struct_CaseLabel.h>
#include <node/static_hash_const.c>
#include <node/static_str_const.c>
#include <node/static_cmp_label.c>
#include <node/static_pass_SwitchNode.c>
#include <node/new_SwitchNode.c>
/* ] SwitchNode */

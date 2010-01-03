/*
 	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

 */

#ifndef CN_NODEPRIV_H
#define CN_NODEPRIV_H

#include "ci_nodepriv/typedef_struct_OperExprNode.h"

#define CONST_NUMBER 0
#define CONST_STRING 1
#define CONST_NIL 2
#define CONST_LOGIC 3
#define CONST_ISTRING 4
#include "ci_nodepriv/typedef_struct_ConstNode.h"
#include "ci_nodepriv/typedef_struct_DefNode.h"
#include "ci_nodepriv/typedef_struct_VarNode.h"
#include "ci_nodepriv/typedef_struct_CreateVarNode.h"
#include "ci_nodepriv/typedef_struct_ExprNode.h"
#include "ci_nodepriv/typedef_struct_CallNode.h"
#include "ci_nodepriv/typedef_struct_AssignNode.h"
#include "ci_nodepriv/typedef_struct_AssignFieldNode.h"
#include "ci_nodepriv/typedef_struct_NewArrayNode.h"
#include "ci_nodepriv/typedef_struct_FieldNode.h"
#include "ci_nodepriv/typedef_struct_ObjNode.h"
#include "ci_nodepriv/typedef_struct_CodeNode.h"
#include "ci_nodepriv/typedef_struct_LangNode.h"
#include "ci_nodepriv/typedef_struct_ArrElNode.h"
#include "ci_nodepriv/typedef_struct_CallNameNode.h"
#include "ci_nodepriv/typedef_struct_OpNode.h"
#include "ci_nodepriv/typedef_struct_IncrNode.h"
#include "ci_nodepriv/typedef_struct_IfNode.h"
#include "ci_nodepriv/typedef_struct_WhileNode.h"
#include "ci_nodepriv/typedef_struct_LoopExitNode.h"
#include "ci_nodepriv/typedef_struct_ForNode.h"
#include "ci_nodepriv/typedef_struct_SeqNode.h"
#include "ci_nodepriv/typedef_struct_RefNode.h"
#include "ci_nodepriv/typedef_struct_CodestrNode.h"
#include "ci_nodepriv/typedef_struct_SwitchNode.h"
#include "ci_nodepriv/typedef_struct_ExprListNode.h"
#include "ci_nodepriv/typedef_struct_ExprArrNode.h"

#endif

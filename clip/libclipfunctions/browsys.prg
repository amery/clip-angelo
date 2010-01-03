**********************************************************************
#INCLUDE "ci_button.ch"
#INCLUDE "ci_browsys.ch"
#INCLUDE "ci_inkey.ch"
#INCLUDE "ci_Set.ch"
#INCLUDE "ci_SetCurs.ch"
#INCLUDE "ci_tbrowse.ch"
#INCLUDE "ci_llibg.ch"

#DEFINE NVALTYPE_CHAR      67
#DEFINE NVALTYPE_DATE      68
#DEFINE NVALTYPE_LOGICAL   76
#DEFINE NVALTYPE_NUMERIC   78
#DEFINE NVALTYPE_BLOCK     66
#DEFINE NVALTYPE_ARRAY     65
#DEFINE NVALTYPE_NIL       85
**********************************************************************
#include "browsys/static_InsToggle.ch"
#include "browsys/static_MakeColumn.ch"
**********************************************************************
#include "browsys/FieldNBlock.ch"
#include "browsys/FieldNPut.ch"
#include "browsys/tbAddCol.ch"
#include "browsys/tbBblock.ch"
#include "browsys/tbCreate.ch"
#include "browsys/tbClose.ch"
#include "browsys/tbDelCol.ch"
#include "browsys/tbDisplay.ch"
#include "browsys/tbEditCell.ch"
#include "browsys/tbFblock.ch"
#include "browsys/tbGoBot.ch"
#include "browsys/tbGoTop.ch"
#include "browsys/tbInsCol.ch"
#include "browsys/tbModal.ch"
#include "browsys/tbMouse.ch"
#include "browsys/tbSblock.ch"
#include "browsys/tbSkip.ch"
**********************************************************************

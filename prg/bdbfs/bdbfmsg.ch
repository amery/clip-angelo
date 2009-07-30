/*
    Copyright (C) 1998-2004 Yevgen Bondar <elb@lg.bank.gov.ua>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#define _MSG_F1 [Help]
#define _MSG_AF10 [Os Exit]
#define Brow_M_Keys {[],[Print],[Append],[Set Dbf],[Set IDX],[Go To],[Locate],;
		     [Pack],[Seek],[Menu],[S/Locat],[UsrMenu]}
#define Brow_A_Keys {[Info],[Struct.],[Copy],[Repl /C],[Reindex],;
		{||IF(IsShift(),[Total],[Sum])},;
		[Contin.],[Zap],[MakeInd],[],[Filter],[Filer]}
#define Brow_C_Keys {{||IF(IsShift(),[SaveAop],[Macro])},;
		{||IF(m->_replay,[RstFlds],[SetFlds])},;
		[Copy To],[Repl /V],[Set var],[Count],;
		{||IF(IsShift(),[LocBack],[Recall])},;
		{||IF(IsShift(),[Invert],[Delete])},;
		[Ap/from],[Ch.Dir],;
		{||IF(IsShift(),[Cur.Loc],[Sort])},[ReadIni]}
#define Brow_S_Keys  {[SaveVue],[Modify],[Insert],[MakeEmp],[RestVue],;
		[Compute],[TagNumb],[TagName],[MakeTag],[DelTag],[Set CDX],[Set Tag]}

#define Modi_M_Keys  {[],[],[Append],[],[Insert],;
		[],[],[Delete],[MakeOpt],[Q/Save],[],[]}

#define Setf_M_Keys {[],[],[New],[],[Restore],;
		[],[Rec.Rst],[Del.Rst],[],[Set],[],[]}
#define Get_M_Keys {[],[],{||IF(Select()=1,[Append],[])},[EntMode],[],;
		[],[],[],[],[],[],[]}

#define _MSG_GET_CF10 [ASCII]

#define _MSG_GN_F4 [History]
#define _MSG_GN_AF4 [Glb.Hst]
#define _MSG_GN_F7 [InsFunc]
#define _MSG_GN_F9 [Select]
#define _MSG_GN_AF2 [Fields]
#define Val_M_Keys {[],[FdName],[Head],[History],[FdValue],;
		    [DbfName],[],[],[],[InsMenu],[],[]}
#define Val_C_Keys {[],[FdsName],[FdsHead],[Res.Hst],[Quoted],[],[],[],[],[],[],[]}

#define Print_M_Keys {[],[],[],[],[],[],[],[],[Config],[],[],[]}

#define _MSG_DEL_F8 [Delete]

#define Str_M_Keys {[],[Print],[],[File],[Ext.Str],[Struct],[],[],[Config],[],[],[]}

#define Reslt_M_Keys {[],[To Calc],[],[],[ToField],[],[],[],[],[InsMenu],[],[]}

#define Base_M_Keys {[],[],[],[],[],[],[],[],[],[],[],[]}
#define SaveF10Key [Save]

#define Memo_M_Keys {[],[Print],[Write],[Load],{||IF(_lWrap,[UnWrap],[Wrap])},;
		     [SysClpb],[],[],[Config],[Save],[],[]}

#define FB_M_Keys {[],[Print],[],[],[],[],;
		  [Find],{||IF(lWasF8,[Win],[Dos])},[],[Exit],[],[]}
#define FB_A_Key [FindNxt]
#define FB_C_Key [Sel.CP]

#define EMAIL 'elb@lg.bank.gov.ua'
#define EMAIL2 'Bondar_Eugen@yahoo.com'
#define BDBF_VERSION [Bdbf-C,1.27a]

#IFNDEF ENGLISH

#define ANY [������� ����� ������� ��� �p���������]
#define ATTACHING [���ᮥ�������]
#define APPEND_SELF [���ᮥ���塞 �� ᥡ� ��?]
#define A_COPY {[�������],[��᫥����],[�⪠�]}
#define A_SETF_BROW {Padc([����],nScroll),Padc([�������� ����],nScroll),[������],[��ਭ�]}
#define BADNAME [����୮� ��� ��� ������ ������]
#define BADWRITE [������ ������]
#define BAD_EXPR [����୮� ��ࠦ���� ]
#define BAD_EXT [����୮� �����७��(BAK)]
#define BASE_STRUC [��������� ���� ]
#define BYTE [ �]
#define CALC_EXPR [, ����஥ ���� ����᫨��]
#define CALC_FIELD [��� ������塞�� ����. ������ ����������.]
#define CHECK_DBF_FORCED {[��������஢����� Visual FoxPro],;
			  [dBase IV  � ����],;
			  [FlagShip],;
			  [FlexMemo],;
			  [dBase II  ��� Foxbase 1.0],;
			  [�������⥭]}
#define CHECK_DBF_TRY { [�����⨥ �⮩ ���� � �⠭���⭮� ०��� ����������],;
			[�।������⥫���� ��ଠ�: ]+NeedForced,;
			[],;
			[���������� ������� � �����஢����� ०���?],;
			[(��. ⠪�� ����᭥��� � �.34 bdbfs.txt)]}
#define CLIPBOARD  [����� ������]
#define COLOR_COND [�᫮��� ���ᢥ⪨]
#define CONDITION [�᫮���:]
#define COPYING [����஢����]
#define _CopyRight '������: Bdbfs <File>[.DBF|.MEM|.INI] [/bw] [/i=index] [��⮤���⢨� (��. �.33 bdbfs.txt)]'+;
		   _CRLF+'�����প�: '+EMAIL
#define CORRUPT_OR_LOCKED _corrupted+[ ��� �����஢��]
#define COUNTING [��������]
#define DATEFORMAT [dd/mm/yyyy]
#define DELETING [㤠�����]
#define DISK_KAT _give+[���� � ��⠫��]
#define DOS_RET [������� EXIT ��� �������� !]
#define DRV_DEFAULT [������� �ࠩ��� �� 㬮�砭��]
#define DRV_FOR_COPY [�ࠩ��� ��� ����� ����]
#define DRV_NTX [�������� �� �����ন������ ��� �ࠩ���� NTX]
#define EMPTY_BASE [ ���� �����  ]
#define EMPTY_IS_ALL [(����� - ��� ������)]
#define ENVIR_F [���࠭����� �।�]
#define EXPR_TOTAL [��ࠦ���� �����]
#define FIELD_ED [�������� ���� ]
#define FIELD_ERR [�訡�筮 ������: ]
#define FLDS_COPYING [���� ( ����� - ��� )]
#define FLDS_SELECT [�롨ࠩ�� ����]
#define FLDS_TOTAL [����, ������� ���� �㬬�஢���]
#define FORCE_PROHIBITED [��� �����饭� � "�����஢�����" ०���]
#define FULL_ED [Up Down PgUp PgDn - ��� ��।�������, Esc - ��� ��室� � ������ ��࠭]
#define FUNC_FIND [���᪠]
#define F_ATTACHING [��� ���ᮥ�������]
#define F_COPYING [��� ����஢����]
#define F_CREATE [������ 䠩� ]
#define F_ENVIR [��� ���࠭���� �।�]
#define F_FLTRING [�⡮��]
#define F_INDEXING [��� �������஢����]
#define F_SEARCH [��� ���᪠:]
#define F_SHOW {[��ᬮ�����],[�⪠�]}
#define F_SORTING [��� �����஢��]
#define GET_PSW [��஫�:]
#define GIVE_VAR _give+[��६����� � ��ࠦ���� (xVar:=xVal)]
#define GLOB_FIND [���⥪�⭮�� ���᪠]
#define IND_FILE [��������� 䠩� ]
#define INDEXING [����������]
#define INDEX_CHOICE [�롨ࠩ�� ������]
#define INDEX_CORRUPT [������]+_corrupted
#define INDEX_NO L_A_SIGN+[ ��� �������]
#define INDEX_NOT_MATCH [������ �� ᮮ⢥������� ����. ���������஢���?]
#define INI_F [���樠����樨]
#define INSERTING [��⠢��]
#define INVERTING [�������஢����]
#define IS_UNIQ [���������� ������]
#define IT_RECS [�ᥣ� ]+SpaceString(m->__Count)+ [ ����ᥩ]
#define I_SEARCH [����� �� �������.]
#define KBYTE [ ��]
#define LONGEST [���ᨬ��쭠� �����: ]
#define L_YES_SET [YyTt��1]
#define L_NO_SET [NnFf��0]
#define MEMOFIELD [���ᬮ�� ����-���� ]
#define MEMO_NO_MATCH {[���� ���� ⨯� MEMO, � ���������-�������.],;
		       [���ࠢ��� ���������?]}
#define MENU_MAX() Menu2({[���ᨬ��],[�������]},1,WHAT_FIND+_ABORT)
#translate MENU_RECS(<txt>) => Menu2({ALL_RECS,\[��⠢訥��]},;
			   _scope,\[�롨ࠩ�� ������� ]+<txt>)
#define MENU_SCOPE {[1-��],[2-���],[3-�ய������],[4-�⪠�]}
#define MSG_MNU_YESNO Menu2({[1-��],[0-���]},_choice,_mess,[�ࠢ��쭮?])
#define NEED_DEL_FILE [. ������� ���?]
#define NEED_FINISH [�����稢��� ࠡ���]
#define NEED_OEM2ANSI [�८�ࠧ��뢠�� ⥪�� � WIN-����஢��?]
#define NEED_PACK [����⮦��� �⬥祭��� ������]
#define NEED_REBUILD [�������� ��� �� ������]
#define NEED_REINDEX [��२�������㥬 ����]
#define NEED_REPLACE [��������: ]
#define NEED_ZAP [����⮦��� ��� ������]
#define NODIGITAL [����饥 ���� �� ��᫮���]
#define NOCHARACTER [����饥 ���� �� ᨬ���쭮�]
#define NOOP_IND [�������� �����饭� ��� �������]
#define NO_FIND_EXPR [�� ������ �᫮��� ���᪠]
#define NO_FLTR [������ ��� 䨫����]
#define NO_FLTRING [�� �믮����� �᫮��� �⡮��]
#define NO_DPZ [��� ⠪�� ������]
#define NO_KAT [ - ��⠫��� ��� ��� ������]
#define NO_ORDER [��� �������� ��������� 䠩���]
#define NO_SPACE [��� ����� �� ��᪥]
#define N_RECORD [������]
#define N_RECORD_COL [������: ]
#define PACKING [㯠�����]
#define PROCESSED [��ࠡ�⠭�: ]
#define READY [������ !]
#define RECALLING [����⠭�������]
#define REC_GO [��⠭�� �� ������ �����:]
#define REC_FIND [������ ������ ��:]
#define REC_FIND_C {[������],[���饭��],[������ � �������(��)]}
#define REC_SM [���饭�� �� ⥪�饩 ������ (+ -):]
#define REMOVING [㤠�����]
#define REMOVED [ ������� ]+NTRIM(_tally)+ [ ����ᥩ]
#define REPLACING [����饭��]
#define REPL_EXPR [,����஥ �㤥� ����饭� � ���� ]
#define RESULT_IS [��������� ��ࠦ����: ]
#define SEC_M [ �.]
#define SELECT_BASE [�롨ࠩ�� ����]
#define SELECT_CP [�롨ࠩ�� ����஢�� ����]
#define SET_FIELDS [��⠭���� �����]
#define SET_OF_OEM  [��������������������������������������������������������������������������]
#define SET_OF_ANSI [��������������������������������������������������������������*�����������]
#define SET_OF_USA  [QWERTYUIOP{}ASDFGHJKL:"ZXCVBNM<>qwertyuiop]+"[]"+[asdfghjkl;'zxcvbnm,.]
//#define SET_OF_USA  [QWERTYUIOP{}ASDFGHJKL:"ZXCVBNM<>qwertyuiop]
//+"[]"+[asdfghjkl;'zxcvbnm,.]
#define SET_OF_MYKB [����������������������������������㪥��������뢠�஫�����ᬨ����]
#define SET_OF_M_NO [123456789���������������������������]
#define SORTING [�����஢��]
#define SUMMING [�㬬�஢����]
#define SUM_IS [�㬬� = ]
#define TAG_HEAD PAD([ ��������],14)+PAD([���������],45)+PAD([�������],18)
#define TAG_NAME [��� ��]
#define TAG_NAME_DEL [��� 㤠�塞��� ��]
#define TAG_NO L_A_SIGN+[ ��� ��]
#define TAG_NUMBER [����� ��]
#define TimerOff() [�����祭� �६���:]+LTRIM(STR(Seconds()-m->_told,9,2))+ ;
		   [ �. ��ࠡ�⠭�: ]+NTRIM(m->_tally) + [ ����ᥩ]
#define TOO_SHORT {[��室��� ��ப� ᫨誮� ����⪠.],;
		   [���⨥ ����� �� ���� ��䥪��.],[��� �� ᦨ����?]}
#define TXT_NAME _give+[��� ⥪�⮢��� 䠩��]
#define UNSUPPORT [ - �������� ��ଠ�]
#define USE_HISTORY [�ᯮ���㥬 �������]
#define MSG_USERMENU_NO [���짮��⥫�᪮� ���� �� ��।�����]
#define WAIT_WIND [���������,���� ]
#define WHAT_COPY [�����㥬 ������: ]
#define WHAT_FIND [��� �饬 ?]
#define YESNO [�� ],[���]
#define _NEEDOVER [ 㦥 ����������.��९����� ?]

// Private
	#define PRV_ZU [������� �᫮��� ]
	#define PRV_ZIF [������� ��� 䠩�� ]
	#define PRV_ZIFN [������� ��� ������ 䠩�� ]
	#define PRV_IV [��������� ���� ]
	#define PRV_NOFILE [���� �� ������ ]
	#define PRV_NOMEMO [�訡�� �������� ]
	#define PRV_ABORT [ (Esc - �⪠�)]

// Static
	#define _STAT_CORRUPTED [ ���०���]
	#define _STAT_ELSEUse [���-�� ��� ������ ��� ����!]
	#define _STAT_END_EDIT [��室 � �������: F10, ��� ������: Esc]
	#define _STAT_ERTIP [��ᮮ⢥��⢨� ⨯�� ]
	#define _STAT_GIVE [������� ]
	#define _STAT_GIVE_NAME L_A_SIGN+[ ������ ���]
	#define _STAT_NFD [��ࠦ���� �� �������]
	#define _STAT_NOINDEX [������ �� ��⠭�����]
	#define _STAT_ZE [������� ��ࠦ���� ]

// InsKeys
	#define MSG_ClpVal [���祭�� �� ������ ������]
	#define MSG_cTitleIns [��⠢�� � ������� �������: ]
	#define MSG_aIns {[�������� ����],[���祭�� �� ����],[��� ����],;
		[��������� ����],MSG_ClpVal,;
		'��� ��� ����','��������� ��� ����',;
		'���祭�� �� ���� � ����窠�','������'}
	#define MSG_PUTTITLE [����祭��� १������: ]
	#define MSG_PUTWHERE {[����襬 � ⥪�饥 ����],;
			      [����襬 � ����� ������],;
			      [����襬 � ���⥬��� ����� ������],;
			      [�८�ࠧ㥬 � "��������" ��ଠ�]}
	#define MSG_FLDHEAD_INS [��������� ����]
	#define MSG_FLDNAME_INS [��� ����]

// PRINT
	#define ABORT_PRINT [Esc-��� �४��饭��]
	#define ALL_RECS [��� ������]
	#define BASE_STAT [ �� �����ﭨ� �� ]
	#define CENTR_NEED [����஢�� ����� ��� �뢮��]
	#define COUNTER_NAME [�/�]
	#define DEF_HBORDER [�������⥫� �����: � ���������]
	#define DEF_BORDER [� ⥫� ������]
	#define DGT_HEAD_1ST [������ ������� ��: ��ࢮ� �����]
	#define DGT_HEAD_ALL [��᫥������ ������]
	#define END_CHAR [����筠�]+_prn_seq
	#define F_OUT_AP [�����뢠�� 䠩� �뢮��]
	#define F_OUT_PRINT [�뢮� � 䠩� ]
	#define FOROTBOR [��� �⡮�� ����ᥩ]
	#define GROUP_HEAD [��� ]
	#define IS_PRINT [������]
	#define LASTUPDATED [������ ]
	#define LEFT_BORD [����� �࠭���]
	#define MEM_NUM_LINE [��ப � ����-�����]
	#define MEM_WIDE_PRT [��ਭ� ����-�����]
	#define NEED_FF [�ண�� ����� ���⠬�]
	#define NEED_HEAD [��������� ������� �� ������ �����]
	#define NEED_SUM [�㬬�஢��� ��᫮���]
	#define NUM_LF [��ॢ���� ��ப� ��᫥ ������ ��ப�]
	#define NUM_PAGE [������ ����஢ ���⮢]
	#define OUT_DEV [���ன�⢮ �뢮��]
	#define ON_PAGE_TOTAL [�� �����]
	#define PAGE_LEN [����� �����(0-�����)]
	#define PLIST [���� ]
	#define PRNT_TOTAL [�⮣� ]
	#define PRNT_TOTAL_ALL [�ᥣ� ]
	#define PRN_SEQ [ ��᫥����⥫쭮��� �ਭ����]
	#define REC_NEED [������ ����஢ ����ᥩ]
	#define REC_RIGHT [������ ��ࠢ�?]
	#define SELECT_PRINT [�롨ࠥ� ����ன�� ������]
	#define STAND_HEAD [�⠭���⭠� 蠯��]
	#define START_CHAR [����⮢��]+_prn_seq
	#define TITLE_ALL [����� �� ������ �����]
	#define WHERE_DEV [� 䠩�],[� HTML-䠩�],[�� �ਭ���]
	#define WHERE_OUT [�㤠 �뢮����? (ESC-�⪠�, F9 - ����ன��)]
	#define ZERO_NEED [������ �㫥��� ���祭��]
	#define _MSG_PR_F9 [Config]

// ModiStru
	#define BAD_NAME [����୮� ��� ����]
	#define BAD_LEN [����ୠ� �����]
	#define BE_CANCEL [��⠢�� ��� ��� �뫮]
	#define DEL_FIELD [����塞 ����]
	#define EXIST_NAME [����� ��� 㦥 ����]
	#define FLD_TYPE [�롨ࠩ�� ⨯ ����]
	#define HEAD_BROW [���塞 ���������]
	#define MAKE_CHANGE [������� ��������� � ���������]
	#define MODIFICATION [��������� ���������]
	#define NEED_OPTIM [��⨬����㥬 ���������]
	#define FIELDS_OPTIM [ ����� ��⨬���஢���]

// MainMenu
	#define _MM_MAINMENU {[���⥬�],[����],[����],[����],[������],[����~�],[����ଠ���],[���~祥]}

	#define _MM_SYSMENU  {	[�ࠩ��� ��� �� 㬮�砭�� ]+'',;
				[����஢�� ⠡����],;
				[��⠭���� ]+'',;
				[���࠭��� ��⠭����],;
				[����᫨�� ��ࠦ����],;
				[������ ��६�����],;
				[������],;
				[� �ணࠬ��],;
				[��室]}

	#define _MM_SETMENU {	[��⠭���� ������],;
				[�����],;
				[��⠭���� SET],;
				[���稥 ��⠭����-1],;
				[���稥 ��⠭����-2]}

	#define _MM_FILEMENU {	[������� ����],;
				[������� �।������],;
				[�������� ���������],;
				[������� �����],;
				[�஢����� ���������],;
				[���࠭��� ���㦥���],;
				[����⠭����� ���㦥���],;
				[��������� ini-䠩�],;
				[������� 䠩� ����-��������],;
				[��室 � DOS],;
				[������� ����/��⠫��],;
				[������ � 䠩����],;
				[�������஢��� ⥪��],;
				[��ᯮ���஢��� � HTML],;
				[��ᯮ���஢��� � XML],;
				[��ᯮ���஢��� � XLS];
				}

	#define _MM_BASEMENU {	[�������� �� ��㣮�],;
				[�����஢��� � ������],;
				[�����஢��� � �८�ࠧ������� MEMO],;
				[�������� �� 㤠�����],;
				[����� ����⪨ �� 㤠�����],;
				[�������஢��� ����⪨],;
				[���������],;
				[�����஢���],;
				[������� TOTAL],;
				[�������஢���],;
				[���������஢���],;
				[��⠭����� ������],;
				[������ � ���� ]+'',;
				[�������� ������],;
				[��⠭����� 䨫��� ]+'',;
				[��������� �������⢮],;
				[����������],;
				[������� 䠩�-�����];
				}

	#define _MM_RECORDMENU {[�������� ������],;
				[��⠢��� ������],;
				[����஢��� ]+'',;
				[��������],;
				[������ � �����],;
				[� ������ �� ������],;
				[�������� � ���孥�],;
				[�������� � ������],;
				[��३�� �� ������],;
				[�������஢���],;
				[�뢥��� � 䠩�];
				}

	#define _MM_FIELDMENU  {[�㬬�஢���],;
				[�������� ��� ������ ����⠭⮩ ]+'',;
				[�������� ��� ������ ��ࠦ����� ]+'',;
				[�������� ⮫쪮 � ⥪�饩 ������ ]+'',;
				[��������],;
				[�⬥���� ���ࠢ�����],;
				[�८�ࠧ����� ]+'',;
				[��஢���� ]+'',;
				[�����஢���],;
				[������஢���],;
				[������� ����஫���� �㬬� ��ப�],;
				[������ ���],;
				[�����],;
				[�������],;
				[����஢��� ]+'',;
				IF(m->_ptr<=m->_bdbfBrow:Freeze,[���],[��])+[��஧���],;
				[��⠭����� �᫮��� ���ᢥ⪨ � ����],;
				[����� �᫮��� ���ᢥ⪨];
				}
	#define _MM_FIELDMENU_HK [�]

	#define _MM_REPLMENU {[� ���⢥ত�����],[��� ���⢥ত����]}

	#define _MM_TRANSMENU { [� ���������],;
				[� ��������],;
				[� ���������],;
				[� ��������� �� ���� ������],;
				[� ����஢�� Windows(1251)],;
				[� ����஢�� DOS(866)],;
				[QWERTY->������],;
				[������->QWERTY];
				}

	#define _MM_FINDMENU   {[��३�� � ������],;
				[�� �������],;
				[�� �᫮���],;
				[�த������ �� �᫮���],;
				[�� �᫮��� �����],;
				[�த������ �����],;
				[�� ���⥪���],;
				[�� ���⥪��� (��ࠦ����)],;
				[� ⥪�饬 ���� ]+'',;
				[���ᨬ�� � ����],;
				[������� � ����],;
				[����� ������� � ����],;
				[�㡫� � �������],;
				[����� �� ��᪥ � �������],;
				[�த������ �� ��᪥];
	}

	#define _MM_INFOMENU  {[���㦥���,����,�������],;
				[��������� ����],;
				[������� ASCII]}

	#define _MM_OTHERMENU  {[�ந����� �����],;
				IF(m->_replay,[����⠭����� ��� ����],[��⠭����� �㦭�� ����]),;
				[�����樨 � ����஬ ]+'',;
				[���� ���짮��⥫� ]+'',;
				[������᪨� �����樨 ]+''}

	#define _MM_TAGMENU    {[��⠭����� ���],;
				[������� ���],;
				[�������� ���],;
				[��⠭����� ��㣮� CDX],;
				[������� ��� �� ������];
				}

	#define _MM_COPYMENU   {[������],;
				[�����],;
				[�����],;
				[��ࠢ�],;
				[�� ������],;
				[� �����],;
				[�� ������ ���⥬�],;
				[� ����� ���⥬�]}

	#define _MM_CLIPBDMENU {[�� ���� � �����],;
				[�������� �����],;
				[����� � ����],;
				[����� + ����],;
				[����� - ����],;
				[����� * ����],;
				[����� / ����],;
				[������ � �����],;
				[� ������ �� ������]}


//VIEWEDIT
	#define _MSG_VF_PRWAIT [������]
	#define _MSG_VF_CORRUPTED [ ���०���]
	#define _MSG_VF_PRINTED   [ ��ப ������⠭�]
	#define FB_PRINT1 [����� ��������:]
	#define FB_PRINT2 [���� 䠩�]
	#define FB_PRINT3 [��⠢����� �����]
	#define FB_PRINT4 [�⪠�]
	#define FB_SRCH  [��� �᪠�� �㤥� (ESC-�⪠�)]
	#define FB_ALSO  [ ࠧ ������� � ������ ��� (������� ����� �������) ]
	#define SAVE_AND_EXIT [��������� �� ���࠭���! ����⢨⥫쭮 �����?]
	#define SAVE_AND_EXIT_ALT {[���], [��], [���࠭��� � �����]}

//ACTIONS
	#define _MSG_A_AREINDEX [२��������� (������ ����५)]
	#define _MSG_A_CASE_SENSITIVE [���� �������� ����]
	#define _MSG_A_CHARONLY [�����ন������ ⮫쪮 ��� ᨬ������� �����ᮢ]
	#define _MSG_A_FLOCK [�訡�� �����஢�� 䠩��. ���������?]
	#define _MSG_A_RLOCK [�訡�� �����஢�� ������. ���������?]
	#define _MSG_A_UMENU [���� ���짮��⥫�]

	#define _MSG_A_F10 [ (F10 - ������)]
	#define _MSG_A_SETS [��⠭���� �ணࠬ��]

	#define _MSG_A_C_TOP	[��⠭���� 梥⮢]
	#define _MSG_A_C_ED	[�᭮���� 梥� ।��樨    ]
	#define _MSG_A_C_HLP	[������,�������,१������� ]
	#define _MSG_A_C_MARK	[��થ�� � ᯥ�ᨬ����     ]
	#define _MSG_A_C_MM	[������� ����              ]
	#define _MSG_A_C_HIDE	[���롨ࠥ��� ������ ����]
	#define _MSG_A_C_HD	[��������� �������         ]
	#define _MSG_A_C_INT	[��⥭ᨢ�����]
	#define _MSG_A_C_BLINK	[������� 䮭�]

	#define _MSG_A_O_TOP	[��������� ���⥬��� ��६�����]
	#define _MSG_A_O_FORCED	[�����஢����� ०��]
	#define _MSG_A_O_IND	[�����뢠�� ���������]
	#define _MSG_A_O_FRQ	[������� ���������� ����������(� �������)]
	#define _MSG_A_O_MSK	[��᪠ ��� �� 㬮�砭��]
	#define _MSG_A_O_MAC	[����� �� Ctrl+F1]
	#define _MSG_A_O_TIME	[�����뢠�� �६� �ᯮ������ ������]
	#define _MSG_A_O_ED	[�������� ���� � ��ப�� ����� (�����-���஥����)]
	#define _MSG_A_O_VF	[���ᬮ��騪 䠩��� ������ (�����-���஥����)    ]
	#define _MSG_A_O_TAB	[��ਭ� ⠡���樨 ��� ।��樨 ����]
	#define _MSG_A_O_WRAP	[����� ��ப ��� ।��樨 ����]
	#define _MSG_A_O_NAME	[��ਭ� ���� ��� ���ᬮ��� 䠩���]
	#define _MSG_A_O_ASAVE	[����-���࠭����]
	#define _MSG_A_O_AREST	[����-����⠭�������]
	#define _MSG_A_O_UNDO	[������ ������ UNDO]
	#define _MSG_A_O_PG	[���� ���᪠ ��������]
	#define _MSG_A_O_MAXF	[���ᨬ��쭮� ��᫮ �����]
	#define _MSG_A_O_PCKASK [���⢥ত��� 㯠�����]
	#define _MSG_A_O_PCKFND [�᪠�� 㤠������ ��। 㯠������]
*
	#define _MSG_A_O_CLCH	[�롮� �� ����७���� ������]
	#define _MSG_A_O_WCLCH	[�롮� �� ���⥬���� ������]
	#define _MSG_A_O_WCLCNV	[OEM/ANSI ��� ���⥬��� � WIN-����஬]
	#define _MSG_A_O_EXOEM	[OEM->ANSI ��� ��ᯮ��� �� 㬮�砭��]
	#define _MSG_A_O_EXOEMQ	[�����訢��� OEM->ANSI ��� ��ᯮ���]
	#define _MSG_A_O_GSEXP	[�����뢠�� ��ࠦ���� ���⥪�⭮�� ���᪠]
	#define _MSG_A_O_GSCOND	[�������⥫쭮� �᫮��� ���⥪�⭮�� ���᪠]
	#define _MSG_A_O_FASS	[������� �� <Enter> � 䠩����]

	#define _MSG_C_INI_H1	[Data Driver]
	#define _MSG_C_INI_H2	[���⥬��� ��६�����]
	#define _MSG_C_INI_H3	[�����]
	#define _MSG_C_INI_H4	[�ਭ���]
	#define _MSG_C_INI_H5	[�⠭������� Clipper SETs]
	#define _MSG_C_INI_H6	[���짮��⥫�᪮� ���� - ���������]
	#define _MSG_C_INI_H7	[���짮��⥫�᪮� ���� - �������]
	#define _MSG_C_INI_HAOP	[����, �������, �᫮���]

	#define _MSG_A_MF_F3	[���!]
	#define _MSG_A_MF_F4	[�ࠢ�!]

	#define _MSG_A_IF_TITLE [�롥���� ���室���� �㭪���: ]
	#define _MSG_A_IF_TRANS [�८�ࠧ����� ]
	#define _MSG_A_IF_AINS {;
			[�������� ᨬ����(StrTran)],;
			[�࠭᫨஢��� ᨬ����(ChrTran)],;
			cTrans+[� ������� �������],;
			cTrans+[� ������ ॣ����],;
			cTrans+[C ���������],;
			cTrans+[� ��������� �� ���� ������],;
			cTrans+[� ����஢�� Windows(1251)],;
			cTrans+[� ����஢�� DOS(866)],;
			cTrans+[QWERTY->������],;
			cTrans+[������->QWERTY],;
			[��஢����],;
			[�����஢���],;
			[������஢���],;
			[������� ����஫���� �㬬� ��ப�],;
			[�������஢���],;
			[������ ���],;
			[�����],;
			[�������],;
			[��������᪨� ��������];
		}

	#define _MSG_A_JUSTH {[��ࠢ������:]}
	#define _MSG_A_JUSTA {[�����],[�� 業���],[��ࠢ�],;
				[�� ��ਭ�],[����浪�],[���⨥]}

	#define _MSG_A_FILTH {[��⠭���� 䨫���:]}
	#define _MSG_A_FILTA {[�� �᫮���],;
				[������ ⥪�饩 �祩��],;
				[��ࠢ��� ⥪�饩 �祩��],;
				[����訬 ⥪�饩 �祩��],;
				[����訬 ⥪�饩 �祩��],;
				[����訬 ��� ࠢ��� ⥪�饩 �祩��],;
				[����訬 ��� ࠢ��� ⥪�饩 �祩��],;
				[��� 䨫����]}

	#define _MSG_A_SIMPH {[�饬 ��ࠦ���� � ⥪�饬 ����:]}
	#define _MSG_A_SIMPA {	[������ ⥪�饩 �祩��],;
				[��ࠢ��� ⥪�饩 �祩��],;
				[����襥 ⥪�饩 �祩��],;
				[����襥 ⥪�饩 �祩��],;
				[����襥 ��� ࠢ��� ⥪�饩 �祩��],;
				[����襥 ��� ࠢ��� ⥪�饩 �祩��],;
				[��㣮�]}

	#define _MSG_A_DS_F2 [Print]
	#define _MSG_A_DS_F4 [File]
	#define _MSG_A_DS_F5 [Ext.Str]
	#define _MSG_A_DS_F6 [Struct]
	#define _MSG_A_DS_F9 [Config]
	#define _MSG_A_DS_F10 [MakePrg]

	#define _MSG_A_ST_F2 [Print]
	#define _MSG_A_ST_F3 [AddTag]
	#define _MSG_A_ST_F4 [MakePrg]
	#define _MSG_A_ST_F5 [Reindex]
	#define _MSG_A_ST_F6 [Rebuild]
	#define _MSG_A_ST_F8 [DelTag]
	#define _MSG_A_ST_F9 [Config]


	#define _MSG_A_SDF_FILE_TYPE [��� 䠩��:]
	#define _MSG_A_SDF_F_DELIM [�������⥫� ⥪�⮢�� �����:]
	#define _MSG_A_SDF_A_FILETYPE {[SDF],[DELIMITED],[DELIMITED WITH BLANK],[DBF]}

	#define _MSG_A_CSIZE_NOERR [�訡�� �� �����㦥��]
	#define _MSG_A_CSIZE	{[�����㦥�� ���宦����� � ��।������ ࠧ���� ����!],;
				 [������ ������᪨�:     ]+NTRIM(dbSize),;
				 [������ �� ���������:   ]+NTRIM(xSize),;
				 [������ 䠩�� �� ��᪥: ]+NTRIM(fSize),;
				[],[��� ����� �� �᭮��?];
				}
	#define _MSG_A_CSIZE_MENU1 {[������᪨�],[(४�������)]}
	#define _MSG_A_CSIZE_MENU2 [���������]
	#define _MSG_A_CSIZE_MENU3 [��᪮���]
	#define _MSG_A_CSIZE_MENU4 [�⪠�]

	#define _MSG_A_NOPACK [��� ����ᥩ ��� 㤠�����]

	#define _MSG_A_GFCOND [�������⥫쭮� �᫮��� ���᪠]

//HELP
	#define _MSG_H_NO	[���� ��� ������ ��� �⮣� ࠧ����...]
	#define _MSG_H_TITLE	[����ଠ��� �� �������� (RDD: ]
	#define _MSG_H_TAGS	[���     �� ]
	#define _MSG_H_TAGN	[��� ��]
	#define _MSG_H_FE	[��ࠦ���� 䨫����]
	#define _MSG_H_ISU	[���������� ?]
	#define _MSG_H_ISD	[��뢠�騩 ?]
	#define _MSG_H_RYO	[���� RYO ?]
	#define _MSG_H_KEYS	[���祩 � ��]
	#define _MSG_H_CK	[����� �����]

	#define _MSG_H_OKEY	{[�������� ��� ������ ��ଠ�� DBF],;
				BDBF_VERSION,[],;
				[�����: ������� �������],;
				[�㣠���, ������ࠤ᪠� 8, (0642) 58-08-19(p)],;
				[E-MAIL: ]+EMAIL,;
				EMAIL2;
				}

	#define CODEPAGE	[������� ��࠭���:]+CHR(0)
	#define CURR_DIR	[����騩 ��⠫��:]+CHR(0)
	#define DATE_UPDATE	[���� ���������� ����:]+CHR(0)
	#define DRV_CURRENT	[�ࠩ��� (⨯):]+CHR(0)
	#define FILE_SIZE	[������ 䠩�� �� ��᪥:]+CHR(0)
	#define FREE_MEM	[�������� ������:]+CHR(0)
	#define FREE_SPACE	[�������� ����� �� ��᪥:]+CHR(0)
	#define HEAD_SIZE	[������ ���������:]+CHR(0)
	#define REC_SIZE	[������ ������:]+CHR(0)
	#define N_USER		[���짮��⥫�:]+CHR(0)
	#define ON_SERVER	[ �� ��ࢥ�� ]+CHR(0)
	#define MEMO_BLOCK_SIZE [������ ����� MEMO:]+CHR(0)
	#define MEMO_FILE_SIZE  [������ MEMO-䠩��:]+CHR(0)
	#define MEMO_B_SIZE_C   [������ ����� � ���:]+CHR(0)
	#define OS_VER		[������ ��:]+CHR(0)
	#define TOTAL_FIELDS	[�ᥣ� ����� � ����:]+CHR(0)


// GetSys
	#define _MSG_INSOVR	[��⠢��],[������ ]
	#define _MSG_EMODE	{[Usual Enter],[Lock Enter ],[Enter /skip],[Enter /stop]}
	#define _MSG_ASCII	'������� ASCII'
	#define _MSG_ASC_SLCT	'���࠭�: '

// ErrorSys
	#define NOT_READY [�ਭ��� �� ��⮢.���������]
	#define NET_ERR [�訡�� ����]

//Filer
	#define _MSG_FI_TOP	[�롨ࠩ�� 䠩�: ]
	#define _MSG_FI_F2	[����]
	#define _MSG_FI_F5	[�����]
	#define _MSG_FI_F9	[��᪠]
	#define _MSG_FI_F10	[����]
	#define _MSG_FI_MASK	[������� ����� �⡮��]
	#define _MSG_FI_DSCR	[��� ���ᠭ�� ��� �⮣� 䠩��]

	#define _MSG_FI_FILE	"����:"+CHR(0)
	#define _MSG_FI_DIR	"��⠫��:"+CHR(0)
	#define _MSG_FI_SIZE	"������:"+CHR(0)
	#define _MSG_FI_UPD	"������:"+CHR(0)
	#define _MSG_FI_ATTR	"��ਡ���:"+CHR(0)
	#define _MSG_FI_IND	"������:"+CHR(0)
	#define _MSG_FI_KEY	"<KEY>"+CHR(0)

//ClipLib
	#define _MSG_CL_NOF	[��������! ��� ��ࠡ�⪥ �뫨 �訡�� ��९�������]
	#define _MSG_CL_NOONE	[�� ������ �� ������ �ࠢ��쭮�� ���� ��� ��ࠦ����]

#ELSE

#define ANY [Press any key for continue]
#define ATTACHING [appending]
#define APPEND_SELF [Append from itself?]
#define A_COPY {[Current],[Last],[Abort]}
#define A_SETF_BROW {Padc([Field],nScroll),Padc([Field name],nScroll),[Picture],[Wide]}
#define BADNAME [Bad name or writing is prohibited]
#define BADWRITE [Writing is prohibited]
#define BAD_EXPR [Wrong expression ]
#define BAD_EXT [Bad extension(BAK)]
#define BASE_STRUC [Base structure ]
#define BYTE [ b]
#define CALC_EXPR [for calculating ]
#define CALC_FIELD [Can not replace calculated field]
#define CHECK_DBF_FORCED {[��������஢����� Visual FoxPro],;
			  [dBase IV  and above],;
			  [FlagShip],;
			  [FlexMemo],;
			  [dBase II  or Foxbase 1.0],;
			  [Unknown]}
#define CHECK_DBF_TRY  {[The opening of this base is unsupported in standard mode],;
			[Possible format: ]+NeedForced,;
			[],;
			[Should we try open it in forced mode ?],;
			[(See also ch.34 bdbfs.txt for explaining)]}
#define CLIPBOARD  [Clipboard]
#define COLOR_COND [Color separation condition]
#define CONDITION [Condition:]
#define COPYING [copying]
#define _CopyRight 'Usage: Bdbfs <File>[.DBF|.MEM|.INI] [/bw] [/i=index] [Auto-actions (see ch.33 bdbfs.txt)]'+;
	_CRLF+'Support: '+EMAIL
#define CORRUPT_OR_LOCKED _corrupted+[ or locked]
#define COUNTING [counting]
#define DATEFORMAT [mm/dd/yyyy]
#define DELETING [deleting]
#define DISK_KAT _Give +[the disk and dir]
#define DOS_RET [Type EXIT for return !]
#define DRV_DEFAULT [Select default RDD]
#define DRV_FOR_COPY [RDD for new base]
#define DRV_NTX [Not supported for RDD NTX]
#define EMPTY_BASE [ EMPTY BASE  ]
#define EMPTY_IS_ALL [ ( Empty means "ALL" )]
#define ENVIR_F [of environment]
#define EXPR_TOTAL [Key expression]
#define FIELD_ED [Field redaction ]
#define FIELD_ERR [Error field: ]
#define FLDS_COPYING [Fields ( Empty means "ALL" )]
#define FLDS_SELECT [Select the fields]
#define FLDS_TOTAL [Fields to TOTAL]
#define FORCE_PROHIBITED [It is prohibited in "forced" mode]
#define FULL_ED [Up Down PgUp PgDn, Esc - browse]
#define FUNC_FIND [searching]
#define F_ATTACHING [for appending]
#define F_COPYING [for copying]
#define F_CREATE [File created: ]
#define F_ENVIR [for environment saving]
#define F_FLTRING [filter]
#define F_INDEXING [for indexing]
#define F_SEARCH [for searching:]
#define F_SHOW {[Show it?],[Abort]}
#define F_SORTING [for sorting]
#define GET_PSW [Password:]
#define GIVE_VAR _Give +[the variable and expression (f.e. xVar:=xVal)]
#define GLOB_FIND [context searching]
#define IND_FILE [Index file ]
#define INDEXING [indexing]
#define INDEX_CHOICE [Select index]
#define INDEX_CORRUPT [index ]+_corrupted
#define INDEX_NO L_A_SIGN+[ No Index]
#define INDEX_NOT_MATCH [Index does not match the base. Reindex?]
#define INI_F [of initialization]
#define INSERTING [inserting]
#define INVERTING [inverting]
#define IS_COMPUT [Can not replace computable field]
#define IS_UNIQ [Unique index]
#define IT_RECS [Total ]+SpaceString(m->__Count)+ [ records]
#define I_SEARCH [searching through index.]
#define KBYTE [ Kb]
#define L_YES_SET [YyTt1]
#define L_NO_SET [NnFf0]
#define LONGEST [Longest is: ]
#define MEMOFIELD [Memo-field view ]
#define MEMO_NO_MATCH {[MEMO-field exists, but it[s no signed in header],;
		       [Correct header?]}
#define MENU_MAX() Menu2({[Maximum],[Minimum]},1,WHAT_FIND+_ABORT)
#translate MENU_RECS(<txt>) => Menu2({ALL_RECS,\[Rest]}, _scope,;
			 \[Select the scope of ]+<txt>)
#define MENU_SCOPE {[1-Yes],[2-No],[3-Skip],[0-Abort]}
#define MSG_MNU_YESNO Menu2({[1-YES],[0-NO]},_choice,_mess,[ARE YOU SURE?])
#define NEED_DEL_FILE [. Erase it?]
#define NEED_FINISH [Finish working]
#define NEED_OEM2ANSI [Convert OEM to ANSI?]
#define NEED_PACK [Delete marked records]
#define NEED_REBUILD [Create all tags anew]
#define NEED_REINDEX [Reindex the base]
#define NEED_REPLACE [Replace: ]
#define NEED_ZAP [ZAP the base]
#define NODIGITAL [Current field is not numeric]
#define NOCHARACTER [Current field is not character]
#define NOOP_IND [The operation is prohibited when index is open]
#define NO_FIND_EXPR [There is not of searching condition]
#define NO_FLTR [Record is out of filter]
#define NO_FLTRING [No such records]
#define NO_DPZ [No such record]
#define NO_KAT [ - directory does not exist or locked]
#define NO_ORDER [No opened index files]
#define NO_SPACE [No free disk space]
#define N_RECORD [Record]
#define N_RECORD_COL [Record: ]
#define PACKING [packing]
#define PROCESSED [Processed: ]
#define READY [READY !]
#define RECALLING [recalling]
#define REC_GO [Go to the record:]
#define REC_FIND [Find record based on: ]
#define REC_FIND_C {[Record number],[Shift from current],[Key number]}
#define REC_SM [Position from current record (+ -): ]
#define REMOVED NTRIM(_tally)+ [ records was killed]
#define REMOVING [removing]
#define REPLACING [replacing]
#define REPL_EXPR [ for replacing ]
#define RESULT_IS [The result of: ]
#define SEC_M [ s.]
#define SELECT_BASE [Select the base]
#define SELECT_CP [Select default codepage]
#define SET_FIELDS [Set fields:]
#define SET_OF_OEM  "��������������������������������������������������������������������������"
#define SET_OF_ANSI "��������������������������������������������������������������*�����������"
#define SET_OF_USA  [QWERTYUIOP{}ASDFGHJKL:"ZXCVBNM<>qwertyuiop] + "[]" + [asdfghjkl;'zxcvbnm,.]
//#define SET_OF_USA  [QWERTYUIOP{}ASDFGHJKL: ZXCVBNM<>qwertyuiop]
//+ "[]" + [asdfghjkl;'zxcvbnm,.]
#define SET_OF_MYKB "����������������������������������㪥��������뢠�஫�����ᬨ����"
#define SET_OF_M_NO [123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ#]
#define SORTING [sorting]
#define SUMMING [summarizing]
#define SUM_IS [The sum  = ]
#define TAG_HEAD PAD([NAME],14)+PAD([EXPRESSION],45)+PAD([CONDITION],18)
#define TAG_NAME [tag name]
#define TAG_NAME_DEL [tag name for removing]
#define TAG_NO L_A_SIGN+[ No Order]
#define TAG_NUMBER [tag number]
#define TimerOff()  [Time:]+LTRIM(STR(Seconds()-m->_told,9,2))+ ;
	[ Processed: ]+NTRIM(m->_tally)
#define TOO_SHORT {[String is too short.],;
		   [Probably It will not be effective.],[Compress it ?]}
#define TXT_NAME _give+[filename]
#define UNSUPPORT [ - unsupported format]
#define USE_HISTORY [Use history]
#define MSG_USERMENU_NO [User-menu not defined]
#define WAIT_WIND [Waiting, ]
#define WHAT_COPY [What record: ]
#define WHAT_FIND [What do we need ?]
#define YESNO [Yes],[No ]
#define _NEEDOVER [ already exists. Overwrite ?]

// Private
	#define PRV_ZU       [Input the condition of ]
	#define PRV_ZIF      [File name ]
	#define PRV_ZIFN     [New file name: ]
	#define PRV_IV       [Index key ]
	#define PRV_NOFILE   [File not found ]
	#define PRV_NOMEMO   [Can not open file ]
	#define PRV_ABORT    [ (Esc - abort)]

// Static
	#define _STAT_CORRUPTED [ is corrupted]
	#define _STAT_ELSEUse [Base is in use by another.]
	#define _STAT_END_EDIT [F10 - Exit and save, Esc - Abort]
	#define _STAT_ERTIP [Wrong type ]
	#define _STAT_GIVE [Enter ]
	#define _STAT_GIVE_NAME L_A_SIGN+[ Other name]
	#define _STAT_NFD [Not found]
	#define _STAT_NOINDEX [No index set]
	#define _STAT_ZE [Input the expression ]

// InsKeys
	#define MSG_ClpVal [Clipboard value]
	#define MSG_cTitleIns [Insert at the cursor: ]
	#define MSG_aIns {[Base name],[Field value],[Field name],;
		[Field header],MSG_ClpVal,[Any field name],;
		[Any field header],[Quoted field value],[Symbol]}
	#define MSG_PUTTITLE [Put this result into: ]
	#define MSG_PUTWHERE {[Current field],[Clipboard],[System clipboard]}
	#define MSG_FLDHEAD_INS [Insert header of field]
	#define MSG_FLDNAME_INS [Insert name of field]

// PRINT
	#define ABORT_PRINT [ESC-for aborting]
	#define ALL_RECS [All records]
	#define BASE_STAT [ : situation by ]
	#define CENTR_NEED [Center justifying]
	#define COUNTER_NAME [NN]
	#define DEF_HBORDER [Field's delimiter: in header]
	#define DEF_BORDER [in body]
	#define DGT_HEAD_1ST [Head's numbers: on 1st page]
	#define DGT_HEAD_ALL [on next pages]
	#define END_CHAR [Ending]+_prn_seq
	#define F_OUT_AP [Append to output file?]
	#define F_OUT_PRINT [printing to ]
	#define FOROTBOR [printing]
	#define GROUP_HEAD [Code ]
	#define IS_PRINT [printing]
	#define LASTUPDATED [Last update: ]
	#define LEFT_BORD [ Left margin]
	#define MEM_NUM_LINE [Lines in memo-fields]
	#define MEM_WIDE_PRT [Memo-fields width]
	#define NEED_FF [Page eject]
	#define NEED_HEAD [Head's names on each page]
	#define NEED_SUM [Summing of Numeric]
	#define NUM_LF [Line feed number after each line]
	#define NUM_PAGE [Page numbers]
	#define ON_PAGE_TOTAL [On page]
	#define OUT_DEV [Print device]
	#define PAGE_LEN [Lines per page(0-no paging)]
	#define PLIST [Page ]
	#define PRNT_TOTAL [Total in group]
	#define PRNT_TOTAL_ALL [Total ]
	#define PRN_SEQ [ print codes]
	#define REC_NEED [Record numbers print]
	#define REC_RIGHT [At right side?]
	#define SELECT_PRINT [Print tuning]
	#define STAND_HEAD [Standard title]
	#define START_CHAR [Starting]+_prn_seq
	#define TITLE_ALL [Title on each page]
	#define WHERE_DEV [To file],[To HTML],[To printer]
	#define WHERE_OUT [Device of output? (ESC-abort, F9-tune)]
	#define ZERO_NEED [Print of zero values]
	#define _MSG_PR_F9 [Config]

// ModiStru
	#define BAD_NAME [Bad field name]
	#define BAD_LEN [Wrong field length]
	#define BE_CANCEL [Cancel modification]
	#define DEL_FIELD [Delete the field]
	#define FLD_TYPE [Select field type]
	#define EXIST_NAME [The name already exists]
	#define HEAD_BROW [Modify Structure]
	#define MAKE_CHANGE [Make permanent changes]
	#define MODIFICATION [modification]
	#define NEED_OPTIM [Make optimization]
	#define FIELDS_OPTIM [ fields are optimized]

// MainMenu
	#define _MM_MAINMENU {[System],[File],[Base],[Fiel~d],[Record],[Fi~nd],[Info],[Other]}
	#define _MM_SYSMENU  {  [Default RDD ]+'',;
				[Default codepage],;
				[Settings ]+'',;
				[Save settings],;
				[Calculate],;
				[Set variable],;
				[Help],;
				[About],;
				[Quit]}

	#define _MM_SETMENU {	[Print],;
				[Colors],;
				[SET-s],;
				[Other-1],;
				[Other-2]}

	#define _MM_FILEMENU {	[Open base],;
				[Reopen last],;
				[Change structure],;
				[Create new],;
				[Check header],;
				[Save environment],;
				[Restore environment],;
				[Read ini-file],;
				[Create Auto-Open file],;
				[Shell to DOS],;
				[Change disk/directory],;
				[Filer],;
				[Modify text],;
				[Export to HTML],;
				[Export to XML],;
				[Export to XLS];
				}

	#define _MM_BASEMENU {	[Append from],;
				[Copy To],;
				[Copy with MEMO converting],;
				[Delete],;
				[Restore],;
				[Invert deleted],;
				[Pack],;
				[Sort],;
				[Total],;
				[Index],;
				[Reindex],;
				[Set index],;
				[TAGS ]+'',;
				[Descend index],;
				[Set filter ]+'',;
				[Count],;
				[Print],;
				[Read report template];
				}

	#define _MM_RECORDMENU {[Add new],;
				[Insert before],;
				[Copy ]+'',;
				[Clear],;
				[Write to clipboard],;
				[Read from clipboard],;
				[Change with upper record],;
				[Change with lower record],;
				[Go To],;
				[Edit],;
				[Print To File];
				}

	#define _MM_FIELDMENU  {[Sum],;
				[Replace all by constant ]+'',;
				[Replace all by expression ]+'',;
				[Replace only current ]+'',;
				[Clear],;
				[Undo replace],;
				[Translate ]+'',;
				[Align ]+'',;
				[Crypt],;
				[DeCrypt],;
				[Check sum of record],;
				[Delete slim],;
				[Compress],;
				[DeCompress],;
				[Copy ]+'',;
				IF(m->_ptr<=m->_bdbfBrow:Freeze,[Unf],[F])+[reese],;
				[Set color condition for current field],;
				[Reset normal color];
				}
	#define _MM_FIELDMENU_HK [D]

	#define _MM_REPLMENU {[With confirming],[Without confirming]}

	#define _MM_TRANSMENU { [UPPER CASE],;
				[lower case],;
				[Proper],;
				[All Words Proper],;
				[OemToAnsi],;
				[AnsiToOem],;
				[To National keyboard],;
				[To USA keyboard];
			       }

	#define _MM_FINDMENU   {[Go to],;
				[Seek],;
				[Locate],;
				[Continue],;
				[Locate backwards],;
				[Continue backwards],;
				[Context (all fields)],;
				[Context with expression],;
				[In current field only ]+'',;
				[Maximum in field],;
				[Minimum in field],;
				[Longest in field],;
				[Double in index],;
				[WildSeek in index],;
				[Continue WildSeek];
	}

	#define _MM_INFOMENU  {[Environment,base,indexes],;
				[Base structure],;
				[ASCII-Table]}

	#define _MM_OTHERMENU  {[Play macro],;
				IF(m->_replay,[Restore fields],[Set fields]),;
				[Clipboard operations ]+'',;
				[User menu ]+''}

	#define _MM_TAGMENU    {[Set tag],;
				[Delete tag],;
				[Add tag],;
				[Set another CDX],;
				[Create all tags anew];
				}

	#define _MM_COPYMENU   {[Upper field ],;
				[Lower field],;
				[Left field],;
				[Right field],;
				[From clipboard],;
				[To clipboard],;
				[From System clipboard],;
				[To System clipboard]}

	#define _MM_CLIPBDMENU {[Read field to clipboard],;
				[Show clipboard],;
				[Write clipboard to field],;
				[Clipboard + field],;
				[Clipboard - field],;
				[Clipboard * field],;
				[Clipboard / field],;
				[Write current record to clipboard],;
				[Write to current record from clipboard]}


//VIEWEDIT
	#define _MSG_VF_PRWAIT [it is printing]
	#define _MSG_VF_CORRUPTED [ is corrupted]
	#define _MSG_VF_PRINTED   [ lines printed]
	#define FB_PRINT1 [PRINT:]
	#define FB_PRINT2 [Whole file]
	#define FB_PRINT3 [Rest of file]
	#define FB_PRINT4 [Abort]
	#define FB_SRCH  [Enter Search Phrase: ]
	#define FB_ALSO  [ Occurrences Found - No More Finds (Press Any Key) ]
	#define SAVE_AND_EXIT [File not saved! Are you sure you want to exit?]
	#define SAVE_AND_EXIT_ALT {[No], [Yes], [Save and quit]}

//ACTIONS
	#define _MSG_A_AREINDEX [reindexing (index does not match)]
	#define _MSG_A_CASE_SENSITIVE [CASE SENSITIVE]
	#define _MSG_A_CHARONLY [Current index is not character!]
	#define _MSG_A_FLOCK [Can not lock file. Repeat?]
	#define _MSG_A_RLOCK [Can not lock record. Repeat?]

	#define _MSG_A_UMENU [User menu]

	#define _MSG_A_F10 [ (F10 - saving)]
	#define _MSG_A_SETS [Program settings]

	#define _MSG_A_C_TOP	[Color settings]
	#define _MSG_A_C_ED	[Main edition color         ]
	#define _MSG_A_C_HLP	[Help, dialogues, results   ]
	#define _MSG_A_C_MARK	[Markers & special symbols  ]
	#define _MSG_A_C_MM	[Main menu                  ]
	#define _MSG_A_C_HIDE	[Unselectable menu elements ]
	#define _MSG_A_C_HD	[Panels headers             ]
	#define _MSG_A_C_INT	[Intensity]
	#define _MSG_A_C_BLINK	[Blinking]

	#define _MSG_A_O_TOP	[Different system variables]
	#define _MSG_A_O_FORCED [Forced mode]
	#define _MSG_A_O_IND	[Indicator]
	#define _MSG_A_O_FRQ	[Indicator refreshing frequency (in records)]
	#define _MSG_A_O_MSK	[Default database mask]
	#define _MSG_A_O_MAC	[Macro (called on Ctrl+F1)]
	#define _MSG_A_O_TIME	[Command timing]
	#define _MSG_A_O_ED	[Memo and wide fields editor (empty-embedded)]
	#define _MSG_A_O_VF	[Print files viewer (empty-embedded)         ]
	#define _MSG_A_O_TAB	[Tab width in memo fields]
	#define _MSG_A_O_WRAP	[Wrap position in memo fields]
	#define _MSG_A_O_NAME	[File names width in FILER]
	#define _MSG_A_O_ASAVE	[Auto-Save]
	#define _MSG_A_O_AREST	[Auto-Restore]
	#define _MSG_A_O_UNDO	[Max UNDO count]
	#define _MSG_A_O_PG	[Plugins search path]
	#define _MSG_A_O_MAXF	[Max number of shown fields]
	#define _MSG_A_O_PCKASK [PACK confirm]
	#define _MSG_A_O_PCKFND [Locate for Deleted before PACK]
*
	#define _MSG_A_O_CLCH	[Choice from internal clipboard]
	#define _MSG_A_O_WCLCH	[Choice fron Sys clipboard]
	#define _MSG_A_O_WCLCNV	[OEM/ANSI with Sys clipboard]
	#define _MSG_A_O_EXOEM	[OEM->ANSI during the exports]
	#define _MSG_A_O_EXOEMQ	[Ask for OEM->ANSI before the exports]
	#define _MSG_A_O_GSEXP	[Expand the context search expression]
	#define _MSG_A_O_GSCOND	[Additional condition of context search]
	#define _MSG_A_O_FASS	[File associations in FILER]

	#define _MSG_C_INI_H1	[Data Driver]
	#define _MSG_C_INI_H2	[System variables]
	#define _MSG_C_INI_H3	[Colors]
	#define _MSG_C_INI_H4	[Printer]
	#define _MSG_C_INI_H5	[Clipper standard SETs]
	#define _MSG_C_INI_H6	[User menu prompts]
	#define _MSG_C_INI_H7	[User menu actions]
	#define _MSG_C_INI_HAOP	[Fields, orders, conditions]

	#define _MSG_A_MF_F3	[View]
	#define _MSG_A_MF_F4	[Edit]

	#define _MSG_A_IF_TITLE [Select function: ]
	#define _MSG_A_IF_TRANS []
	#define _MSG_A_IF_AINS {;
			[Change symbols(StrTran)],;
			[Translate symbols(ChrTran)],;
			[UPPER CASE],;
			[lower case],;
			[Proper],;
			[All Words Proper],;
			[OemToAnsi],;
			[AnsiToOem],;
			[To national keyboard],;
			[To USA keyboard],;
			[Align],;
			[Crypt],;
			[DeCrypt],;
			[Check sum of record],;
			[Descend],;
			[Delete slim],;
			[Compress],;
			[DeCompress],;
			[Soundex];
		}

	#define _MSG_A_JUSTH {[Align:]}
	#define _MSG_A_JUSTA {[Left],[Center],[Right],[Justify],[Spread],[Reduce]}

	#define _MSG_A_FILTH {[SET FILTER TO:]}
	#define _MSG_A_FILTA {[Condition],;
				[Equal to Current Cell],;
				[Not Equal to Current Cell],;
				[Greater Than Current Cell],;
				[Less Than Current Cell],;
				[Greater or Equal Than Current Cell],;
				[Less or Equal Than Current Cell],;
				[No Filter]}

	#define _MSG_A_SIMPH {[Find Expression in Current Field:]}
	#define _MSG_A_SIMPA {	[Equal to Current Cell],;
				[Not Equal to Current Cell],;
				[Greater Than Current Cell],;
				[Less Than Current Cell],;
				[Greater or Equal Than Current Cell],;
				[Less or Equal Than Current Cell],;
				[Another]}

	#define _MSG_A_DS_F2 [Print]
	#define _MSG_A_DS_F3 [AddTag]
	#define _MSG_A_DS_F4 [File]
	#define _MSG_A_DS_F5 [Ext.Str]
	#define _MSG_A_DS_F6 [Struct]
	#define _MSG_A_DS_F8 [DelTag]
	#define _MSG_A_DS_F9 [Config]
	#define _MSG_A_DS_F10 [MakePrg]

	#define _MSG_A_ST_F2 [Print]
	#define _MSG_A_ST_F3 [AddTag]
	#define _MSG_A_ST_F4 [MakePrg]
	#define _MSG_A_ST_F5 [Reindex]
	#define _MSG_A_ST_F6 [Rebuild]
	#define _MSG_A_ST_F8 [DelTag]
	#define _MSG_A_ST_F9 [Config]


	#define _MSG_A_SDF_FILE_TYPE [File type]
	#define _MSG_A_SDF_F_DELIM [Text fields delimiter:]
	#define _MSG_A_SDF_A_FILETYPE {[SDF],[DELIMITED],[DELIMITED WITH BLANK],[DBF]}

	#define _MSG_A_CSIZE_NOERR [No errors found]
	#define _MSG_A_CSIZE	{[The divergences in definition of the database size were found out],;
				 [Logical size:      ]+NTRIM(dbSize),;
				 [Header-based size: ]+NTRIM(xSize),;
				 [Disk file size:    ]+NTRIM(fSize),;
				[],[What is right?];
				}
	#define _MSG_A_CSIZE_MENU1 {[Logical],[(recommended)]}
	#define _MSG_A_CSIZE_MENU2 [Header]
	#define _MSG_A_CSIZE_MENU3 [Disk]
	#define _MSG_A_CSIZE_MENU4 [Abort]

	#define _MSG_A_NOPACK [Deleted records are not found]

	#define _MSG_A_GFCOND [Additional condition]
//HELP
	#define _MSG_H_NO	[Sorry, no help here]
	#define _MSG_H_TITLE	[TAGS INFORMATION (RDD : ]
	#define _MSG_H_TAGS	[TAG      /  ]
	#define _MSG_H_TAGN	[Tag Name]
	#define _MSG_H_FE	[Filter expression]
	#define _MSG_H_ISU	[Is unique ?]
	#define _MSG_H_ISD	[Is descent ?]
	#define _MSG_H_RYO	[RYO-type ?]
	#define _MSG_H_KEYS	[Keys total]
	#define _MSG_H_CK	[Current key]

	#define _MSG_H_OKEY	{[DBF - editor],;
				BDBF_VERSION,[],;
				[Author: Eugen Bondar],;
				[Lugansk, Ukraine],;
				[E-MAIL: ]+EMAIL,;
				EMAIL2;
				}

	#define CODEPAGE	[Code page:]+CHR(0)
	#define CURR_DIR	[Current dir:]+CHR(0)
	#define DATE_UPDATE	[Last update:]+CHR(0)
	#define DRV_CURRENT	[Driver (type):]+CHR(0)
	#define FILE_SIZE	[Disk size:]+CHR(0)
	#define FREE_MEM	[Free memory:]+CHR(0)
	#define FREE_SPACE	[Free disk space:]+CHR(0)
	#define HEAD_SIZE	[Header size:]+CHR(0)
	#define REC_SIZE	[Record size:]+CHR(0)
	#define N_USER		[User:]+CHR(0)
	#define ON_SERVER	[ on server]+CHR(0)
	#define MEMO_BLOCK_SIZE [Memo block size:]+CHR(0)
	#define MEMO_FILE_SIZE  [Memo-file size:]+CHR(0)
	#define MEMO_B_SIZE_C   [Block size in it:]+CHR(0)
	#define OS_VER		[OS version:]+CHR(0)
	#define TOTAL_FIELDS	[Total fields:]+CHR(0)

// GetSys
	#define _MSG_INSOVR	[Ins],[Ovr]
	#define _MSG_EMODE	{[Usual Enter],[Lock Enter ],[Enter /skip],[Enter /stop]}
	#define _MSG_ASCII	[ASCII-Table]
	#define _MSG_ASC_SLCT	[Selected: ]

// ErrorSys
	#define NOT_READY [Printer is not ready.Repeat]
	#define NET_ERR [Network error]

// FILER
	#define _MSG_FI_TOP	[Select file: ]
	#define _MSG_FI_F2	[Disk]
	#define _MSG_FI_F5	[Go Home]
	#define _MSG_FI_F9	[Mask]
	#define _MSG_FI_F10	[Info]
	#define _MSG_FI_DSCR	[There is no description for this file]
	#define _MSG_FI_MASK	[Input file mask]

	#define _MSG_FI_FILE	[File:]+CHR(0)
	#define _MSG_FI_DIR	[Directory:]+CHR(0)
	#define _MSG_FI_SIZE	[Size:]+CHR(0)
	#define _MSG_FI_UPD	[Last updated:]+CHR(0)
	#define _MSG_FI_ATTR	[Attributes:]+CHR(0)
	#define _MSG_FI_IND	[Index information:]+CHR(0)
	#define _MSG_FI_KEY	[<KEY>]+CHR(0)

//ClipLib
	#define _MSG_CL_NOF	[Warning! It was fixed a numeric overflow.]
	#define _MSG_CL_NOONE	[Any correct field or expression is not given]

#ENDIF

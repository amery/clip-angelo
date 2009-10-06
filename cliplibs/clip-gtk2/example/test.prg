#include <ci_clip-gtk2.ch>
#include <ci_inkey.ch>

static hb, uTmp1 := 'no1', uTmp2 := 'no2', st, nEntry_ := {0,0}, bEntry_ := {.F.,.F.}, sEntry_ := {"Enter a access code","Enter a parol"}

function main()
	local w,  ;	// ��������� �� ������ ���� (������� ���� ���������)
	f,  ;	//
	vb,  ;	//
	sw,  ;	//
	qbtn, en, hb0, hb1, hb2, bn1, bn2

	/* Initialize GTK+ system */
	gtk_init()

	/* Create main window */
	/***************************************************/
	w:= gtk_WindowNew(,"main application window")
	gtk_SignalConnect(w,"delete-event",{|wid,e|gtk_Quit()})

	/* Create vertical box container */
	vb:= gtk_VBoxNew()
	gtk_ContainerAdd (w, vb)

	/* Create horizontal box containers and fill it */
	hb0 := gtk_HBoxNew()
	gtk_BoxPackStart(vb,hb0,.T.,.T.,2)

	nEntry_[1] :=gtk_EntryNew()
	gtk_EntrySetMaxLength(nEntry_[1],10)
	gtk_BoxPackStart(hb0,gtk_LabelNew(,"your code"),.F.,.F.,2)
	gtk_BoxPackStart(hb0,nEntry_[1],.F.,.F.,2)
	gtk_WidgetSetFocus(nEntry_[1])

	gtk_SignalConnect(nEntry_[1],GTK_EVENT, @KEY_HANDLER())
//��� �����
//����������� ���� ����� ������ �� �����������
// �� ������. ������ ������, ������ �� �����, ��� � ������� ����� ������.
//��� ��� ������ ������ ������������� - ������ �� �������� ?

//Re: ��� ���� � ���, ��� GTK_EVENT �������� � ���� ������ ��� �������,
// ������ �������� ������, ������� �������, ����������� � ������.
// ��� �� ������ ����� ����������� ��� �������. ���������� ��� ������
// �������. � ��� KEY_HANDLER() ������ ������� .t. || .f.. ���� return NIL,
// �� ���������, ��� ���������� �������� .t. � ��������� ������� ���������������
// .f. - ������, ��� ������ �������� ���������, �� ��� ������ ���������� �����������
// ��������� �������. � ����� ������, ����� ������������ ������ GTK_EVENT
// ������� KEY_HANDLER() ������ ���������� .f.. �� ������ ��������, ��� ��������
// ������-������ ������ ����������, � �� � ��� �� �������������� �� �� �����
// ����� � ��� - ��������, ������� NIL � ��������� ������� ������������.
// ������ ��� ������� ����������, �� �� ����� ������������.


// ��� ����� ��������� ��, �� ������ ������� ������� ���� �������� ������,
//�� �� �������� ����������� ������. ������ ������� � ��� ����������� ��������
//���������
//	gtk_SignalConnect(nEntry_[1],"key-press-event", @KEY_HANDLER())
//	gtk_SignalConnect(nEntry_[1],"focus-out-event", @KEY_HANDLER())

//RE:��� ������� �������� ������, ��� ���������� ������ ����� ������ ��������
//������ key-press-event ��� focus-out-event. ��� � KEY_HANDLER ��������������.


// � ������ - ����� ��������� ���������� �������� �� ���� ���� ����:
// 	gtk_SignalConnect(w,"delete-event",{|wid,e|gtk_Quit()})
// ��� � ��� ����������� ���������� �� �� ��������, ��� � � ���� ����
//( � ������ ������ wid,e) ?

// RE: ��, �������! ������ ��������� � ���� �������
// gtk_SignalConnect(w,"delete-event",{|wid,e|gtk_Quit()})
// �� � ������ ����������� ������� ������� �� �����������. ������ ��������� -
// quit � ���.

// ���� �� ��� � ���������� ������ ��������, �� ������������ � ��������
//� ������ ������������ ���������� ?

// RE: ����. ��� ������ ������� ��������� ������� � ������� �������� �
// widget.c � ������ ����� ���� ������ widget_signals ��� � ��� � �������
// ��� ��������� �������. ��������,
//static SignalTable widget_signals[] =
//{
//       /* signals */
//       {"map",                 GSF( widget_signal_handler ),   ESF( object_emit
//       {"unmap",               GSF( widget_signal_handler ),   ESF( object_emit
//	.....
// ��, ��� � �������� - ��� �������.
// ��� ������� ����������� ������� ����� ����, � ����� � �� ���� � ����������
// � ��������, �������� � widget_signals ���� �������. ���� ��� ����, �� ���
// ������� �������� ������� � ��������������� �����. ��������, ��� �������
// button, � ����� button.c ���� ������ button_signals.
// �� �������� ���������� �������. ����� �������� �������, ����� ��� �����������.
// �� ������� ����� ��� ������� widget_signal_handler() � ���� ������ ����������
// �������� ������, ������� � �����-������ ��� ���� ��������(���� ����), ����
// �������� ������ ���� ������ ����, �� ������ ����. ��� �������  ����� ��������
// ��������� ���������� �������� �� � ������ ��� map
// ���� ��� ������� ����������� ������, �� ��� ����� �������� ���������. ������
// ��� ����� ���������� ����� ���� ��������, ��� ��� ���-������.
// ��� ��������, ��� ������� "size-allocate"  ����� ������ ����������
// handle_size_allocate_signal() � ������� ����� ������������� �����������
// ������ �������� ��� ���������������� ������� - map . ��  ����� ���������
// 4 ��������: X, Y, WIDTH, HEIGHT. �� ���������������� ������� �������
// ������, ������ � ���� ������ - map.	  �� widget.c
// �������, ��� ������� ����������, �� ��� ��� � ����. ���������� ���������
// ������������ ���������� ������������� �� gtk , ������ � ��� ���� �
// ������ ������������ ��� ����� ��� ����� ������� ��������� ������������ ����������
// �������� ������� ���������?


	hb1 := gtk_HBoxNew()
	gtk_BoxPackStart(vb,hb1,.T.,.T.,2)

	nEntry_[2] :=gtk_EntryNew()
	gtk_EntrySetMaxLength(nEntry_[2],40)
	gtk_BoxPackStart(hb1,gtk_LabelNew(,"your parol"),.T.,.F.,2)
	gtk_BoxPackStart(hb1,nEntry_[2],.T.,.F.,2)
	gtk_SignalConnect(nEntry_[2],"key-press-event", @KEY_HANDLER())
	gtk_SignalConnect(nEntry_[2],"focus-out-event", @KEY_HANDLER())

	hb2 := gtk_HBoxNew()
	gtk_BoxPackStart(vb,hb2,.T.,.T.,2)

	bn1 :=gtk_ButtonNew(,"OK")
	gtk_SignalConnect(bn1,GTK_CLICKED_SIGNAL,{||Check_Entry_()})
	gtk_BoxPackStart(hb2,bn1,.T.,.T.,2)

	bn2 :=gtk_ButtonNew(,"Cancel")
	gtk_SignalConnect(bn2,GTK_CLICKED_SIGNAL,{||gtk_Quit()})
	gtk_BoxPackStart(hb2,bn2,.T.,.T.,2)
	gtk_SignalConnect(bn2,"deactivate", @CHECK_OK_())

	st := gtk_StatusBarNew()
	gtk_BoxPackEnd(vb,st,.F.,.F.)
	gtk_StatusBarSetText(st,"Enter a access code and parol")

	/* Show main window */
	/***************************************************/
	gtk_WidgetShowAll( w )

	/* Do GTK+ events */
	gtk_main()
return

STATIC FUNCTION Check_Entry_()
Local nTmp
FOR nTmp = 1 TO LEN(nEntry_)
    IF .NOT. bEntry_[nTmp]
	      gtk_StatusBarSetText(st,sEntry_[nTmp])
	      gtk_WidgetSetFocus(nEntry_[nTmp])
	RETURN .T.
    ENDIF
NEXT
gtk_Quit()
RETURN .T.

STATIC FUNCTION Check_OK_(nWidg_, Event)
// ����� �� ������������ ����� ������ ��� �������� ������������� ������� �� ������� ????????
// RE: �� ������, ������ ������� � ��� ������ ��������� ������������� �������?
Local nTmp, sTmp

FOR nTmp = 1 TO LEN(nEntry_)
    IF nWidg_ == nEntry_[nTmp]
	    sTmp := gtk_EntryGetText(nEntry_[nTmp])
	    IF LEN(TRIM(sTmp)) = 0				// �� ����� ������
		bEntry_[nTmp] = .F.                         // ������ �� �������
		  qout('fak '+ntoc(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
	    ELSE
		bEntry_[nTmp] = .T.                         // ������ �������
		  qout('OK '+ntoc(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
	    ENDIF
    ENDIF
NEXT
qout(';;;;OK ')      // ������� � ������ ��� ��������� � ������������� �����
RETURN .T.

STATIC FUNCTION KEY_HANDLER(nWidg_, Event)
LOCAL sTmp, sTmp1 := 'Enter ', nTmp, ;
bReady_ := .T.		// ���������� ���� ����� �����
// ��� ���������� ������ ������� ������� ������� ??????
// RE: ������-�� ������ �������/������ ����� ������� �������� EVENT ��� �������
// ��� �������, � ��������� ����� ���������� � ����� object.c ������� handle_events()
// � object_emit_event()
// ������ ����� �� � clip-gtk.ch
Local nTmp1
nTmp1 = Event:event

IF nTmp1 = GTK_KEY_PRESS
    FOR nTmp = 1 TO LEN(nEntry_)
       IF nWidg_ == nEntry_[nTmp]
	   EXIT
       ENDIF
    NEXT
    qout('GTK_KEY_PRESS'+STR(nTmp1)+'/'+STR(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
    IF Event:keyval == K_ENTER
	FOR nTmp = 1 TO LEN(nEntry_)
	    IF nWidg_ == nEntry_[nTmp]
		sTmp := gtk_EntryGetText(nEntry_[nTmp])
		IF LEN(TRIM(sTmp)) = 0				// �� ����� ������
			  gtk_StatusBarSetText(st,sEntry_[nTmp])      // ������� � ������ ��� ��������� � ������������� �����
			  gtk_WidgetSetFocus(nEntry_[nTmp])           // ������������� �����
		    bEntry_[nTmp] = .F.                         // ������ �� �������
		    bReady_ := .F.                              // ������, �� ��� ������ ������
			  qout(sEntry_[nTmp])      // ������� � ������ ��� ��������� � ������������� �����
		ELSE
		    bEntry_[nTmp] = .T.                         // ������ �������
		    IF nTmp = LEN(nEntry_)           // Enter �� ��������� ���� �����
			Check_Entry_()
		    ENDIF
		ENDIF
		EXIT
	    ENDIF
	 NEXT
*        IF .NOT. bReady_          // ���� �� ��� ������ ������
*    	       qout('Check_Entry_')      // ������� � ������ ��� ��������� � ������������� �����
	    Check_Entry_()
*        ENDIF
    ENDIF
ELSE
    FOR nTmp = 1 TO LEN(nEntry_)
       IF nWidg_ == nEntry_[nTmp]
	   EXIT
       ENDIF
    NEXT
    IF nTmp1 = GTK_LEAVE_NOTIFY
	qout('GTK_LEAVE_NOTIFY'+STR(nTmp1)+'/'+STR(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
    ELSEIF nTmp1 = GTK_ENTER_NOTIFY
	qout('GTK_ENTER_NOTIFY'+STR(nTmp1)+'/'+STR(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
    ELSEIF nTmp1 = GTK_FOCUS_OUT_EVENT
	qout('GTK_FOCUS_OUT_EVENT'+STR(nTmp1)+'/'+STR(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
    ELSEIF nTmp1 = GTK_FOCUS_CHANGE
	qout('GTK_FOCUS_CHANGE'+STR(nTmp1)+'/'+STR(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
    ELSE
	qout('Event'+STR(nTmp1)+'/'+STR(nTmp))      // ������� � ������ ��� ��������� � ������������� �����
    ENDIF
ENDIF
RETURN .f.

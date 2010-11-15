#ifndef _CLIP_GTK_DEFINED
#define _CLIP_GTK_DEFINED

#define EG_NOWIDGET		101
#define EG_WIDGETTYPE		102
#define EG_NOOBJECT		103
#define EG_OBJECTTYPE		104

/* Maximum value of color in RGB */
#define CLIP_GTK_MAX_COLOR	65535


/* ������� */
/*
 *   Nothing: No event occurred.
 *   Delete: A window delete event was sent by the window manager.
 *	     The specified window should be deleted.
 *   Destroy: A window has been destroyed.
 *   Expose: Part of a window has been uncovered.
 *   NoExpose: Same as expose, but no expose event was generated.
 *   VisibilityNotify: A window has become fully/partially/not obscured.
 *   MotionNotify: The mouse has moved.
 *   ButtonPress: A mouse button was pressed.
 *   ButtonRelease: A mouse button was release.
 *   KeyPress: A key was pressed.
 *   KeyRelease: A key was released.
 *   EnterNotify: A window was entered.
 *   LeaveNotify: A window was exited.
 *   FocusChange: The focus window has changed. (The focus window gets
 *		  keyboard events).
 *   Resize: A window has been resized.
 *   Map: A window has been mapped. (It is now visible on the screen).
 *   Unmap: A window has been unmapped. (It is no longer visible on
 *	    the screen).
 */
#define GTK_UNKNOWN_EVENT	-100
#define GTK_NOTHING		-1
#define GTK_DELETE		0
#define GTK_DESTROY		1
#define GTK_EXPOSE		2
#define GTK_MOTION_NOTIFY	3
#define GTK_BUTTON_PRESS	4
#define GTK_2BUTTON_PRESS	5
#define GTK_3BUTTON_PRESS	6
#define GTK_BUTTON_RELEASE	7
#define GTK_KEY_PRESS		8
#define GTK_KEY_RELEASE		9
#define GTK_ENTER_NOTIFY	10
#define GTK_LEAVE_NOTIFY	11
#define GTK_FOCUS_CHANGE	12
#define GTK_CONFIGURE		13
#define GTK_MAP			14
#define GTK_UNMAP		15
#define GTK_PROPERTY_NOTIFY	16
#define GTK_SELECTION_CLEAR	17
#define GTK_SELECTION_REQUEST 	18
#define GTK_SELECTION_NOTIFY	19
#define GTK_PROXIMITY_IN	20
#define GTK_PROXIMITY_OUT	21
#define GTK_DRAG_ENTER        	22
#define GTK_DRAG_LEAVE        	23
#define GTK_DRAG_MOTION       	24
#define GTK_DRAG_STATUS       	25
#define GTK_DROP_START        	26
#define GTK_DROP_FINISHED     	27
#define GTK_CLIENT_EVENT	28
#define GTK_VISIBILITY_NOTIFY 	29
#define GTK_NO_EXPOSE		30
#define GTK_EVENT		HASH___GTK_EVENT


/* signals */
#define GTK_PRESSED_SIGNAL		HASH___GTK_PRESSED_SIGNAL
#define GTK_RELEASED_SIGNAL		HASH___GTK_RELEASED_SIGNAL
#define GTK_CLICKED_SIGNAL		HASH___GTK_CLICKED_SIGNAL
#define GTK_ENTER_SIGNAL		HASH___GTK_ENTER_SIGNAL
#define GTK_LEAVE_SIGNAL		HASH___GTK_LEAVE_SIGNAL
#define GTK_SET_FOCUS_SIGNAL		HASH___GTK_SET_FOCUS_SIGNAL
#define GTK_TOGGLED_SIGNAL		HASH___GTK_TOGGLED_SIGNAL
#define GTK_DRAW_INDICATOR_SIGNAL	HASH___GTK_DRAW_INDICATOR_SIGNAL
#define GTK_FOCUS_IN_EVENT		HASH___GTK_FOCUS_IN_EVENT
#define GTK_FOCUS_OUT_EVENT		HASH___GTK_FOCUS_OUT_EVENT
#define GTK_DEACTIVATE_SIGNAL		HASH___DEACTIVATE_SIGNAL
#define GTK_DISCONNECT_SIGNAL		HASH___DISCONNECT_SIGNAL
#define GTK_SELECTION_DONE_SIGNAL	HASH___SELECTION_DONE_SIGNAL
#define GTK_MOVE_CURRENT_SIGNAL		HASH___MOVE_CURRENT_SIGNAL
#define GTK_ACTIVATE_CURRENT_SIGNAL	HASH___ACTIVATE_CURRENT_SIGNAL
#define GTK_CANCEL_SIGNAL		HASH___CANCEL_SIGNAL
#define GTK_ACTIVATE_SIGNAL		HASH___ACTIVATE_SIGNAL
#define GTK_ACTIVATE_ITEM_SIGNAL	HASH___ACTIVATE_ITEM_SIGNAL
#define GTK_CHANGED_SIGNAL		HASH___CHANGED_SIGNAL
#define GTK_VALUE_CHANGED_SIGNAL	HASH___VALUE_CHANGED_SIGNAL
#define GTK_INSERT_TEXT_SIGNAL		HASH___INSERT_TEXT_SIGNAL
#define GTK_DELETE_TEXT_SIGNAL		HASH___DELETE_TEXT_SIGNAL
#define GTK_SET_EDITABLE_SIGNAL		HASH___SET_EDITABLE_SIGNAL
#define GTK_MOVE_CURSOR_SIGNAL		HASH___MOVE_CURSOR_SIGNAL
#define GTK_MOVE_WORD_SIGNAL		HASH___MOVE_WORD_SIGNAL
#define GTK_MOVE_PAGE_SIGNAL		HASH___MOVE_PAGE_SIGNAL
#define GTK_MOVE_TO_ROW_SIGNAL		HASH___MOVE_TO_ROW_SIGNAL
#define GTK_MOVE_TO_COLUMN_SIGNAL	HASH___MOVE_TO_COLUMN_SIGNAL
#define GTK_KILL_CHAR_SIGNAL		HASH___KILL_CHAR_SIGNAL
#define GTK_KILL_WORD_SIGNAL		HASH___KILL_WORD_SIGNAL
#define GTK_KILL_LINE_SIGNAL		HASH___KILL_LINE_SIGNAL
#define GTK_CUT_CLIPBOARD_SIGNAL	HASH___CUT_CLIPBOARD_SIGNAL
#define GTK_COPY_CLIPBOARD_SIGNAL	HASH___COPY_CLIPBOARD_SIGNAL
#define GTK_PASTE_CLIPBOARD_SIGNAL	HASH___PASTE_CLIPBOARD_SIGNAL
#define GTK_SHOW_SIGNAL			HASH___SHOW_SIGNAL
#define GTK_HIDE_SIGNAL			HASH___HIDE_SIGNAL
#define GTK_MAP_SIGNAL			HASH___MAP_SIGNAL
#define GTK_UNMAP_SIGNAL		HASH___UNMAP_SIGNAL
#define GTK_REALIZE_SIGNAL		HASH___REALIZE_SIGNAL
#define GTK_UNREALIZE_SIGNAL		HASH___UNREALIZE_SIGNAL
#define GTK_DRAW_SIGNAL			HASH___DRAW_SIGNAL
#define GTK_DRAW_FOCUS_SIGNAL		HASH___DRAW_FOCUS_SIGNAL
#define GTK_DRAW_DEFAULT_SIGNAL		HASH___DRAW_DEFAULT_SIGNAL
#define GTK_SIZE_REQUEST_SIGNAL		HASH___SIZE_REQUEST_SIGNAL
#define GTK_SIZE_ALLOCATE_SIGNAL	HASH___SIZE_ALLOCATE_SIGNAL
#define GTK_STATE_CHANGED_SIGNAL	HASH___STATE_CHANGED_SIGNAL
#define GTK_PARENT_SET_SIGNAL		HASH___PARENT_SET_SIGNAL
#define GTK_STYLE_SET_SIGNAL		HASH___STYLE_SET_SIGNAL
#define GTK_ADD_ACCELERATOR_SIGNAL	HASH___ADD_ACCELERATOR_SIGNAL
#define GTK_REMOVE_ACCELERATOR_SIGNAL	HASH___REMOVE_ACCELERATOR_SIGNAL
#define GTK_GRAB_FOCUS_SIGNAL		HASH___GRAB_FOCUS_SIGNAL
#define GTK_SELECT_SIGNAL		HASH___SELECT_SIGNAL
#define GTK_TOGGLE_SIGNAL		HASH___TOGGLE_SIGNAL
#define GTK_DESELECT_SIGNAL		HASH___DESELECT_SIGNAL
#define GTK_SELECTION_GET_SIGNAL	HASH___SELECTION_GET_SIGNAL
#define GTK_SELECTION_RECEIVED_SIGNAL	HASH___SELECTION_RECEIVED_SIGNAL
#define GTK_DRAG_BEGIN_SIGNAL		HASH___DRAG_BEGIN_SIGNAL
#define GTK_DRAG_END_SIGNAL		HASH___DRAG_END_SIGNAL
#define GTK_DRAG_DATA_DELETE_SIGNAL	HASH___DRAG_DATA_DELETE_SIGNAL
#define GTK_DRAG_LEAVE_SIGNAL		HASH___DRAG_LEAVE_SIGNAL
#define GTK_DRAG_MOTION_SIGNAL		HASH___DRAG_MOTION_SIGNAL
#define GTK_DRAG_DROP_SIGNAL		HASH___DRAG_DROP_SIGNAL
#define GTK_DRAG_DATA_GET_SIGNAL	HASH___DRAG_DATA_GET_SIGNAL
#define GTK_DRAG_DATA_RECEIVED_SIGNAL	HASH___DRAG_DATA_RECEIVED_SIGNAL
#define GTK_DEBUG_MSG_SIGNAL		HASH___DEBUG_MSG_SIGNAL
#define GTK_TEXT_PUSHED_SIGNAL		HASH___TEXT_PUSHED_SIGNAL
#define GTK_TEXT_POPPED_SIGNAL		HASH___TEXT_POPPED_SIGNAL
#define GTK_SWITCH_PAGE_SIGNAL		HASH___SWITCH_PAGE_SIGNAL
#define GTK_ADD_SIGNAL			HASH___ADD_PAGE_SIGNAL
#define GTK_REMOVE_SIGNAL		HASH___REMOVE_SIGNAL
#define GTK_CHECK_RESIZE_SIGNAL		HASH___CHECK_RESIZE_SIGNAL
#define GTK_FOCUS_SIGNAL		HASH___FOCUS_SIGNAL
#define GTK_SET_FOCUS_CHILD_SIGNAL	HASH___SET_FOCUS_CHILD_SIGNAL
#define GTK_OK_BUTTON_CLICKED_SIGNAL	HASH___OK_BUTTON_CLICKED_SIGNAL
#define GTK_APPLY_BUTTON_CLICKED_SIGNAL	HASH___APPLY_BUTTON_CLICKED_SIGNAL
#define GTK_CANCEL_BUTTON_CLICKED_SIGNAL	HASH___CANCEL_BUTTON_CLICKED_SIGNAL
#define GTK_HELP_BUTTON_CLICKED_SIGNAL	HASH___HELP_BUTTON_CLICKED_SIGNAL
#define GTK_COLOR_CHANGED_SIGNAL	HASH___COLOR_CHANGED_SIGNAL
#define GTK_SELECT_ROW_SIGNAL           HASH___SELECT_ROW_SIGNAL
#define GTK_UNSELECT_ROW_SIGNAL         HASH___UNSELECT_ROW_SIGNAL
#define GTK_ROW_MOVE_SIGNAL             HASH___ROW_MOVE_SIGNAL
#define GTK_CLICK_COLUMN_SIGNAL         HASH___CLICK_COLUMN_SIGNAL
#define GTK_RESIZE_COLUMN_SIGNAL        HASH___RESIZE_COLUMN_SIGNAL
#define GTK_TOGGLE_FOCUS_ROW_SIGNAL     HASH___TOGGLE_FOCUS_ROW_SIGNAL
#define GTK_SELECT_ALL_SIGNAL           HASH___SELECT_ALL_SIGNAL
#define GTK_UNSELECT_ALL_SIGNAL         HASH___UNSELECT_ALL_SIGNAL
#define GTK_UNDO_SELECTION_SIGNAL       HASH___UNDO_SELECTION_SIGNAL
#define GTK_START_SELECTION_SIGNAL      HASH___START_SELECTION_SIGNAL
#define GTK_END_SELECTION_SIGNAL        HASH___END_SELECTION_SIGNAL
#define GTK_TOGGLE_ADD_MODE_SIGNAL      HASH___TOGGLE_ADD_MODE_SIGNAL
#define GTK_EXTEND_SELECTION_SIGNAL     HASH___EXTEND_SELECTION_SIGNAL
#define GTK_SCROLL_VERTICAL_SIGNAL      HASH___SCROLL_VERTICAL_SIGNAL
#define GTK_SCROLL_HORIZONTAL_SIGNAL    HASH___SCROLL_HORIZONTAL_SIGNAL
#define GTK_ABORT_COLUMN_RESIZE_SIGNAL  HASH___ABORT_COLUMN_RESIZE_SIGNAL
#define GTK_TREE_SELECT_ROW_SIGNAL	HASH___TREE_SELECT_ROW_SIGNAL
#define GTK_TREE_UNSELECT_ROW_SIGNAL	HASH___TREE_UNSELECT_ROW_SIGNAL
#define GTK_TREE_EXPAND_SIGNAL		HASH___TREE_EXPAND_SIGNAL
#define GTK_TREE_COLLAPSE_SIGNAL	HASH___TREE_COLLAPSE_SIGNAL
#define GTK_TREE_MOVE_SIGNAL		HASH___TREE_MOVE_SIGNAL
#define GTK_CHANGE_FOCUS_ROW_EXPANSION_SIGNAL	HASH___CHANGE_FOCUS_ROW_EXPANSION_SIGNAL
#define GTK_DRAW_BACKGROUND_SIGNAL	HASH___DRAW_BACKGROUND_SIGNAL
#define GTK_CLEAR_BACKGROUND_SIGNAL	HASH___CLEAR_BACKGROUND_SIGNAL
#define GTK_DRAW_TROUGH_SIGNAL		HASH___DRAW_TROUGH_SIGNAL
#define GTK_DRAW_SLIDER_SIGNAL		HASH___DRAW_SLIDER_SIGNAL
#define GTK_STEP_FORW_SIGNAL		HASH___STEP_FORW_SIGNAL
#define GTK_STEP_BACK_SIGNAL		HASH___STEP_BACK_SIGNAL
#define GTK_SLIDER_UPDATE_SIGNAL	HASH___SLIDER_UPDATE_SIGNAL
#define GTK_TROUGH_CLICK_SIGNAL		HASH___TROUGH_CLICK_SIGNAL
#define GTK_TROUGH_KEYS_SIGNAL		HASH___TROUGH_KEYS_SIGNAL
#define GTK_MOTION_SIGNAL		HASH___MOTION_SIGNAL
#define GTK_DESTROY_SIGNAL		HASH___DESTROY_SIGNAL

#define GTK_MONTH_CHANGED_SIGNAL	HASH___GTK_MONTH_CHANGED_SIGNAL
#define GTK_DAY_SELECTED_SIGNAL		HASH___DAY_SELECTED_SIGNAL
#define GTK_DAY_SELECTED_DOUBLE_CLICK_SIGNAL	HASH___GTK_DAY_SELECTED_DOUBLE_CLICK_SIGNAL
#define GTK_PREV_MONTH_SIGNAL		HASH___GTK_PREV_MONTH_SIGNAL
#define GTK_NEXT_MONTH_SIGNAL		HASH___GTK_NEXT_MONTH_SIGNAL
#define GTK_PREV_YEAR_SIGNAL		HASH___GTK_PREV_YEAR_SIGNAL
#define GTK_NEXT_YEAR_SIGNAL		HASH___GTK_NEXT_YEAR_SIGNAL

#define GTK_CURVE_TYPE_CHANGED_SIGNAL	HASH___GTK_CURVE_TYPE_CHANGED_SIGNAL

#define GTK_CHILD_ATTACHED_SIGNAL	HASH___GTK_CHILD_ATTACHED_SIGNAL
#define GTK_CHILD_DETACHED_SIGNAL	HASH___GTK_CHILD_DETACHED_SIGNAL

#define GTK_ENABLE_DEVICE_SIGNAL	HASH___GTK_ENABLE_DEVICE_SIGNAL
#define GTK_DISABLE_DEVICE_SIGNAL	HASH___GTK_DISABLE_DEVICE_SIGNAL

#define GTK_SELECTION_CHANGED_SIGNAL	HASH___GTK_SELECTION_CHANGED_SIGNAL
#define GTK_SELECT_CHILD_SIGNAL		HASH___GTK_SELECT_CHILD_SIGNAL
#define GTK_UNSELECT_CHILD_SIGNAL	HASH___GTK_UNSELECT_CHILD_SIGNAL

#define GTK_ORIENTATION_CHANGED_SIGNAL	HASH___GTK_ORIENTATION_CHANGED_SIGNAL
#define GTK_STYLE_CHANGED_SIGNAL	HASH___GTK_STYLE_CHANGED_SIGNAL

#define GTK_START_QUERY_SIGNAL		HASH___GTK_START_QUERY_SIGNAL
#define GTK_STOP_QUERY_SIGNAL		HASH___GTK_STOP_QUERY_SIGNAL
#define GTK_WIDGET_ENTERED_SIGNAL	HASH___GTK_WIDGET_ENTERED_SIGNAL
#define GTK_WIDGET_SELECTED_SIGNAL	HASH___GTK_WIDGET_SELECTED_SIGNAL

#define GTK_EXPAND_SIGNAL		HASH___EXPAND_SIGNAL
#define GTK_COLLAPSE_SIGNAL		HASH___COLLAPSE_SIGNAL

/* extext signals */
#define GTK_PROPERTY_TEXT_INSERT_SIGNAL HASH___PROPERTY_TEXT_INSERT_SIGNAL
#define GTK_PROPERTY_TEXT_REMOVE_SIGNAL HASH___PROPERTY_TEXT_REMOVE_SIGNAL
#define GTK_PROPERTY_DESTROY_SIGNAL     HASH___PROPERTY_DESTROY_SIGNAL
#define GTK_UNDO_CHANGED_SIGNAL         HASH___UNDO_CHANGED_SIGNAL
#define GTK_PROPERTY_MARK_SIGNAL        HASH___PROPERTY_MARK_SIGNAL
#define GTK_LINE_INSERT_SIGNAL          HASH___LINE_INSERT_SIGNAL
#define GTK_LINE_REMOVE_SIGNAL          HASH___LINE_REMOVE_SIGNAL
#define GTK_MARGIN_ENTER_SIGNAL         HASH___MARGIN_ENTER_SIGNAL
#define GTK_MARGIN_EXIT_SIGNAL          HASH___MARGIN_EXIT_SIGNAL
#define GTK_MARGIN_CLICKED_SIGNAL       HASH___MARGIN_CLICKED_SIGNAL


/* set window position */
#define GTK_WIN_POS_NONE		0
#define GTK_WIN_POS_CENTER		1
#define GTK_WIN_POS_MOUSE		2
#define GTK_WIN_POS_CENTER_ALWAYS	3
#define GTK_WIN_POS_CENTER_ON_PARENT	4



/* ������������ ��������� */

#define GDK_SHIFT_MASK		    1
#define GDK_LOCK_MASK		    2
#define GDK_CONTROL_MASK	    4
#define GDK_MOD1_MASK		    8
#define GDK_MOD2_MASK		   16
#define GDK_MOD3_MASK		   32
#define GDK_MOD4_MASK		   64
#define GDK_MOD5_MASK		  128
#define GDK_BUTTON1_MASK	  256
#define GDK_BUTTON2_MASK	  512
#define GDK_BUTTON3_MASK	 1024
#define GDK_BUTTON4_MASK	 2048
#define GDK_BUTTON5_MASK	 4096
#define GDK_RELEASE_MASK	 8192
#define GDK_MODIFIER_MASK	16383

/* Accelerator flags */
    /* should the accelerator appear in the widget's display? */
#define GTK_ACCEL_VISIBLE		1
    /* should the signal associated with this accelerator be also visible? */
#define GTK_ACCEL_SIGNAL_VISIBLE	2
    /* may the accelerator be removed again? */
#define GTK_ACCEL_LOCKED		4
#define GTK_ACCEL_MASK			7

/* ������� */
#define GTK_OBJ_OBJECT			HASH___GTK_OBJ_OBJECT
#define GDK_OBJECT_FONT			HASH___GDK_OBJECT_FONT
#define GDK_OBJECT_GC			HASH___GDK_OBJECT_GC
#define GDK_OBJECT_CURSOR		HASH___GDK_OBJECT_CURSOR
#define GDK_OBJECT_WINDOW		HASH___GDK_OBJECT_WINDOW
#define GDK_OBJECT_REGION		HASH___GDK_OBJECT_REGION
#define GDK_OBJECT_COLORMAP		HASH___GDK_OBJECT_COLORMAP

#define GTK_OBJECT_CTREE_NODE		HASH___GTK_OBJECT_CTREE_NODE
#define GTK_OBJ_WIDGET			HASH___GTK_OBJ_WIDGET
#define GTK_OBJECT_ITEM_FACTORY		HASH___GTK_OBJECT_ITEM_FACTORY

/* ���� �������� */
#define GTK_WIDGET_OBJECT		HASH___GTK_WIDGET_OBJECT
#define GTK_WIDGET_WIDGET		HASH___GTK_WIDGET_WIDGET
#define GTK_WIDGET_UNKNOWN		HASH___GTK_WIDGET_UNKNOWN
#define GTK_WIDGET_CONTAINER		HASH___GTK_WIDGET_CONTAINER
#define GTK_WIDGET_BOX			HASH___GTK_WIDGET_BOX
#define GTK_WIDGET_SEPARATOR		HASH___GTK_WIDGET_SEPARATOR
#define GTK_WIDGET_EDITABLE		HASH___GTK_WIDGET_EDITABLE
#define GTK_WIDGET_ITEM			HASH___GTK_WIDGET_ITEM
#define GTK_WIDGET_ADJUSTMENT		HASH___GTK_WIDGET_ADJUSTMENT
#define GTK_WIDGET_DATA			HASH___GTK_WIDGET_DATA
#define GTK_WIDGET_RANGE		HASH___GTK_WIDGET_RANGE
#define GTK_WIDGET_MISC			HASH___GTK_WIDGET_MISC
#define GTK_WIDGET_RULER		HASH___GTK_WIDGET_RULER
#define GTK_OBJECT_ACCEL_GROUP		HASH___GTK_OBJECT_ACCEL_GROUP


/* Windows & Dialogs */
#define GTK_WIDGET_WINDOW			HASH___GTK_WIDGET_WINDOW
#define GTK_WIDGET_DIALOG			HASH___GTK_WIDGET_DIALOG
#define GTK_WIDGET_INPUT_DIALOG			HASH___GTK_WIDGET_INPUT_DIALOG
#define GTK_WIDGET_FONT_SELECTION_DIALOG	HASH___GTK_WIDGET_FONT_SELECTION_DIALOG
#define GTK_WIDGET_COLOR_SELECTION_DIALOG	HASH___GTK_WIDGET_COLOR_SELECTION_DIALOG
#define GTK_WIDGET_FILE_SELECTION		HASH___GTK_WIDGET_FILE_SELECTION
/* Labels & Buttons */
#define GTK_WIDGET_LABEL		HASH___GTK_WIDGET_LABEL
#define GTK_WIDGET_ACCEL_LABEL		HASH___GTK_WIDGET_ACCEL_LABEL
#define GTK_WIDGET_BUTTON		HASH___GTK_WIDGET_BUTTON
#define GTK_WIDGET_TOGGLE_BUTTON	HASH___GTK_WIDGET_TOGGLE_BUTTON
#define GTK_WIDGET_CHECK_BUTTON		HASH___GTK_WIDGET_CHECK_BUTTON
#define GTK_WIDGET_SPIN_BUTTON		HASH___GTK_WIDGET_SPIN_BUTTON
#define GTK_WIDGET_RADIO		HASH___GTK_WIDGET_RADIO
#define GTK_WIDGET_RADIO_BOX		HASH___GTK_WIDGET_RADIO_BOX
#define GTK_WIDGET_RADIO_GROUP		HASH___GTK_WIDGET_RADIO_GROUP
/* Menus & Menu Bars */
#define GTK_WIDGET_MENU_SHELL		HASH___GTK_WIDGET_MENU_SHELL
#define GTK_WIDGET_MENU_BAR		HASH___GTK_WIDGET_MENU_BAR
#define GTK_WIDGET_MENU			HASH___GTK_WIDGET_MENU
#define GTK_WIDGET_MENU_ITEM		HASH___GTK_WIDGET_MENU_ITEM
#define GTK_WIDGET_CHECK_MENU_ITEM	HASH___GTK_WIDGET_CHECK_MENU_ITEM
#define GTK_WIDGET_RADIO_MENU_ITEM	HASH___GTK_WIDGET_RADIO_MENU_ITEM
#define GTK_WIDGET_RADIO_MENU_GROUP	HASH___GTK_WIDGET_RADIO_MENU_GROUP
#define GTK_WIDGET_TEAROFF_MENU_ITEM	HASH___GTK_WIDGET_TEAROFF_MENU_ITEM
/* Data Entry Widgets */
#define GTK_WIDGET_TEXT			HASH___GTK_WIDGET_TEXT
#define GTK_WIDGET_ENTRY		HASH___GTK_WIDGET_ENTRY
#define GTK_WIDGET_COMBO		HASH___GTK_WIDGET_COMBO
#define GTK_WIDGET_COMBO_SIMPLE		HASH___GTK_WIDGET_COMBO_SIMPLE
#define GTK_WIDGET_OPTION_MENU		HASH___GTK_WIDGET_OPTION_MENU
#define GTK_WIDGET_SCALE		HASH___GTK_WIDGET_SCALE
#define GTK_WIDGET_HSCALE		HASH___GTK_WIDGET_HSCALE
#define GTK_WIDGET_VSCALE		HASH___GTK_WIDGET_VSCALE
/* Containers */
#define GTK_WIDGET_ALIGNMENT		HASH___GTK_WIDGET_ALIGNMENT
#define GTK_WIDGET_LAYOUT		HASH___GTK_WIDGET_LAYOUT
#define GTK_WIDGET_FIXED		HASH___GTK_WIDGET_FIXED
#define GTK_WIDGET_HBOX			HASH___GTK_WIDGET_HBOX
#define GTK_WIDGET_VBOX			HASH___GTK_WIDGET_VBOX
#define GTK_WIDGET_BUTTON_BOX		HASH___GTK_WIDGET_BUTTON_BOX
#define GTK_WIDGET_HBUTTON_BOX		HASH___GTK_WIDGET_HBUTTON_BOX
#define GTK_WIDGET_VBUTTON_BOX		HASH___GTK_WIDGET_VBUTTON_BOX
#define GTK_WIDGET_SCROLLED_WINDOW	HASH___GTK_WIDGET_SCROLLED_WINDOW
#define GTK_WIDGET_FRAME		HASH___GTK_WIDGET_FRAME
#define GTK_WIDGET_ASPECT_FRAME		HASH___GTK_WIDGET_ASPECT_FRAME
#define GTK_WIDGET_HPANED		HASH___GTK_WIDGET_HPANED
#define GTK_WIDGET_VPANED		HASH___GTK_WIDGET_VPANED
#define GTK_WIDGET_NOTEBOOK		HASH___GTK_WIDGET_NOTEBOOK
#define GTK_WIDGET_EVENT_BOX		HASH___GTK_WIDGET_EVENT_BOX
#define GTK_WIDGET_HANDLE_BOX		HASH___GTK_WIDGET_HANDLE_BOX
#define GTK_WIDGET_PANED		HASH___GTK_WIDGET_PANED
#define GTK_WIDGET_TOOLBAR		HASH___GTK_WIDGET_TOOLBAR
#define GTK_WIDGET_PACKER		HASH___GTK_WIDGET_PACKER
#define GTK_WIDGET_TABLE		HASH___GTK_WIDGET_TABLE
#define GTK_WIDGET_VIEWPORT		HASH___GTK_WIDGET_VIEWPORT
/* Lists & Trees */
#define GTK_WIDGET_LIST			HASH___GTK_WIDGET_LIST
#define GTK_WIDGET_LIST_ITEM		HASH___GTK_WIDGET_LIST_ITEM
#define GTK_WIDGET_CLIST		HASH___GTK_WIDGET_CLIST
#define GTK_WIDGET_CTREE		HASH___GTK_WIDGET_CTREE
#define GTK_WIDGET_TREE			HASH___GTK_WIDGET_TREE
#define GTK_WIDGET_TREE_ITEM		HASH___GTK_WIDGET_TREE_ITEM
/* Tooltips */
#define GTK_WIDGET_TIPS_QUERY		HASH___GTK_WIDGET_TIPS_QUERY
#define GTK_WIDGET_TOOLTIPS		HASH___GTK_WIDGET_TOOLTIPS
/* Graphical Widgets */
#define GTK_WIDGET_PIXMAP 		HASH___GTK_WIDGET_PIXMAP
#define GTK_WIDGET_DRAWING_AREA		HASH___GTK_WIDGET_DRAWING_AREA
#define GTK_WIDGET_CURVE		HASH___GTK_WIDGET_CURVE
#define GTK_WIDGET_GAMMA_CURVE		HASH___GTK_WIDGET_GAMMA_CURVE
/* Misc. Widgets */
#define GTK_WIDGET_SCROLLBAR 		HASH___GTK_WIDGET_SCROLLBAR
#define GTK_WIDGET_HSCROLLBAR 		HASH___GTK_WIDGET_HSCROLLBAR
#define GTK_WIDGET_VSCROLLBAR 		HASH___GTK_WIDGET_VSCROLLBAR
#define GTK_WIDGET_HSEPARATOR 		HASH___GTK_WIDGET_HSEPARATOR
#define GTK_WIDGET_VSEPARATOR 		HASH___GTK_WIDGET_VSEPARATOR
#define GTK_WIDGET_STATUSBAR 		HASH___GTK_WIDGET_STATUSBAR
#define GTK_WIDGET_FONT_SELECTION	HASH___GTK_WIDGET_FONT_SELECTION
#define GTK_WIDGET_COLOR_SELECTION	HASH___GTK_WIDGET_COLOR_SELECTION
#define GTK_WIDGET_PROGRESS		HASH___GTK_WIDGET_PROGRESS
#define GTK_WIDGET_PROGRESS_BAR		HASH___GTK_WIDGET_PROGRESS_BAR
#define GTK_WIDGET_ARROW		HASH___GTK_WIDGET_ARROW
#define GTK_WIDGET_CALENDAR		HASH___GTK_WIDGET_CALENDAR
#define GTK_WIDGET_HRULER		HASH___GTK_WIDGET_HRULER
#define GTK_WIDGET_VRULER		HASH___GTK_WIDGET_VRULER

/* extext widget */
#define GTK_WIDGET_EXTEXT		HASH___GTK_WIDGET_EXTEXT

/* Justify constants */
#define GTK_JUSTIFY_LEFT		0
#define GTK_JUSTIFY_RIGHT		1
#define GTK_JUSTIFY_CENTER		2
#define GTK_JUSTIFY_FILL		3

// Resize mode constants
#define GTK_RESIZE_PARENT		0
#define GTK_RESIZE_QUEUE		1
#define GTK_RESIZE_IMMEDIATE		2

// Corners type
#define GTK_CORNER_TOP_LEFT		0
#define GTK_CORNER_BOTTOM_LEFT		1
#define GTK_CORNER_TOP_RIGHT		2
#define GTK_CORNER_BOTTOM_RIGHT		3

// Relief styles
#define GTK_RELIEF_NORMAL		0
#define GTK_RELIEF_HALF			1
#define GTK_RELIEF_NONE			2

// Menu shadow types
#define GTK_SHADOW_NONE			0
#define GTK_SHADOW_IN			1
#define GTK_SHADOW_OUT			2
#define GTK_SHADOW_ETCHED_IN		3
#define GTK_SHADOW_ETCHED_OUT		4

// Range update types
#define GTK_UPDATE_CONTINUOUS		0
#define GTK_UPDATE_DISCONTINUOUS	1
#define GTK_UPDATE_DELAYED		2

/* GtkPositionType */
#define GTK_POS_LEFT			0
#define GTK_POS_RIGHT			1
#define GTK_POS_TOP			2
#define GTK_POS_BOTTOM			3

// Progress bar orientations
#define GTK_PROGRESS_LEFT_TO_RIGHT	0
#define GTK_PROGRESS_RIGHT_TO_LEFT	1
#define GTK_PROGRESS_BOTTOM_TO_TOP	1
#define GTK_PROGRESS_TOP_TO_BOTTOM	3

// Progress bar styles
#define GTK_PROGRESS_CONTINUOUS		0
#define GTK_PROGRESS_DISCRETE		1

// Selection modes
#define GTK_SELECTION_SINGLE		0
#define GTK_SELECTION_BROWSE		1
#define GTK_SELECTION_MULTIPLE		2
#define GTK_SELECTION_EXTENDED		3

#ifndef __GTK_CLIST_H__
// CList cell types
#define GTK_CELL_EMPTY			0
#define GTK_CELL_TEXT			1
#define GTK_CELL_PIXMAP			2
#define GTK_CELL_PIXTEXT		3
#define GTK_CELL_WIDGET			4
#endif

// Scroll types
#define GTK_SCROLL_NONE			0
#define GTK_SCROLL_STEP_BACKWARD	1
#define GTK_SCROLL_STEP_FORWARD		2
#define GTK_SCROLL_PAGE_BACKWARD	3
#define GTK_SCROLL_PAGE_FORWARD		4
#define GTK_SCROLL_JUMP			5

// Visibility types
#define GTK_VISIBILITY_NONE		0
#define GTK_VISIBILITY_PARTIAL		1
#define GTK_VISIBILITY_FULL		2

// Button actions
#define GTK_BUTTON_IGNORED		1
#define GTK_BUTTON_SELECTS		2
#define GTK_BUTTON_DRAGS		4
#define GTK_BUTTON_EXPANDS		8

// CTree expansion types
#define GTK_CTREE_EXPANSION_EXPAND		0
#define GTK_CTREE_EXPANSION_EXPAND_RECURSIVE	1
#define GTK_CTREE_EXPANSION_COLLAPSE		2
#define GTK_CTREE_EXPANSION_COLLAPSE_RECURSIVE	3
#define GTK_CTREE_EXPANSION_TOGGLE		4
#define GTK_CTREE_EXPANSION_TOGGLE_RECURSIVE	5

// GDKWMDecoration constants
#define GDK_DECOR_ALL			 1
#define GDK_DECOR_BORDER		 2
#define GDK_DECOR_RESIZEH		 4
#define GDK_DECOR_TITLE			 8
#define GDK_DECOR_MENU			16
#define GDK_DECOR_MINIMIZE		32
#define GDK_DECOR_MAXIMIZE		64

// GDKWMFunction constants
#define GDK_FUNC_ALL			 1
#define GDK_FUNC_RESIZE			 2
#define GDK_FUNC_MOVE			 4
#define GDK_FUNC_MINIMIZE		 8
#define GDK_FUNC_MAXIMIZE		16
#define GDK_FUNC_CLOSE			32

// Widget states
#define GTK_STATE_NORMAL		0 // The state during normal operation
#define GTK_STATE_ACTIVE		1 // The widget is currently active
#define GTK_STATE_PRELIGHT		2 // The mouse pointer is over the widget
#define GTK_STATE_SELECTED		3
#define GTK_STATE_INSENSITIVE		4 // The state of the widget can not be altered by the user.
					  // Its appearance will usually indicate this

// Cell border style
#define GTK_CELL_NO_BORDER		0
#define GTK_CELL_LEFT_BORDER		1
#define GTK_CELL_RIGHT_BORDER		2
#define GTK_CELL_TOP_BORDER		4
#define GTK_CELL_BOTTOM_BORDER		8

// Line styles
#define GTK_LINE_SOLID			0 // lines are drawn solid
#define GTK_LINE_ON_OFF_DASH		1 // even segments are drawn; odd segments are not drawn
#define GTK_LINE_DOUBLE_DASH		2 // even segments are normally. Odd segments are drawn
					  // in the background color if the fill style is
					  // GDK_SOLID, or in the background color masked
					  // by the stipple if the fill style is GDK_STIPPLED.

/* A set of bit flags used to specify the filter being set when calling */
/* gtk_font_selection_dialog_set_filter() or gtk_font_selection_set_filter(). */

/* the base filter, which can't be changed by the user. */
#define GTK_FONT_FILTER_BASE		0
/* the user filter, which can be changed from within
 the Filter page of the GtkFontSelection widget. */
#define GTK_FONT_FILTER_USER		1

/* A set of bit flags used to specify the type of fonts shown when calling
 *  gtk_font_selection_dialog_set_filter() or gtk_font_selection_set_filter(). */
#define GTK_FONT_BITMAP			1 /* bitmap fonts. */
#define GTK_FONT_SCALABLE               2 /* scalable fonts. */
#define GTK_FONT_SCALABLE_BITMAP        4 /* scaled bitmap fonts. */
#define GTK_FONT_ALL                    7 /* a bitwise combination of all of the above. */

#define GTK_CALENDAR_SHOW_HEADING        1 /* Specifies that the month and year should be displayed. */
#define GTK_CALENDAR_SHOW_DAY_NAMES      2 /* Specifies that three letter day descriptions should be present. */
#define GTK_CALENDAR_NO_MONTH_CHANGE     4 /* Prevents the user from switching months with the calendar. */
#define GTK_CALENDAR_SHOW_WEEK_NUMBERS   8 /* Displays each week numbers of the current year, down the left side of the calendar. */
#define GTK_CALENDAR_WEEK_START_MONDAY  16 /* Starts the calendar week on Monday, instead of the default Sunday. */

#define GTK_CURVE_TYPE_LINEAR		 0 /* linear interpolation */
#define GTK_CURVE_TYPE_SPLINE		 1 /* spline interpolation */
#define GTK_CURVE_TYPE_FREE		 2 /* free form curve */

/* used to set the type of new elements that are added to a GtkToolbar. */
#define GTK_TOOLBAR_CHILD_SPACE		 0
#define GTK_TOOLBAR_CHILD_BUTTON	 1
#define GTK_TOOLBAR_CHILD_TOGGLEBUTTON	 2
#define GTK_TOOLBAR_CHILD_RADIOBUTTON	 3
#define GTK_TOOLBAR_CHILD_WIDGET	 4

/* Orientation of GtkToolbar. */
#define GTK_ORIENTATION_HORIZONTAL	 0
#define GTK_ORIENTATION_VERTICAL	 1

/* Styles of GtkToolbar. */
#define GTK_TOOLBAR_ICONS		 0 /* Buttons should display only icons in the toolbar. */
#define GTK_TOOLBAR_TEXT		 1 /* Buttons should display only text labels in the toolbar. */
#define GTK_TOOLBAR_BOTH		 2 /* Buttons should display text and icons in the toolbar. */

/* Styles of spaces in GtkToolbar. */
#define GTK_TOOLBAR_SPACE_EMPTY		 0
#define GTK_TOOLBAR_SPACE_LINE		 1

/* Window types */
#define GTK_WINDOW_TOPLEVEL		0 /* A window for a typical application. */
#define GTK_WINDOW_DIALOG		1 /* A window for transient messages and dialogs. */
#define GTK_WINDOW_POPUP		3 /* A window for popups. */

/* Widget's flags */
#define GTK_TOPLEVEL         		16
#define GTK_NO_WINDOW        		32
#define GTK_REALIZED         		64
#define GTK_MAPPED           		128
#define GTK_VISIBLE          		256
#define GTK_SENSITIVE        		512
#define GTK_PARENT_SENSITIVE 		1024
#define GTK_CAN_FOCUS        		2048
#define GTK_HAS_FOCUS        		4096
#define GTK_CAN_DEFAULT      		8192
#define GTK_HAS_DEFAULT      		16384
#define GTK_HAS_GRAB			32768
#define GTK_RC_STYLE			65536
#define GTK_COMPOSITE_CHILD  		131072
#define GTK_NO_REPARENT      		262144
#define GTK_APP_PAINTABLE    		524288
#define GTK_RECEIVES_DEFAULT 		1048576

#ifndef __GTK_WIDGET_H__
#define GTK_WIDGET_FLAGS(wid)		  (gtk_WidgetFlags(wid))
#define GTK_WIDGET_TOPLEVEL(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_TOPLEVEL) != 0)
#define GTK_WIDGET_NO_WINDOW(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_NO_WINDOW) != 0)
#define GTK_WIDGET_REALIZED(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_REALIZED) != 0)
#define GTK_WIDGET_MAPPED(wid)		  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_MAPPED) != 0)
#define GTK_WIDGET_VISIBLE(wid)		  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_VISIBLE) != 0)
#define GTK_WIDGET_DRAWABLE(wid)	  (GTK_WIDGET_VISIBLE(wid) .and. GTK_WIDGET_MAPPED(wid))
#define GTK_WIDGET_SENSITIVE(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_SENSITIVE) != 0)
#define GTK_WIDGET_PARENT_SENSITIVE(wid)  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_PARENT_SENSITIVE) != 0)
#define GTK_WIDGET_IS_SENSITIVE(wid)	  (GTK_WIDGET_SENSITIVE(wid) .and. GTK_WIDGET_PARENT_SENSITIVE(wid))
#define GTK_WIDGET_CAN_FOCUS(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_CAN_FOCUS) != 0)
#define GTK_WIDGET_HAS_FOCUS(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_HAS_FOCUS) != 0)
#define GTK_WIDGET_CAN_DEFAULT(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_CAN_DEFAULT) != 0)
#define GTK_WIDGET_HAS_DEFAULT(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_HAS_DEFAULT) != 0)
#define GTK_WIDGET_HAS_GRAB(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_HAS_GRAB) != 0)
#define GTK_WIDGET_RC_STYLE(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_RC_STYLE) != 0)
#define GTK_WIDGET_COMPOSITE_CHILD(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_COMPOSITE_CHILD) != 0)
#define GTK_WIDGET_APP_PAINTABLE(wid)	  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_APP_PAINTABLE) != 0)
#define GTK_WIDGET_RECEIVES_DEFAULT(wid)  (NumAnd(GTK_WIDGET_FLAGS(wid), GTK_RECEIVES_DEFAULT) != 0)

#define GTK_WIDGET_SET_FLAGS(wid,flags)   (gtk_WidgetSetFlags(wid,flags))
#define GTK_WIDGET_UNSET_FLAGS(wid,flags) (gtk_WidgetUnsetFlags(wid,flags))

#define GTK_WIDGET_STATE(wid)		  (gtk_WidgetGetState (wid))
#define GTK_WIDGET_SAVED_STATE(wid)	  (gtk_WidgetGetSavedState (wid))


/* Macros for extracting a widgets private_flags from GtkWidget. */
#define PRIVATE_GTK_USER_STYLE		  1
#define PRIVATE_GTK_REDRAW_PENDING	  2
#define PRIVATE_GTK_RESIZE_PENDING	  4
#define PRIVATE_GTK_RESIZE_NEEDED	  8
#define PRIVATE_GTK_LEAVE_PENDING	 16
#define PRIVATE_GTK_HAS_SHAPE_MASK	 32
#define PRIVATE_GTK_IN_REPARENT		 64
#define PRIVATE_GTK_IS_OFFSCREEN	128
#define PRIVATE_GTK_FULLDRAW_PENDING	256

#define GTK_PRIVATE_FLAGS(wid)            (gtk_WidgetPrivateFlags(wid))
#define GTK_WIDGET_USER_STYLE(obj)	  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_USER_STYLE) != 0)
#define GTK_WIDGET_REDRAW_PENDING(obj)	  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_REDRAW_PENDING) != 0)
#define GTK_CONTAINER_RESIZE_PENDING(obj) (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_RESIZE_PENDING) != 0)
#define GTK_WIDGET_RESIZE_NEEDED(obj)	  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_RESIZE_NEEDED) != 0)
#define GTK_WIDGET_LEAVE_PENDING(obj)	  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_LEAVE_PENDING) != 0)
#define GTK_WIDGET_HAS_SHAPE_MASK(obj)	  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_HAS_SHAPE_MASK) != 0)
#define GTK_WIDGET_IN_REPARENT(obj)	  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_IN_REPARENT) != 0)
#define GTK_WIDGET_IS_OFFSCREEN(obj)	  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_IS_OFFSCREEN) != 0)
#define GTK_WIDGET_FULLDRAW_PENDING(obj)  (NumAnd(GTK_PRIVATE_FLAGS (obj), PRIVATE_GTK_FULLDRAW_PENDING) != 0)


#endif

/* GtkPackerOptions */
#define GTK_PACK_EXPAND			1 	/*< nick=expand >*/
#define GTK_FILL_X			2
#define GTK_FILL_Y			4

/* GtkSideType */
#define GTK_SIDE_TOP			0
#define GTK_SIDE_BOTTOM			1
#define GTK_SIDE_LEFT			2
#define GTK_SIDE_RIGHT			3

/* GtkAnchorType */
#define GTK_ANCHOR_CENTER		0
#define GTK_ANCHOR_NORTH		1
#define GTK_ANCHOR_NORTH_WEST		2
#define GTK_ANCHOR_NORTH_EAST		3
#define GTK_ANCHOR_SOUTH		4
#define GTK_ANCHOR_SOUTH_WEST		5
#define GTK_ANCHOR_SOUTH_EAST		6
#define GTK_ANCHOR_WEST			7
#define GTK_ANCHOR_EAST			8
#define GTK_ANCHOR_N			GTK_ANCHOR_NORTH
#define GTK_ANCHOR_NW			GTK_ANCHOR_NORTH_WEST
#define GTK_ANCHOR_NE			GTK_ANCHOR_NORTH_EAST
#define GTK_ANCHOR_S 			GTK_ANCHOR_SOUTH
#define GTK_ANCHOR_SW			GTK_ANCHOR_SOUTH_WEST
#define GTK_ANCHOR_SE			GTK_ANCHOR_SOUTH_EAST
#define GTK_ANCHOR_W 			GTK_ANCHOR_WEST
#define GTK_ANCHOR_E 			GTK_ANCHOR_EAST

/* GtkSpinButtonUpdatePolicy */
#define GTK_UPDATE_ALWAYS		0
#define GTK_UPDATE_IF_VALID		1

/* GtkSpinType */
#define GTK_SPIN_STEP_FORWARD		0
#define GTK_SPIN_STEP_BACKWARD		1
#define GTK_SPIN_PAGE_FORWARD		2
#define GTK_SPIN_PAGE_BACKWARD		3
#define GTK_SPIN_HOME			4
#define GTK_SPIN_END			5
#define GTK_SPIN_USER_DEFINED		6


#ifndef __GTK_TREE_ITEM_H__
#define GTK_TREE_ITEM_SUBTREE(wid)	gtk_TreeItemGetSubTree(wid)
#endif

#define GTK_TREE_VIEW_LINE		0 /* When an item is selected the entire GtkTreeItem is highlighted. */
#define GTK_TREE_VIEW_ITEM		1 /* When an item is selected only the selected item's child widget is highlighted. */

#ifndef __GTK_TREE_H__
#define GTK_IS_ROOT_TREE(obj)		gtk_TreeIsRootTree(obj)
#define GTK_TREE_ROOT_TREE(obj)		gtk_TreeRootTree(obj)
#define GTK_TREE_SELECTION(obj)		gtk_TreeGetSelection(obj)
#endif

/* Determines when a scroll bar will be visible.  */
#define GTK_POLICY_ALWAYS		0 /* The scrollbar is always visible. */
#define GTK_POLICY_AUTOMATIC		1 /* The scrollbar will appear and disappear */
					  /* as necessary. For example, when all of a */
					  /* GtkCList can not be seen. */
#define GTK_POLICY_NEVER		2 /* The scrollbar will never appear. */

/* An enumeration describing the way in which a device axis (valuator)
 * maps onto the predefined valuator types that GTK+ understands. */
#define GDK_AXIS_IGNORE			 0 /* the axis is ignored. */
#define GDK_AXIS_X			 1 /* the axis is used as the x axis. */
#define GDK_AXIS_Y			 2 /* the axis is used as the y axis. */
#define GDK_AXIS_PRESSURE		 3 /* the axis is used for pressure information. */
#define GDK_AXIS_XTILT			 4 /* the axis is used for x tilt information. */
#define GDK_AXIS_YTILT			 5 /* the axis is used for y tilt information. */
#define GDK_AXIS_LAST			 6 /* a constant equal to the numerically highest axis value. */


/* An enumeration used to specify which extension
 * events are desired for a particular widget. */
#define GDK_EXTENSION_EVENTS_NONE	0 /* no extension events are desired. */
#define GDK_EXTENSION_EVENTS_ALL	1 /* all extension events are desired. */
#define GDK_EXTENSION_EVENTS_CURSOR	2 /* extension events are desired only if */
					  /* a cursor will be displayed for the device. */


/* An enumeration describing the type of an input device in general terms. */
#define GDK_SOURCE_MOUSE	0 /* the device is a mouse. (This will be reported for */
				  /* the core pointer, even if it is something else, */
				  /* such as a trackball.) */
#define GDK_SOURCE_PEN		1 /* the device is a stylus of a graphics tablet or similar device. */
#define GDK_SOURCE_ERASER	2 /* the device is an eraser. Typically, this would be the other */
				  /* end of a stylus on a graphics tablet. */
#define GDK_SOURCE_CURSOR	3 /* the device is a graphics tablet "puck" or similar device. */

/* An enumeration that describes the mode of an input device. */
#define GDK_MODE_DISABLED	0 /* the device is disabled and will not report any events.   */
#define GDK_MODE_SCREEN		1 /* the device is enabled. The device's coordinate space     */
				  /* maps to the entire screen.                               */
#define GDK_MODE_WINDOW		2 /* the device is enabled. The device's coordinate space     */
				  /* is mapped to a single window. The manner in which this   */
				  /* window is chosen is undefined, but it will typically be  */
				  /* the same way in which the focus window for key events is */
				  /* determined.                                              */

/* The method for determining which pixels are included in a region, when
 * creating a GdkRegion from a polygon. The fill rule is only relevant
 * for polygons which overlap themselves. */
#define GDK_EVEN_ODD_RULE	0 /* areas which are overlapped an odd number of   */
				  /* times are included in the region, while areas */
				  /* overlapped an even number of times are not.   */
#define GDK_WINDING_RULE	1 /* overlapping areas are always included. */


/* Specifies the possible values returned by gdk_region_rect_in(). */
#define GDK_OVERLAP_RECTANGLE_IN	0 /* if the rectangle is inside the GdkRegion. */
#define GDK_OVERLAP_RECTANGLE_OUT	1 /* if the rectangle is outside the GdkRegion.*/
#define GDK_OVERLAP_RECTANGLE_PART	2 /* if the rectangle is partly inside the GdkRegion. */



/* enum GdkLineStyle */

/* Determines how lines are drawn. */
#define GDK_LINE_SOLID			0 /* lines are drawn solid. 		       */
#define GDK_LINE_ON_OFF_DASH		1 /* even segments are drawn; odd segments     */
					  /* are not drawn.                            */
#define GDK_LINE_DOUBLE_DASH		2 /* even segments are normally. Odd segments  */
					  /* are drawn in the background color if the  */
					  /* fill style is GDK_SOLID, or in the        */
					  /* background color masked by the stipple if */
					  /* the fill style is GDK_STIPPLED.           */

/* enum GdkCapStyle */

 /* Determines how the end of lines are drawn. */
 /* the same as GDK_CAP_BUTT for lines of non-zero width. for zero width lines,   */
 /* the final point on the line will not be drawn.         			  */
#define GDK_CAP_NOT_LAST		0
 /* the ends of the lines are drawn squared off and extending to the coordinates  */
 /* of the end point.                                                             */
#define GDK_CAP_BUTT			1
 /* the ends of the lines are drawn as semicircles with the diameter equal to the */
 /* line width and centered at the end point.                                     */
#define GDK_CAP_ROUND			2
 /* the ends of the lines are drawn squared off and extending half the width of   */
 /* the line beyond the end point.                                                */
#define GDK_CAP_PROJECTING		3



/* enum GdkJoinStyle */

/* Determines how the joins between segments of a polygon are drawn. */
 /* the sides of each line are extended to meet at an angle. */
#define GDK_JOIN_MITER			0
 /* the sides of the two lines are joined by a circular arc. */
#define GDK_JOIN_ROUND			1
 /* the sides of the two lines are joined by a straight line */
 /* which makes an equal angle with each line.               */
#define GDK_JOIN_BEVEL			2

/* enum GdkGCValuesMask */
/* A set of bit flags used to indicate which fields GdkGCValues structure are set. */
#define GDK_GC_FOREGROUND		     1
#define GDK_GC_BACKGROUND		     2
#define GDK_GC_FONT			     4
#define GDK_GC_FUNCTION			     8
#define GDK_GC_FILL			    16
#define GDK_GC_TILE			    32
#define GDK_GC_STIPPLE			    64
#define GDK_GC_CLIP_MASK		   128
#define GDK_GC_SUBWINDOW		   256
#define GDK_GC_TS_X_ORIGIN		   512
#define GDK_GC_TS_Y_ORIGIN		  1024
#define GDK_GC_CLIP_X_ORIGIN		  2048
#define GDK_GC_CLIP_Y_ORIGIN		  4096
#define GDK_GC_EXPOSURES		  8192
#define GDK_GC_LINE_WIDTH		 16384
#define GDK_GC_LINE_STYLE		 32768
#define GDK_GC_CAP_STYLE		 65536
#define GDK_GC_JOIN_STYLE		131072

/* enum GdkFunction */
/* Determines how the bit values for the source pixels are combined with the
bit values for destination pixels to produce the final result. The sixteen
values here correspond to the 16 different possible 2x2 truth tables. Only
a couple of these values are usually useful; for colored images, only
GDK_COPY, GDK_XOR and GDK_INVERT are generally useful. For bitmaps, GDK_AND
and GDK_OR are also useful. */
#define GDK_COPY			 0
#define GDK_INVERT			 1
#define GDK_XOR				 2
#define GDK_CLEAR			 3
#define GDK_AND				 4
#define GDK_AND_REVERSE			 5
#define GDK_AND_INVERT			 6
#define GDK_NOOP			 7
#define GDK_OR				 8
#define GDK_EQUIV			 9
#define GDK_OR_REVERSE			10
#define GDK_COPY_INVERT			11
#define GDK_OR_INVERT			12
#define GDK_NAND			13
#define GDK_SET				14

/* enum GdkFill */
/* Determines how primitives are drawn. */
 /* draw with the foreground color.		*/
#define GDK_SOLID			0
 /* draw with a tiled pixmap.			*/
#define GDK_TILED			1
 /* draw using the stipple bitmap. Pixels corresponding to bits in the stipple 	*/
 /* bitmap that are set will be drawn in the foreground color; pixels */
 /* corresponding to bits that are not set will be left untouched.*/
#define GDK_STIPPLED			2
 /* draw using the stipple bitmap. Pixels corresponding to bits in the stipple */
 /* bitmap that are set will be drawn in the foreground color; pixels */
 /* corresponding to bits that are not set will be drawn with the background color. */
#define GDK_OPAQUE_STIPPLED		3

/* enum GdkSubwindowMode */

#define GDK_CLIP_BY_CHILDREN		0
#define GDK_INCLUDE_INFERIORS		1

/* Determines how drawing onto a window will affect child windows of that window.
 GDK_CLIP_BY_CHILDREN only draw onto the window itself.
 GDK_INCLUDE_INFERIORS Draw onto the window and child windows.                     */


/* A set of bit-flags to indicate which events a window is to receive.
 * Most of these masks map onto one or more of the GdkEventType event types above.

 * GDK_POINTER_MOTION_HINT_MASK is a special mask which is used to reduce the
 * number of GDK_MOTION_NOTIFY events received. Normally a GDK_MOTION_NOTIFY
 * event is received each time the mouse moves. However, if the application
 * spends a lot of time processing the event (updating the display, for example),
 * it can easily lag behind the position of the mouse. When using the
 * GDK_POINTER_MOTION_HINT_MASK the server will only send GDK_MOTION_NOTIFY
 * events when the application asks for them, by calling gdk_window_get_pointer(). */
#define GDK_EXPOSURE_MASK             2
#define GDK_POINTER_MOTION_MASK       4
#define GDK_POINTER_MOTION_HINT_MASK  8
#define GDK_BUTTON_MOTION_MASK        16
#define GDK_BUTTON1_MOTION_MASK       32
#define GDK_BUTTON2_MOTION_MASK       64
#define GDK_BUTTON3_MOTION_MASK       128
#define GDK_BUTTON_PRESS_MASK         256
#define GDK_BUTTON_RELEASE_MASK       512
#define GDK_KEY_PRESS_MASK            1024
#define GDK_KEY_RELEASE_MASK          2048
#define GDK_ENTER_NOTIFY_MASK         4096
#define GDK_LEAVE_NOTIFY_MASK         8192
#define GDK_FOCUS_CHANGE_MASK         16384
#define GDK_STRUCTURE_MASK            32768
#define GDK_PROPERTY_CHANGE_MASK      65536
#define GDK_VISIBILITY_NOTIFY_MASK    131072
#define GDK_PROXIMITY_IN_MASK         262144
#define GDK_PROXIMITY_OUT_MASK        524288
#define GDK_SUBSTRUCTURE_MASK         1048576
#define GDK_ALL_EVENTS_MASK           1048575

#ifndef __GDK_TYPES_H__
// Cursor types
#define GDK_NUM_GLYPHS			154
#define GDK_X_CURSOR			0
#define GDK_ARROW			2
#define GDK_BASED_ARROW_DOWN		4
#define GDK_BASED_ARROW_UP		6
#define GDK_BOAT			8
#define GDK_BOGOSITY			10
#define GDK_BOTTOM_LEFT_CORNER		12
#define GDK_BOTTOM_RIGHT_CORNER		14
#define GDK_BOTTOM_SIDE			16
#define GDK_BOTTOM_TEE			18
#define GDK_BOX_SPIRAL			20
#define GDK_CENTER_PTR			22
#define GDK_CIRCLE			24
#define GDK_CLOCK			26
#define GDK_COFFEE_MUG			28
#define GDK_CROSS			30
#define GDK_CROSS_REVERSE		32
#define GDK_CROSSHAIR			34
#define GDK_DIAMOND_CROSS		36
#define GDK_DOT				38
#define GDK_DOTBOX			40
#define GDK_DOUBLE_ARROW		42
#define GDK_DRAFT_LARGE			44
#define GDK_DRAFT_SMALL			46
#define GDK_DRAPED_BOX			48
#define GDK_EXCHANGE			50
#define GDK_FLEUR			52
#define GDK_GOBBLER			54
#define GDK_GUMBY			56
#define GDK_HAND1			58
#define GDK_HAND2			60
#define GDK_HEART			62
#define GDK_ICON			64
#define GDK_IRON_CROSS			66
#define GDK_LEFT_PTR			68
#define GDK_LEFT_SIDE			70
#define GDK_LEFT_TEE			72
#define GDK_LEFTBUTTON			74
#define GDK_LL_ANGLE			76
#define GDK_LR_ANGLE			78
#define GDK_MAN				80
#define GDK_MIDDLEBUTTON		82
#define GDK_MOUSE			84
#define GDK_PENCIL     			86
#define GDK_PIRATE			88
#define GDK_PLUS			90
#define GDK_QUESTION_ARROW		92
#define GDK_RIGHT_PTR			94
#define GDK_RIGHT_SIDE			96
#define GDK_RIGHT_TEE			98
#define GDK_RIGHTBUTTON			100
#define GDK_RTL_LOGO			102
#define GDK_SAILBOAT			104
#define GDK_SB_DOWN_ARROW		106
#define GDK_SB_H_DOUBLE_ARROW		108
#define GDK_SB_LEFT_ARROW		110
#define GDK_SB_RIGHT_ARROW		112
#define GDK_SB_UP_ARROW			114
#define GDK_SB_V_DOUBLE_ARROW		116
#define GDK_SHUTTLE			118
#define GDK_SIZING			120
#define GDK_SPIDER			122
#define GDK_SPRAYCAN			124
#define GDK_STAR			126
#define GDK_TARGET			128
#define GDK_TCROSS			130
#define GDK_TOP_LEFT_ARROW		132
#define GDK_TOP_LEFT_CORNER		134
#define GDK_TOP_RIGHT_CORNER		136
#define GDK_TOP_SIDE			138
#define GDK_TOP_TEE			140
#define GDK_TREK			142
#define GDK_UL_ANGLE			144
#define GDK_UMBRELLA			146
#define GDK_UR_ANGLE			148
#define GDK_WATCH			150
#define GDK_XTERM			152
#endif

/* GtkAttachOptions */

/*
Denotes the expansion properties that a widget will have when it (or it's parent) is resized.

GTK_EXPAND the widget should expand to take up any extra space in its container that has been allocated.
GTK_SHRINK the widget should shrink as and when possible.
GTK_FILL the widget should fill the space allocated to it.
*/

#define GTK_EXPAND	1
#define GTK_SHRINK	2
#define GTK_FILL	4


/* Key codes */
#define GDK_VoidSymbol 0xFFFFFF
#define GDK_BackSpace 0xFF08
#define GDK_Tab 0xFF09
#define GDK_Linefeed 0xFF0A
#define GDK_Clear 0xFF0B
#define GDK_Return 0xFF0D
#define GDK_Pause 0xFF13
#define GDK_Scroll_Lock 0xFF14
#define GDK_Sys_Req 0xFF15
#define GDK_Escape 0xFF1B
#define GDK_Delete 0xFFFF
#define GDK_Multi_key 0xFF20
#define GDK_SingleCandidate 0xFF3C
#define GDK_MultipleCandidate 0xFF3D
#define GDK_PreviousCandidate 0xFF3E
#define GDK_Kanji 0xFF21
#define GDK_Muhenkan 0xFF22
#define GDK_Henkan_Mode 0xFF23
#define GDK_Henkan 0xFF23
#define GDK_Romaji 0xFF24
#define GDK_Hiragana 0xFF25
#define GDK_Katakana 0xFF26
#define GDK_Hiragana_Katakana 0xFF27
#define GDK_Zenkaku 0xFF28
#define GDK_Hankaku 0xFF29
#define GDK_Zenkaku_Hankaku 0xFF2A
#define GDK_Touroku 0xFF2B
#define GDK_Massyo 0xFF2C
#define GDK_Kana_Lock 0xFF2D
#define GDK_Kana_Shift 0xFF2E
#define GDK_Eisu_Shift 0xFF2F
#define GDK_Eisu_toggle 0xFF30
#define GDK_Zen_Koho 0xFF3D
#define GDK_Mae_Koho 0xFF3E
#define GDK_Home 0xFF50
#define GDK_Left 0xFF51
#define GDK_Up 0xFF52
#define GDK_Right 0xFF53
#define GDK_Down 0xFF54
#define GDK_Prior 0xFF55
#define GDK_Page_Up 0xFF55
#define GDK_Next 0xFF56
#define GDK_Page_Down 0xFF56
#define GDK_End 0xFF57
#define GDK_Begin 0xFF58
#define GDK_Select 0xFF60
#define GDK_Print 0xFF61
#define GDK_Execute 0xFF62
#define GDK_Insert 0xFF63
#define GDK_Undo 0xFF65
#define GDK_Redo 0xFF66
#define GDK_Menu 0xFF67
#define GDK_Find 0xFF68
#define GDK_Cancel 0xFF69
#define GDK_Help 0xFF6A
#define GDK_Break 0xFF6B
#define GDK_Mode_switch 0xFF7E
#define GDK_script_switch 0xFF7E
#define GDK_Num_Lock 0xFF7F
#define GDK_KP_Space 0xFF80
#define GDK_KP_Tab 0xFF89
#define GDK_KP_Enter 0xFF8D
#define GDK_KP_F1 0xFF91
#define GDK_KP_F2 0xFF92
#define GDK_KP_F3 0xFF93
#define GDK_KP_F4 0xFF94
#define GDK_KP_Home 0xFF95
#define GDK_KP_Left 0xFF96
#define GDK_KP_Up 0xFF97
#define GDK_KP_Right 0xFF98
#define GDK_KP_Down 0xFF99
#define GDK_KP_Prior 0xFF9A
#define GDK_KP_Page_Up 0xFF9A
#define GDK_KP_Next 0xFF9B
#define GDK_KP_Page_Down 0xFF9B
#define GDK_KP_End 0xFF9C
#define GDK_KP_Begin 0xFF9D
#define GDK_KP_Insert 0xFF9E
#define GDK_KP_Delete 0xFF9F
#define GDK_KP_Equal 0xFFBD
#define GDK_KP_Multiply 0xFFAA
#define GDK_KP_Add 0xFFAB
#define GDK_KP_Separator 0xFFAC
#define GDK_KP_Subtract 0xFFAD
#define GDK_KP_Decimal 0xFFAE
#define GDK_KP_Divide 0xFFAF
#define GDK_KP_0 0xFFB0
#define GDK_KP_1 0xFFB1
#define GDK_KP_2 0xFFB2
#define GDK_KP_3 0xFFB3
#define GDK_KP_4 0xFFB4
#define GDK_KP_5 0xFFB5
#define GDK_KP_6 0xFFB6
#define GDK_KP_7 0xFFB7
#define GDK_KP_8 0xFFB8
#define GDK_KP_9 0xFFB9
#define GDK_F1 0xFFBE
#define GDK_F2 0xFFBF
#define GDK_F3 0xFFC0
#define GDK_F4 0xFFC1
#define GDK_F5 0xFFC2
#define GDK_F6 0xFFC3
#define GDK_F7 0xFFC4
#define GDK_F8 0xFFC5
#define GDK_F9 0xFFC6
#define GDK_F10 0xFFC7
#define GDK_F11 0xFFC8
#define GDK_L1 0xFFC8
#define GDK_F12 0xFFC9
#define GDK_L2 0xFFC9
#define GDK_F13 0xFFCA
#define GDK_L3 0xFFCA
#define GDK_F14 0xFFCB
#define GDK_L4 0xFFCB
#define GDK_F15 0xFFCC
#define GDK_L5 0xFFCC
#define GDK_F16 0xFFCD
#define GDK_L6 0xFFCD
#define GDK_F17 0xFFCE
#define GDK_L7 0xFFCE
#define GDK_F18 0xFFCF
#define GDK_L8 0xFFCF
#define GDK_F19 0xFFD0
#define GDK_L9 0xFFD0
#define GDK_F20 0xFFD1
#define GDK_L10 0xFFD1
#define GDK_F21 0xFFD2
#define GDK_R1 0xFFD2
#define GDK_F22 0xFFD3
#define GDK_R2 0xFFD3
#define GDK_F23 0xFFD4
#define GDK_R3 0xFFD4
#define GDK_F24 0xFFD5
#define GDK_R4 0xFFD5
#define GDK_F25 0xFFD6
#define GDK_R5 0xFFD6
#define GDK_F26 0xFFD7
#define GDK_R6 0xFFD7
#define GDK_F27 0xFFD8
#define GDK_R7 0xFFD8
#define GDK_F28 0xFFD9
#define GDK_R8 0xFFD9
#define GDK_F29 0xFFDA
#define GDK_R9 0xFFDA
#define GDK_F30 0xFFDB
#define GDK_R10 0xFFDB
#define GDK_F31 0xFFDC
#define GDK_R11 0xFFDC
#define GDK_F32 0xFFDD
#define GDK_R12 0xFFDD
#define GDK_F33 0xFFDE
#define GDK_R13 0xFFDE
#define GDK_F34 0xFFDF
#define GDK_R14 0xFFDF
#define GDK_F35 0xFFE0
#define GDK_R15 0xFFE0
#define GDK_Shift_L 0xFFE1
#define GDK_Shift_R 0xFFE2
#define GDK_Control_L 0xFFE3
#define GDK_Control_R 0xFFE4
#define GDK_Caps_Lock 0xFFE5
#define GDK_Shift_Lock 0xFFE6
#define GDK_Meta_L 0xFFE7
#define GDK_Meta_R 0xFFE8
#define GDK_Alt_L 0xFFE9
#define GDK_Alt_R 0xFFEA
#define GDK_Super_L 0xFFEB
#define GDK_Super_R 0xFFEC
#define GDK_Hyper_L 0xFFED
#define GDK_Hyper_R 0xFFEE
#define GDK_ISO_Lock 0xFE01
#define GDK_ISO_Level2_Latch 0xFE02
#define GDK_ISO_Level3_Shift 0xFE03
#define GDK_ISO_Level3_Latch 0xFE04
#define GDK_ISO_Level3_Lock 0xFE05
#define GDK_ISO_Group_Shift 0xFF7E
#define GDK_ISO_Group_Latch 0xFE06
#define GDK_ISO_Group_Lock 0xFE07
#define GDK_ISO_Next_Group 0xFE08
#define GDK_ISO_Next_Group_Lock 0xFE09
#define GDK_ISO_Prev_Group 0xFE0A
#define GDK_ISO_Prev_Group_Lock 0xFE0B
#define GDK_ISO_First_Group 0xFE0C
#define GDK_ISO_First_Group_Lock 0xFE0D
#define GDK_ISO_Last_Group 0xFE0E
#define GDK_ISO_Last_Group_Lock 0xFE0F
#define GDK_ISO_Left_Tab 0xFE20
#define GDK_ISO_Move_Line_Up 0xFE21
#define GDK_ISO_Move_Line_Down 0xFE22
#define GDK_ISO_Partial_Line_Up 0xFE23
#define GDK_ISO_Partial_Line_Down 0xFE24
#define GDK_ISO_Partial_Space_Left 0xFE25
#define GDK_ISO_Partial_Space_Right 0xFE26
#define GDK_ISO_Set_Margin_Left 0xFE27
#define GDK_ISO_Set_Margin_Right 0xFE28
#define GDK_ISO_Release_Margin_Left 0xFE29
#define GDK_ISO_Release_Margin_Right 0xFE2A
#define GDK_ISO_Release_Both_Margins 0xFE2B
#define GDK_ISO_Fast_Cursor_Left 0xFE2C
#define GDK_ISO_Fast_Cursor_Right 0xFE2D
#define GDK_ISO_Fast_Cursor_Up 0xFE2E
#define GDK_ISO_Fast_Cursor_Down 0xFE2F
#define GDK_ISO_Continuous_Underline 0xFE30
#define GDK_ISO_Discontinuous_Underline 0xFE31
#define GDK_ISO_Emphasize 0xFE32
#define GDK_ISO_Center_Object 0xFE33
#define GDK_ISO_Enter 0xFE34
#define GDK_dead_grave 0xFE50
#define GDK_dead_acute 0xFE51
#define GDK_dead_circumflex 0xFE52
#define GDK_dead_tilde 0xFE53
#define GDK_dead_macron 0xFE54
#define GDK_dead_breve 0xFE55
#define GDK_dead_abovedot 0xFE56
#define GDK_dead_diaeresis 0xFE57
#define GDK_dead_abovering 0xFE58
#define GDK_dead_doubleacute 0xFE59
#define GDK_dead_caron 0xFE5A
#define GDK_dead_cedilla 0xFE5B
#define GDK_dead_ogonek 0xFE5C
#define GDK_dead_iota 0xFE5D
#define GDK_dead_voiced_sound 0xFE5E
#define GDK_dead_semivoiced_sound 0xFE5F
#define GDK_dead_belowdot 0xFE60
#define GDK_First_Virtual_Screen 0xFED0
#define GDK_Prev_Virtual_Screen 0xFED1
#define GDK_Next_Virtual_Screen 0xFED2
#define GDK_Last_Virtual_Screen 0xFED4
#define GDK_Terminate_Server 0xFED5
#define GDK_AccessX_Enable 0xFE70
#define GDK_AccessX_Feedback_Enable 0xFE71
#define GDK_RepeatKeys_Enable 0xFE72
#define GDK_SlowKeys_Enable 0xFE73
#define GDK_BounceKeys_Enable 0xFE74
#define GDK_StickyKeys_Enable 0xFE75
#define GDK_MouseKeys_Enable 0xFE76
#define GDK_MouseKeys_Accel_Enable 0xFE77
#define GDK_Overlay1_Enable 0xFE78
#define GDK_Overlay2_Enable 0xFE79
#define GDK_AudibleBell_Enable 0xFE7A
#define GDK_Pointer_Left 0xFEE0
#define GDK_Pointer_Right 0xFEE1
#define GDK_Pointer_Up 0xFEE2
#define GDK_Pointer_Down 0xFEE3
#define GDK_Pointer_UpLeft 0xFEE4
#define GDK_Pointer_UpRight 0xFEE5
#define GDK_Pointer_DownLeft 0xFEE6
#define GDK_Pointer_DownRight 0xFEE7
#define GDK_Pointer_Button_Dflt 0xFEE8
#define GDK_Pointer_Button1 0xFEE9
#define GDK_Pointer_Button2 0xFEEA
#define GDK_Pointer_Button3 0xFEEB
#define GDK_Pointer_Button4 0xFEEC
#define GDK_Pointer_Button5 0xFEED
#define GDK_Pointer_DblClick_Dflt 0xFEEE
#define GDK_Pointer_DblClick1 0xFEEF
#define GDK_Pointer_DblClick2 0xFEF0
#define GDK_Pointer_DblClick3 0xFEF1
#define GDK_Pointer_DblClick4 0xFEF2
#define GDK_Pointer_DblClick5 0xFEF3
#define GDK_Pointer_Drag_Dflt 0xFEF4
#define GDK_Pointer_Drag1 0xFEF5
#define GDK_Pointer_Drag2 0xFEF6
#define GDK_Pointer_Drag3 0xFEF7
#define GDK_Pointer_Drag4 0xFEF8
#define GDK_Pointer_Drag5 0xFEFD
#define GDK_Pointer_EnableKeys 0xFEF9
#define GDK_Pointer_Accelerate 0xFEFA
#define GDK_Pointer_DfltBtnNext 0xFEFB
#define GDK_Pointer_DfltBtnPrev 0xFEFC
#define GDK_3270_Duplicate 0xFD01
#define GDK_3270_FieldMark 0xFD02
#define GDK_3270_Right2 0xFD03
#define GDK_3270_Left2 0xFD04
#define GDK_3270_BackTab 0xFD05
#define GDK_3270_EraseEOF 0xFD06
#define GDK_3270_EraseInput 0xFD07
#define GDK_3270_Reset 0xFD08
#define GDK_3270_Quit 0xFD09
#define GDK_3270_PA1 0xFD0A
#define GDK_3270_PA2 0xFD0B
#define GDK_3270_PA3 0xFD0C
#define GDK_3270_Test 0xFD0D
#define GDK_3270_Attn 0xFD0E
#define GDK_3270_CursorBlink 0xFD0F
#define GDK_3270_AltCursor 0xFD10
#define GDK_3270_KeyClick 0xFD11
#define GDK_3270_Jump 0xFD12
#define GDK_3270_Ident 0xFD13
#define GDK_3270_Rule 0xFD14
#define GDK_3270_Copy 0xFD15
#define GDK_3270_Play 0xFD16
#define GDK_3270_Setup 0xFD17
#define GDK_3270_Record 0xFD18
#define GDK_3270_ChangeScreen 0xFD19
#define GDK_3270_DeleteWord 0xFD1A
#define GDK_3270_ExSelect 0xFD1B
#define GDK_3270_CursorSelect 0xFD1C
#define GDK_3270_PrintScreen 0xFD1D
#define GDK_3270_Enter 0xFD1E
#define GDK_space 0x020
#define GDK_exclam 0x021
#define GDK_quotedbl 0x022
#define GDK_numbersign 0x023
#define GDK_dollar 0x024
#define GDK_percent 0x025
#define GDK_ampersand 0x026
#define GDK_apostrophe 0x027
#define GDK_quoteright 0x027
#define GDK_parenleft 0x028
#define GDK_parenright 0x029
#define GDK_asterisk 0x02a
#define GDK_plus 0x02b
#define GDK_comma 0x02c
#define GDK_minus 0x02d
#define GDK_period 0x02e
#define GDK_slash 0x02f
#define GDK_0 0x030
#define GDK_1 0x031
#define GDK_2 0x032
#define GDK_3 0x033
#define GDK_4 0x034
#define GDK_5 0x035
#define GDK_6 0x036
#define GDK_7 0x037
#define GDK_8 0x038
#define GDK_9 0x039
#define GDK_colon 0x03a
#define GDK_semicolon 0x03b
#define GDK_less 0x03c
#define GDK_equal 0x03d
#define GDK_greater 0x03e
#define GDK_question 0x03f
#define GDK_at 0x040
#define GDK_A 0x041
#define GDK_B 0x042
#define GDK_C 0x043
#define GDK_D 0x044
#define GDK_E 0x045
#define GDK_F 0x046
#define GDK_G 0x047
#define GDK_H 0x048
#define GDK_I 0x049
#define GDK_J 0x04a
#define GDK_K 0x04b
#define GDK_L 0x04c
#define GDK_M 0x04d
#define GDK_N 0x04e
#define GDK_O 0x04f
#define GDK_P 0x050
#define GDK_Q 0x051
#define GDK_R 0x052
#define GDK_S 0x053
#define GDK_T 0x054
#define GDK_U 0x055
#define GDK_V 0x056
#define GDK_W 0x057
#define GDK_X 0x058
#define GDK_Y 0x059
#define GDK_Z 0x05a
#define GDK_bracketleft 0x05b
#define GDK_backslash 0x05c
#define GDK_bracketright 0x05d
#define GDK_asciicircum 0x05e
#define GDK_underscore 0x05f
#define GDK_grave 0x060
#define GDK_quoteleft 0x060
#define GDK_a 0x061
#define GDK_b 0x062
#define GDK_c 0x063
#define GDK_d 0x064
#define GDK_e 0x065
#define GDK_f 0x066
#define GDK_g 0x067
#define GDK_h 0x068
#define GDK_i 0x069
#define GDK_j 0x06a
#define GDK_k 0x06b
#define GDK_l 0x06c
#define GDK_m 0x06d
#define GDK_n 0x06e
#define GDK_o 0x06f
#define GDK_p 0x070
#define GDK_q 0x071
#define GDK_r 0x072
#define GDK_s 0x073
#define GDK_t 0x074
#define GDK_u 0x075
#define GDK_v 0x076
#define GDK_w 0x077
#define GDK_x 0x078
#define GDK_y 0x079
#define GDK_z 0x07a
#define GDK_braceleft 0x07b
#define GDK_bar 0x07c
#define GDK_braceright 0x07d
#define GDK_asciitilde 0x07e
#define GDK_nobreakspace 0x0a0
#define GDK_exclamdown 0x0a1
#define GDK_cent 0x0a2
#define GDK_sterling 0x0a3
#define GDK_currency 0x0a4
#define GDK_yen 0x0a5
#define GDK_brokenbar 0x0a6
#define GDK_section 0x0a7
#define GDK_diaeresis 0x0a8
#define GDK_copyright 0x0a9
#define GDK_ordfeminine 0x0aa
#define GDK_guillemotleft 0x0ab
#define GDK_notsign 0x0ac
#define GDK_hyphen 0x0ad
#define GDK_registered 0x0ae
#define GDK_macron 0x0af
#define GDK_degree 0x0b0
#define GDK_plusminus 0x0b1
#define GDK_twosuperior 0x0b2
#define GDK_threesuperior 0x0b3
#define GDK_acute 0x0b4
#define GDK_mu 0x0b5
#define GDK_paragraph 0x0b6
#define GDK_periodcentered 0x0b7
#define GDK_cedilla 0x0b8
#define GDK_onesuperior 0x0b9
#define GDK_masculine 0x0ba
#define GDK_guillemotright 0x0bb
#define GDK_onequarter 0x0bc
#define GDK_onehalf 0x0bd
#define GDK_threequarters 0x0be
#define GDK_questiondown 0x0bf
#define GDK_Agrave 0x0c0
#define GDK_Aacute 0x0c1
#define GDK_Acircumflex 0x0c2
#define GDK_Atilde 0x0c3
#define GDK_Adiaeresis 0x0c4
#define GDK_Aring 0x0c5
#define GDK_AE 0x0c6
#define GDK_Ccedilla 0x0c7
#define GDK_Egrave 0x0c8
#define GDK_Eacute 0x0c9
#define GDK_Ecircumflex 0x0ca
#define GDK_Ediaeresis 0x0cb
#define GDK_Igrave 0x0cc
#define GDK_Iacute 0x0cd
#define GDK_Icircumflex 0x0ce
#define GDK_Idiaeresis 0x0cf
#define GDK_ETH 0x0d0
#define GDK_Eth 0x0d0
#define GDK_Ntilde 0x0d1
#define GDK_Ograve 0x0d2
#define GDK_Oacute 0x0d3
#define GDK_Ocircumflex 0x0d4
#define GDK_Otilde 0x0d5
#define GDK_Odiaeresis 0x0d6
#define GDK_multiply 0x0d7
#define GDK_Ooblique 0x0d8
#define GDK_Ugrave 0x0d9
#define GDK_Uacute 0x0da
#define GDK_Ucircumflex 0x0db
#define GDK_Udiaeresis 0x0dc
#define GDK_Yacute 0x0dd
#define GDK_THORN 0x0de
#define GDK_Thorn 0x0de
#define GDK_ssharp 0x0df
#define GDK_agrave 0x0e0
#define GDK_aacute 0x0e1
#define GDK_acircumflex 0x0e2
#define GDK_atilde 0x0e3
#define GDK_adiaeresis 0x0e4
#define GDK_aring 0x0e5
#define GDK_ae 0x0e6
#define GDK_ccedilla 0x0e7
#define GDK_egrave 0x0e8
#define GDK_eacute 0x0e9
#define GDK_ecircumflex 0x0ea
#define GDK_ediaeresis 0x0eb
#define GDK_igrave 0x0ec
#define GDK_iacute 0x0ed
#define GDK_icircumflex 0x0ee
#define GDK_idiaeresis 0x0ef
#define GDK_eth 0x0f0
#define GDK_ntilde 0x0f1
#define GDK_ograve 0x0f2
#define GDK_oacute 0x0f3
#define GDK_ocircumflex 0x0f4
#define GDK_otilde 0x0f5
#define GDK_odiaeresis 0x0f6
#define GDK_division 0x0f7
#define GDK_oslash 0x0f8
#define GDK_ugrave 0x0f9
#define GDK_uacute 0x0fa
#define GDK_ucircumflex 0x0fb
#define GDK_udiaeresis 0x0fc
#define GDK_yacute 0x0fd
#define GDK_thorn 0x0fe
#define GDK_ydiaeresis 0x0ff
#define GDK_Aogonek 0x1a1
#define GDK_breve 0x1a2
#define GDK_Lstroke 0x1a3
#define GDK_Lcaron 0x1a5
#define GDK_Sacute 0x1a6
#define GDK_Scaron 0x1a9
#define GDK_Scedilla 0x1aa
#define GDK_Tcaron 0x1ab
#define GDK_Zacute 0x1ac
#define GDK_Zcaron 0x1ae
#define GDK_Zabovedot 0x1af
#define GDK_aogonek 0x1b1
#define GDK_ogonek 0x1b2
#define GDK_lstroke 0x1b3
#define GDK_lcaron 0x1b5
#define GDK_sacute 0x1b6
#define GDK_caron 0x1b7
#define GDK_scaron 0x1b9
#define GDK_scedilla 0x1ba
#define GDK_tcaron 0x1bb
#define GDK_zacute 0x1bc
#define GDK_doubleacute 0x1bd
#define GDK_zcaron 0x1be
#define GDK_zabovedot 0x1bf
#define GDK_Racute 0x1c0
#define GDK_Abreve 0x1c3
#define GDK_Lacute 0x1c5
#define GDK_Cacute 0x1c6
#define GDK_Ccaron 0x1c8
#define GDK_Eogonek 0x1ca
#define GDK_Ecaron 0x1cc
#define GDK_Dcaron 0x1cf
#define GDK_Dstroke 0x1d0
#define GDK_Nacute 0x1d1
#define GDK_Ncaron 0x1d2
#define GDK_Odoubleacute 0x1d5
#define GDK_Rcaron 0x1d8
#define GDK_Uring 0x1d9
#define GDK_Udoubleacute 0x1db
#define GDK_Tcedilla 0x1de
#define GDK_racute 0x1e0
#define GDK_abreve 0x1e3
#define GDK_lacute 0x1e5
#define GDK_cacute 0x1e6
#define GDK_ccaron 0x1e8
#define GDK_eogonek 0x1ea
#define GDK_ecaron 0x1ec
#define GDK_dcaron 0x1ef
#define GDK_dstroke 0x1f0
#define GDK_nacute 0x1f1
#define GDK_ncaron 0x1f2
#define GDK_odoubleacute 0x1f5
#define GDK_udoubleacute 0x1fb
#define GDK_rcaron 0x1f8
#define GDK_uring 0x1f9
#define GDK_tcedilla 0x1fe
#define GDK_abovedot 0x1ff
#define GDK_Hstroke 0x2a1
#define GDK_Hcircumflex 0x2a6
#define GDK_Iabovedot 0x2a9
#define GDK_Gbreve 0x2ab
#define GDK_Jcircumflex 0x2ac
#define GDK_hstroke 0x2b1
#define GDK_hcircumflex 0x2b6
#define GDK_idotless 0x2b9
#define GDK_gbreve 0x2bb
#define GDK_jcircumflex 0x2bc
#define GDK_Cabovedot 0x2c5
#define GDK_Ccircumflex 0x2c6
#define GDK_Gabovedot 0x2d5
#define GDK_Gcircumflex 0x2d8
#define GDK_Ubreve 0x2dd
#define GDK_Scircumflex 0x2de
#define GDK_cabovedot 0x2e5
#define GDK_ccircumflex 0x2e6
#define GDK_gabovedot 0x2f5
#define GDK_gcircumflex 0x2f8
#define GDK_ubreve 0x2fd
#define GDK_scircumflex 0x2fe
#define GDK_kra 0x3a2
#define GDK_kappa 0x3a2
#define GDK_Rcedilla 0x3a3
#define GDK_Itilde 0x3a5
#define GDK_Lcedilla 0x3a6
#define GDK_Emacron 0x3aa
#define GDK_Gcedilla 0x3ab
#define GDK_Tslash 0x3ac
#define GDK_rcedilla 0x3b3
#define GDK_itilde 0x3b5
#define GDK_lcedilla 0x3b6
#define GDK_emacron 0x3ba
#define GDK_gcedilla 0x3bb
#define GDK_tslash 0x3bc
#define GDK_ENG 0x3bd
#define GDK_eng 0x3bf
#define GDK_Amacron 0x3c0
#define GDK_Iogonek 0x3c7
#define GDK_Eabovedot 0x3cc
#define GDK_Imacron 0x3cf
#define GDK_Ncedilla 0x3d1
#define GDK_Omacron 0x3d2
#define GDK_Kcedilla 0x3d3
#define GDK_Uogonek 0x3d9
#define GDK_Utilde 0x3dd
#define GDK_Umacron 0x3de
#define GDK_amacron 0x3e0
#define GDK_iogonek 0x3e7
#define GDK_eabovedot 0x3ec
#define GDK_imacron 0x3ef
#define GDK_ncedilla 0x3f1
#define GDK_omacron 0x3f2
#define GDK_kcedilla 0x3f3
#define GDK_uogonek 0x3f9
#define GDK_utilde 0x3fd
#define GDK_umacron 0x3fe
#define GDK_overline 0x47e
#define GDK_kana_fullstop 0x4a1
#define GDK_kana_openingbracket 0x4a2
#define GDK_kana_closingbracket 0x4a3
#define GDK_kana_comma 0x4a4
#define GDK_kana_conjunctive 0x4a5
#define GDK_kana_middledot 0x4a5
#define GDK_kana_WO 0x4a6
#define GDK_kana_a 0x4a7
#define GDK_kana_i 0x4a8
#define GDK_kana_u 0x4a9
#define GDK_kana_e 0x4aa
#define GDK_kana_o 0x4ab
#define GDK_kana_ya 0x4ac
#define GDK_kana_yu 0x4ad
#define GDK_kana_yo 0x4ae
#define GDK_kana_tsu 0x4af
#define GDK_kana_tu 0x4af
#define GDK_prolongedsound 0x4b0
#define GDK_kana_A 0x4b1
#define GDK_kana_I 0x4b2
#define GDK_kana_U 0x4b3
#define GDK_kana_E 0x4b4
#define GDK_kana_O 0x4b5
#define GDK_kana_KA 0x4b6
#define GDK_kana_KI 0x4b7
#define GDK_kana_KU 0x4b8
#define GDK_kana_KE 0x4b9
#define GDK_kana_KO 0x4ba
#define GDK_kana_SA 0x4bb
#define GDK_kana_SHI 0x4bc
#define GDK_kana_SU 0x4bd
#define GDK_kana_SE 0x4be
#define GDK_kana_SO 0x4bf
#define GDK_kana_TA 0x4c0
#define GDK_kana_CHI 0x4c1
#define GDK_kana_TI 0x4c1
#define GDK_kana_TSU 0x4c2
#define GDK_kana_TU 0x4c2
#define GDK_kana_TE 0x4c3
#define GDK_kana_TO 0x4c4
#define GDK_kana_NA 0x4c5
#define GDK_kana_NI 0x4c6
#define GDK_kana_NU 0x4c7
#define GDK_kana_NE 0x4c8
#define GDK_kana_NO 0x4c9
#define GDK_kana_HA 0x4ca
#define GDK_kana_HI 0x4cb
#define GDK_kana_FU 0x4cc
#define GDK_kana_HU 0x4cc
#define GDK_kana_HE 0x4cd
#define GDK_kana_HO 0x4ce
#define GDK_kana_MA 0x4cf
#define GDK_kana_MI 0x4d0
#define GDK_kana_MU 0x4d1
#define GDK_kana_ME 0x4d2
#define GDK_kana_MO 0x4d3
#define GDK_kana_YA 0x4d4
#define GDK_kana_YU 0x4d5
#define GDK_kana_YO 0x4d6
#define GDK_kana_RA 0x4d7
#define GDK_kana_RI 0x4d8
#define GDK_kana_RU 0x4d9
#define GDK_kana_RE 0x4da
#define GDK_kana_RO 0x4db
#define GDK_kana_WA 0x4dc
#define GDK_kana_N 0x4dd
#define GDK_voicedsound 0x4de
#define GDK_semivoicedsound 0x4df
#define GDK_kana_switch 0xFF7E
#define GDK_Arabic_comma 0x5ac
#define GDK_Arabic_semicolon 0x5bb
#define GDK_Arabic_question_mark 0x5bf
#define GDK_Arabic_hamza 0x5c1
#define GDK_Arabic_maddaonalef 0x5c2
#define GDK_Arabic_hamzaonalef 0x5c3
#define GDK_Arabic_hamzaonwaw 0x5c4
#define GDK_Arabic_hamzaunderalef 0x5c5
#define GDK_Arabic_hamzaonyeh 0x5c6
#define GDK_Arabic_alef 0x5c7
#define GDK_Arabic_beh 0x5c8
#define GDK_Arabic_tehmarbuta 0x5c9
#define GDK_Arabic_teh 0x5ca
#define GDK_Arabic_theh 0x5cb
#define GDK_Arabic_jeem 0x5cc
#define GDK_Arabic_hah 0x5cd
#define GDK_Arabic_khah 0x5ce
#define GDK_Arabic_dal 0x5cf
#define GDK_Arabic_thal 0x5d0
#define GDK_Arabic_ra 0x5d1
#define GDK_Arabic_zain 0x5d2
#define GDK_Arabic_seen 0x5d3
#define GDK_Arabic_sheen 0x5d4
#define GDK_Arabic_sad 0x5d5
#define GDK_Arabic_dad 0x5d6
#define GDK_Arabic_tah 0x5d7
#define GDK_Arabic_zah 0x5d8
#define GDK_Arabic_ain 0x5d9
#define GDK_Arabic_ghain 0x5da
#define GDK_Arabic_tatweel 0x5e0
#define GDK_Arabic_feh 0x5e1
#define GDK_Arabic_qaf 0x5e2
#define GDK_Arabic_kaf 0x5e3
#define GDK_Arabic_lam 0x5e4
#define GDK_Arabic_meem 0x5e5
#define GDK_Arabic_noon 0x5e6
#define GDK_Arabic_ha 0x5e7
#define GDK_Arabic_heh 0x5e7
#define GDK_Arabic_waw 0x5e8
#define GDK_Arabic_alefmaksura 0x5e9
#define GDK_Arabic_yeh 0x5ea
#define GDK_Arabic_fathatan 0x5eb
#define GDK_Arabic_dammatan 0x5ec
#define GDK_Arabic_kasratan 0x5ed
#define GDK_Arabic_fatha 0x5ee
#define GDK_Arabic_damma 0x5ef
#define GDK_Arabic_kasra 0x5f0
#define GDK_Arabic_shadda 0x5f1
#define GDK_Arabic_sukun 0x5f2
#define GDK_Arabic_switch 0xFF7E
#define GDK_Serbian_dje 0x6a1
#define GDK_Macedonia_gje 0x6a2
#define GDK_Cyrillic_io 0x6a3
#define GDK_Ukrainian_ie 0x6a4
#define GDK_Ukranian_je 0x6a4
#define GDK_Macedonia_dse 0x6a5
#define GDK_Ukrainian_i 0x6a6
#define GDK_Ukranian_i 0x6a6
#define GDK_Ukrainian_yi 0x6a7
#define GDK_Ukranian_yi 0x6a7
#define GDK_Cyrillic_je 0x6a8
#define GDK_Serbian_je 0x6a8
#define GDK_Cyrillic_lje 0x6a9
#define GDK_Serbian_lje 0x6a9
#define GDK_Cyrillic_nje 0x6aa
#define GDK_Serbian_nje 0x6aa
#define GDK_Serbian_tshe 0x6ab
#define GDK_Macedonia_kje 0x6ac
#define GDK_Byelorussian_shortu 0x6ae
#define GDK_Cyrillic_dzhe 0x6af
#define GDK_Serbian_dze 0x6af
#define GDK_numerosign 0x6b0
#define GDK_Serbian_DJE 0x6b1
#define GDK_Macedonia_GJE 0x6b2
#define GDK_Cyrillic_IO 0x6b3
#define GDK_Ukrainian_IE 0x6b4
#define GDK_Ukranian_JE 0x6b4
#define GDK_Macedonia_DSE 0x6b5
#define GDK_Ukrainian_I 0x6b6
#define GDK_Ukranian_I 0x6b6
#define GDK_Ukrainian_YI 0x6b7
#define GDK_Ukranian_YI 0x6b7
#define GDK_Cyrillic_JE 0x6b8
#define GDK_Serbian_JE 0x6b8
#define GDK_Cyrillic_LJE 0x6b9
#define GDK_Serbian_LJE 0x6b9
#define GDK_Cyrillic_NJE 0x6ba
#define GDK_Serbian_NJE 0x6ba
#define GDK_Serbian_TSHE 0x6bb
#define GDK_Macedonia_KJE 0x6bc
#define GDK_Byelorussian_SHORTU 0x6be
#define GDK_Cyrillic_DZHE 0x6bf
#define GDK_Serbian_DZE 0x6bf
#define GDK_Cyrillic_yu 0x6c0
#define GDK_Cyrillic_a 0x6c1
#define GDK_Cyrillic_be 0x6c2
#define GDK_Cyrillic_tse 0x6c3
#define GDK_Cyrillic_de 0x6c4
#define GDK_Cyrillic_ie 0x6c5
#define GDK_Cyrillic_ef 0x6c6
#define GDK_Cyrillic_ghe 0x6c7
#define GDK_Cyrillic_ha 0x6c8
#define GDK_Cyrillic_i 0x6c9
#define GDK_Cyrillic_shorti 0x6ca
#define GDK_Cyrillic_ka 0x6cb
#define GDK_Cyrillic_el 0x6cc
#define GDK_Cyrillic_em 0x6cd
#define GDK_Cyrillic_en 0x6ce
#define GDK_Cyrillic_o 0x6cf
#define GDK_Cyrillic_pe 0x6d0
#define GDK_Cyrillic_ya 0x6d1
#define GDK_Cyrillic_er 0x6d2
#define GDK_Cyrillic_es 0x6d3
#define GDK_Cyrillic_te 0x6d4
#define GDK_Cyrillic_u 0x6d5
#define GDK_Cyrillic_zhe 0x6d6
#define GDK_Cyrillic_ve 0x6d7
#define GDK_Cyrillic_softsign 0x6d8
#define GDK_Cyrillic_yeru 0x6d9
#define GDK_Cyrillic_ze 0x6da
#define GDK_Cyrillic_sha 0x6db
#define GDK_Cyrillic_e 0x6dc
#define GDK_Cyrillic_shcha 0x6dd
#define GDK_Cyrillic_che 0x6de
#define GDK_Cyrillic_hardsign 0x6df
#define GDK_Cyrillic_YU 0x6e0
#define GDK_Cyrillic_A 0x6e1
#define GDK_Cyrillic_BE 0x6e2
#define GDK_Cyrillic_TSE 0x6e3
#define GDK_Cyrillic_DE 0x6e4
#define GDK_Cyrillic_IE 0x6e5
#define GDK_Cyrillic_EF 0x6e6
#define GDK_Cyrillic_GHE 0x6e7
#define GDK_Cyrillic_HA 0x6e8
#define GDK_Cyrillic_I 0x6e9
#define GDK_Cyrillic_SHORTI 0x6ea
#define GDK_Cyrillic_KA 0x6eb
#define GDK_Cyrillic_EL 0x6ec
#define GDK_Cyrillic_EM 0x6ed
#define GDK_Cyrillic_EN 0x6ee
#define GDK_Cyrillic_O 0x6ef
#define GDK_Cyrillic_PE 0x6f0
#define GDK_Cyrillic_YA 0x6f1
#define GDK_Cyrillic_ER 0x6f2
#define GDK_Cyrillic_ES 0x6f3
#define GDK_Cyrillic_TE 0x6f4
#define GDK_Cyrillic_U 0x6f5
#define GDK_Cyrillic_ZHE 0x6f6
#define GDK_Cyrillic_VE 0x6f7
#define GDK_Cyrillic_SOFTSIGN 0x6f8
#define GDK_Cyrillic_YERU 0x6f9
#define GDK_Cyrillic_ZE 0x6fa
#define GDK_Cyrillic_SHA 0x6fb
#define GDK_Cyrillic_E 0x6fc
#define GDK_Cyrillic_SHCHA 0x6fd
#define GDK_Cyrillic_CHE 0x6fe
#define GDK_Cyrillic_HARDSIGN 0x6ff
#define GDK_Greek_ALPHAaccent 0x7a1
#define GDK_Greek_EPSILONaccent 0x7a2
#define GDK_Greek_ETAaccent 0x7a3
#define GDK_Greek_IOTAaccent 0x7a4
#define GDK_Greek_IOTAdiaeresis 0x7a5
#define GDK_Greek_OMICRONaccent 0x7a7
#define GDK_Greek_UPSILONaccent 0x7a8
#define GDK_Greek_UPSILONdieresis 0x7a9
#define GDK_Greek_OMEGAaccent 0x7ab
#define GDK_Greek_accentdieresis 0x7ae
#define GDK_Greek_horizbar 0x7af
#define GDK_Greek_alphaaccent 0x7b1
#define GDK_Greek_epsilonaccent 0x7b2
#define GDK_Greek_etaaccent 0x7b3
#define GDK_Greek_iotaaccent 0x7b4
#define GDK_Greek_iotadieresis 0x7b5
#define GDK_Greek_iotaaccentdieresis 0x7b6
#define GDK_Greek_omicronaccent 0x7b7
#define GDK_Greek_upsilonaccent 0x7b8
#define GDK_Greek_upsilondieresis 0x7b9
#define GDK_Greek_upsilonaccentdieresis 0x7ba
#define GDK_Greek_omegaaccent 0x7bb
#define GDK_Greek_ALPHA 0x7c1
#define GDK_Greek_BETA 0x7c2
#define GDK_Greek_GAMMA 0x7c3
#define GDK_Greek_DELTA 0x7c4
#define GDK_Greek_EPSILON 0x7c5
#define GDK_Greek_ZETA 0x7c6
#define GDK_Greek_ETA 0x7c7
#define GDK_Greek_THETA 0x7c8
#define GDK_Greek_IOTA 0x7c9
#define GDK_Greek_KAPPA 0x7ca
#define GDK_Greek_LAMDA 0x7cb
#define GDK_Greek_LAMBDA 0x7cb
#define GDK_Greek_MU 0x7cc
#define GDK_Greek_NU 0x7cd
#define GDK_Greek_XI 0x7ce
#define GDK_Greek_OMICRON 0x7cf
#define GDK_Greek_PI 0x7d0
#define GDK_Greek_RHO 0x7d1
#define GDK_Greek_SIGMA 0x7d2
#define GDK_Greek_TAU 0x7d4
#define GDK_Greek_UPSILON 0x7d5
#define GDK_Greek_PHI 0x7d6
#define GDK_Greek_CHI 0x7d7
#define GDK_Greek_PSI 0x7d8
#define GDK_Greek_OMEGA 0x7d9
#define GDK_Greek_alpha 0x7e1
#define GDK_Greek_beta 0x7e2
#define GDK_Greek_gamma 0x7e3
#define GDK_Greek_delta 0x7e4
#define GDK_Greek_epsilon 0x7e5
#define GDK_Greek_zeta 0x7e6
#define GDK_Greek_eta 0x7e7
#define GDK_Greek_theta 0x7e8
#define GDK_Greek_iota 0x7e9
#define GDK_Greek_kappa 0x7ea
#define GDK_Greek_lamda 0x7eb
#define GDK_Greek_lambda 0x7eb
#define GDK_Greek_mu 0x7ec
#define GDK_Greek_nu 0x7ed
#define GDK_Greek_xi 0x7ee
#define GDK_Greek_omicron 0x7ef
#define GDK_Greek_pi 0x7f0
#define GDK_Greek_rho 0x7f1
#define GDK_Greek_sigma 0x7f2
#define GDK_Greek_finalsmallsigma 0x7f3
#define GDK_Greek_tau 0x7f4
#define GDK_Greek_upsilon 0x7f5
#define GDK_Greek_phi 0x7f6
#define GDK_Greek_chi 0x7f7
#define GDK_Greek_psi 0x7f8
#define GDK_Greek_omega 0x7f9
#define GDK_Greek_switch 0xFF7E
#define GDK_leftradical 0x8a1
#define GDK_topleftradical 0x8a2
#define GDK_horizconnector 0x8a3
#define GDK_topintegral 0x8a4
#define GDK_botintegral 0x8a5
#define GDK_vertconnector 0x8a6
#define GDK_topleftsqbracket 0x8a7
#define GDK_botleftsqbracket 0x8a8
#define GDK_toprightsqbracket 0x8a9
#define GDK_botrightsqbracket 0x8aa
#define GDK_topleftparens 0x8ab
#define GDK_botleftparens 0x8ac
#define GDK_toprightparens 0x8ad
#define GDK_botrightparens 0x8ae
#define GDK_leftmiddlecurlybrace 0x8af
#define GDK_rightmiddlecurlybrace 0x8b0
#define GDK_topleftsummation 0x8b1
#define GDK_botleftsummation 0x8b2
#define GDK_topvertsummationconnector 0x8b3
#define GDK_botvertsummationconnector 0x8b4
#define GDK_toprightsummation 0x8b5
#define GDK_botrightsummation 0x8b6
#define GDK_rightmiddlesummation 0x8b7
#define GDK_lessthanequal 0x8bc
#define GDK_notequal 0x8bd
#define GDK_greaterthanequal 0x8be
#define GDK_integral 0x8bf
#define GDK_therefore 0x8c0
#define GDK_variation 0x8c1
#define GDK_infinity 0x8c2
#define GDK_nabla 0x8c5
#define GDK_approximate 0x8c8
#define GDK_similarequal 0x8c9
#define GDK_ifonlyif 0x8cd
#define GDK_implies 0x8ce
#define GDK_identical 0x8cf
#define GDK_radical 0x8d6
#define GDK_includedin 0x8da
#define GDK_includes 0x8db
#define GDK_intersection 0x8dc
#define GDK_union 0x8dd
#define GDK_logicaland 0x8de
#define GDK_logicalor 0x8df
#define GDK_partialderivative 0x8ef
#define GDK_function 0x8f6
#define GDK_leftarrow 0x8fb
#define GDK_uparrow 0x8fc
#define GDK_rightarrow 0x8fd
#define GDK_downarrow 0x8fe
#define GDK_blank 0x9df
#define GDK_soliddiamond 0x9e0
#define GDK_checkerboard 0x9e1
#define GDK_ht 0x9e2
#define GDK_ff 0x9e3
#define GDK_cr 0x9e4
#define GDK_lf 0x9e5
#define GDK_nl 0x9e8
#define GDK_vt 0x9e9
#define GDK_lowrightcorner 0x9ea
#define GDK_uprightcorner 0x9eb
#define GDK_upleftcorner 0x9ec
#define GDK_lowleftcorner 0x9ed
#define GDK_crossinglines 0x9ee
#define GDK_horizlinescan1 0x9ef
#define GDK_horizlinescan3 0x9f0
#define GDK_horizlinescan5 0x9f1
#define GDK_horizlinescan7 0x9f2
#define GDK_horizlinescan9 0x9f3
#define GDK_leftt 0x9f4
#define GDK_rightt 0x9f5
#define GDK_bott 0x9f6
#define GDK_topt 0x9f7
#define GDK_vertbar 0x9f8
#define GDK_emspace 0xaa1
#define GDK_enspace 0xaa2
#define GDK_em3space 0xaa3
#define GDK_em4space 0xaa4
#define GDK_digitspace 0xaa5
#define GDK_punctspace 0xaa6
#define GDK_thinspace 0xaa7
#define GDK_hairspace 0xaa8
#define GDK_emdash 0xaa9
#define GDK_endash 0xaaa
#define GDK_signifblank 0xaac
#define GDK_ellipsis 0xaae
#define GDK_doubbaselinedot 0xaaf
#define GDK_onethird 0xab0
#define GDK_twothirds 0xab1
#define GDK_onefifth 0xab2
#define GDK_twofifths 0xab3
#define GDK_threefifths 0xab4
#define GDK_fourfifths 0xab5
#define GDK_onesixth 0xab6
#define GDK_fivesixths 0xab7
#define GDK_careof 0xab8
#define GDK_figdash 0xabb
#define GDK_leftanglebracket 0xabc
#define GDK_decimalpoint 0xabd
#define GDK_rightanglebracket 0xabe
#define GDK_marker 0xabf
#define GDK_oneeighth 0xac3
#define GDK_threeeighths 0xac4
#define GDK_fiveeighths 0xac5
#define GDK_seveneighths 0xac6
#define GDK_trademark 0xac9
#define GDK_signaturemark 0xaca
#define GDK_trademarkincircle 0xacb
#define GDK_leftopentriangle 0xacc
#define GDK_rightopentriangle 0xacd
#define GDK_emopencircle 0xace
#define GDK_emopenrectangle 0xacf
#define GDK_leftsinglequotemark 0xad0
#define GDK_rightsinglequotemark 0xad1
#define GDK_leftdoublequotemark 0xad2
#define GDK_rightdoublequotemark 0xad3
#define GDK_prescription 0xad4
#define GDK_minutes 0xad6
#define GDK_seconds 0xad7
#define GDK_latincross 0xad9
#define GDK_hexagram 0xada
#define GDK_filledrectbullet 0xadb
#define GDK_filledlefttribullet 0xadc
#define GDK_filledrighttribullet 0xadd
#define GDK_emfilledcircle 0xade
#define GDK_emfilledrect 0xadf
#define GDK_enopencircbullet 0xae0
#define GDK_enopensquarebullet 0xae1
#define GDK_openrectbullet 0xae2
#define GDK_opentribulletup 0xae3
#define GDK_opentribulletdown 0xae4
#define GDK_openstar 0xae5
#define GDK_enfilledcircbullet 0xae6
#define GDK_enfilledsqbullet 0xae7
#define GDK_filledtribulletup 0xae8
#define GDK_filledtribulletdown 0xae9
#define GDK_leftpointer 0xaea
#define GDK_rightpointer 0xaeb
#define GDK_club 0xaec
#define GDK_diamond 0xaed
#define GDK_heart 0xaee
#define GDK_maltesecross 0xaf0
#define GDK_dagger 0xaf1
#define GDK_doubledagger 0xaf2
#define GDK_checkmark 0xaf3
#define GDK_ballotcross 0xaf4
#define GDK_musicalsharp 0xaf5
#define GDK_musicalflat 0xaf6
#define GDK_malesymbol 0xaf7
#define GDK_femalesymbol 0xaf8
#define GDK_telephone 0xaf9
#define GDK_telephonerecorder 0xafa
#define GDK_phonographcopyright 0xafb
#define GDK_caret 0xafc
#define GDK_singlelowquotemark 0xafd
#define GDK_doublelowquotemark 0xafe
#define GDK_cursor 0xaff
#define GDK_leftcaret 0xba3
#define GDK_rightcaret 0xba6
#define GDK_downcaret 0xba8
#define GDK_upcaret 0xba9
#define GDK_overbar 0xbc0
#define GDK_downtack 0xbc2
#define GDK_upshoe 0xbc3
#define GDK_downstile 0xbc4
#define GDK_underbar 0xbc6
#define GDK_jot 0xbca
#define GDK_quad 0xbcc
#define GDK_uptack 0xbce
#define GDK_circle 0xbcf
#define GDK_upstile 0xbd3
#define GDK_downshoe 0xbd6
#define GDK_rightshoe 0xbd8
#define GDK_leftshoe 0xbda
#define GDK_lefttack 0xbdc
#define GDK_righttack 0xbfc
#define GDK_hebrew_doublelowline 0xcdf
#define GDK_hebrew_aleph 0xce0
#define GDK_hebrew_bet 0xce1
#define GDK_hebrew_beth 0xce1
#define GDK_hebrew_gimel 0xce2
#define GDK_hebrew_gimmel 0xce2
#define GDK_hebrew_dalet 0xce3
#define GDK_hebrew_daleth 0xce3
#define GDK_hebrew_he 0xce4
#define GDK_hebrew_waw 0xce5
#define GDK_hebrew_zain 0xce6
#define GDK_hebrew_zayin 0xce6
#define GDK_hebrew_chet 0xce7
#define GDK_hebrew_het 0xce7
#define GDK_hebrew_tet 0xce8
#define GDK_hebrew_teth 0xce8
#define GDK_hebrew_yod 0xce9
#define GDK_hebrew_finalkaph 0xcea
#define GDK_hebrew_kaph 0xceb
#define GDK_hebrew_lamed 0xcec
#define GDK_hebrew_finalmem 0xced
#define GDK_hebrew_mem 0xcee
#define GDK_hebrew_finalnun 0xcef
#define GDK_hebrew_nun 0xcf0
#define GDK_hebrew_samech 0xcf1
#define GDK_hebrew_samekh 0xcf1
#define GDK_hebrew_ayin 0xcf2
#define GDK_hebrew_finalpe 0xcf3
#define GDK_hebrew_pe 0xcf4
#define GDK_hebrew_finalzade 0xcf5
#define GDK_hebrew_finalzadi 0xcf5
#define GDK_hebrew_zade 0xcf6
#define GDK_hebrew_zadi 0xcf6
#define GDK_hebrew_qoph 0xcf7
#define GDK_hebrew_kuf 0xcf7
#define GDK_hebrew_resh 0xcf8
#define GDK_hebrew_shin 0xcf9
#define GDK_hebrew_taw 0xcfa
#define GDK_hebrew_taf 0xcfa
#define GDK_Hebrew_switch 0xFF7E
#define GDK_Thai_kokai 0xda1
#define GDK_Thai_khokhai 0xda2
#define GDK_Thai_khokhuat 0xda3
#define GDK_Thai_khokhwai 0xda4
#define GDK_Thai_khokhon 0xda5
#define GDK_Thai_khorakhang 0xda6
#define GDK_Thai_ngongu 0xda7
#define GDK_Thai_chochan 0xda8
#define GDK_Thai_choching 0xda9
#define GDK_Thai_chochang 0xdaa
#define GDK_Thai_soso 0xdab
#define GDK_Thai_chochoe 0xdac
#define GDK_Thai_yoying 0xdad
#define GDK_Thai_dochada 0xdae
#define GDK_Thai_topatak 0xdaf
#define GDK_Thai_thothan 0xdb0
#define GDK_Thai_thonangmontho 0xdb1
#define GDK_Thai_thophuthao 0xdb2
#define GDK_Thai_nonen 0xdb3
#define GDK_Thai_dodek 0xdb4
#define GDK_Thai_totao 0xdb5
#define GDK_Thai_thothung 0xdb6
#define GDK_Thai_thothahan 0xdb7
#define GDK_Thai_thothong 0xdb8
#define GDK_Thai_nonu 0xdb9
#define GDK_Thai_bobaimai 0xdba
#define GDK_Thai_popla 0xdbb
#define GDK_Thai_phophung 0xdbc
#define GDK_Thai_fofa 0xdbd
#define GDK_Thai_phophan 0xdbe
#define GDK_Thai_fofan 0xdbf
#define GDK_Thai_phosamphao 0xdc0
#define GDK_Thai_moma 0xdc1
#define GDK_Thai_yoyak 0xdc2
#define GDK_Thai_rorua 0xdc3
#define GDK_Thai_ru 0xdc4
#define GDK_Thai_loling 0xdc5
#define GDK_Thai_lu 0xdc6
#define GDK_Thai_wowaen 0xdc7
#define GDK_Thai_sosala 0xdc8
#define GDK_Thai_sorusi 0xdc9
#define GDK_Thai_sosua 0xdca
#define GDK_Thai_hohip 0xdcb
#define GDK_Thai_lochula 0xdcc
#define GDK_Thai_oang 0xdcd
#define GDK_Thai_honokhuk 0xdce
#define GDK_Thai_paiyannoi 0xdcf
#define GDK_Thai_saraa 0xdd0
#define GDK_Thai_maihanakat 0xdd1
#define GDK_Thai_saraaa 0xdd2
#define GDK_Thai_saraam 0xdd3
#define GDK_Thai_sarai 0xdd4
#define GDK_Thai_saraii 0xdd5
#define GDK_Thai_saraue 0xdd6
#define GDK_Thai_sarauee 0xdd7
#define GDK_Thai_sarau 0xdd8
#define GDK_Thai_sarauu 0xdd9
#define GDK_Thai_phinthu 0xdda
#define GDK_Thai_maihanakat_maitho 0xdde
#define GDK_Thai_baht 0xddf
#define GDK_Thai_sarae 0xde0
#define GDK_Thai_saraae 0xde1
#define GDK_Thai_sarao 0xde2
#define GDK_Thai_saraaimaimuan 0xde3
#define GDK_Thai_saraaimaimalai 0xde4
#define GDK_Thai_lakkhangyao 0xde5
#define GDK_Thai_maiyamok 0xde6
#define GDK_Thai_maitaikhu 0xde7
#define GDK_Thai_maiek 0xde8
#define GDK_Thai_maitho 0xde9
#define GDK_Thai_maitri 0xdea
#define GDK_Thai_maichattawa 0xdeb
#define GDK_Thai_thanthakhat 0xdec
#define GDK_Thai_nikhahit 0xded
#define GDK_Thai_leksun 0xdf0
#define GDK_Thai_leknung 0xdf1
#define GDK_Thai_leksong 0xdf2
#define GDK_Thai_leksam 0xdf3
#define GDK_Thai_leksi 0xdf4
#define GDK_Thai_lekha 0xdf5
#define GDK_Thai_lekhok 0xdf6
#define GDK_Thai_lekchet 0xdf7
#define GDK_Thai_lekpaet 0xdf8
#define GDK_Thai_lekkao 0xdf9
#define GDK_Hangul 0xff31
#define GDK_Hangul_Start 0xff32
#define GDK_Hangul_End 0xff33
#define GDK_Hangul_Hanja 0xff34
#define GDK_Hangul_Jamo 0xff35
#define GDK_Hangul_Romaja 0xff36
#define GDK_Hangul_Codeinput 0xff37
#define GDK_Hangul_Jeonja 0xff38
#define GDK_Hangul_Banja 0xff39
#define GDK_Hangul_PreHanja 0xff3a
#define GDK_Hangul_PostHanja 0xff3b
#define GDK_Hangul_SingleCandidate 0xff3c
#define GDK_Hangul_MultipleCandidate 0xff3d
#define GDK_Hangul_PreviousCandidate 0xff3e
#define GDK_Hangul_Special 0xff3f
#define GDK_Hangul_switch 0xFF7E
#define GDK_Hangul_Kiyeog 0xea1
#define GDK_Hangul_SsangKiyeog 0xea2
#define GDK_Hangul_KiyeogSios 0xea3
#define GDK_Hangul_Nieun 0xea4
#define GDK_Hangul_NieunJieuj 0xea5
#define GDK_Hangul_NieunHieuh 0xea6
#define GDK_Hangul_Dikeud 0xea7
#define GDK_Hangul_SsangDikeud 0xea8
#define GDK_Hangul_Rieul 0xea9
#define GDK_Hangul_RieulKiyeog 0xeaa
#define GDK_Hangul_RieulMieum 0xeab
#define GDK_Hangul_RieulPieub 0xeac
#define GDK_Hangul_RieulSios 0xead
#define GDK_Hangul_RieulTieut 0xeae
#define GDK_Hangul_RieulPhieuf 0xeaf
#define GDK_Hangul_RieulHieuh 0xeb0
#define GDK_Hangul_Mieum 0xeb1
#define GDK_Hangul_Pieub 0xeb2
#define GDK_Hangul_SsangPieub 0xeb3
#define GDK_Hangul_PieubSios 0xeb4
#define GDK_Hangul_Sios 0xeb5
#define GDK_Hangul_SsangSios 0xeb6
#define GDK_Hangul_Ieung 0xeb7
#define GDK_Hangul_Jieuj 0xeb8
#define GDK_Hangul_SsangJieuj 0xeb9
#define GDK_Hangul_Cieuc 0xeba
#define GDK_Hangul_Khieuq 0xebb
#define GDK_Hangul_Tieut 0xebc
#define GDK_Hangul_Phieuf 0xebd
#define GDK_Hangul_Hieuh 0xebe
#define GDK_Hangul_A 0xebf
#define GDK_Hangul_AE 0xec0
#define GDK_Hangul_YA 0xec1
#define GDK_Hangul_YAE 0xec2
#define GDK_Hangul_EO 0xec3
#define GDK_Hangul_E 0xec4
#define GDK_Hangul_YEO 0xec5
#define GDK_Hangul_YE 0xec6
#define GDK_Hangul_O 0xec7
#define GDK_Hangul_WA 0xec8
#define GDK_Hangul_WAE 0xec9
#define GDK_Hangul_OE 0xeca
#define GDK_Hangul_YO 0xecb
#define GDK_Hangul_U 0xecc
#define GDK_Hangul_WEO 0xecd
#define GDK_Hangul_WE 0xece
#define GDK_Hangul_WI 0xecf
#define GDK_Hangul_YU 0xed0
#define GDK_Hangul_EU 0xed1
#define GDK_Hangul_YI 0xed2
#define GDK_Hangul_I 0xed3
#define GDK_Hangul_J_Kiyeog 0xed4
#define GDK_Hangul_J_SsangKiyeog 0xed5
#define GDK_Hangul_J_KiyeogSios 0xed6
#define GDK_Hangul_J_Nieun 0xed7
#define GDK_Hangul_J_NieunJieuj 0xed8
#define GDK_Hangul_J_NieunHieuh 0xed9
#define GDK_Hangul_J_Dikeud 0xeda
#define GDK_Hangul_J_Rieul 0xedb
#define GDK_Hangul_J_RieulKiyeog 0xedc
#define GDK_Hangul_J_RieulMieum 0xedd
#define GDK_Hangul_J_RieulPieub 0xede
#define GDK_Hangul_J_RieulSios 0xedf
#define GDK_Hangul_J_RieulTieut 0xee0
#define GDK_Hangul_J_RieulPhieuf 0xee1
#define GDK_Hangul_J_RieulHieuh 0xee2
#define GDK_Hangul_J_Mieum 0xee3
#define GDK_Hangul_J_Pieub 0xee4
#define GDK_Hangul_J_PieubSios 0xee5
#define GDK_Hangul_J_Sios 0xee6
#define GDK_Hangul_J_SsangSios 0xee7
#define GDK_Hangul_J_Ieung 0xee8
#define GDK_Hangul_J_Jieuj 0xee9
#define GDK_Hangul_J_Cieuc 0xeea
#define GDK_Hangul_J_Khieuq 0xeeb
#define GDK_Hangul_J_Tieut 0xeec
#define GDK_Hangul_J_Phieuf 0xeed
#define GDK_Hangul_J_Hieuh 0xeee
#define GDK_Hangul_RieulYeorinHieuh 0xeef
#define GDK_Hangul_SunkyeongeumMieum 0xef0
#define GDK_Hangul_SunkyeongeumPieub 0xef1
#define GDK_Hangul_PanSios 0xef2
#define GDK_Hangul_KkogjiDalrinIeung 0xef3
#define GDK_Hangul_SunkyeongeumPhieuf 0xef4
#define GDK_Hangul_YeorinHieuh 0xef5
#define GDK_Hangul_AraeA 0xef6
#define GDK_Hangul_AraeAE 0xef7
#define GDK_Hangul_J_PanSios 0xef8
#define GDK_Hangul_J_KkogjiDalrinIeung 0xef9
#define GDK_Hangul_J_YeorinHieuh 0xefa
#define GDK_Korean_Won 0xeff

/* gdk_drawRgb's functions */
#define GDK_RGB_DITHER_NONE     0
#define GDK_RGB_DITHER_NORMAL   1
#define GDK_RGB_DITHER_MAX      2

/* gdk_pixbuf's functions  */
#define GDK_INTERP_NEAREST      0
#define GDK_INTERP_TILES        1
#define GDK_INTERP_BILINEAR     2
#define GDK_INTERP_HYPER        3

#define CLIP_GTK_TYPE_MENU_BAR		0
#define CLIP_GTK_TYPE_MENU		1
#define CLIP_GTK_TYPE_OPTION_MENU	2

#ifndef GDK_TYPE_BITMAP
#define GDK_TYPE_BITMAP		HASH___GDK_TYPE_BITMAP
#endif

#ifndef GDK_TYPE_PIXMAP
#define GDK_TYPE_PIXMAP		HASH___GDK_TYPE_PIXMAP
#endif

#ifndef GDK_TYPE_WINDOW
#define GDK_TYPE_WINDOW		HASH___GDK_TYPE_WINDOW
#endif

#ifndef GDK_TYPE_COLORMAP
#define GDK_TYPE_COLORMAP	HASH___GDK_TYPE_COLORMAP
#endif

#endif



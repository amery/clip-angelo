/*
        eglade.prg
        Example editor using clip-libglade2.
        Editor de prueba usando clip-libglade2.
*/


static _xmlfic


function xmlfic()
return( _xmlfic )



function main( glade_file )

        local xml, widget

        _xmlfic := iif( empty( glade_file ), "editor.glade", glade_file )

        if( ! file( _xmlfic ))
                ? "Not found interface file: "+_xmlfic
                ?
quit
        endif

        gtk_init()


        xml := glade_xml_new( _xmlfic, "main_window" )


        if( xml == nil )
                ? "** Parser error, file: "+_xmlfic
                ?
return( 1 )
        endif



        // Editor main window.
        widget := glade_xml_get_widget( xml, "main_window" )

        /*
          Autoconnect the functions with name equal to property 'handler' of
        signals in xml file.
          Warning : thats functions can't be statics.

          Conecta autom�ticamente las funciones que tengan el mismo nombre
        que la propiedad 'handler' de las se�ales con �stas.
          Nota: Las funciones no pueden ser 'static'.
        */
        glade_xml_signal_autoconnect( xml )


        /*
          clip-gtk bug : 'delete-event' signal name is wrong,
        must be 'delete_event'.
          clip-gtk utiliza el nombre antiguo de la se�al
        'delete_event'.
        */
        gtk_signalConnect( widget, "delete-event",;
           {|w,e| on_main_window_delete_event(w,e)} )


        /*
          Warning : the toplevel widgets must have the property
        'visible=False'

          Nota: Si la propiedad 'visible'=True la ventana se
        muestra de forma autom�tica y sobrar�a la siguiente linea.
        */
        gtk_widgetShow( widget )


        // running...
        gtk_main()
        ?
        ?

return( 0 )



/* Funciones gestoras de se�ales. */

/*
  Main window delete_event handler
  Se�al recibida de la ventana principal
*/
function on_main_window_delete_event( wid, event )
        ? "Press Ctrl+Q to quit..."
return( .t. )



/*
  Option menu: New
  Men�: Nuevo
*/
procedure on_New_activate( wid )
        ? "New file..."
return


/*
  Option menu: Open
  Men�: Abrir
*/
procedure on_Open_activate( wid )
        local cfic

        ? "Running file selection to open..."
        cfic := cSelec_fic()
        ? "File to open: ", cfic
return


/*
  Option menu: Save
  Men�: Guardar
*/
procedure on_Save_activate( wid )
        local cfic

        ? "Running file selection dialog to save..."
        cfic := csaveSelec_fic()
        ? "Save file to: ", cfic

return


/*
  Option menu: Save as...
  Men�: Guardar como...
*/
procedure on_Save_As_activate( wid )
        ? "Save file as..."
        on_Save_activate(wid)
return


/*
  Option menu: Quit
  Men�: Salir
*/
procedure on_Exit_activate( wid )
        ? "Terminated"
        gtk_quit()
return


/*
  Option menu: Cut
  Men�: Cortar
*/
procedure on_Cut_activate( wid )
        ? "Cut..."
return

/*
  Option menu: Copy
  Men�: Copiar
*/
procedure on_Copy_activate( wid )
        ? "Copy..."
return

/*
  Optin menu: Paste
  Men� : Pegar
*/
procedure on_Paste_activate( wid )
        ? "Paste..."
return

/*
  Option menu: Clear
  Men�: Borrar
*/
procedure on_Clear_activate( wid )
        ? "Clear..."
return


/*
  Option menu: Change Font...
  Men�: Cambiar Fuente...
*/
procedure on_Font_activate( wid )

        local newfont

        newfont := cSelec_font()
        ? "Selected Font: ", newfont

return


/*
  Option menu: About...
  Men�: About...
*/
procedure on_About_activate( wid )
        about()
return


/*
  Toolbar: New
  Toolbar: Nuevo
*/
procedure on_new_button_clicked( wid )
        ? "toolbar: New File..."
return


/*
  Toolbar: Open
  Toolbar: Abrir
*/
procedure on_open_button_clicked( wid )
        ? "toolbar: Open file..."
        on_open_activate(wid)
return


/*
  Toolbar: Save
  Toolbar: Guardar
*/
procedure on_save_button_clicked( wid )
        ? "toolbar: Save..."
        on_Save_activate(wid)
return


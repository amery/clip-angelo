function _recover_menuitem(obj)
   obj:isPopUp	:= @__isPopUp()
   obj:isAccel	:= @mi_isAccel()
   obj:_status	:= @mi_status()
return obj

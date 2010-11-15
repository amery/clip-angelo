/*-------------------------------------------------------------------------*/
/*   This is a part of CLIP-UI library					   */
/*						                 	   */
/*   Copyright (C) 2005 by E/AS Software Foundation 	           	   */
/*   Author: Andrey Cherepanov <skull@eas.lrn.ru>			   */
/*   									   */
/*   This program is free software; you can redistribute it and/or modify  */
/*   it under the terms of the GNU General Public License as               */
/*   published by the Free Software Foundation; either version 2 of the    */
/*   License, or (at your option) any later version.                       */
/*-------------------------------------------------------------------------*/
#include "ci_clip-ui.ch"

static driver := getDriver()

/* UITimer class */
function UITimer( period, action, data )
	local obj := map()

	obj:id := driver:startTimer(period*1000, action, data)
	obj:className := "UITimer"

	obj:period := period*1000	// in seconds
	obj:action := action
	obj:data   := data

	_recover_UITIMER(obj)
return obj

function _recover_UITIMER( obj )
	obj:start	:= @ui_start()
	obj:stop	:= @ui_stop()
return obj

/* Restart timer */
static function ui_start( self, period, action, data )
	self:stop()
	self:period := iif(empty(period),self:period,period*1000)	// in seconds
	self:action := iif(empty(action),self:action,action)
	self:data   := iif(empty(data),self:data,data)

	self:id := driver:startTimer(self:period, self:action, self:data)
return

/* Stop timer */
static function ui_stop( self )
	driver:stopTimer( self:id )
return

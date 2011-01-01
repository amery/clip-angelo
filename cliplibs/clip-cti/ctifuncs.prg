/*
	Copyright (C) 2002  ITK
	CLIP text interface library
	Author	: Alexey M. Tkachenko (alexey@itk.ru)
	License	: (GPL) http://www.itk.ru/clipper/license.html
*/

#include "ci_cti.ch"

/* Function for inheritance CTI objects.
   Creates virtual methods table.
   Adds classname to object`s genealogy for simplification object cast definiton */
function cti_inherit(obj, cClassName)
	local s_obj := map()
	local k//, superclass := "__"+obj:classname+"__"

	if HASH_CLASSNAME $ obj
		/* Make virtual methods table */
		if .not. HASH___VIRTUAL__ $ obj; obj:__VIRTUAL__ := map(); endif

		for k in obj keys
			if valtype(obj[k])=="B"
				s_obj[k] := clone(obj[k])
			endif
		next
		obj:__VIRTUAL__[obj:classname] := s_obj
	endif

	/* Set widget classname and name by default */
	obj:classname	:= cClassName
	obj:name	:= obj:classname

	/* Add widget classname to it`s genealogy */
	if !HASH___GENEALOGY $ obj; obj:__genealogy := ""; endif

	obj:__genealogy += ":"+obj:classname+":"
return obj

/* Returns reference to object with identificator "id" */
function cti_get_object_by_id(id)
	if id==nil
		return nil
	endif
return cti_object_list_get(id)

/* Checks obj for implement of class cClassName */
function cti_check_cast(obj,cClassName)
	if .not. (valtype(obj)=="O" .and. HASH___GENEALOGY $ obj)
		return FALSE
	endif
return cClassName $ obj:__genealogy

/* Convert variable value to numeric, including NIL */
function cti_cnum(var)
	switch (valtype(var))
		case "N"
		return var

		case "C"
		return val(var)
	end
return 0

/* Convert variable value to string, trimming all spaces */
function cti_cstr(var)
	switch (valtype(var))
		case "N"
		return alltrim(str(var))
	end
return 0

/* Returns version of CTI */
function cti_version()
return CTI_VERSION

/* Justificate text */
function cti_text_justify(text,width,justify)
	local justified_text

	justify := iif(valtype(justify)=="N",justify,CTI_JUSTIFY_LEFT)
	switch (justify)
		case CTI_JUSTIFY_LEFT
		justified_text := padr(text,width)

		case CTI_JUSTIFY_CENTER
		justified_text := padc(text,width)

		case CTI_JUSTIFY_RIGHT
		justified_text := padl(text,width)
	end
return justified_text

function cti_min(var1,var2)
	if var1==nil; return var2; endif
	if var2==nil; return var1; endif
return min(var1,var2)

function cti_max(var1,var2)
	if var1==nil; return var2; endif
	if var2==nil; return var1; endif
return max(var1,var2)

function cti_bound(Value,lBound,rBound)
	if lBound <= rBound
		if Value < lBound; return lBound; endif
		if Value > rBound; return rBound; endif
		return Value
	endif
	if lBound > rBound
		if Value > lBound; return lBound; endif
		if Value < rBound; return rBound; endif
		return Value
	endif
return

function cti_text_parse_accelerator(text,accel_key,accel_pos)
	local pos

	if valtype(text)!="C"; return text; endif

	pos := at(CTI_ACCEL_CHAR, text)
	if pos > 0
		text := stuff(text,pos,1,"")
		accel_key := substr(text,pos,1)
		accel_pos := pos
	else
		accel_key := ""
		accel_pos := -1
	endif
return text

function cti_get_app()
	memvar CtiApplication
return CtiApplication

/*
	Copyright (C) 2001  ITK
	Author   : Uri Hnykin <uri@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
/*

   small documentation see in integer.h

   (c) itk 2000, www.itk.ru
   autor - uri, uri@itk.ru

*/

#include <ci_integer.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <ci_clip.h>

#include <integer/integer_sign.c>
#include <integer/integer_len.c>
#include <integer/integer_vec.c>
#include <integer/integer_realLen.c>
#include <integer/integer_destroy.c>
#include <integer/integer_init.c>
#include <integer/integer_long_init.c>
#include <integer/integer_long2_init.c>
#include <integer/integer_fromString.c>
#include <integer/integer_mul.c>
#include <integer/integer_mula.c>
#include <integer/integer_long_mul.c>
#include <integer/integer_long_mula.c>
#include <integer/integer_pow.c>
#include <integer/integer_powa.c>
#include <integer/integer_gcd.c>
#include <integer/integer_sunscale.c>
#include <integer/integer_sdivide.c>
#include <integer/integer_div.c>
#include <integer/integer_diva.c>
#include <integer/integer_mod.c>
#include <integer/integer_lg.c>
#include <integer/integer_sqrt.c>
#include <integer/integer_sadd.c>
#include <integer/integer_long_add.c>
#include <integer/integer_long_adda.c>
#include <integer/integer_add.c>
#include <integer/integer_adda.c>
#include <integer/integer_inv.c>
#include <integer/integer_inverse.c>
#include <integer/integer_ssub.c>
#include <integer/integer_long_sub.c>
#include <integer/integer_long_suba.c>
#include <integer/integer_sub.c>
#include <integer/integer_suba.c>
#include <integer/integer_lshifta.c>
#include <integer/integer_lshift.c>
#include <integer/integer_rshifta.c>
#include <integer/integer_rshift.c>
#include <integer/integer_getBit.c>
#include <integer/integer_setBit.c>
#include <integer/integer_setBita.c>
#include <integer/integer_or.c>
#include <integer/integer_long_or.c>
#include <integer/integer_ora.c>
#include <integer/integer_and.c>
#include <integer/integer_long_and.c>
#include <integer/integer_xora.c>
#include <integer/integer_xor.c>
#include <integer/integer_long_xor.c>
#include <integer/integer_anda.c>
#include <integer/integer_copy.c>
#include <integer/integer_assign.c>
#include <integer/integer_toString2.c>
#include <integer/integer_toString.c>
#include <integer/integer_toLong.c>
#include <integer/integer_abscmp.c>
#include <integer/integer_cmp.c>
#include <integer/integer_abs.c>
#include <integer/integer_even.c>
#include <integer/integer_clear.c>
#include <integer/integer_empty.c>
#include <integer/integer_printf.c>
#include <integer/integer_unscale.c>
#include <integer/integer_resize.c>

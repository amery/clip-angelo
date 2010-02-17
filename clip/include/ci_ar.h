
/* Typed in by Richard */
/* minor changes for linux */
/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#ifndef CI__AR_H
#define CI__AR_H

#define ARMAG "!<arch>\n"
#define SARMAG 8
#define ARFMAG "`\n"

struct ar_hdr
{
   char ar_name_of_ar_hdr[16], ar_date_of_ar_hdr[12], ar_uid_of_ar_hdr[6], ar_gid_of_ar_hdr[6], ar_mode_of_ar_hdr[8], ar_size_of_ar_hdr[10], ar_fmag_of_ar_hdr[2];
};

#endif /* _AR_H */

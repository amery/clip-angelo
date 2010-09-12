#ifndef CN_DBINAMES_DEFINED
#define CN_DBINAMES_CH


/* codes for DBRECORDINFO() */
__dbri_names:=map()
__dbri_names[1]:="DELETED"
__dbri_names[2]:="LOCKED"
__dbri_names[3]:="RECSIZE"
__dbri_names[4]:="RECNO"
__dbri_names[5]:="UPDATED"

/* DBOI_* are defined in ORDINFO.CH */
__dboi_names:=map()
__dboi_names[1 ]:="CONDITION"
__dboi_names[2 ]:="EXPRESSION"
__dboi_names[3 ]:="POSITION"
__dboi_names[4 ]:="RECNO"
__dboi_names[5 ]:="NAME"
__dboi_names[6 ]:="NUMBER"
__dboi_names[7 ]:="BAGNAME"
__dboi_names[8 ]:="BAGEXT"
__dboi_names[9 ]:="ORDERCOUNT"
__dboi_names[10]:="FILEHANDLE"
__dboi_names[11]:="ISCOND"
__dboi_names[12]:="ISDESC"
__dboi_names[13]:="UNIQUE"

__dboi_names[20]:="FULLPATH"
__dboi_names[24]:="KEYTYPE"
__dboi_names[25]:="KEYSIZE"
__dboi_names[26]:="KEYCOUNT"
__dboi_names[27]:="SETCODEBLOCK"
__dboi_names[28]:="KEYDEC"
__dboi_names[29]:="HPLOCKING"
__dboi_names[35]:="LOCKOFFSET"
__dboi_names[36]:="KEYADD"
__dboi_names[37]:="KEYDELETE"
__dboi_names[38]:="KEYVAL"
__dboi_names[39]:="SCOPETOP"
__dboi_names[40]:="SCOPEBOTTOM"
__dboi_names[41]:="SCOPETOPCLEAR"
__dboi_names[42]:="SCOPEBOTTOMCLEAR"
__dboi_names[45]:="CUSTOM"
__dboi_names[46]:="SKIPUNIQUE"
__dboi_names[50]:="KEYSINCLUDED"
__dboi_names[51]:="KEYNORAW"
__dboi_names[52]:="KEYCOUNTRAW"
__dboi_names[53]:="OPTLEVEL"
__dboi_names[60]:="STRICTREAD"
__dboi_names[61]:="OPTIMIZE"
__dboi_names[62]:="AUTOOPEN"
__dboi_names[63]:="AUTOORDER"
__dboi_names[64]:="AUTOSHARE"


/* codes for DBINFO() */
__dbi_names:=map()
__dbi_names[1 ]:="ISDBF"
__dbi_names[2 ]:="CANPUTREC"
__dbi_names[3 ]:="GETHEADERSIZE"
__dbi_names[4 ]:="LASTUPDATE"
__dbi_names[5 ]:="GETDELIMITER"
__dbi_names[6 ]:="SETDELIMITER"
__dbi_names[7 ]:="GETRECSIZE"
__dbi_names[8 ]:="GETLOCKARRAY"
__dbi_names[9 ]:="TABLEEXT"
__dbi_names[10]:="FULLPATH"
__dbi_names[20]:="ISFLOCK"
__dbi_names[22]:="CHILDCOUNT"
__dbi_names[23]:="FILEHANDLE"
__dbi_names[26]:="BOF"
__dbi_names[27]:="EOF"
__dbi_names[28]:="DBFILTER"
__dbi_names[29]:="FOUND"
__dbi_names[30]:="FCOUNT"
__dbi_names[31]:="LOCKCOUNT"
__dbi_names[32]:="VALIDBUFFER"
__dbi_names[33]:="ALIAS"
__dbi_names[34]:="GETSCOPE"
__dbi_names[35]:="LOCKOFFSET"
__dbi_names[36]:="SHARED"
__dbi_names[37]:="MEMOEXT"
__dbi_names[38]:="MEMOHANDLE"
__dbi_names[39]:="MEMOBLOCKSIZE"
__dbi_names[101]:="DB_VERSION"
__dbi_names[102]:="RDD_VERSION"
/* uri defined */
__dbi_names[-1]:="RECNO"
__dbi_names[-2]:="LASTREC"
__dbi_names[1000]:="USER"

__blob_names:=map()
__blob_names[201]:="BLOB_DIRECT_EXPORT"
__blob_names[202]:="BLOB_DIRECT_GET"
__blob_names[203]:="BLOB_DIRECT_IMPORT"
__blob_names[204]:="BLOB_DIRECT_PUT"
__blob_names[205]:="BLOB_ROOT_GET"
__blob_names[206]:="BLOB_ROOT_PUT"
__blob_names[207]:="BLOB_ROOT_LOCK"
__blob_names[208]:="BLOB_ROOT_UNLOCK"
__blob_names[209]:="BLOB_DIRECT_LEN"
__blob_names[210]:="BLOB_DIRECT_TYPE"
__blob_names[211]:="BLOB_INTEGRITY"
__blob_names[212]:="BLOB_OFFSET"
__blob_names[213]:="BLOB_RECOVER"

#endif

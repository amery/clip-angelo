#!/bin/bash -ux
##
## 															config dir
##

#
# 		local variables defined here
#
beep_on

echo We NEED to check if certains programs are installed  	>&0
echo for a user friendly display and compatibility.			>&0
echo ""																		>&0
echo ""																		>&0
#read -p "press <enter> to continue"
WaitForCheck="y"
while [[ $WaitForCheck != [yYnN] ]] ; do
	WaitForCheck="z"
	echo "Ask to wait for library install in checking installed packages : " >&0
	read -n 1 WaitForCheck
done
export WaitForCheck
#
# 1. 	type of file search : Check_Bin Check_Include Check_Library
# 2. 	filename
# 3. 	file_extension : xyz if none
# 4. 	package name
# 5. 	requested
# 6.	start of search
# 7. 	name(s) of packages to be installed to have the filename
#
# 1			2					3		4				5		6						7
#
Check_Bin 	banner 			xyz 	all 			Yes	/usr/bin 			"banner-xx-$ARCH.(rpm/deb)"
Check_Bin 	bison 			xyz 	all 			Yes 	/usr/bin 			"bison-xx-$ARCH.(rpm/deb)"
Check_Bin 	cvs 				xyz 	all 			Yes 	/usr/bin 			"cvs-xx-$ARCH.(rpm/deb)"
Check_Bin 	flex 				xyz 	all 			Yes 	/usr/bin 			"flex-xx-$ARCH.(rpm/deb)"
Check_Bin 	gcc 				xyz 	all 			Yes 	/usr/bin 			"gcc-xx-$ARCH.(rpm/deb)"
Check_Bin 	gnubeep 			xyz 	all 			Yes 	/usr/bin 			"gnubeep-xx-$ARCH.(rpm/deb)"
#Check_Bin 	imake 			xyz 	clip 			Yes 	/usr/bin 			"imake-xx-$ARCH.(rpm/deb)"
Check_Bin 	ld 				xyz 	all 			Yes 	/usr/bin 			"binutils-xx-$ARCH.(rpm/deb)"
Check_Bin 	make 				xyz 	all 			Yes 	/usr/bin 			"make-xx-$ARCH.(rpm/deb)"
Check_Bin 	makedepend 		xyz 	all 			Yes 	/usr/X11R6/bin/ 	"makedepend-xx-$ARCH.(rpm/deb)"
#Check_Bin 	mplayer 			xyz 	all 			No 	/usr/X11R6/bin/ 	"mplayer-xx-$ARCH.(rpm/deb)"
#Check_Bin 	mrxvt 			xyz 	clip 			Yes 	/usr/bin 			"mrxvt-xx-$ARCH.(rpm/deb)"
Check_Bin 	msgfmt 			xyz 	all 			Yes 	/usr/bin 			"gettext-devel-xx-$ARCH.(rpm/deb)"
Check_Bin 	nm 				xyz 	all 			Yes 	/usr/bin 			"binutils-xx-$ARCH.(rpm/deb)"
Check_Bin 	pkg-config 		xyz 	all 			Yes 	/usr/bin 			"pkgconfig-xx-$ARCH.(rpm/deb)"
Check_Bin 	ranlib 			xyz 	all 			Yes 	/usr/bin 			"binutils-xx-$ARCH.(rpm/deb)"
#Check_Bin 	rxvt 				xyz 	clip			Yes 	/usr/bin				"rvxt-unicode-xx-$ARCH.(rpm/deb)"
Check_Bin 	strip 			xyz 	all 			Yes 	/usr/bin 			"binutils-xx-$ARCH.(rpm/deb)"
#Check_Bin 	urxvt 			xyz 	clip 			Yes 	/usr/bin 			"rxvt-unicode-xx-$ARCH.(rpm/deb)"
Check_Bin xclip 				xyz 	clip 			No 	/usr/bin 			"http://sourceforge.net/projects/xclip"
#Check_Bin xterm 				xyz 	clip 			No 	/usr/bin 			xterm-xx-$ARCH.(rpm/deb)
Check_Bin wget 				xyz 	all 			Yes 	/usr/bin 			"wget-xx-$ARCH.(rpm/deb)"

Check_Include 		expat.h 					xyz clip-xml 			Yes 	/usr/include 										"libexpat-devel-xx-$ARCH.(rpm/deb)"
#Check_Include 	float.h 					xyz 	clip 				Yes 	/usr/include 										"gcc4xx-xx-$ARCH.(rpm/deb)"
Check_Include 		gdkconfig.h  			xyz   clip-gtk2 		yes 	/usr/lib$C64  										"libgtk+2-devel-$ARCH.(rpm/deb)"
Check_Include 		sys/io.h 				xyz 	clip 				Yes 	/usr/include 										"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		jpeglib.h 				xyz 	clip				Yes 	/usr/include/sys 									"lib(m)jpeg-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		libpq-fe.h 				xyz 	clip-postgres 	Yes 	/usr/include/pgsql 								"postgresql-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		sys/mman.h 				xyz 	clip				Yes 	/usr/include 										"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		mysql.h 					xyz 	clip-mysql 		Yes 	/usr/include/mysql 								"libmysql-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		occi.h 					xyz 	clip-oracle 	yes 	/usr/include/oracle/*/client$C64/  			"oracle-instantclient-basic-xx-$ARCH.(deb/rpm)" \
																																				"oracle-instantclient-devel-xx-$ARCH.(deb/rpm)" \
																																				"can be downloaded at" \
																																				"http://www.oracle.com/ instantclient"
Check_Include 		oci.h 					xyz 	clip				Yes 	/usr/lib/*/client$C64/ 							"libpng-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		png.h 					xyz 	clip				Yes 	/usr/include/sys 									"libpng-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		readline/readline.h 	xyz 	clip 				Yes 	/usr/include 										"libreadline-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		regex.h 					xyz 	clip 				Yes 	/usr/include 										"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		sqlext.h 				xyz 	clip-odbc 		Yes 	/usr/include 										"libunixODBC1-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		odbc.h 					xyz 	clip-odbc 		Yes 	/usr/local/easysoft/oob/client/include 	"can be downloaded at" \
																																				"http://www.easysoft.com/products/data_access"
Check_Include 		protocol.h 				xyz 	clip-dbtcp 		Yes 	/opt/dbtcp 											"dbtcp.xx.tgz" \
																																				"can be downloaded at" \
																																				"http://packages.qa.debian.org/d/dbtcp.html"
Check_Include 		sys/vfs.h 				xyz 	clip				Yes 	/usr/include 										"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		sys/statvfs.h 			xyz 	clip				Yes 	/usr/include 										"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		stropts.h 				xyz 	clip				Yes 	/usr/include 										"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		sys/time.h 				xyz 	clip 				Yes 	/usr/include 										"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		utmp.h 					xyz 	clip				Yes 	/usr/include/sys 									"glibc-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		X11/xpm.h 				xyz 	libclip-gd 		Yes 	/usr/include 										"libxpm-static-devel-xx-$ARCH.(rpm/deb)"
Check_Include 		zlib.h 					xyz 	clip-gzip 		Yes 	/usr/lib$C64 										"zlib-devel-xx-$ARCH.(rpm/deb)"

Check_Library 		BrokenLocale 		.a 	clip 				Yes 	/usr/lib$C64 							"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		bz2 					.so	clip-bz2 		Yes 	/usr/lib$C64 							"libbzip2-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		c 						.so 	clip 				Yes 	/usr/lib$C64 							"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		clntsh 				.so 	clip-oracle		yes 	/usr/lib/oracle/*/client$C64/  	"oracle-instantclient-basic-xx-$ARCH.(deb/rpm)" \
																																"oracle-instantclient-devel-xx-$ARCH.(deb/rpm)" \
																																"can be downloaded at" \
																																"http://www.oracle.com/ instantclient"
Check_Library 		cryptopp 			.so	clip-crypto 	Yes 	/usr/lib$C64 							"libcrypto-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		dbftp 				.a 	clip-dbtcp 		Yes 	/opt/dbtcp 								"dbtcp.xx.tgz" \
																																"can be downloaded at" \
																																"http://packages.qa.debian.org/d/dbtcp.html"
Check_Library 		dl 					.so 	clip 				Yes 	/usr/lib$C64 							"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		esoobS 				.so 	clip-odbc 		Yes 	/usr/lib$C64							"libunixODBC1-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		esoobclient 		.so	clip-odbc 		Yes 	/usr/local/easysoft/oob/client 	"can be downloaded at" \
																																"http://www.easysoft.com/products/data_access"
Check_Library 		expat 				.so 	clip-xml 		Yes 	/usr/lib$C64 							"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		fcgi 					.so 	clip-fcgi 		Yes 	/usr/lib$C64 							"libfcgi-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		freetype 			.so 	clip-gd 			Yes	/usr/lib$C64 							"libfreetype-static-devel-xx-$ARCH.(rpm/dev)"
Check_Library 		gd 					.so 	clip-gd 			Yes	/usr/lib$C64 							"libgd-static-devel-xx-$ARCH.(rpm/dev)"
Check_Library 		gettextlib 			.so 	clip 				Yes 	/usr/lib$C64 							"gettext-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		glade-2.0 			.so 	clip-glade2 	Yes 	/usr/lib$C64 							"libglade-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		gpm 					.so 	clip 				Yes 	/usr/lib$C64 							"gpm-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		iconv 				.so	clip				Yes 	/usr/local								"http://www.gnu.org/software/libiconv"
Check_Library 		jpeg 					.so 	clip-gd 			Yes 	/usr/lib$C64 							"libjpeg-static-xx-$ARCH.(rpm/deb)"
Check_Library 		m 						.so 	clip 				Yes	/usr/lib$C64 							"glibc-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		mysys 				.so 	clip-mysql		Yes 	/usr/lib$C64 							"libmysql-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		mysqlclient 		.so 	clip-mysql		Yes 	/usr/lib$C64 							"libmysql-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		ncurses 				.so 	clip 				Yes 	/usr/lib$C64 							"libncurses-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		ncursesw 			.so 	clip 				Yes 	/usr/lib$C64 							"libncursesw-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		png 					.so 	clip-gd 			Yes 	/usr/lib$C64 							"libpng-static-xx-$ARCH.(rpm/deb)"
Check_Library 		pgtypes 				.so 	clip-postgres 	Yes 	/usr/lib$C64 							"postgresql-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		pq 					.so 	clip-postgres 	Yes 	/usr/lib$C64 							"postgres-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		pth 					.so 	clip 				Yes 	/usr/lib$C64 							"libpth-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		readline 			.so 	clip 				Yes 	/usr/lib$C64 							"lib(tcl)readline-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		ttf 					.so 	clip-gd 			Yes 	/usr/lib$C64 							"libfreetype-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		X11 					.so 	all 				Yes	/usr/lib$C64 							"libxorg-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		z 						.so 	clip-gzip 		Yes 	/usr/lib$C64 							"zlib-devel-xx-$ARCH.(rpm/deb)"

if [[ "$C64" != "64" ]] ; then
	Check_Include 	ibase.h 	xyz 	clip-interbase 	yes 	/opt/interbase/include 	"ib2009-linux-xx.tar.gz" "can be downloaded at" "http://www.codegear.com/downloads/free/interbase"
	Check_Library 	gds 		xyz 	clip-interbase 	yes 	/opt/interbase/lib 		"ib2009-linux-xx.tar.gz" "can be downloaded at" "http://www.codegear.com/downloads/free/interbase"
fi


Check_Library 		gtk-x11-2.0 		.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		gdk-x11-2.0 		.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		atk-1.0 				.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		gdk_pixbuf-2.0 	.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		pangocairo-1.0 	.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		pango-1.0 			.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		cairo 				.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		gobject-2.0 		.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		gmodule-2.0 		.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		dl 					.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		glib-2.0 			.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		freetype 			.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		fontconfig 			.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		Xrender 				.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		X11 					.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		Xext 					.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		png12 				.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		z 						.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"
Check_Library 		m 						.so 	clip-gd 			Yes 	/usr/lib$C64 							"lib-static-devel-xx-$ARCH.(rpm/deb)"


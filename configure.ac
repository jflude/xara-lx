AC_INIT([XaraLX], [0.7])
AC_CONFIG_SRCDIR([wxOil/camelot.cpp])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_DEFUN([AC_DATAROOTDIR_CHECKED])

AC_PREFIX_PROGRAM(pkg-config)
PKG_PROG_PKG_CONFIG([0.10.1])

AM_OPTIONS_WXCONFIG
reqwx=2.4.0


# BinReloc sets CFLAGS and CXXFLAGS, so we have to undo this
inCFLAGS=$CFLAGS
inCXXFLAGS=$CXXFLAGS

AM_BINRELOC

CFLAGS=$inCFLAGS
CXXFLAGS=$inCXXFLAGS

# libpng
PKG_CHECK_MODULES([libpng], [libpng16])
dnl AM_CFLAGS += $(libpng_CFLAGS)
dnl LDADD += $(libpng_LIBS)

# Process command line arguments
AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug],
                             [Enable debug information and checks]),
              DebugEnable="$enableval", DebugEnable="no")

AC_ARG_ENABLE(static-exec,
              AS_HELP_STRING([--enable-static-exec],
                             [Enable build of a static executable]),
              StaticEnable="$enableval", StaticEnable="no")

AC_ARG_ENABLE(international,
              AS_HELP_STRING([--disable-international],
                             [Disable building of international resources]),
              InternationalEnable="$enableval", InternationalEnable="yes")

# AC_ARG_ENABLE(svnversion,
#              AS_HELP_STRING([--disable-svnversion],
#                             [Disable svn versioning]),
#              SvnVersionEnable="$enableval", SvnVersionEnable="yes")

AC_ARG_ENABLE(xarlib,
              AS_HELP_STRING([--enable-xarlib],
                             [Enable build of the xarlib filter library]),
              XarLibEnable="$enableval", XarLibEnable="no")

# Note that as each filter is a separate package, if you got your code from svn you will need to
# do "autoreconf -f -i" in filters/SVGFilter (and all the others); the tarballs should be OK
AC_ARG_ENABLE(filters,
              AS_HELP_STRING([--enable-filters],
                             [Enable build of the filters]),
              FiltersEnable="$enableval", FiltersEnable="no")

OPT_FLAGS34="-fno-strict-aliasing"
WARN_FLAGS34="-Wstrict-aliasing=2"

# Sort out compile lags
if test "$DebugEnable" = "yes"; then
	DEBUG_FLAGS="-ggdb -D_DEBUG"
	OPT_FLAGS="-O0"
	WARN_FLAGS=""
else
	DEBUG_FLAGS=""
	OPT_FLAGS="-O3"
	WARN_FLAGS=""
fi

# Note mysteriously we still use --enable-debug to control build flags even when static enabled
if test "$StaticEnable" = "yes"; then
	DebugEnable="no"
fi

# Check that AUTOMAKE is new enough
AutomakeVersion=`automake --version | awk '{ if($3 ~ /automake\)/) print $4}'`
AC_MSG_CHECKING([Automake ($AutomakeVersion)])
AutomakeVersion=`echo $AutomakeVersion | $AWK 'BEGIN { FS = "."; } { printf "% d", ($1 * 1000 + $2) * 1000 + $3;}'`
if test "$AutomakeVersion" -lt 1009000; then
	AC_MSG_ERROR([Version 1.9.0 or later of AUTOMAKE is needed])
else
	AC_MSG_RESULT([Version OK])
fi

# Test for GCC and version
GccVersion=0
PrecompileEnable="yes"

AC_PROG_CXX
AC_PROG_INSTALL

AC_MSG_CHECKING([Compiler])
if test $ac_compiler_gnu = "yes"; then
	GccVersion=`$CXX -v 2>&1 | $AWK '{ if ($2 ~ /version/) print $3 }'`
	GccVersion=`echo $GccVersion | $AWK 'BEGIN { FS = "."; } { printf "% d", ($1 * 1000 + $2) * 1000 + $3;}'`

	if test -n "$GccVersion" && test "$GccVersion" -ge 3004000; then
		AC_MSG_RESULT([gcc >= 3.4.0, PreCompiled headers enabled])
		WARN_FLAGS="$WARNFLAGS $WARN_FLAGS34 -std=c++11"
		OPT_FLAGS="$OPT_FLAGS $OPT_FLAGS34"
		PrecompileEnable="yes"
	else
		AC_MSG_RESULT([gcc < 3.4.0, PreCompiled headers disabled])
		PrecompileEnable="no"
	fi
else
	AC_MSG_RESULT([non-gcc, PreCompiled headers disabled])
	PrecompileEnable="no"
fi

# Setup our build flags (if not set by user)
if test -z "$CFLAGS"; then
	CFLAGS="-Wall -fno-pie -Wno-unknown-pragmas -g -fexceptions $OPT_FLAGS $WARN_FLAGS $DEBUG_FLAGS"
else
	CFLAGS="-Wall -fno-pie -Wno-unknown-pragmas -g -fexceptions $OPT_FLAGS $WARN_FLAGS $DEBUG_FLAGS $CFLAGS"
fi
if test -z "$CXXFLAGS"; then
	CXXFLAGS="-Wall -fno-pie -Wno-unknown-pragmas -g -fexceptions $OPT_FLAGS $WARN_FLAGS $DEBUG_FLAGS"
else
	CXXFLAGS="-Wall -fno-pie -Wno-unknown-pragmas -g -fexceptions $OPT_FLAGS $WARN_FLAGS $DEBUG_FLAGS $CXXFLAGS"
fi

# Set language - this should be taken from a configure option
# Note this MUST be in ISO8859-1 form

# Stop automake directly including stuff
XRCINCLUDE=include

# Disable generation of shared libtool libraries - that's libXar. Note this has NOTHING to
# do with static-exec (the executables)
# We don't need dlopen right now and it's not supported on Darwin
# AC_LIBTOOL_DLOPEN
AC_PROG_RANLIB
AC_C_BIGENDIAN
AC_CHECK_SIZEOF(void*)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(__int64)

WXCONFIG=wx-config
AC_ARG_WITH(wx-config,
[[  --with-wx-config=FILE   Use the given path to wx-config when determining
                            wxWidgets configuration; defaults to "wx-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        WXCONFIG=$withval
    fi
])

XARALANGUAGE=EN
AC_ARG_WITH(language,
[[  --with-language=LANG    Use ISO8859-1 country code LANG for resource
                            language; defaults to "EN"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        XARALANGUAGE=$withval
    fi
])

RESOURCE_DIR_DEFINE=
AC_ARG_WITH(resource-dir,
[[  --with-resource-dir=DIR The absoulte path to root of resource directory
                            structure; defaults to using binreloc before
                            falling back to /usr/share/xaralx]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        RESOURCE_DIR_DEFINE="-DRESOURCE_DIR=\\\"$withval\\\""
    fi
])

AC_ARG_WITH([cairo],
	[AS_HELP_STRING([--with-cairo], [Render using Cairo instead of CDraw])],
	[],
	[with_cairo=no])

wxversion=0

AC_DEFUN([WXTEST],
[
	AC_REQUIRE([AC_PROG_AWK])
	AC_MSG_CHECKING([wxWidgets version])
	if wxversion=`$WXCONFIG --version`; then
		AC_MSG_RESULT([$wxversion])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([wxWidgets is required. Try --with-wx-config.])
	fi])

# Call WXTEST func
WXTEST

GTK_CFLAGS=""
GTK_LIBS=""

HAVE_WXRC="no"
WXRC=""
# Verify minimus requires
vers=`echo $wxversion | $AWK 'BEGIN { FS = "."; } { printf "% d", ($1 * 1000 + $2) * 1000 + $3;}'`
if test -n "$vers" && test "$vers" -ge 2006003; then
   	WX_CPPFLAGS="`$WXCONFIG --static=$StaticEnable --debug=$DebugEnable --unicode=yes --cppflags | sed -e 's/-I/-isystem /g'`"
	WX_CXXFLAGS="`$WXCONFIG --static=$StaticEnable --debug=$DebugEnable --unicode=yes --cxxflags | sed -e 's/-I/-isystem /g' | sed -e 's/-fno-exceptions//'`"
	WX_LIBS="`$WXCONFIG --static=$StaticEnable --debug=$DebugEnable --unicode=yes --libs aui std`"
	WX_LIBS2="`echo $WX_LIBS | sed -e 's/-pthread//g'`"

	# See if we are running on GTK
	WX_GTK="no"
	AC_MSG_CHECKING([wxWidgets GTK usage])
	if echo $WX_CPPFLAGS | grep -q '__WXGTK__' ; then
		WX_GTK="yes"
		AC_MSG_RESULT([found])
		PKG_CHECK_MODULES(GTK, [gtk+-3.0])
	else
		AC_MSG_RESULT([not found])
	fi

	if test "$InternationalEnable" = "yes"; then
		AC_MSG_CHECKING([wxWidgets wxrc utility])
		WXRC="`$WXCONFIG --utility=wxrc`"

		if test "$WXRC" = "" ; then
			AC_MSG_RESULT([not found, disabling international build])
			HAVE_WXRC="no"
			InternationalEnable="no"
		else
			HAVE_WXRC="yes"
			AC_MSG_RESULT([found])
		fi
	fi

else
	AC_MSG_ERROR([wxWidgets 2.6.3 or newer is required])
fi

# Test for libxml2
XML2CONFIG=xml2-config
AC_ARG_WITH(xml2-config,
[[  --with-xml2-config=FILE Use the given path to xml2-config when determining
                            libxml2 configuration; defaults to "xml2-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        XML2CONFIG=$withval
    fi
])

xml2version=0

AC_DEFUN([XML2TEST],
[
	AC_REQUIRE([AC_PROG_AWK])
	AC_MSG_CHECKING([libxml2 version])
	if xml2version=`$XML2CONFIG --version`; then
		AC_MSG_RESULT([$xml2version])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libxml2 is required. Try --with-xml2-config.])
	fi
])

# Call XML2TEST func
XML2TEST

# check libxml2 version
xml2version=`echo $xml2version | $AWK 'BEGIN { FS = "."; } { printf "% d", ($1 * 1000 + $2) * 1000 + $3;}'`
if test -z "$xml2version" || test "$xml2version" -lt 2006000; then
	AC_MSG_ERROR([libxml2 2.6 or newer is required])
fi

LIBXML2_CFLAGS="`$XML2CONFIG --cflags`"
LIBXML2_LIBS="`$XML2CONFIG --libs`"

# Define FTCONFIG
FTCONFIG=freetype-config
AC_ARG_WITH(freetype-config,
[[  --with-freetype-config=FILE   Use the given path to freetype-config when determining
                                  freetype configuration; defaults to "freetype-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        FTCONFIG=$withval
    fi
])

ftversion=0

# In wxGTK builds we need Pango and FreeType for font rendering
AC_DEFUN([FTTEST],
[
	AC_MSG_CHECKING([freetype version])
	if ftversion=`$FTCONFIG --version`; then
		AC_MSG_RESULT([$ftversion])
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([freetype is required. Try --with-freetype-config.])
	fi])

if test $WX_GTK = "yes"; then
	# GTK build, so call FTTEST function to test for FreeType
	FTTEST
	FT_CFLAGS="`$FTCONFIG --cflags`"

	# check for the Pango package
	PkgError="no"
	PKG_CHECK_MODULES(PANGOX, pangox,
			  [
				PANGO_CFLAGS="$PANGOX_CFLAGS"
			  ],
			  [
				PkgError="yes"
			  ]
			  )
	if test PkgError = "yes"; then
		AC_MSG_ERROR([pangox library not found])
	fi
else
	# non-GTK build, i.e., MacOS, so Pango and FreeType are not required
	FT_CFLAGS=""
	PANGO_CFLAGS=""
fi

TOPDIR=$srcdir;
if test `echo $srcdir | cut -c1` != "/"; then
	TOPDIR="../$srcdir";
fi

if test "$StaticEnable" = "yes"; then
	CPPFLAGS="$CPPFLAGS -static -static-libgcc"
	CXXFLAGS="$CXXFLAGS -static -static-libgcc"
	if test "$PrecompileEnable" = "yes"; then
		CPPFLAGS="$CPPFLAGS -ffunction-sections -fdata-sections"
		CXXFLAGS="$CXXFLAGS -ffunction-sections -fdata-sections"
		WX_LIBS="$WX_LIBS"
	fi

	# Force as much as possible to be statically linked
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lXrandr/-Wl,-Bstatic -lXrandr -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lXext/-Wl,-Bstatic -lXext -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lXinerama/-Wl,-Bstatic -lXinerama -Wl,-Bdynamic/g'`"
	# WX_LIBS="`echo $WX_LIBS | sed -e 's/-lXxf86vm/-Wl,-Bstatic -lXxf86vm -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lXrender/-Wl,-Bstatic -lXrender -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lXfixes/-Wl,-Bstatic -lXfixes -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lexpat/-Wl,-Bstatic -lexpat -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-ljpeg/-Wl,-Bstatic -ljpeg -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lpng/-Wl,-Bstatic -lpng -Wl,-Bdynamic/g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-ltiff/-Wl,-Bstatic -ltiff -Wl,-Bdynamic/g'`"
	# These are handled by explicitly linking xml2 (see below)
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lxml2//g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lz//g'`"
	WX_LIBS="`echo $WX_LIBS | sed -e 's/-lm//g'`"
	WX_LIBS2="`echo $WX_LIBS | sed -e 's/-pthread//g'`"
	# libxml2 linking needs fix-up too
	LIBXML2_LIBS="`echo $LIBXML2_LIBS | sed -e 's/-lxml2/-Wl,-Bstatic -lxml2 -Wl,-Bdynamic/g'`"
	LIBXML2_LIBS="`echo $LIBXML2_LIBS | sed -e 's/-lz/-Wl,-Bstatic -lz -Wl,-Bdynamic/g'`"
	LIBXML2_LIBS="`echo $LIBXML2_LIBS | sed -e 's/-lm/-Wl,-Bstatic -lm -Wl,-Bdynamic/g'`"
fi

# pass the Pango, FreeType and GTK flags (required for wxGTK font rendering)
CPPFLAGS="$CPPFLAGS  $WX_CPPFLAGS $PANGO_CFLAGS $FT_CFLAGS $GTK_CFLAGS $LIBXML2_CFLAGS $RESOURCE_DIR_DEFINE"
CXXFLAGS="$CXXFLAGS  $WX_CPPFLAGS $PANGO_CFLAGS $FT_CFLAGS $GTK_CFLAGS $LIBXML2_CFLAGS $RESOURCE_DIR_DEFINE"

AC_MSG_CHECKING([Linker])
case $host in
    *-*-darwin*)
		AC_MSG_RESULT([Darwin ld])
        DarwinLink="yes";;
    *)
		AC_MSG_RESULT([GNU ld])
        DarwinLink="no";;
esac

CDRAW_LIB_DIR=""
AC_MSG_CHECKING([CDraw location])
case $target in
    *-*-darwin*)
	AC_MSG_RESULT([libs/darwin])
	CDRAW_LIB_DIR="libs/darwin";;
    i?86-*)
	AC_MSG_RESULT([libs/x86])
	CDRAW_LIB_DIR="libs/x86";;
    amd64-* | x86_64-*)
	AC_MSG_RESULT([libpng-1.6.29/.libs])
	PNG_LIB_DIR="libpng-1.6.29/.libs"
	AC_MSG_RESULT([libs/x86_64])
	CDRAW_LIB_DIR="libs/x86_64";;
    powerpc-*)
        AC_MSG_RESULT([libs/ppc])
        CDRAW_LIB_DIR="libs/ppc";;
    *)
	AC_MSG_ERROR([Architecture $target not supported])
esac
CDRAW_LIB_ALT_DIR="libs/x86_64/cdraw-alt"
PODIR=""
BUILDRESFLAGS=""
if test "$InternationalEnable" = "yes" ; then
# These two macros must be left aligned in order for autoreconf to
# find them.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.14.3])
	PODIR=po
	BUILDRESFLAGS="-i $BUILDRESFLAGS"
fi

#if test "$SvnVersionEnable" = "yes" ; then
#	BUILDRESFLAGS="-s $BUILDRESFLAGS"
#fi

# Force XarLibEnable on if we are to build the filters
if test "$FiltersEnable" = "yes"; then
	XarLibEnable="yes"
	# ensure the filters have a local XarLib
	export LOCAL_XARLIB_OVERRIDE=yes
	# Add each filter below here
	AC_CONFIG_SUBDIRS([filters/SVGFilter])
fi

# Do not pass CXXFLAGS, CFLAGS in. This sets up CXXLINK which breaks XARLIB compilation. Instead set
XARALXCXXFLAGS="$CXXFLAGS"
XARALXCPPFLAGS="$CPPFLAGS"
CXXFLAGS=""
CPPFLAGS=""

if test "x$with_cairo" = xno; then
	CDRAW_DIR_LDFLAGS="-L$srcdir/$CDRAW_LIB_DIR -L$srcdir/$CDRAW_LIB_ALT_DIR"
	CDRAW_LIB_LDFLAGS="-lCDraw -lCDraw-alt"
	AC_DEFINE([HAVE_CAIRO], [0])
else
	CDRAW_DIR_LDFLAGS=
	CDRAW_LIB_LDFLAGS=
	AC_DEFINE([HAVE_CAIRO], [1])
fi

AM_CONDITIONAL(PRECOMPILE_ENABLE, [test "$PrecompileEnable" = "yes"])
AM_CONDITIONAL(STATIC_ENABLE, [test "$StaticEnable" = "yes"])
AM_CONDITIONAL(DARWIN_LINK, [test "$DarwinLink" = "yes"])
AM_CONDITIONAL(CAIRO_LINK, [test "$with_cairo" = "yes"])
AM_CONDITIONAL(INTERNATIONAL_ENABLE, [test "$InternationalEnable" = "yes"])
#AM_CONDITIONAL(SVNVERSION_ENABLE, [test "$SvnVersionEnable" = "yes"])
AM_CONDITIONAL(XARLIB_ENABLE, [test "$XarLibEnable" = "yes"])
AM_CONDITIONAL(FILTERS_ENABLE, [test "$FiltersEnable" = "yes"])

AC_SUBST(XARALXCXXFLAGS)
AC_SUBST(XARALXCPPFLAGS)
AC_SUBST(TOPDIR)
AC_SUBST(WX_LIBS)
AC_SUBST(WX_LIBS2)
AC_SUBST(XRCINCLUDE)
AC_SUBST(XARALANGUAGE)
AC_SUBST(WXRC)
AC_SUBST(VERSION)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
AC_SUBST(PNG_LIB_DIR)
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(PODIR)
AC_SUBST(BUILDRESFLAGS)
AC_SUBST(CDRAW_DIR_LDFLAGS)
AC_SUBST(CDRAW_LIB_LDFLAGS)

AC_OUTPUT(Makefile PreComp/Makefile Kernel/Makefile wxOil/Makefile tools/Makefile GDraw/Makefile wxXtra/Makefile xarlib/Makefile libs/Makefile libs/x86_64/Makefile libs/x86_64/cdraw-alt/Makefile xarlib/Xar.pc po/Makefile.in)

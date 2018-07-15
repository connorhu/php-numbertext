PHP_ARG_ENABLE(numbertext,
    [Whether to enable the "numbertext" extension],
    [  --enable-numbertext      Enable "numbertext" extension support])

if test $PHP_NUMBERTEXT != "no"; then

    PHP_SUBST(NUMBERTEXT_SHARED_LIBADD)

    LIBNUMBERTEXT_DIR=/usr
    LIBNAME=libnumbertext
    LIBSYMBOL=libnumbertext-1.0
    LIB_LINK_ARGS=$(pkg-config --libs libnumbertext)

    PHP_ADD_INCLUDE($LIBNUMBERTEXT_DIR/include)
    PHP_EVAL_LIBLINE($LIB_LINK_ARGS, NUMBERTEXT_SHARED_LIBADD)

    PHP_REQUIRE_CXX()
    PHP_SUBST(NUMBERTEXT_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, NUMBERTEXT_SHARED_LIBADD)
    PHP_NEW_EXTENSION(numbertext, numbertext.cc, $ext_shared)
fi
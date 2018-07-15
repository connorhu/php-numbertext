#ifndef PHP_NUMBERTEXT_H
#define PHP_NUMBERTEXT_H

#define PHP_NUMBERTEXT_EXTNAME  "numbertext"
#define PHP_NUMBERTEXT_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 

extern "C" {
#include "php.h"
}

extern zend_module_entry numbertext_module_entry;
#define phpext_numbertext_ptr &numbertext_module_entry;

#endif /* PHP_NUMBERTEXT_H */
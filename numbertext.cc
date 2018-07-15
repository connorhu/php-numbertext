#include "php_numbertext.h"
#include "libnumbertext/Numbertext.hxx"
#include <cstring>

zend_class_entry *numbertext_ce;

zend_object_handlers numbertext_object_handlers;
 
struct numbertext_object {
    zend_object std;
    char *language;
    int language_length;
    Numbertext *numbertext;
};

void numbertext_free_storage(void *object TSRMLS_DC)
{
    numbertext_object *obj = (numbertext_object *)object;
    delete obj->numbertext; 

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value numbertext_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    numbertext_object *obj = (numbertext_object *)emalloc(sizeof(numbertext_object));
    memset(obj, 0, sizeof(numbertext_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    // zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    object_properties_init((zend_object*) &(obj->std), type);

    retval.handle = zend_objects_store_put(obj, NULL, numbertext_free_storage, NULL TSRMLS_CC);
    retval.handlers = &numbertext_object_handlers;

    return retval;
}

PHP_METHOD(Numbertext, __construct)
{
    Numbertext *numbertext = NULL;
    zval *object = getThis();

    numbertext = new Numbertext();
    numbertext_object *obj = (numbertext_object *)zend_object_store_get_object(object TSRMLS_CC);
    obj->numbertext = numbertext;
}

PHP_METHOD(Numbertext, load)
{
    Numbertext *numbertext;
    numbertext_object *obj = (numbertext_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    numbertext = obj->numbertext;

    if (numbertext == NULL) {
        return;
    }

    char *path;
    int path_len;

    char *language;
    int language_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &path, &path_len, &language, &language_len) == FAILURE) {
        RETURN_NULL();
    }
    
    if (path == NULL) {
        RETURN_NULL();
    }

    if (language == NULL) {
        RETURN_NULL();
    }
    
    std::string pS(path);
    std::string lS(language);
    
    if (pS.substr(pS.length()-1, 1) != "/") {
        pS.append("/");
    }
    
    numbertext->set_prefix(pS);
    if (!numbertext->load(lS)) {
        RETURN_BOOL(false);
    }
    
    obj->language = language;
    obj->language_length = language_len;
    
    RETURN_BOOL(true);
}

PHP_METHOD(Numbertext, run)
{
    Numbertext *numbertext;
    numbertext_object *obj = (numbertext_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    numbertext = obj->numbertext;
    
    if (numbertext == NULL) {
        return;
    }
    
    char *prefix;
    int prefix_len;
    
    char *number;
    int number_len;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &prefix, &prefix_len, &number, &number_len) == FAILURE) {
        RETURN_NULL();
    }
    
    if (prefix == NULL) {
        RETURN_NULL();
    }

    if (number == NULL) {
        RETURN_NULL();
    }
    
    std::string pS(prefix);
    std::string nS(number);
    std::string lS(obj->language);
    std::string result = pS +" "+ nS;
    
    if (!numbertext->numbertext(result, lS)) {
        RETURN_NULL();
    }
    
    RETURN_STRING(result.c_str(), 1);
}

zend_function_entry numbertext_methods[] = {
    PHP_ME(Numbertext,  __construct,    NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Numbertext,  load,           NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Numbertext,  run,            NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(numbertext)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Numbertext", numbertext_methods);
    numbertext_ce = zend_register_internal_class(&ce TSRMLS_CC);
    numbertext_ce->create_object = numbertext_create_handler;
    memcpy(&numbertext_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    numbertext_object_handlers.clone_obj = NULL;
    
    return SUCCESS;
}

zend_module_entry numbertext_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_NUMBERTEXT_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(numbertext),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_NUMBERTEXT_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_NUMBERTEXT
extern "C" {
ZEND_GET_MODULE(numbertext)
}
#endif


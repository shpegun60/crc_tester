#ifndef __TEMPLATES_C_LANG_H_
#define __TEMPLATES_C_LANG_H_

#include "preprocessor_base.h"

#define TEMPLATE(X, ...) MAP_ALL_TO_ONE(X,UNDERSCORE(),__VA_ARGS__)


#endif /*__TEMPLATES_C_LANG_H_*/

/*
 * Copyright (C) 2015 Andrea Zagli <azagli@libero.it>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _ZAK_FORM_COMMONS_H_
#define _ZAK_FORM_COMMONS_H_

#include <glib-object.h>


#include "formelement.h"
#include "formelementextension.h"
#include "formvalidator.h"


G_BEGIN_DECLS

typedef enum
	{
		ZAK_FORM_COMPARE_TYPE_NONE,
		ZAK_FORM_COMPARE_TYPE_LESSER,
		ZAK_FORM_COMPARE_TYPE_LESSER_EQUAL,
		ZAK_FORM_COMPARE_TYPE_EQUAL,
		ZAK_FORM_COMPARE_TYPE_NOT_EQUAL,
		ZAK_FORM_COMPARE_TYPE_GREATER,
		ZAK_FORM_COMPARE_TYPE_GREATER_EQUAL
	} ZakFormCompareType;

ZakFormCompareType zak_form_get_compare_type_from_string (const gchar *str);
const gchar *zak_form_get_compare_type_stringify (ZakFormCompareType type);

ZakFormElement *zak_form_get_element_by_id (GPtrArray *ar_elements, const gchar *id);


void zak_form_load_modules (void);

typedef ZakFormElement *(* ZakFormElementConstructorFunc) (void);
ZakFormElementConstructorFunc zak_form_get_form_element (const gchar *namespace);

typedef ZakFormElementFilter *(* ZakFormElementFilterConstructorFunc) (void);
ZakFormElementFilterConstructorFunc zak_form_get_form_element_filter (const gchar *namespace);

typedef ZakFormElementValidator *(* ZakFormElementValidatorConstructorFunc) (void);
ZakFormElementValidatorConstructorFunc zak_form_get_form_element_validator (const gchar *namespace);

typedef ZakFormElementExtension *(* ZakFormElementExtensionConstructorFunc) (void);
ZakFormElementExtensionConstructorFunc zak_form_get_form_element_extension (const gchar *namespace);

typedef ZakFormValidator *(* ZakFormValidatorConstructorFunc) (void);
ZakFormValidatorConstructorFunc zak_form_get_form_validator (const gchar *namespace);


G_END_DECLS


#endif /* _ZAK_FORM_COMMONS_H_ */

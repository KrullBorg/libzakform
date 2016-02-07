/*
 * Copyright (C) 2016 Andrea Zagli <azagli@libero.it>
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

#ifndef __ZAK_FORM_VALIDATOR_COMPARE_H__
#define __ZAK_FORM_VALIDATOR_COMPARE_H__


#include <glib-object.h>

#include <libxml/tree.h>

#include "formvalidator.h"


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_VALIDATOR_COMPARE zak_form_validator_compare_get_type ()
G_DECLARE_FINAL_TYPE (ZakFormValidatorCompare, zak_form_validator_compare, ZAK_FORM, VALIDATOR_COMPARE, ZakFormValidator)

ZakFormValidatorCompare *zak_form_validator_compare_new (void);
gboolean zak_form_validator_compare_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, GPtrArray *ar_elements);


G_END_DECLS


#endif /* __ZAK_FORM_VALIDATOR_COMPARE_H__ */

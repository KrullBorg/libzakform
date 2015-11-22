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

#ifndef __ZAK_FORM_ELEMENT_H__
#define __ZAK_FORM_ELEMENT_H__


#include <glib-object.h>

#include "formelementfilter.h"
#include "formelementvalidator.h"


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_ELEMENT zak_form_element_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormElement, zak_form_element, ZAK_FORM, ELEMENT, GObject)

struct _ZakFormElementClass
	{
		GObjectClass parent_class;

		GPtrArray *(*get_messages) (ZakFormElement *element);
	};


void zak_form_element_set_value (ZakFormElement *element, const gchar *value);
gchar *zak_form_element_get_value (ZakFormElement *element);

void zak_form_element_set_default_value (ZakFormElement *element, const gchar *value);
gchar *zak_form_element_get_default_value (ZakFormElement *element);

void zak_form_element_set_visible (ZakFormElement *element, gboolean visible);
gboolean zak_form_element_get_visible (ZakFormElement *element);

void zak_form_element_clear (ZakFormElement *element);

void zak_form_element_add_filter (ZakFormElement *element, ZakFormElementFilter *filter);
void zak_form_element_filter (ZakFormElement *element);

void zak_form_element_add_validator (ZakFormElement *element, ZakFormElementValidator *validator);
gboolean zak_form_element_is_valid (ZakFormElement *element);


G_END_DECLS


#endif /* __ZAK_FORM_ELEMENT_H__ */

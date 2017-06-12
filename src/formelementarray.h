/*
 * Copyright (C) 2017 Andrea Zagli <azagli@libero.it>
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

#ifndef __ZAK_FORM_ELEMENT_ARRAY_H__
#define __ZAK_FORM_ELEMENT_ARRAY_H__


#include <glib-object.h>

#include "formelement.h"


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_ELEMENT_ARRAY                 (zak_form_element_array_get_type ())
#define ZAK_FORM_ELEMENT_ARRAY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), ZAK_FORM_TYPE_ELEMENT_ARRAY, ZakFormElementArray))
#define ZAK_FORM_ELEMENT_ARRAY_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), ZAK_FORM_TYPE_ELEMENT_ARRAY, ZakFormElementArrayClass))
#define ZAK_FORM_IS_ELEMENT_ARRAY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ZAK_FORM_TYPE_ELEMENT_ARRAY))
#define ZAK_FORM_IS_ELEMENT_ARRAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), ZAK_FORM_TYPE_ELEMENT_ARRAY))
#define ZAK_FORM_ELEMENT_ARRAY_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), ZAK_FORM_TYPE_ELEMENT_ARRAY, ZakFormElementArrayClass))

typedef struct _ZakFormElementArray ZakFormElementArray;
typedef struct _ZakFormElementArrayClass ZakFormElementArrayClass;

struct _ZakFormElementArray
	{
		ZakFormElement parent_instance;
	};

struct _ZakFormElementArrayClass
	{
		ZakFormElementClass parent_class;

		guint element_added_signal_id;
	};


GType zak_form_element_array_get_type (void);

ZakFormElement *zak_form_element_array_new (void);

void zak_form_element_array_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode);

void zak_form_element_array_add_element (ZakFormElement *element, ZakFormElement *element_to_add);

GPtrArray *zak_form_element_array_get_elements (ZakFormElement *element);


G_END_DECLS


#endif /* __ZAK_FORM_ELEMENT_ARRAY_H__ */

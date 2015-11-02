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

#ifndef __ZAK_FORM_ELEMENT_FILTER_H__
#define __ZAK_FORM_ELEMENT_FILTER_H__

#include <glib-object.h>


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_ELEMENT_FILTER zak_form_element_filter_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormElementFilter, zak_form_element_filter, ZAK_FORM, ELEMENT_FILTER, GObject)

struct _ZakFormElementFilterClass
{
	GObjectClass parent_cleass;

	gchar *(*filter) (ZakFormElementFilter *self, const gchar *value);
};

gchar *zak_form_element_filter_filter (ZakFormElementFilter *self, const gchar *value);


G_END_DECLS


#endif /* __ZAK_FORM_ELEMENT_FILTER_H__ */

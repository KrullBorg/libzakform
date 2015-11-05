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

#ifndef __ZAK_FORM_FORM_H__
#define __ZAK_FORM_FORM_H__


#include <glib-object.h>

#include <libxml/tree.h>

#include "formelement.h"


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_FORM zak_form_form_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormForm, zak_form_form, ZAK_FORM, FORM, GObject)

struct _ZakFormFormClass
{
	GObjectClass parent_class;

	GPtrArray *(*get_elements) (ZakFormForm *zakform);
};


gboolean zak_form_form_load_from_xml (ZakFormForm *zakform, xmlDoc *xmldoc);
gboolean zak_form_form_load_from_file (ZakFormForm *zakform, const gchar *filename);

gboolean zak_form_form_add_element (ZakFormForm *zakform, ZakFormElement *element);

void zak_form_form_clear (ZakFormForm *zakform);

gboolean zak_form_form_is_valid (ZakFormForm *zakform);


G_END_DECLS


#endif /* __ZAK_FORM_FORM_H__ */
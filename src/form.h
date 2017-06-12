/*
 * Copyright (C) 2015-2017 Andrea Zagli <azagli@libero.it>
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
#include "formiprovider.h"
#include "formvalidator.h"


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_FORM zak_form_form_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormForm, zak_form_form, ZAK_FORM, FORM, GObject)

struct _ZakFormFormClass
{
	GObjectClass parent_class;

	GPtrArray *(*get_elements) (ZakFormForm *zakform);

	void (*element_added) (ZakFormForm *zakform, ZakFormElement *element);
};

typedef ZakFormElement *(* ZakFormElementConstructorFunc) (void);
ZakFormElementConstructorFunc zak_form_form_get_form_element (ZakFormForm *zakform, const gchar *namespace);

typedef ZakFormElementFilter *(* ZakFormElementFilterConstructorFunc) (void);
ZakFormElementFilterConstructorFunc zak_form_form_get_form_element_filter (ZakFormForm *zakform, const gchar *namespace);

typedef ZakFormElementValidator *(* ZakFormElementValidatorConstructorFunc) (void);
ZakFormElementValidatorConstructorFunc zak_form_form_get_form_element_validator (ZakFormForm *zakform, const gchar *namespace);

gboolean zak_form_form_load_from_xml (ZakFormForm *zakform, xmlDoc *xmldoc);
gboolean zak_form_form_load_from_file (ZakFormForm *zakform, const gchar *filename);

gboolean zak_form_form_add_element (ZakFormForm *zakform, ZakFormElement *element);
ZakFormElement *zak_form_form_get_element_by_id (ZakFormForm *zakform, const gchar *id);
GPtrArray *zak_form_form_get_elements (ZakFormForm *zakform);
GPtrArray *zak_form_form_get_elements_by_type (ZakFormForm *zakform, GType type);

gboolean zak_form_form_add_validator (ZakFormForm *zakform, ZakFormValidator *validator);
ZakFormValidator *zak_form_form_get_validator_by_id (ZakFormForm *zakform, const gchar *id);
GPtrArray *zak_form_form_get_validators (ZakFormForm *zakform);
GPtrArray *zak_form_form_get_validators_by_type (ZakFormForm *zakform, GType type);

void zak_form_form_clear (ZakFormForm *zakform);

void zak_form_form_set_as_original (ZakFormForm *zakform);

gboolean zak_form_form_is_valid (ZakFormForm *zakform);
GPtrArray *zak_form_form_get_messages (ZakFormForm *zakform);

gboolean zak_form_form_is_changed (ZakFormForm *zakform);

void zak_form_form_set_editable (ZakFormForm *zakform, gboolean editable);

gboolean zak_form_form_load (ZakFormForm *zakform, ZakFormIProvider *provider);
gboolean zak_form_form_insert (ZakFormForm *zakform, ZakFormIProvider *provider);
gboolean zak_form_form_update (ZakFormForm *zakform, ZakFormIProvider *provider);
gboolean zak_form_form_delete (ZakFormForm *zakform, ZakFormIProvider *provider);


G_END_DECLS


#endif /* __ZAK_FORM_FORM_H__ */

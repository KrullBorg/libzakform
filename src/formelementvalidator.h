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

#ifndef __ZAK_FORM_ELEMENT_VALIDATOR_H__
#define __ZAK_FORM_ELEMENT_VALIDATOR_H__

#include <glib-object.h>

#include <libxml/xpath.h>


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_ELEMENT_VALIDATOR zak_form_element_validator_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormElementValidator, zak_form_element_validator, ZAK_FORM, ELEMENT_VALIDATOR, GObject)

struct _ZakFormElementValidatorClass
{
	GObjectClass parent_class;

	gboolean (*xml_parsing) (ZakFormElementValidator *self, xmlNode *xnode);
	gboolean (*validate) (ZakFormElementValidator *self, const gchar *value);
};

gboolean zak_form_element_validator_xml_parsing (ZakFormElementValidator *self, xmlNode *xnode);
gboolean zak_form_element_validator_validate (ZakFormElementValidator *self, const gchar *value);

void zak_form_element_validator_set_id (ZakFormElementValidator *validator,
                                        const gchar *id);
gchar *zak_form_element_validator_get_id (ZakFormElementValidator *validator);

void zak_form_element_validator_set_message (ZakFormElementValidator *validator,
                                             const gchar *message);
gchar *zak_form_element_validator_get_message (ZakFormElementValidator *validator);

gboolean zak_form_element_validator_get_enabled (ZakFormElementValidator *validator);
void zak_form_element_validator_set_enabled (ZakFormElementValidator *validator, gboolean enabled);


G_END_DECLS


#endif /* __ZAK_FORM_ELEMENT_VALIDATOR_H__ */

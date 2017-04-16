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

#ifndef _ZAK_FORM_VALIDATOR_H_
#define _ZAK_FORM_VALIDATOR_H_

#include <glib-object.h>

#include <libxml/xpath.h>


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_VALIDATOR zak_form_validator_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormValidator, zak_form_validator, ZAK_FORM, VALIDATOR, GObject)

struct _ZakFormValidatorClass
{
	GObjectClass parent_class;

	gboolean (*xml_parsing) (ZakFormValidator *self, xmlNode *xnode, GPtrArray *ar_elements);
	gboolean (*validate) (ZakFormValidator *self);
};

gboolean zak_form_validator_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, GPtrArray *ar_elements);

gchar *zak_form_validator_get_id (ZakFormValidator *validator);
void zak_form_validator_set_id (ZakFormValidator *validator, const gchar *id);

gboolean zak_form_validator_validate (ZakFormValidator *self);

void zak_form_validator_set_message (ZakFormValidator *validator,
                                     const gchar *message);
gchar *zak_form_validator_get_message (ZakFormValidator *validator);

gboolean zak_form_validator_get_enabled (ZakFormValidator *validator);
void zak_form_validator_set_enabled (ZakFormValidator *validator, gboolean enabled);

G_END_DECLS


#endif /* _ZAK_FORM_VALIDATOR_H_ */

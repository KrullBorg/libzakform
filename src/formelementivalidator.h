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

#ifndef __ZAK_FORM_ELEMENT_IVALIDATOR_H__
#define __ZAK_FORM_ELEMENT_IVALIDATOR_H__

#include <glib-object.h>


G_BEGIN_DECLS


#define ZAK_TYPE_FORM_ELEMENT_IVALIDATOR zak_form_element_ivalidator_get_type ()
G_DECLARE_INTERFACE (ZakFormElementIValidator, zak_form_element_ivalidator, ZAK_FORM, ELEMENT_IVALIDATOR, GObject)

struct _ZakFormElementIValidatorInterface
{
	GTypeInterface parent_iface;

	gboolean (*validate) (ZakFormElementIValidator *self, GValue *value);
};

gboolean zak_form_element_ivalidator_validate (ZakFormElementIValidator *self, GValue *value);


G_END_DECLS


#endif /* __ZAK_FORM_ELEMENT_IVALIDATOR_H__ */
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

#include "formelementifilter.h"

G_DEFINE_INTERFACE (ZakFormElementIFilter, zak_form_element_ifilter, G_TYPE_OBJECT);

static void
zak_form_element_ifilter_default_init (ZakFormElementIFilterInterface *iface)
{
    /* add properties and signals to the interface here */
}

GValue
*zak_form_element_ifilter_filter (ZakFormElementIFilter *self, GValue *value)
{
	g_return_val_if_fail (ZAK_FORM_IS_ELEMENT_IFILTER (self), NULL);

	return ZAK_FORM_ELEMENT_IFILTER_GET_IFACE (self)->filter (self, value);
}

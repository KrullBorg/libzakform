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

#ifndef __ZAK_FORM_IPROVIDER_H__
#define __ZAK_FORM_IPROVIDER_H__

#include <glib-object.h>


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_IPROVIDER zak_form_iprovider_get_type ()
G_DECLARE_INTERFACE (ZakFormIProvider, zak_form_iprovider, ZAK_FORM, IPROVIDER, GObject)

struct _ZakFormIProviderInterface
{
	GTypeInterface parent_iface;

	gboolean (*load) (ZakFormIProvider *self, GPtrArray *elements);
	gboolean (*insert) (ZakFormIProvider *self, GPtrArray *elements);
	gboolean (*update) (ZakFormIProvider *self, GPtrArray *elements);
	gboolean (*delete) (ZakFormIProvider *self, GPtrArray *elements);
};

gboolean zak_form_iprovider_load (ZakFormIProvider *self, GPtrArray *elements);
gboolean zak_form_iprovider_insert (ZakFormIProvider *self, GPtrArray *elements);
gboolean zak_form_iprovider_update (ZakFormIProvider *self, GPtrArray *elements);
gboolean zak_form_iprovider_delete (ZakFormIProvider *self, GPtrArray *elements);


G_END_DECLS


#endif /* __ZAK_FORM_IPROVIDER_H__ */

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

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "formiprovider.h"

G_DEFINE_INTERFACE (ZakFormIProvider, zak_form_iprovider, G_TYPE_OBJECT);

static void
zak_form_iprovider_default_init (ZakFormIProviderInterface *iface)
{

}

gboolean
zak_form_iprovider_load (ZakFormIProvider *self, GPtrArray *elements)
{
	gboolean ret;

	g_return_val_if_fail (ZAK_FORM_IS_IPROVIDER (self), FALSE);

	ret = FALSE;
	if (ZAK_FORM_IPROVIDER_GET_IFACE (self)->load != NULL)
		{
			ZAK_FORM_IPROVIDER_GET_IFACE (self)->load (self, elements);
		}

	return ret;
}

gboolean
zak_form_iprovider_insert (ZakFormIProvider *self, GPtrArray *elements)
{
	gboolean ret;

	g_return_val_if_fail (ZAK_FORM_IS_IPROVIDER (self), FALSE);

	ret = FALSE;
	if (ZAK_FORM_IPROVIDER_GET_IFACE (self)->insert != NULL)
		{
			ZAK_FORM_IPROVIDER_GET_IFACE (self)->insert (self, elements);
		}

	return ret;
}

gboolean
zak_form_iprovider_update (ZakFormIProvider *self, GPtrArray *elements)
{
	gboolean ret;

	g_return_val_if_fail (ZAK_FORM_IS_IPROVIDER (self), FALSE);

	ret = FALSE;
	if (ZAK_FORM_IPROVIDER_GET_IFACE (self)->update != NULL)
		{
			ZAK_FORM_IPROVIDER_GET_IFACE (self)->update (self, elements);
		}

	return ret;
}

gboolean
zak_form_iprovider_delete (ZakFormIProvider *self, GPtrArray *elements)
{
	gboolean ret;

	g_return_val_if_fail (ZAK_FORM_IS_IPROVIDER (self), FALSE);

	ret = FALSE;
	if (ZAK_FORM_IPROVIDER_GET_IFACE (self)->delete != NULL)
		{
			ZAK_FORM_IPROVIDER_GET_IFACE (self)->delete (self, elements);
		}

	return ret;
}

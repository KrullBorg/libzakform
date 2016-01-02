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

#include <glib/gi18n-lib.h>

#ifdef G_OS_WIN32
	#include <windows.h>
#endif

#include "commons.h"

/**
 * zak_form_get_element_by_id:
 * @ar_elements:
 * @id:
 *
 * Returns: the #ZakFormElement with @id.
 */
ZakFormElement
*zak_form_get_element_by_id (GPtrArray *ar_elements, const gchar *id)
{
	guint i;

	ZakFormElement *ret;

	ret = NULL;
	for (i = 0; i < ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (ar_elements, i);

			if (g_strcmp0 (zak_form_element_get_name (element), id) == 0)
				{
					ret = element;
					break;
				}
		}

	return ret;
}

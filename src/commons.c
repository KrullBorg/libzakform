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
 * zak_form_get_compare_type_from_string:
 * @str:
 *
 * Returns: the #ZakFormCompareType from @str.
 */
ZakFormCompareType
zak_form_get_compare_type_from_string (const gchar *str)
{
	ZakFormCompareType ret;

	if (g_strcmp0 (str, "lt") == 0)
		{
			ret = ZAK_FORM_COMPARE_TYPE_LESSER;
		}
	else if (g_strcmp0 (str, "let") == 0)
		{
			ret = ZAK_FORM_COMPARE_TYPE_LESSER_EQUAL;
		}
	else if (g_strcmp0 (str, "e") == 0)
		{
			ret = ZAK_FORM_COMPARE_TYPE_EQUAL;
		}
	else if (g_strcmp0 (str, "ne") == 0)
		{
			ret = ZAK_FORM_COMPARE_TYPE_NOT_EQUAL;
		}
	else if (g_strcmp0 (str, "gt") == 0)
		{
			ret = ZAK_FORM_COMPARE_TYPE_GREATER;
		}
	else if (g_strcmp0 (str, "get") == 0)
		{
			ret = ZAK_FORM_COMPARE_TYPE_GREATER_EQUAL;
		}
	else
		{
			if (str != NULL
			    && g_strcmp0 (str, "") != 0)
				{
					g_warning ("Compare type «%s» not supported.", str);
				}
			ret = ZAK_FORM_COMPARE_TYPE_NONE;
		}

	return ret;
}

/**
 * zak_form_get_compare_type_stringify:
 * @type:
 *
 * Returns: the stringify version of #ZakFormCompareType.
 */
const gchar
*zak_form_get_compare_type_stringify (ZakFormCompareType type)
{
	const gchar *ret;

	switch (type)
		{
		case ZAK_FORM_COMPARE_TYPE_LESSER:
			ret = _("lesser than");
			break;

		case ZAK_FORM_COMPARE_TYPE_LESSER_EQUAL:
			ret = _("lesser or equal to");
			break;

		case ZAK_FORM_COMPARE_TYPE_EQUAL:
			ret = _("equal to");
			break;

		case ZAK_FORM_COMPARE_TYPE_NOT_EQUAL:
			ret = _("different from");
			break;

		case ZAK_FORM_COMPARE_TYPE_GREATER:
			ret = _("greater than");
			break;

		case ZAK_FORM_COMPARE_TYPE_GREATER_EQUAL:
			ret =  _("greater or equal to");
			break;

		default:
			ret = _("NOT SUPPORTED");
			break;
		}

	return ret;
}

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

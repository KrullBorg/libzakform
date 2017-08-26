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

#include <gmodule.h>

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

static GPtrArray *ar_modules;

void
zak_form_load_modules (void)
{
	gchar *modulesdir;
	GDir *dir;
	GError *error;

	GModule *module;
	const gchar *filename;

	if (g_module_supported ())
		{
			modulesdir = (gchar *)g_getenv ("LIBZAKFORM_MODULESDIR");
			if (modulesdir == NULL)
				{
#ifdef G_OS_WIN32

					gchar *moddir;
					gchar *p;

					moddir = g_win32_get_package_installation_directory_of_module (backend_dll);

					p = g_strrstr (moddir, g_strdup_printf ("%c", G_DIR_SEPARATOR));
					if (p != NULL
						&& (g_ascii_strcasecmp (p + 1, "src") == 0
							|| g_ascii_strcasecmp (p + 1, ".libs") == 0))
						{
							modulesdir = g_strdup (MODULESDIR);
						}
					else
						{
							modulesdir = g_build_filename (moddir, "lib", PACKAGE, "modules", NULL);
						}

#else

					modulesdir = g_strdup (MODULESDIR);

#endif
				}

			/* load myself as module (for filters and validators) */
			module = g_module_open (NULL, G_MODULE_BIND_LAZY);
			if (module == NULL)
				{
					g_warning (_("Unable to load module of myself"));
				}
			else
				{
					if (ar_modules == NULL)
						{
							ar_modules = g_ptr_array_new ();
						}
					g_ptr_array_add (ar_modules, (gpointer)module);
				}

			/* for each file in MODULESDIR */
			error = NULL;
			dir = g_dir_open (modulesdir, 0, &error);
			if (dir != NULL && error == NULL)
				{
					while ((filename = g_dir_read_name (dir)) != NULL)
						{
							/* trying to open the module */
							module = g_module_open (filename, G_MODULE_BIND_LAZY);
							if (module != NULL)
								{
									if (ar_modules == NULL)
										{
											ar_modules = g_ptr_array_new ();
										}
									g_ptr_array_add (ar_modules, (gpointer)module);
								}
							else
								{
									g_warning (_("Unable to load %s: %s."), filename, g_module_error ());
								}
						}

					g_dir_close (dir);
				}
			else
				{
					g_warning (_("Unable to open modules dir: %s."),
							   error != NULL && error->message != NULL ? error->message : _("no details"));
				}
		}
	else
		{
			g_warning (_("Modules not supported by this operating system."));
		}
}

/**
 * _zak_form_get_module_new:
 * @namespace:
 *
 * Returns: the constructor (_new) function for the module.
 */
static gpointer
_zak_form_get_module_new (const gchar *namespace)
{
	gpointer ret;

	guint i;

	for (i = 0; i < ar_modules->len; i++)
		{
			if (g_module_symbol ((GModule *)g_ptr_array_index (ar_modules, i),
			                     g_strconcat (namespace, "_new", NULL),
			                     (gpointer *)&ret))
				{
					break;
				}
		}

	return ret;
}

/**
 * zak_form_get_form_element:
 * @namespace:
 *
 * Returns:
 */
ZakFormElementConstructorFunc
zak_form_get_form_element (const gchar *namespace)
{
	ZakFormElementConstructorFunc element_constructor;

	element_constructor = (ZakFormElementConstructorFunc)_zak_form_get_module_new (namespace);

	return element_constructor;
}

/**
 * zak_form_get_form_element_filter:
 * @namespace:
 *
 * Returns:
 */
ZakFormElementFilterConstructorFunc
zak_form_get_form_element_filter (const gchar *namespace)
{
	ZakFormElementFilterConstructorFunc filter_constructor;

	filter_constructor = (ZakFormElementFilterConstructorFunc)_zak_form_get_module_new (namespace);

	return filter_constructor;
}

/**
 * zak_form_get_form_element_validator:
 * @namespace:
 *
 * Returns:
 */
ZakFormElementValidatorConstructorFunc
zak_form_get_form_element_validator (const gchar *namespace)
{
	ZakFormElementValidatorConstructorFunc validator_constructor;

	validator_constructor = (ZakFormElementValidatorConstructorFunc)_zak_form_get_module_new (namespace);

	return validator_constructor;
}

/**
 * zak_form_get_form_element_extension:
 * @namespace:
 *
 * Returns:
 */
ZakFormElementExtensionConstructorFunc
zak_form_get_form_element_extension (const gchar *namespace)
{
	ZakFormElementExtensionConstructorFunc extension_constructor;

	extension_constructor = (ZakFormElementExtensionConstructorFunc)_zak_form_get_module_new (namespace);

	return extension_constructor;
}

/**
 * zak_form_get_form_validator:
 * @namespace:
 *
 * Returns:
 */
ZakFormValidatorConstructorFunc
zak_form_get_form_validator (const gchar *namespace)
{
	ZakFormValidatorConstructorFunc validator_constructor;

	validator_constructor = (ZakFormValidatorConstructorFunc)_zak_form_get_module_new (namespace);

	return validator_constructor;
}

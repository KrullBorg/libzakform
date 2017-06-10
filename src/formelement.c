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

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <locale.h>

#include <libzakutils/libzakutils.h>

#include "zakformmarshal.h"
#include "formelement.h"

enum
{
	PROP_0,
	PROP_NAME,
	PROP_LONG_NAME,
	PROP_IS_KEY,
	PROP_TYPE,
	PROP_VALUE,
	PROP_DEFAULT_VALUE,
	PROP_ORIGINAL_VALUE,
	PROP_FORMAT,
	PROP_VISIBLE,
	PROP_EDITABLE,
	PROP_TO_LOAD,
	PROP_TO_SAVE
};

static void zak_form_element_class_init (ZakFormElementClass *class);
static void zak_form_element_init (ZakFormElement *zak_form_element);

static void zak_form_element_construct (ZakFormElement *element,
                                        const gchar *id,
                                        GHashTable *ht_attrs);

static void zak_form_element_set_property (GObject *object,
                                           guint property_id,
                                           const GValue *value,
                                           GParamSpec *pspec);
static void zak_form_element_get_property (GObject *object,
                                           guint property_id,
                                           GValue *value,
                                           GParamSpec *pspec);

static void zak_form_element_dispose (GObject *gobject);
static void zak_form_element_finalize (GObject *gobject);

static void zak_form_element_xml_parsing (ZakFormElement *element, xmlNode *xmlnode);

typedef struct
	{
		gchar *name;
		gchar *long_name;
		gboolean is_key;
		gchar *type;
		GValue *value;
		GValue *default_value;
		GValue *original_value;
		GHashTable *format;
		gboolean visible;
		gboolean editable;
		gboolean to_load;
		gboolean to_save;

		GPtrArray *pa_extensions;
		GPtrArray *pa_filters;
		GPtrArray *pa_validators;
		GPtrArray *pa_messages;
	} ZakFormElementPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (ZakFormElement, zak_form_element, G_TYPE_OBJECT)

static void
zak_form_element_class_init (ZakFormElementClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_form_element_set_property;
	object_class->get_property = zak_form_element_get_property;
	object_class->dispose = zak_form_element_dispose;
	object_class->finalize = zak_form_element_finalize;

	class->xml_parsing = zak_form_element_xml_parsing;

	g_object_class_install_property (object_class, PROP_NAME,
	                                 g_param_spec_string ("name",
	                                                      "Name",
	                                                      "Name",
	                                                      "",
	                                                      G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_LONG_NAME,
	                                 g_param_spec_string ("long-name",
	                                                      "Long name",
	                                                      "Long name",
	                                                      NULL,
	                                                      G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_IS_KEY,
	                                 g_param_spec_boolean ("is-key",
	                                                       "Is key",
	                                                       "Is key",
	                                                       TRUE,
	                                                       G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_TYPE,
	                                 g_param_spec_string ("type",
	                                                      "Type",
	                                                      "Type",
	                                                      "",
	                                                      G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_VALUE,
	                                 g_param_spec_string ("value",
	                                                      "Value",
	                                                      "Value",
	                                                      "",
	                                                      G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_DEFAULT_VALUE,
	                                 g_param_spec_string ("default-value",
	                                                      "Default value",
	                                                      "Default value",
	                                                      "",
	                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (object_class, PROP_ORIGINAL_VALUE,
	                                 g_param_spec_string ("original-value",
	                                                      "Original value",
	                                                      "Original value",
	                                                      "",
	                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (object_class, PROP_FORMAT,
	                                 g_param_spec_boxed ("format",
	                                                     "Format",
	                                                     "Format",
	                                                     G_TYPE_HASH_TABLE,
	                                                     G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_VISIBLE,
	                                 g_param_spec_boolean ("visible",
	                                                       "Visible",
	                                                       "Visible",
	                                                       TRUE,
	                                                       G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_EDITABLE,
	                                 g_param_spec_boolean ("editable",
	                                                       "Editable",
	                                                       "Editable",
	                                                       TRUE,
	                                                       G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_TO_LOAD,
	                                 g_param_spec_boolean ("to-load",
	                                                       "To load",
	                                                       "To load",
	                                                       TRUE,
	                                                       G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_TO_SAVE,
	                                 g_param_spec_boolean ("to-save",
	                                                       "To save",
	                                                       "To save",
	                                                       TRUE,
	                                                       G_PARAM_READWRITE));

	/**
	 * ZakFormElement::before_validating:
	 * @element:
	 *
	 */
	class->before_validating_signal_id = g_signal_new ("before-validating",
	                                                   G_TYPE_FROM_CLASS (object_class),
	                                                   G_SIGNAL_RUN_LAST,
	                                                   0,
	                                                   NULL,
	                                                   NULL,
	                                                   _zak_form_marshal_BOOLEAN__POINTER,
	                                                   G_TYPE_BOOLEAN,
	                                                   1, G_TYPE_POINTER);

	/**
	 * ZakFormElement::after_validating:
	 * @element:
	 *
	 */
	class->after_validating_signal_id = g_signal_new ("after-validating",
	                                                  G_TYPE_FROM_CLASS (object_class),
	                                                  G_SIGNAL_RUN_LAST,
	                                                  0,
	                                                  NULL,
	                                                  NULL,
	                                                  _zak_form_marshal_BOOLEAN__POINTER,
	                                                  G_TYPE_BOOLEAN,
	                                                  1, G_TYPE_POINTER);
}

static void
zak_form_element_init (ZakFormElement *zak_form_element)
{
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (zak_form_element);

	priv->name = g_strdup ("");
	priv->long_name = NULL;
	priv->is_key = FALSE;
	priv->type = g_strdup ("");
	priv->value = zak_utils_gvalue_new_string ("");
	priv->default_value = zak_utils_gvalue_new_string ("");
	priv->original_value = zak_utils_gvalue_new_string ("");
	priv->format = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
	priv->visible = TRUE;
	priv->editable = TRUE;
	priv->to_load = TRUE;
	priv->to_save = TRUE;

	priv->pa_extensions = g_ptr_array_new ();
	priv->pa_filters = g_ptr_array_new ();
	priv->pa_validators = g_ptr_array_new ();
	priv->pa_messages = NULL;
}

/**
 * zak_form_element_add_extension:
 * @element:
 * @extension:
 *
 */
void
zak_form_element_add_extension (ZakFormElement *element, GObject *extension)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	g_ptr_array_add (priv->pa_extensions, extension);
}

/**
 * zak_form_element_get_extensions:
 * @zakform:
 *
 * Returns: a #GPtrArray with the list of extensions in the form element.
 */
GPtrArray
*zak_form_element_get_extensions (ZakFormElement *element)
{
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	return priv->pa_extensions;
}

/**
 * zak_form_element_get_extensions_by_type:
 * @element:
 * @type:
 *
 * Returns: a #GPtrArray with the list of extensions in the form element filtered by @type.
 */
GPtrArray
*zak_form_element_get_extensions_by_type (ZakFormElement *element, GType type)
{
	GPtrArray *ar;
	GObject *extension;

	guint i;

	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	ar = g_ptr_array_new ();
	for (i = 0; i < priv->pa_extensions->len; i++)
		{
			extension = (GObject *)g_ptr_array_index (priv->pa_extensions, i);
			if (G_OBJECT_TYPE (extension) == type)
				{
					g_ptr_array_add (ar, extension);
				}
		}

	return ar;
}

/**
 * zak_form_element_add_filter:
 * @element:
 * @filter:
 *
 */
void
zak_form_element_add_filter (ZakFormElement *element, ZakFormElementFilter *filter)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	g_ptr_array_add (priv->pa_filters, filter);
}

/**
 * zak_form_element_filter:
 * @element:
 *
 */
void
zak_form_element_filter (ZakFormElement *element)
{
	guint i;

	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	for (i = 0; i < priv->pa_filters->len; i++)
		{
			gchar *value;
			gchar *val;

			value = zak_form_element_get_value (element);

			val = zak_form_element_filter_filter ((ZakFormElementFilter *)g_ptr_array_index (priv->pa_filters, i),
			                                      value);
			zak_form_element_set_value (element, val);
		}
}

/**
 * zak_form_element_get_filters:
 * @zakform:
 *
 * Returns: a #GPtrArray with the list of ZakFormElementFilter's in the form element.
 */
GPtrArray
*zak_form_element_get_filters (ZakFormElement *element)
{
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	return priv->pa_filters;
}

/**
 * zak_form_element_get_filter_by_id:
 * @element:
 * @id:
 *
 */
ZakFormElementFilter
*zak_form_element_get_filter_by_id (ZakFormElement *element, const gchar *id)
{
	ZakFormElementFilter *filter;

	guint i;

	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	filter = NULL;

	for (i = 0; i < priv->pa_filters->len; i++)
		{
			if (g_strcmp0 (zak_form_element_filter_get_id ((ZakFormElementFilter *)g_ptr_array_index (priv->pa_filters, i)), id) == 0)
				{
					filter = (ZakFormElementFilter *)g_ptr_array_index (priv->pa_filters, i);
					break;
				}
		}

	return filter;
}

/**
 * zak_form_element_get_filters_by_type:
 * @element:
 * @type:
 *
 * Returns: a #GPtrArray with the list of ZakFormElementFilter's in the form element filtered by @type.
 */
GPtrArray
*zak_form_element_get_filters_by_type (ZakFormElement *element, GType type)
{
	GPtrArray *ar;
	ZakFormElementFilter *filter;

	guint i;

	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	ar = g_ptr_array_new ();
	for (i = 0; i < priv->pa_filters->len; i++)
		{
			filter = (ZakFormElementFilter *)g_ptr_array_index (priv->pa_filters, i);
			if (G_OBJECT_TYPE (filter) == type)
				{
					g_ptr_array_add (ar, filter);
				}
		}

	return ar;
}

/**
 * zak_form_element_set_name:
 * @element:
 * @name:
 *
 */
void
zak_form_element_set_name (ZakFormElement *element, const gchar *name)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	if (priv->name != NULL)
		{
			g_free (priv->name);
		}

	priv->name = g_strdup (name);
}

/**
 * zak_form_element_get_name:
 * @element:
 *
 */
gchar
*zak_form_element_get_name (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	gchar *ret;

	priv = zak_form_element_get_instance_private (element);

	ret = g_strdup (priv->name);

	return ret;
}

/**
 * zak_form_element_set_long_name:
 * @element:
 * @long_name:
 *
 */
void
zak_form_element_set_long_name (ZakFormElement *element, const gchar *long_name)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	if (priv->long_name != NULL)
		{
			g_free (priv->long_name);
		}

	if (long_name == NULL)
		{
			priv->long_name = NULL;
		}
	else
		{
			priv->long_name = g_strdup (long_name);
		}
}

/**
 * zak_form_element_get_long_name:
 * @element:
 *
 */
gchar
*zak_form_element_get_long_name (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	gchar *ret;

	priv = zak_form_element_get_instance_private (element);

	if (priv->long_name == NULL)
		{
			ret = zak_form_element_get_name (element);
		}
	else
		{
			ret = g_strdup (priv->long_name);
		}

	return ret;
}

/**
 * zak_form_element_set_is_key:
 * @element:
 * @is_key:
 *
 */
void
zak_form_element_set_is_key (ZakFormElement *element, gboolean is_key)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	priv->is_key = is_key;
}

/**
 * zak_form_element_get_is_key:
 * @element:
 *
 */
gboolean
zak_form_element_get_is_key (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	return priv->is_key;
}

/**
 * zak_form_element_set_provider_type:
 * @element:
 * @type:
 *
 */
void
zak_form_element_set_provider_type (ZakFormElement *element, const gchar *type)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	if (priv->type != NULL)
		{
			g_free (priv->type);
		}

	priv->type = g_strdup (type);
}

/**
 * zak_form_element_get_type:
 * @element:
 *
 */
gchar
*zak_form_element_get_provider_type (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	gchar *ret;

	priv = zak_form_element_get_instance_private (element);

	ret = g_strdup (priv->type);

	return ret;
}

/**
 * zak_form_element_format_gvalue:
 * @element:
 * @value:
 *
 */
GValue
*zak_form_element_format_gvalue (ZakFormElement *element, GValue *value)
{
	GValue *ret;

	gchar *type;
	GHashTable *format;

	type = zak_form_element_get_provider_type (element);
	format = zak_form_element_get_format (element);

	if (g_ascii_strcasecmp (type, "integer") == 0)
		{
			gchar *thousands_saparator;
			gchar *formatted;

			thousands_saparator = (gchar *)g_hash_table_lookup (format, "thousands_separator");
			formatted = zak_utils_format_money_full (g_strtod (g_value_get_string (value), NULL), 0, thousands_saparator, NULL);

			ret = zak_utils_gvalue_new_string (formatted);
		}
	else if (g_ascii_strcasecmp (type, "float") == 0)
		{
			gchar *thousands_saparator;
			gchar *formatted;
			gchar *decimals;
			gchar *currency_symbol;

			thousands_saparator = (gchar *)g_hash_table_lookup (format, "thousands_separator");
			decimals = (gchar *)g_hash_table_lookup (format, "decimals");
			currency_symbol = (gchar *)g_hash_table_lookup (format, "currency_symbol");
			formatted = zak_utils_format_money_full (g_strtod (g_value_get_string (value), NULL), strtol (decimals, NULL, 10), thousands_saparator, currency_symbol);

			ret = zak_utils_gvalue_new_string (formatted);
		}
	else if (g_ascii_strcasecmp (type, "string") == 0)
		{
			ret = zak_utils_gvalue_new_string (g_value_get_string (value));
		}
	else if (g_ascii_strcasecmp (type, "boolean") == 0)
		{
			ret = zak_utils_gvalue_new_string (g_value_get_string (value));
		}
	else if (g_ascii_strcasecmp (type, "date") == 0
			 || g_ascii_strcasecmp (type, "time") == 0
			 || g_ascii_strcasecmp (type, "datetime") == 0)
		{
			GDateTime *gdt;

			gchar *datetime_format;

			const gchar *_value;

			_value = g_value_get_string (value);

			datetime_format = (gchar *)g_hash_table_lookup (format, "content");

			if (g_strcmp0 (_value, "@now") == 0)
				{
					gdt = g_date_time_new_now_local ();
				}
			else
				{
					if (g_ascii_strcasecmp (type, "date") == 0)
						{
							gdt = zak_utils_get_gdatetime_from_string (_value, "%Y-%m-%d");
						}
					else if (g_ascii_strcasecmp (type, "time") == 0)
						{
							gdt = zak_utils_get_gdatetime_from_string (_value, "%H:%M:%S");
						}
					else /* if (g_ascii_strcasecmp (type, "datetime") == 0) */
						{
							gdt = zak_utils_get_gdatetime_from_string (_value, NULL);
						}
				}
			ret = zak_utils_gvalue_new_string (zak_utils_gdatetime_format (gdt, datetime_format));

			if (gdt != NULL)
				{
					g_date_time_unref (gdt);
				}
		}
	else
		{
			ret = zak_utils_gvalue_new_string (g_value_get_string (value));
		}

	return ret;
}

/**
 * zak_form_element_unformat_gvalue:
 * @element:
 * @value:
 *
 */
GValue
*zak_form_element_unformat_gvalue (ZakFormElement *element, GValue *value)
{
	GValue *ret;

	gchar *_value;
	gchar *type;
	GHashTable *format;

	gchar *thousands_saparator;
	gchar *currency_symbol;
	gdouble unformatted;

	GDateTime *gdt;
	gchar *datetime_format;

	if (value == NULL)
		{
			_value = zak_form_element_get_value (element);
		}
	else
		{
			_value = g_strdup (g_value_get_string (value));
		}
	type = zak_form_element_get_provider_type (element);
	format = zak_form_element_get_format (element);

	if (g_ascii_strcasecmp (type, "integer") == 0)
		{
			thousands_saparator = (gchar *)g_hash_table_lookup (format, "thousands_separator");
			currency_symbol = (gchar *)g_hash_table_lookup (format, "currency_symbol");

			unformatted = zak_utils_unformat_money_full (_value, thousands_saparator, currency_symbol);

			ret = zak_utils_gvalue_new_string (zak_utils_format_money_full (unformatted, 0, "", NULL));
		}
	else if (g_ascii_strcasecmp (type, "float") == 0)
		{
			thousands_saparator = (gchar *)g_hash_table_lookup (format, "thousands_separator");
			currency_symbol = (gchar *)g_hash_table_lookup (format, "currency_symbol");

			unformatted = zak_utils_unformat_money_full (_value, thousands_saparator, currency_symbol);

			char *cur = g_strdup (setlocale (LC_NUMERIC, NULL));

			setlocale (LC_NUMERIC, "C");

			ret = zak_utils_gvalue_new_string (g_strdup_printf ("%f", unformatted));

			setlocale (LC_NUMERIC, cur);

			g_free (cur);
		}
	else if (g_ascii_strcasecmp (type, "string") == 0)
		{
			ret = zak_utils_gvalue_new_string (_value);
		}
	else if (g_ascii_strcasecmp (type, "boolean") == 0)
		{
			ret = zak_utils_gvalue_new_string (zak_utils_string_to_boolean (_value) ? "1" : "0");
		}
	else if (g_ascii_strcasecmp (type, "date") == 0
			 || g_ascii_strcasecmp (type, "time") == 0
			 || g_ascii_strcasecmp (type, "datetime") == 0)
		{
			datetime_format = (gchar *)g_hash_table_lookup (format, "content");
			gdt = zak_utils_get_gdatetime_from_string (_value, datetime_format);

			if (gdt == NULL)
				{
					ret = zak_utils_gvalue_new_string ("");
				}
			else
				{
					if (g_ascii_strcasecmp (type, "date") == 0)
						{
							ret = zak_utils_gvalue_new_string (zak_utils_gdatetime_format (gdt, "%F"));
						}
					else if (g_ascii_strcasecmp (type, "time") == 0)
						{
							ret = zak_utils_gvalue_new_string (zak_utils_gdatetime_format (gdt, "%T"));
						}
					else if (g_ascii_strcasecmp (type, "datetime") == 0)
						{
							ret = zak_utils_gvalue_new_string (zak_utils_gdatetime_format (gdt, "%F %T"));
						}
				}

			if (gdt != NULL)
				{
					g_date_time_unref (gdt);
				}
		}

	return ret;
}

/**
 * zak_form_element_format:
 * @element:
 * @value:
 *
 */
gchar
*zak_form_element_format (ZakFormElement *element, const gchar *value)
{
	return g_strdup (g_value_get_string (zak_form_element_format_gvalue (element, zak_utils_gvalue_new_string (value))));
}

/**
 * zak_form_element_unformat:
 * @element:
 * @value:
 *
 */
gchar
*zak_form_element_unformat (ZakFormElement *element, const gchar *value)
{
	return g_strdup (g_value_get_string (zak_form_element_unformat_gvalue (element, zak_utils_gvalue_new_string (value))));
}

/**
 * zak_form_element_set_value_gvalue:
 * @element:
 * @value:
 *
 */
gboolean
zak_form_element_set_value_gvalue (ZakFormElement *element, GValue *value)
{
	ZakFormElementPrivate *priv;

	gboolean ret;

	ret = TRUE;

	priv = zak_form_element_get_instance_private (element);

	if (priv->value != NULL)
		{
			g_value_unset (priv->value);
		}

	priv->value = zak_form_element_format_gvalue (element, value);

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->set_value != NULL)
		{
			ret = ZAK_FORM_ELEMENT_GET_CLASS (element)->set_value (element, priv->value);
		}

	return ret;
}

/**
 * zak_form_element_get_value_gvalue:
 * @element:
 *
 */
GValue
*zak_form_element_get_value_gvalue (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	GValue *ret;

	priv = zak_form_element_get_instance_private (element);

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->get_value != NULL)
		{
			ret = ZAK_FORM_ELEMENT_GET_CLASS (element)->get_value (element);
		}
	else
		{
			ret = priv->value;
		}

	return ret;
}

/**
 * zak_form_element_set_value:
 * @element:
 * @value:
 *
 */
gboolean
zak_form_element_set_value (ZakFormElement *element, const gchar *value)
{
	return zak_form_element_set_value_gvalue (element, zak_utils_gvalue_new_string (value));
}

/**
 * zak_form_element_get_value:
 * @element:
 *
 */
gchar
*zak_form_element_get_value (ZakFormElement *element)
{
	return g_strdup (g_value_get_string (zak_form_element_get_value_gvalue (element)));
}

/**
 * zak_form_element_set_default_value_gvalue:
 * @element:
 * @value:
 *
 */
gboolean
zak_form_element_set_default_value_gvalue (ZakFormElement *element, GValue *value)
{
	ZakFormElementPrivate *priv;

	gboolean ret;

	ret = TRUE;

	priv = zak_form_element_get_instance_private (element);

	if (priv->default_value != NULL)
		{
			g_value_unset (priv->default_value);
			g_value_init (priv->default_value, G_VALUE_TYPE (value));
		}

	g_value_copy (value, priv->default_value);

	return ret;
}

/**
 * zak_form_element_get_default_value_gvalue:
 * @element:
 *
 */
GValue
*zak_form_element_get_default_value_gvalue (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	return priv->default_value;
}

/**
 * zak_form_element_set_default_value:
 * @element:
 * @value:
 *
 */
gboolean
zak_form_element_set_default_value (ZakFormElement *element, const gchar *value)
{
	return zak_form_element_set_default_value_gvalue (element, zak_utils_gvalue_new_string (value));
}

/**
 * zak_form_element_get_default_value:
 * @element:
 *
 */
gchar
*zak_form_element_get_default_value (ZakFormElement *element)
{
	return g_strdup (g_value_get_string (zak_form_element_get_default_value_gvalue (element)));
}

/**
 * zak_form_element_set_original_value_gvalue:
 * @element:
 * @value:
 *
 */
gboolean
zak_form_element_set_original_value_gvalue (ZakFormElement *element, GValue *value)
{
	ZakFormElementPrivate *priv;

	gboolean ret;

	ret = TRUE;

	priv = zak_form_element_get_instance_private (element);

	if (priv->original_value != NULL)
		{
			g_value_unset (priv->original_value);
			g_value_init (priv->original_value, G_VALUE_TYPE (value));
		}

	g_value_copy (value, priv->original_value);

	return ret;
}

/**
 * zak_form_element_get_original_value_gvalue:
 * @element:
 *
 */
GValue
*zak_form_element_get_original_value_gvalue (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	return priv->original_value;
}

/**
 * zak_form_element_get_original_value:
 * @element:
 *
 */
gchar
*zak_form_element_get_original_value (ZakFormElement *element)
{
	return g_strdup (g_value_get_string (zak_form_element_get_original_value_gvalue (element)));
}

/**
 * zak_form_element_set_original_value:
 * @element:
 *
 */
gboolean
zak_form_element_set_original_value (ZakFormElement *element, const gchar *value)
{
	return zak_form_element_set_original_value_gvalue (element, zak_utils_gvalue_new_string (value));
}

/**
 * zak_form_element_set_as_original_value:
 * @element
 *
 */
void
zak_form_element_set_as_original_value (ZakFormElement *element)
{
	zak_form_element_set_original_value_gvalue (element, zak_form_element_get_value_gvalue (element));
}

/**
 * zak_form_element_is_changed:
 * @element:
 *
 */
gboolean
zak_form_element_is_changed (ZakFormElement *element)
{
	return (g_strcmp0 (zak_form_element_get_original_value (element), zak_form_element_get_value (element)) != 0);
}

/**
 * zak_form_element_set_format:
 * @element:
 * @format:
 *
 */
gboolean
zak_form_element_set_format (ZakFormElement *element, GHashTable *format)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	if (priv->format != NULL)
		{
			g_hash_table_destroy (priv->format);
		}

	priv->format = g_hash_table_ref (format);

	return TRUE;
}

/**
 * zak_form_element_get_format:
 * @element:
 *
 */
GHashTable
*zak_form_element_get_format (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	GHashTable *ret;

	priv = zak_form_element_get_instance_private (element);

	ret = g_hash_table_ref (priv->format);

	return ret;
}

/**
 * zak_form_element_set_visible:
 * @element:
 * @visible:
 *
 */
void
zak_form_element_set_visible (ZakFormElement *element, gboolean visible)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	priv->visible = visible;

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->set_visible != NULL)
		{
			ZAK_FORM_ELEMENT_GET_CLASS (element)->set_visible (element, priv->visible);
		}
}

/**
 * zak_form_element_get_visible:
 * @element:
 *
 */
gboolean
zak_form_element_get_visible (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	gboolean ret;

	priv = zak_form_element_get_instance_private (element);

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->get_visible != NULL)
		{
			ret = ZAK_FORM_ELEMENT_GET_CLASS (element)->get_visible (element);
		}
	else
		{
			ret = priv->visible;
		}

	return ret;
}

/**
 * zak_form_element_set_editable:
 * @element:
 * @editable:
 *
 */
void
zak_form_element_set_editable (ZakFormElement *element, gboolean editable)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	priv->editable = editable;

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->set_editable != NULL)
		{
			ZAK_FORM_ELEMENT_GET_CLASS (element)->set_editable (element, priv->editable);
		}
}

/**
 * zak_form_element_get_editable:
 * @element:
 *
 */
gboolean
zak_form_element_get_editable (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	gboolean ret;

	priv = zak_form_element_get_instance_private (element);

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->get_editable != NULL)
		{
			ret = ZAK_FORM_ELEMENT_GET_CLASS (element)->get_editable (element);
		}
	else
		{
			ret = priv->editable;
		}

	return ret;
}

/**
 * zak_form_element_set_to_load:
 * @element:
 * @to_load:
 *
 */
void
zak_form_element_set_to_load (ZakFormElement *element, gboolean to_load)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	priv->to_load = to_load;
}

/**
 * zak_form_element_get_to_load:
 * @element:
 *
 */
gboolean
zak_form_element_get_to_load (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	return priv->to_load;
}

/**
 * zak_form_element_set_to_save:
 * @element:
 * @to_save:
 *
 */
void
zak_form_element_set_to_save (ZakFormElement *element, gboolean to_save)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	priv->to_save = to_save;
}

/**
 * zak_form_element_get_to_save:
 * @element:
 *
 */
gboolean
zak_form_element_get_to_save (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	return priv->to_save;
}

/**
 * zak_form_element_clear:
 * @element:
 *
 */
void
zak_form_element_clear (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	zak_form_element_set_value_gvalue (element, priv->default_value);
}

/**
 * zak_form_element_add_validator:
 * @element:
 * @validator:
 *
 */
void
zak_form_element_add_validator (ZakFormElement *element, ZakFormElementValidator *validator)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	g_ptr_array_add (priv->pa_validators, validator);
}

/**
 * zak_form_element_get_validator_by_id:
 * @element:
 * @id:
 *
 */
ZakFormElementValidator
*zak_form_element_get_validator_by_id (ZakFormElement *element, const gchar *id)
{
	ZakFormElementValidator *validator;

	guint i;

	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	validator = NULL;

	for (i = 0; i < priv->pa_validators->len; i++)
		{
			if (g_strcmp0 (zak_form_element_validator_get_id ((ZakFormElementValidator *)g_ptr_array_index (priv->pa_validators, i)), id) == 0)
				{
					validator = (ZakFormElementValidator *)g_ptr_array_index (priv->pa_validators, i);
					break;
				}
		}

	return validator;
}

/**
 * zak_form_element_is_valid:
 * @element:
 *
 */
gboolean
zak_form_element_is_valid (ZakFormElement *element)
{
	gboolean ret;
	gboolean ret_callback;

	gchar *value;
	guint i;

	ZakFormElementPrivate *priv;

	ZakFormElementClass *klass = ZAK_FORM_ELEMENT_GET_CLASS (element);

	priv = zak_form_element_get_instance_private (element);

	ret = TRUE;
	ret_callback = FALSE;

	zak_form_element_filter (element);

	if (priv->pa_messages != NULL)
		{
			g_ptr_array_unref (priv->pa_messages);
			priv->pa_messages = NULL;
		}

	priv->pa_messages = g_ptr_array_new ();

	/* signal before_validating */
	g_signal_emit (element, klass->before_validating_signal_id, 0, (gpointer)priv->pa_messages, &ret_callback);
	if (ret_callback)
		{
			ret = FALSE;
		}

	value = zak_form_element_get_value (element);

	for (i = 0; i < priv->pa_validators->len; i++)
		{
			ZakFormElementValidator *validator = (ZakFormElementValidator *)g_ptr_array_index (priv->pa_validators, i);
			if (!zak_form_element_validator_validate (validator, value))
				{
					g_ptr_array_add (priv->pa_messages, (gpointer)g_strdup (zak_form_element_validator_get_message (validator)));

					ret = FALSE;
				}
		}

	/* signal after_validating */
	g_signal_emit (element, klass->after_validating_signal_id, 0, (gpointer)priv->pa_messages, &ret_callback);
	if (ret_callback)
		{
			ret = FALSE;
		}

	return ret;
}

/**
 * zak_form_element_get_messages:
 * @element:
 *
 */
GPtrArray
*zak_form_element_get_messages (ZakFormElement *element)
{
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	return priv->pa_messages;
}

/**
 * zak_form_element_get_validators:
 * @zakform:
 *
 * Returns: a #GPtrArray with the list of ZakFormElementValidator's in the form element.
 */
GPtrArray
*zak_form_element_get_validators(ZakFormElement *element)
{
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	return priv->pa_validators;
}

/**
 * zak_form_element_get_validators_by_type:
 * @element:
 * @type:
 *
 * Returns: a #GPtrArray with the list of ZakFormElementValidator's in the form element filtered by @type.
 */
GPtrArray
*zak_form_element_get_validators_by_type (ZakFormElement *element, GType type)
{
	GPtrArray *ar;
	ZakFormElementFilter *filter;

	guint i;

	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (element);

	ar = g_ptr_array_new ();
	for (i = 0; i < priv->pa_validators->len; i++)
		{
			filter = (ZakFormElementFilter *)g_ptr_array_index (priv->pa_validators, i);
			if (G_OBJECT_TYPE (filter) == type)
				{
					g_ptr_array_add (ar, filter);
				}
		}

	return ar;
}

/* PRIVATE */
static void
zak_form_element_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElement *zak_form_element = (ZakFormElement *)object;
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (zak_form_element);

	switch (property_id)
		{
		case PROP_NAME:
		    zak_form_element_set_name (zak_form_element, g_value_dup_string (value));
			break;

		case PROP_LONG_NAME:
		    zak_form_element_set_long_name (zak_form_element, g_value_dup_string (value));
			break;

		case PROP_IS_KEY:
		    zak_form_element_set_is_key (zak_form_element, g_value_get_boolean (value));
			break;

		case PROP_TYPE:
		    zak_form_element_set_provider_type (zak_form_element, g_value_dup_string (value));
			break;

		case PROP_VALUE:
		    zak_form_element_set_value (zak_form_element, g_value_dup_string (value));
			break;

		case PROP_DEFAULT_VALUE:
		    zak_form_element_set_default_value (zak_form_element, g_value_get_string (value));
			break;

		case PROP_ORIGINAL_VALUE:
		    zak_form_element_set_original_value (zak_form_element, g_value_get_string (value));
			break;

		case PROP_FORMAT:
		    zak_form_element_set_format (zak_form_element, (GHashTable *)g_value_get_boxed (value));
			break;

		case PROP_VISIBLE:
		    zak_form_element_set_visible (zak_form_element, g_value_get_boolean (value));
			break;

		case PROP_EDITABLE:
		    zak_form_element_set_editable (zak_form_element, g_value_get_boolean (value));
			break;

		case PROP_TO_LOAD:
		    zak_form_element_set_to_load (zak_form_element, g_value_get_boolean (value));
			break;

		case PROP_TO_SAVE:
		    zak_form_element_set_to_save (zak_form_element, g_value_get_boolean (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElement *zak_form_element = (ZakFormElement *)object;
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (zak_form_element);

	switch (property_id)
		{
		case PROP_NAME:
			g_value_set_string (value, zak_form_element_get_name (zak_form_element));
			break;

		case PROP_LONG_NAME:
			g_value_set_string (value, zak_form_element_get_long_name (zak_form_element));
			break;

		case PROP_IS_KEY:
			g_value_set_boolean (value, zak_form_element_get_is_key (zak_form_element));
			break;

		case PROP_TYPE:
			g_value_set_string (value, zak_form_element_get_provider_type (zak_form_element));
			break;

		case PROP_VALUE:
			g_value_set_string (value, zak_form_element_get_value (zak_form_element));
			break;

		case PROP_DEFAULT_VALUE:
			g_value_set_string (value, zak_form_element_get_default_value (zak_form_element));
			break;

		case PROP_ORIGINAL_VALUE:
			g_value_set_string (value, zak_form_element_get_original_value (zak_form_element));
			break;

		case PROP_FORMAT:
			g_value_set_boxed (value, zak_form_element_get_format (zak_form_element));
			break;

		case PROP_VISIBLE:
			g_value_set_boolean (value, zak_form_element_get_visible (zak_form_element));
			break;

		case PROP_EDITABLE:
			g_value_set_boolean (value, zak_form_element_get_editable (zak_form_element));
			break;

		case PROP_TO_LOAD:
			g_value_set_boolean (value, zak_form_element_get_to_load (zak_form_element));
			break;

		case PROP_TO_SAVE:
			g_value_set_boolean (value, zak_form_element_get_to_save (zak_form_element));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_dispose (GObject *gobject)
{
	ZakFormElement *zak_form_element = (ZakFormElement *)gobject;
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_finalize (GObject *gobject)
{
	ZakFormElement *zak_form_element = (ZakFormElement *)gobject;
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static void
zak_form_element_xml_parsing (ZakFormElement *element, xmlNode *xmlnode)
{
	xmlNode *cur;

	gboolean to_unlink;
	xmlNode *xnode_tmp;

	cur = xmlnode->children;
	while (cur)
		{
			to_unlink = FALSE;

			if (xmlStrEqual (cur->name, (const xmlChar *)"name"))
				{
					zak_form_element_set_name (element, (const gchar *)xmlNodeGetContent (cur));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"long-name"))
				{
					zak_form_element_set_long_name (element, (const gchar *)xmlNodeGetContent (cur));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"is-key"))
				{
					zak_form_element_set_is_key (element, xmlStrEqual (xmlNodeGetContent (cur), (const xmlChar *)"TRUE"));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"type"))
				{
					zak_form_element_set_provider_type (element, (const gchar *)xmlNodeGetContent (cur));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"default-value"))
				{
					zak_form_element_set_default_value (element, (const gchar *)xmlNodeGetContent (cur));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"format"))
				{
					GHashTable *ht;

					ht = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);

					xmlAttr *attr;
					attr = cur->properties;
					while (attr)
						{
							g_hash_table_insert (ht, g_strdup ((gchar *)attr->name), g_strdup ((const gchar *)xmlNodeGetContent (attr->children)));

							attr = attr->next;
						}

					g_hash_table_insert (ht, "content", g_strdup ((const gchar *)xmlNodeGetContent (cur)));

					zak_form_element_set_format (element, ht);
					g_hash_table_unref (ht);
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"visible"))
				{
					zak_form_element_set_visible (element, xmlStrEqual (xmlNodeGetContent (cur), (const xmlChar *)"TRUE"));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"editable"))
				{
					zak_form_element_set_editable (element, xmlStrEqual (xmlNodeGetContent (cur), (const xmlChar *)"TRUE"));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"to-load"))
				{
					zak_form_element_set_to_load (element, xmlStrEqual (xmlNodeGetContent (cur), (const xmlChar *)"TRUE"));
					to_unlink = TRUE;
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"to-save"))
				{
					zak_form_element_set_to_save (element, xmlStrEqual (xmlNodeGetContent (cur), (const xmlChar *)"TRUE"));
					to_unlink = TRUE;
				}

			if (to_unlink)
				{
					xnode_tmp = cur;
				}

			cur = cur->next;

			if (to_unlink)
				{
					xmlUnlinkNode (xnode_tmp);
					xmlFreeNode (xnode_tmp);
				}
		}
}

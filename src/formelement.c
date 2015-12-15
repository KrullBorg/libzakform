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

#include <libzakutils/libzakutils.h>

#include "formelement.h"

enum
{
	PROP_0,
	PROP_NAME,
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
		gboolean is_key;
		gchar *type;
		gchar *value;
		gchar *default_value;
		gchar *original_value;
		GHashTable *format;
		gboolean visible;
		gboolean editable;
		gboolean to_load;
		gboolean to_save;

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
}

static void
zak_form_element_init (ZakFormElement *zak_form_element)
{
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (zak_form_element);

	priv->name = g_strdup ("");
	priv->is_key = FALSE;
	priv->type = g_strdup ("");
	priv->value = g_strdup ("");
	priv->format = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
	priv->visible = TRUE;
	priv->editable = TRUE;
	priv->to_load = TRUE;
	priv->to_save = TRUE;

	priv->pa_filters = NULL;
	priv->pa_validators = NULL;
	priv->pa_messages = NULL;
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

	if (priv->pa_filters == NULL)
		{
			priv->pa_filters = g_ptr_array_new ();
		}

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

	if (priv->pa_filters == NULL)
		{
			return;
		}

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

static gchar
*zak_form_element_format (ZakFormElement *element, const gchar *value)
{
	gchar *ret;

	gchar *type;
	GHashTable *format;

	type = zak_form_element_get_provider_type (element);
	format = zak_form_element_get_format (element);

	if (g_ascii_strcasecmp (type, "integer") == 0)
		{
			gchar *thousands_saparator;
			gchar *formatted;

			thousands_saparator = (gchar *)g_hash_table_lookup (format, "thousands_separator");
			formatted = zak_utils_format_money_full (g_strtod (value, NULL), 0, thousands_saparator, NULL);

			ret = g_strdup (formatted);
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
			formatted = zak_utils_format_money_full (g_strtod (value, NULL), strtol (decimals, NULL, 10), thousands_saparator, currency_symbol);

			ret = g_strdup (formatted);
		}
	else if (g_ascii_strcasecmp (type, "string") == 0)
		{
			ret = g_strdup (value);
		}
	else if (g_ascii_strcasecmp (type, "boolean") == 0)
		{
			ret = g_strdup (value);
		}
	else if (g_ascii_strcasecmp (type, "date") == 0
			 || g_ascii_strcasecmp (type, "time") == 0
			 || g_ascii_strcasecmp (type, "datetime") == 0)
		{
			GDateTime *gdt;

			gchar *datetime_format;

			datetime_format = (gchar *)g_hash_table_lookup (format, "content");
			gdt = zak_utils_get_gdatetime_from_string (value, NULL);
			ret = zak_utils_gdatetime_format (gdt, datetime_format);

			if (gdt != NULL)
				{
					g_date_time_unref (gdt);
				}
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
	ZakFormElementPrivate *priv;

	gboolean ret;

	ret = TRUE;

	priv = zak_form_element_get_instance_private (element);

	if (priv->value != NULL)
		{
			g_free (priv->value);
		}

	priv->value = zak_form_element_format (element, value);

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->set_value != NULL)
		{
			ret = ZAK_FORM_ELEMENT_GET_CLASS (element)->set_value (element, priv->value);
		}

	return ret;
}

/**
 * zak_form_element_get_value:
 * @element:
 *
 */
gchar
*zak_form_element_get_value (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	gchar *ret;

	priv = zak_form_element_get_instance_private (element);

	if (ZAK_FORM_ELEMENT_GET_CLASS (element)->get_value != NULL)
		{
			ret = ZAK_FORM_ELEMENT_GET_CLASS (element)->get_value (element);
		}
	else
		{
			ret = g_strdup (priv->value);
		}

	return ret;
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
	ZakFormElementPrivate *priv;

	gboolean ret;

	ret = TRUE;

	priv = zak_form_element_get_instance_private (element);

	if (priv->default_value != NULL)
		{
			g_free (priv->default_value);
		}

	priv->default_value = g_strdup (value);

	return ret;
}

/**
 * zak_form_element_get_default_value:
 * @element:
 *
 */
gchar
*zak_form_element_get_default_value (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	return g_strdup (priv->default_value);
}

/**
 * zak_form_element_set_original_value:
 * @element:
 * @value:
 *
 */
gboolean
zak_form_element_set_original_value (ZakFormElement *element, const gchar *value)
{
	ZakFormElementPrivate *priv;

	gboolean ret;

	ret = TRUE;

	priv = zak_form_element_get_instance_private (element);

	if (priv->original_value != NULL)
		{
			g_free (priv->original_value);
		}

	priv->original_value = g_strdup (value);

	return ret;
}

/**
 * zak_form_element_get_original_value:
 * @element:
 *
 */
gchar
*zak_form_element_get_original_value (ZakFormElement *element)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	return g_strdup (priv->original_value);
}

/**
 * zak_form_element_set_as_original_value:
 * @element
 *
 */
void
zak_form_element_set_as_original_value (ZakFormElement *element)
{
	zak_form_element_set_original_value (element, zak_form_element_get_value (element));
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

	zak_form_element_set_value (element, priv->default_value);
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

	if (priv->pa_validators == NULL)
		{
			priv->pa_validators = g_ptr_array_new ();
		}

	g_ptr_array_add (priv->pa_validators, validator);
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

	gchar *value;
	guint i;

	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	ret = TRUE;

	zak_form_element_filter (element);

	if (priv->pa_messages != NULL)
		{
			g_ptr_array_unref (priv->pa_messages);
			priv->pa_messages = NULL;
		}

	if (priv->pa_validators != NULL)
		{
			value = zak_form_element_get_value (element);

			for (i = 0; i < priv->pa_validators->len; i++)
				{
					ZakFormElementValidator *validator = (ZakFormElementValidator *)g_ptr_array_index (priv->pa_validators, i);
					if (!zak_form_element_validator_validate (validator, value))
						{
							if (priv->pa_messages == NULL)
								{
									priv->pa_messages = g_ptr_array_new ();
								}
							g_ptr_array_add (priv->pa_messages, (gpointer)g_strdup (zak_form_element_validator_get_message (validator)));

							ret = FALSE;
						}
				}
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

	cur = xmlnode->children;
	while (cur)
		{
		    if (xmlStrcmp (cur->name, (const xmlChar *)"name") == 0)
				{
					zak_form_element_set_name (element, (const gchar *)xmlNodeGetContent (cur));
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"is-key") == 0)
				{
					zak_form_element_set_is_key (element, xmlStrEqual ((const gchar *)xmlNodeGetContent (cur), "TRUE"));
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"type") == 0)
				{
					zak_form_element_set_provider_type (element, (const gchar *)xmlNodeGetContent (cur));
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"default-value") == 0)
				{
					zak_form_element_set_default_value (element, (const gchar *)xmlNodeGetContent (cur));
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"format") == 0)
				{
					GHashTable *ht;

					ht = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);

					xmlAttr *attr;
					attr = cur->properties;
					while (attr)
						{
							g_hash_table_insert (ht, g_strdup (attr->name), g_strdup ((const gchar *)xmlNodeGetContent (attr->children)));

							attr = attr->next;
						}

					g_hash_table_insert (ht, "content", g_strdup ((const gchar *)xmlNodeGetContent (cur)));

					zak_form_element_set_format (element, ht);
					g_hash_table_unref (ht);
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"visible") == 0)
				{
					zak_form_element_set_visible (element, xmlStrEqual ((const gchar *)xmlNodeGetContent (cur), "TRUE"));
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"editable") == 0)
				{
					zak_form_element_set_editable (element, xmlStrEqual ((const gchar *)xmlNodeGetContent (cur), "TRUE"));
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"to-load") == 0)
				{
					zak_form_element_set_to_load (element, xmlStrEqual ((const gchar *)xmlNodeGetContent (cur), "TRUE"));
				}
		    else if (xmlStrcmp (cur->name, (const xmlChar *)"to-save") == 0)
				{
					zak_form_element_set_to_save (element, xmlStrEqual ((const gchar *)xmlNodeGetContent (cur), "TRUE"));
				}

			cur = cur->next;
		}
}

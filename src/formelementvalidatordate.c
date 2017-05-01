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

#include <glib/gi18n-lib.h>

#include <libzakutils/libzakutils.h>

#include "formelementvalidator.h"
#include "formelementvalidatordate.h"

static void zak_form_element_validator_date_class_init (ZakFormElementValidatorDateClass *class);
static void zak_form_element_validator_date_init (ZakFormElementValidatorDate *zak_form_element);

static void zak_form_element_validator_date_set_property (GObject *object,
                                                          guint property_id,
                                                          const GValue *value,
                                                          GParamSpec *pspec);
static void zak_form_element_validator_date_get_property (GObject *object,
                                                          guint property_id,
                                                          GValue *value,
                                                          GParamSpec *pspec);

static void zak_form_element_validator_date_dispose (GObject *gobject);
static void zak_form_element_validator_date_finalize (GObject *gobject);

static gboolean zak_form_element_validator_date_xml_parsing (ZakFormElementValidator *validator, xmlNode *xnode);
static gboolean zak_form_element_validator_date_validate (ZakFormElementValidator *validator_date, const gchar *value);

struct _ZakFormElementValidatorDate
{
	ZakFormElementValidator parent_instance;

	/* Other members, including private data. */
};

#define ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_TYPE_ELEMENT_VALIDATOR_DATE, ZakFormElementValidatorDatePrivate))

typedef struct _ZakFormElementValidatorDatePrivate ZakFormElementValidatorDatePrivate;
struct _ZakFormElementValidatorDatePrivate
	{
		ZakFormCompareType compare_type;
		gchar *compare_value;
		gchar *format;
	};

G_DEFINE_TYPE (ZakFormElementValidatorDate, zak_form_element_validator_date, ZAK_FORM_TYPE_ELEMENT_VALIDATOR)

static void
zak_form_element_validator_date_class_init (ZakFormElementValidatorDateClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	ZakFormElementValidatorClass *parent_class = ZAK_FORM_ELEMENT_VALIDATOR_CLASS (class);

	object_class->set_property = zak_form_element_validator_date_set_property;
	object_class->get_property = zak_form_element_validator_date_get_property;
	object_class->dispose = zak_form_element_validator_date_dispose;
	object_class->finalize = zak_form_element_validator_date_finalize;

	parent_class->xml_parsing = zak_form_element_validator_date_xml_parsing;
	parent_class->validate = zak_form_element_validator_date_validate;

	g_type_class_add_private (object_class, sizeof (ZakFormElementValidatorDatePrivate));
}

static void
zak_form_element_validator_date_init (ZakFormElementValidatorDate *zak_form_element)
{
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (zak_form_element);

	priv->format = NULL;
}

/**
 * zak_form_element_validator_date_new:
 *
 * Returns: the newly created #ZakFormElementValidatorDate object.
 */
ZakFormElementValidatorDate
*zak_form_element_validator_date_new ()
{
	ZakFormElementValidatorDate *zak_form_element_validator_date;

	zak_form_element_validator_date = ZAK_FORM_ELEMENT_VALIDATOR_DATE (g_object_new (zak_form_element_validator_date_get_type (), NULL));

	return zak_form_element_validator_date;
}

/**
 * zak_form_element_validator_date_set_compare_type:
 * @validator:
 * @type:
 *
 */
void
zak_form_element_validator_date_set_compare_type (ZakFormElementValidatorDate *validator, ZakFormCompareType type)
{
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator);

	priv->compare_type = type;
}

/**
 * zak_form_element_validator_date_get_compare_type:
 * @validator:
 *
 */
ZakFormCompareType
zak_form_element_validator_date_get_compare_type (ZakFormElementValidatorDate *validator)
{
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator);

	return priv->compare_type;
}

/**
 * zak_form_element_validator_date_set_compare_value:
 * @validator:
 * @value:
 *
 */
void
zak_form_element_validator_date_set_compare_value (ZakFormElementValidatorDate *validator, const gchar *value)
{
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator);

	if (priv->compare_value != NULL)
		{
			g_free (priv->compare_value);
			priv->compare_value = NULL;
		}

	if (value != NULL)
		{
			priv->compare_value = g_strdup (value);
		}
}

/**
 * zak_form_element_validator_date_get_compare_value:
 * @validator:
 *
 */
gchar
*zak_form_element_validator_date_get_compare_value (ZakFormElementValidatorDate *validator)
{
	gchar *ret;

	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator);

	if (priv->compare_value == NULL)
		{
			ret = NULL;
		}
	else
		{
			ret = g_strdup (priv->compare_value);
		}

	return ret;
}

/**
 * zak_form_element_validator_date_set_format:
 * @validator:
 * @format:
 *
 */
void
zak_form_element_validator_date_set_format (ZakFormElementValidatorDate *validator, const gchar *format)
{
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator);

	if (priv->format != NULL)
		{
			g_free (priv->format);
		}
	priv->format = g_strdup (format);
}

/**
 * zak_form_element_validator_date_get_format:
 * @validator:
 *
 */
gchar
*zak_form_element_validator_date_get_format (ZakFormElementValidatorDate *validator)
{
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator);

	return g_strdup (priv->format);
}

/**
 * zak_form_element_validator_date_xml_parsing:
 * @validator:
 * @xnode:
 *
 */
static gboolean
zak_form_element_validator_date_xml_parsing (ZakFormElementValidator *validator, xmlNode *xnode)
{
	gchar *prop;

	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (ZAK_FORM_ELEMENT_VALIDATOR_DATE (validator));

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"type_comp");
	zak_form_element_validator_date_set_compare_type (ZAK_FORM_ELEMENT_VALIDATOR_DATE (validator), zak_form_get_compare_type_from_string (prop));
	g_free (prop);

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"value_comp");
	zak_form_element_validator_date_set_compare_value (ZAK_FORM_ELEMENT_VALIDATOR_DATE (validator), prop);
	g_free (prop);

	zak_form_element_validator_date_set_format (ZAK_FORM_ELEMENT_VALIDATOR_DATE (validator), (gchar *)xmlNodeGetContent (xnode));

	return TRUE;
}

/* PRIVATE */
static void
zak_form_element_validator_date_set_property (GObject *object,
                                              guint property_id,
                                              const GValue *value,
                                              GParamSpec *pspec)
{
	ZakFormElementValidatorDate *zak_form_element = (ZakFormElementValidatorDate *)object;
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_validator_date_get_property (GObject *object,
                                              guint property_id,
                                              GValue *value,
                                              GParamSpec *pspec)
{
	ZakFormElementValidatorDate *zak_form_element = (ZakFormElementValidatorDate *)object;
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_validator_date_dispose (GObject *gobject)
{
	ZakFormElementValidatorDate *zak_form_element = (ZakFormElementValidatorDate *)gobject;
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_validator_date_finalize (GObject *gobject)
{
	ZakFormElementValidatorDate *zak_form_element = (ZakFormElementValidatorDate *)gobject;
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (zak_form_element);

	g_free (priv->format);

	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
zak_form_element_validator_date_validate (ZakFormElementValidator *validator_date,
                                          const gchar *value)
{
	gboolean ret;
	gchar *_value;
	GDateTime *gdt;
	GDateTime *gdt_compare;

	gint comp;
	gchar *msg;

	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator_date);

	g_return_val_if_fail (value != NULL, FALSE);

	gdt = NULL;
	gdt_compare = NULL;

	_value = g_strstrip (g_strdup (value));
	if (g_strcmp0 (_value, "") == 0)
		{
			ret = TRUE;
		}
	else
		{
			gdt = zak_utils_get_gdatetime_from_string (_value, priv->format);
			ret = (gdt != NULL);

			if (ret
			    && priv->compare_type > ZAK_FORM_COMPARE_TYPE_NONE)
				{
					if (g_strcmp0 (priv->compare_value, "@now") == 0)
						{
							GDateTime *gdt_tmp;
							gchar *gdt_str;

							gdt_tmp = g_date_time_new_now_local ();
							gdt_str = zak_utils_gdatetime_format (gdt_tmp, priv->format);
							gdt_compare = zak_utils_get_gdatetime_from_string (gdt_str, priv->format);

							g_free (gdt_str);
							g_date_time_unref (gdt_tmp);
						}
					else
						{
							gdt_compare = zak_utils_get_gdatetime_from_string (priv->compare_value, priv->format);
						}

					if (gdt_compare != NULL)
						{
							comp = g_date_time_compare (gdt, gdt_compare);
							switch (comp)
								{
								case -1:
									ret = (priv->compare_type == ZAK_FORM_COMPARE_TYPE_LESSER
									       || priv->compare_type == ZAK_FORM_COMPARE_TYPE_NOT_EQUAL);
									break;

								case 0:
									ret = (priv->compare_type == ZAK_FORM_COMPARE_TYPE_LESSER_EQUAL
									       || priv->compare_type == ZAK_FORM_COMPARE_TYPE_EQUAL
									       || priv->compare_type == ZAK_FORM_COMPARE_TYPE_GREATER_EQUAL);
									break;

								case 1:
									ret = (priv->compare_type == ZAK_FORM_COMPARE_TYPE_GREATER
									       || priv->compare_type == ZAK_FORM_COMPARE_TYPE_NOT_EQUAL);
									break;
								};

							if (!ret)
								{
									msg = g_strdup_printf (_("Value must be %s %s"),
									                       zak_form_get_compare_type_stringify (priv->compare_type),
									                       zak_utils_gdatetime_format (gdt_compare, priv->format));
									zak_form_element_validator_set_message (validator_date, msg);
									g_free (msg);
								}

							g_date_time_unref (gdt_compare);
						}

				}

			if (gdt != NULL)
				{
					g_date_time_unref (gdt);
				}
		}
	g_free (_value);

	return ret;
}

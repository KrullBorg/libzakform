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
gboolean
zak_form_element_validator_date_xml_parsing (ZakFormElementValidator *validator, xmlNode *xnode)
{
	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (ZAK_FORM_ELEMENT_VALIDATOR_DATE (validator));

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

	ZakFormElementValidatorDatePrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_DATE_GET_PRIVATE (validator_date);

	g_return_val_if_fail (value != NULL, FALSE);

	_value = g_strstrip (g_strdup (value));
	if (g_strcmp0 (_value, "") == 0)
		{
			ret = TRUE;
		}
	else
		{
			gdt = zak_utils_get_gdatetime_from_string (_value, priv->format);
			ret = (gdt != NULL);

			if (gdt != NULL)
				{
					g_date_time_unref (gdt);
				}
		}
	g_free (_value);

	return ret;
}

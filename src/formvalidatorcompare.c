/*
 * Copyright (C) 2016-2017 Andrea Zagli <azagli@libero.it>
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

#include "commons.h"
#include "formvalidator.h"
#include "formvalidatorcompare.h"

static void zak_form_validator_compare_class_init (ZakFormValidatorCompareClass *class);
static void zak_form_validator_compare_init (ZakFormValidatorCompare *validator);

static void zak_form_validator_compare_set_property (GObject *object,
                                                     guint property_id,
                                                     const GValue *value,
                                                     GParamSpec *pspec);
static void zak_form_validator_compare_get_property (GObject *object,
                                                     guint property_id,
                                                     GValue *value,
                                                     GParamSpec *pspec);

static void zak_form_validator_compare_dispose (GObject *gobject);
static void zak_form_validator_compare_finalize (GObject *gobject);

static gboolean zak_form_validator_compare_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, GPtrArray *ar_elements);
static gboolean zak_form_validator_compare_validate (ZakFormValidator *validator_notempty);

struct _ZakFormValidatorCompare
{
	ZakFormValidator parent_instance;

	/* Other members, including private data. */
};

#define ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_TYPE_VALIDATOR_COMPARE, ZakFormValidatorComparePrivate))

typedef struct _ZakFormValidatorComparePrivate ZakFormValidatorComparePrivate;
struct _ZakFormValidatorComparePrivate
	{
		guint type;

		ZakFormElement *v1;
		ZakFormElement *v2;
	};

G_DEFINE_TYPE (ZakFormValidatorCompare, zak_form_validator_compare, ZAK_FORM_TYPE_VALIDATOR)

static void
zak_form_validator_compare_class_init (ZakFormValidatorCompareClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	ZakFormValidatorClass *parent_class = ZAK_FORM_VALIDATOR_CLASS (class);

	object_class->set_property = zak_form_validator_compare_set_property;
	object_class->get_property = zak_form_validator_compare_get_property;
	object_class->dispose = zak_form_validator_compare_dispose;
	object_class->finalize = zak_form_validator_compare_finalize;

	parent_class->xml_parsing = zak_form_validator_compare_xml_parsing;
	parent_class->validate = zak_form_validator_compare_validate;

	g_type_class_add_private (object_class, sizeof (ZakFormValidatorComparePrivate));
}

static void
zak_form_validator_compare_init (ZakFormValidatorCompare *validator)
{
	ZakFormValidatorComparePrivate *priv = ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE (validator);

	priv->v1 = NULL;
	priv->v2 = NULL;
}

/**
 * zak_form_validator_compare_new:
 *
 * Returns: the newly created #ZakFormValidatorCompare object.
 */
ZakFormValidatorCompare
*zak_form_validator_compare_new ()
{
	ZakFormValidatorCompare *zak_form_validator_compare;

	zak_form_validator_compare = ZAK_FORM_VALIDATOR_COMPARE (g_object_new (zak_form_validator_compare_get_type (), NULL));

	return zak_form_validator_compare;
}

/**
 * zak_form_validator_compare_xml_parsing:
 * @validator:
 * @xnode:
 * @ar_elements:
 *
 */
static gboolean
zak_form_validator_compare_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, GPtrArray *ar_elements)
{
	gchar *prop;

	ZakFormValidatorComparePrivate *priv = ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE (validator);

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"type_comp");
	priv->type = zak_form_get_compare_type_from_string (prop);
	g_free (prop);

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"element1");
	priv->v1 = zak_form_get_element_by_id (ar_elements, prop);
	if (!ZAK_FORM_IS_ELEMENT (priv->v1))
		{
			g_warning ("Validator compare: element1 isn't a ZakFormElement.");
		}
	g_free (prop);

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"element2");
	priv->v2 = zak_form_get_element_by_id (ar_elements, prop);
	if (!ZAK_FORM_IS_ELEMENT (priv->v2))
		{
			g_warning ("Validator compare: element2 isn't a ZakFormElement.");
		}
	g_free (prop);

	return TRUE;
}

/* PRIVATE */
static void
zak_form_validator_compare_set_property (GObject *object,
                                         guint property_id,
                                         const GValue *value,
                                         GParamSpec *pspec)
{
	ZakFormValidatorCompare *validator = (ZakFormValidatorCompare *)object;
	ZakFormValidatorComparePrivate *priv = ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE (validator);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_validator_compare_get_property (GObject *object,
                                         guint property_id,
                                         GValue *value,
                                         GParamSpec *pspec)
{
	ZakFormValidatorCompare *validator = (ZakFormValidatorCompare *)object;
	ZakFormValidatorComparePrivate *priv = ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE (validator);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_validator_compare_dispose (GObject *gobject)
{
	ZakFormValidatorCompare *validator = (ZakFormValidatorCompare *)gobject;
	ZakFormValidatorComparePrivate *priv = ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE (validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_validator_compare_finalize (GObject *gobject)
{
	ZakFormValidatorCompare *validator = (ZakFormValidatorCompare *)gobject;
	ZakFormValidatorComparePrivate *priv = ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE (validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
zak_form_validator_compare_validate (ZakFormValidator *validator)
{
	gboolean ret;

	gchar *gc1;
	gchar *gc2;

	gint comp;
	gchar *msg;

	ZakFormValidatorComparePrivate *priv = ZAK_FORM_VALIDATOR_COMPARE_GET_PRIVATE (validator);

	ret = FALSE;

	if (!ZAK_FORM_IS_ELEMENT (priv->v1)
		|| !ZAK_FORM_IS_ELEMENT (priv->v2))
		{
			ret = TRUE;
		}
	else
		{
			gc1 = zak_form_element_get_value (priv->v1);
			gc2 = zak_form_element_get_value (priv->v2);

			comp = g_strcmp0 (gc1, gc2);
			if (comp < 0)
				{
					ret = (priv->type == ZAK_FORM_COMPARE_TYPE_LESSER
					       || priv->type == ZAK_FORM_COMPARE_TYPE_NOT_EQUAL);
				}
			else if (comp == 0)
				{
					ret = (priv->type == ZAK_FORM_COMPARE_TYPE_LESSER_EQUAL
					       || priv->type == ZAK_FORM_COMPARE_TYPE_EQUAL
					       || priv->type == ZAK_FORM_COMPARE_TYPE_GREATER_EQUAL);
				}
			else if (comp > 0)
				{
					ret = (priv->type == ZAK_FORM_COMPARE_TYPE_GREATER
					       || priv->type == ZAK_FORM_COMPARE_TYPE_NOT_EQUAL);
				};

			if (!ret)
				{
					msg = g_strdup_printf (_("«%s» must be %s «%s»"),
					                       zak_form_element_get_long_name (priv->v1),
					                       zak_form_get_compare_type_stringify (priv->type),
					                       zak_form_element_get_long_name (priv->v2));
					zak_form_validator_set_message (validator, msg);
					g_free (msg);
				}
		}

	return ret;
}

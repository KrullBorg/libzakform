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

#include <string.h>

#include <libzakutils/libzakutils.h>

#include "formelementfilter.h"
#include "formelementfilterluc.h"

static void zak_form_element_filter_luc_class_init (ZakFormElementFilterLucClass *class);
static void zak_form_element_filter_luc_init (ZakFormElementFilterLuc *zak_form_element);

static void zak_form_element_filter_luc_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_element_filter_luc_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_element_filter_luc_dispose (GObject *gobject);
static void zak_form_element_filter_luc_finalize (GObject *gobject);

static gboolean zak_form_element_filter_luc_xml_parsing (ZakFormElementFilter *filter, xmlNode *xnode);
static gchar *zak_form_element_filter_luc_filter (ZakFormElementFilter *filter_luc, const gchar *value);

enum
	{
		LOWER,
		UPPER,
		CAMEL
	};

struct _ZakFormElementFilterLuc
{
	ZakFormElementFilter parent_instance;

	/* Other members, including private data. */
};

#define ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_TYPE_ELEMENT_FILTER_LUC, ZakFormElementFilterLucPrivate))

typedef struct _ZakFormElementFilterLucPrivate ZakFormElementFilterLucPrivate;
struct _ZakFormElementFilterLucPrivate
	{
		guint type;
	};

G_DEFINE_TYPE (ZakFormElementFilterLuc, zak_form_element_filter_luc, ZAK_FORM_TYPE_ELEMENT_FILTER)

static void
zak_form_element_filter_luc_class_init (ZakFormElementFilterLucClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	ZakFormElementFilterClass *parent_class = ZAK_FORM_ELEMENT_FILTER_CLASS (class);

	object_class->set_property = zak_form_element_filter_luc_set_property;
	object_class->get_property = zak_form_element_filter_luc_get_property;
	object_class->dispose = zak_form_element_filter_luc_dispose;
	object_class->finalize = zak_form_element_filter_luc_finalize;

	parent_class->xml_parsing = zak_form_element_filter_luc_xml_parsing;
	parent_class->filter = zak_form_element_filter_luc_filter;

	g_type_class_add_private (object_class, sizeof (ZakFormElementFilterLucPrivate));
}

static void
zak_form_element_filter_luc_init (ZakFormElementFilterLuc *zak_form_element)
{
	ZakFormElementFilterLucPrivate *priv = ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE (zak_form_element);
}

/**
 * zak_form_element_filter_luc_new:
 *
 * Returns: the newly created #ZakFormElementFilterLuc object.
 */
ZakFormElementFilterLuc
*zak_form_element_filter_luc_new ()
{
	ZakFormElementFilterLuc *zak_form_element_filter_luc;

	zak_form_element_filter_luc = ZAK_FORM_ELEMENT_FILTER_LUC (g_object_new (zak_form_element_filter_luc_get_type (), NULL));

	return zak_form_element_filter_luc;
}

/**
 * zak_form_element_filter_luc_xml_parsing:
 * @filter:
 * @xnode:
 *
 */
static gboolean
zak_form_element_filter_luc_xml_parsing (ZakFormElementFilter *filter, xmlNode *xnode)
{
	gchar *content;

	ZakFormElementFilterLucPrivate *priv = ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE (filter);

	content = (gchar *)xmlNodeGetContent (xnode);
	if (g_strcmp0 (content, "lower") == 0)
		{
			priv->type = LOWER;
		}
	else if (g_strcmp0 (content, "upper") == 0)
		{
			priv->type = UPPER;
		}
	else if (g_strcmp0 (content, "camel") == 0)
		{
			priv->type = CAMEL;
		}

	return TRUE;
}

/* PRIVATE */
static void
zak_form_element_filter_luc_set_property (GObject *object,
                                          guint property_id,
                                          const GValue *value,
                                          GParamSpec *pspec)
{
	ZakFormElementFilterLuc *zak_form_element = (ZakFormElementFilterLuc *)object;
	ZakFormElementFilterLucPrivate *priv = ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_filter_luc_get_property (GObject *object,
                                          guint property_id,
                                          GValue *value,
                                          GParamSpec *pspec)
{
	ZakFormElementFilterLuc *zak_form_element = (ZakFormElementFilterLuc *)object;
	ZakFormElementFilterLucPrivate *priv = ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_filter_luc_dispose (GObject *gobject)
{
	ZakFormElementFilterLuc *zak_form_element = (ZakFormElementFilterLuc *)gobject;
	ZakFormElementFilterLucPrivate *priv = ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_filter_luc_finalize (GObject *gobject)
{
	ZakFormElementFilterLuc *zak_form_element = (ZakFormElementFilterLuc *)gobject;
	ZakFormElementFilterLucPrivate *priv = ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gchar
*zak_form_element_filter_luc_filter (ZakFormElementFilter *filter_luc,
                                     const gchar *value)
{
	gchar *ret;
	gchar *_value;
	guint i;
	guint l;
	GString *str;

	ZakFormElementFilterLuc *zak_form_element = ZAK_FORM_ELEMENT_FILTER_LUC (filter_luc);
	ZakFormElementFilterLucPrivate *priv = ZAK_FORM_ELEMENT_FILTER_LUC_GET_PRIVATE (zak_form_element);

	g_return_val_if_fail (value != NULL, g_strdup (""));

	_value = g_strdup (value);

	l = strlen (_value);
	switch (priv->type)
		{
		case LOWER:
			for (i = 0; i < l; i++)
				{
					_value[i] = g_ascii_tolower (_value[i]);
				}
			break;

		case UPPER:
			for (i = 0; i < l; i++)
				{
					_value[i] = g_ascii_toupper (_value[i]);
				}
			break;

		case CAMEL:
			str = g_string_new (_value);
			zak_utils_gstring_initial_capital (str);
			g_free (_value);
			_value = g_strdup (str->str);
			g_string_free (str, TRUE);
			break;
		}

	ret = g_strdup (_value);
	g_free (_value);

	return ret;
}

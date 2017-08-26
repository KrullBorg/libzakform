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

#include "formelementfilter.h"
#include "formelementfiltertrim.h"

static void zak_form_element_filter_trim_class_init (ZakFormElementFilterTrimClass *class);
static void zak_form_element_filter_trim_init (ZakFormElementFilterTrim *zak_form_element);

static void zak_form_element_filter_trim_set_property (GObject *object,
                                                       guint property_id,
                                                       const GValue *value,
                                                       GParamSpec *pspec);
static void zak_form_element_filter_trim_get_property (GObject *object,
                                                       guint property_id,
                                                       GValue *value,
                                                       GParamSpec *pspec);

static void zak_form_element_filter_trim_dispose (GObject *gobject);
static void zak_form_element_filter_trim_finalize (GObject *gobject);

static gboolean zak_form_element_filter_trim_xml_parsing (ZakFormElementFilter *filter, xmlNode *xnode);
static gchar *zak_form_element_filter_trim_filter (ZakFormElementFilter *filter_trim, const gchar *value);

struct _ZakFormElementFilterTrim
{
	ZakFormElementFilter parent_instance;

	/* Other members, including private data. */
};

#define ZAK_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_TYPE_ELEMENT_FILTER_TRIM, ZakFormElementFilterTrimPrivate))

typedef struct _ZakFormElementFilterTrimPrivate ZakFormElementFilterTrimPrivate;
struct _ZakFormElementFilterTrimPrivate
	{
		gpointer nothing;
	};

G_DEFINE_TYPE (ZakFormElementFilterTrim, zak_form_element_filter_trim, ZAK_FORM_TYPE_ELEMENT_FILTER)

static void
zak_form_element_filter_trim_class_init (ZakFormElementFilterTrimClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	ZakFormElementFilterClass *parent_class = ZAK_FORM_ELEMENT_FILTER_CLASS (class);

	object_class->set_property = zak_form_element_filter_trim_set_property;
	object_class->get_property = zak_form_element_filter_trim_get_property;
	object_class->dispose = zak_form_element_filter_trim_dispose;
	object_class->finalize = zak_form_element_filter_trim_finalize;

	parent_class->xml_parsing = zak_form_element_filter_trim_xml_parsing;
	parent_class->filter = zak_form_element_filter_trim_filter;

	g_type_class_add_private (object_class, sizeof (ZakFormElementFilterTrimPrivate));
}

static void
zak_form_element_filter_trim_init (ZakFormElementFilterTrim *zak_form_element)
{
	ZakFormElementFilterTrimPrivate *priv = ZAK_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_form_element);
}

/**
 * zak_form_element_filter_trim_new:
 *
 * Returns: the newly created #ZakFormElementFilterTrim object.
 */
ZakFormElementFilterTrim
*zak_form_element_filter_trim_new ()
{
	ZakFormElementFilterTrim *zak_form_element_filter_trim;

	zak_form_element_filter_trim = ZAK_FORM_ELEMENT_FILTER_TRIM (g_object_new (zak_form_element_filter_trim_get_type (), NULL));

	return zak_form_element_filter_trim;
}

/**
 * zak_form_element_filter_trim_xml_parsing:
 * @filter:
 * @xnode:
 *
 */
static gboolean
zak_form_element_filter_trim_xml_parsing (ZakFormElementFilter *filter, xmlNode *xnode)
{
	/* nothing to do */

	/* TODO
	 * add properties: left, right, both
	 */
	return TRUE;
}

/* PRIVATE */
static void
zak_form_element_filter_trim_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementFilterTrim *zak_form_element = (ZakFormElementFilterTrim *)object;
	ZakFormElementFilterTrimPrivate *priv = ZAK_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_filter_trim_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementFilterTrim *zak_form_element = (ZakFormElementFilterTrim *)object;
	ZakFormElementFilterTrimPrivate *priv = ZAK_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_filter_trim_dispose (GObject *gobject)
{
	ZakFormElementFilterTrim *zak_form_element = (ZakFormElementFilterTrim *)gobject;
	ZakFormElementFilterTrimPrivate *priv = ZAK_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_filter_trim_finalize (GObject *gobject)
{
	ZakFormElementFilterTrim *zak_form_element = (ZakFormElementFilterTrim *)gobject;
	ZakFormElementFilterTrimPrivate *priv = ZAK_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gchar
*zak_form_element_filter_trim_filter (ZakFormElementFilter *filter_trim,
                                      const gchar *value)
{
	gchar *ret;
	gchar *_value;

	g_return_val_if_fail (value != NULL, g_strdup (""));

	_value = g_strdup (value);

	ret = g_strdup (g_strstrip (_value));

	return ret;
}

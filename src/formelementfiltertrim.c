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

#include "formelementifilter.h"
#include "formelementfiltertrim.h"

static void zak_cgi_form_element_filter_trim_class_init (ZakCgiFormElementFilterTrimClass *class);
static void zak_cgi_form_element_filter_trim_interface_init (ZakCgiFormElementIFilterInterface *iface);
static void zak_cgi_form_element_filter_trim_init (ZakCgiFormElementFilterTrim *zak_cgi_form_element);

static void zak_cgi_form_element_filter_trim_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_cgi_form_element_filter_trim_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_cgi_form_element_filter_trim_dispose (GObject *gobject);
static void zak_cgi_form_element_filter_trim_finalize (GObject *gobject);

static GValue *zak_cgi_form_element_filter_trim_filter (ZakCgiFormElementIFilter *filter_trim, GValue *value);

struct _ZakCgiFormElementFilterTrim
{
	GObject parent_instance;

	/* Other members, including private data. */
};

#define ZAK_CGI_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_CGI_TYPE_FORM_ELEMENT_FILTER_TRIM, ZakCgiFormElementFilterTrimPrivate))

typedef struct _ZakCgiFormElementFilterTrimPrivate ZakCgiFormElementFilterTrimPrivate;
struct _ZakCgiFormElementFilterTrimPrivate
	{
		gpointer nothing;
	};

G_DEFINE_TYPE_WITH_CODE (ZakCgiFormElementFilterTrim, zak_cgi_form_element_filter_trim, G_TYPE_OBJECT,
						 G_IMPLEMENT_INTERFACE (ZAK_CGI_TYPE_FORM_ELEMENT_IFILTER,
												zak_cgi_form_element_filter_trim_interface_init))

static void
zak_cgi_form_element_filter_trim_class_init (ZakCgiFormElementFilterTrimClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_cgi_form_element_filter_trim_set_property;
	object_class->get_property = zak_cgi_form_element_filter_trim_get_property;
	object_class->dispose = zak_cgi_form_element_filter_trim_dispose;
	object_class->finalize = zak_cgi_form_element_filter_trim_finalize;

	g_type_class_add_private (object_class, sizeof (ZakCgiFormElementFilterTrimPrivate));
}

static void
zak_cgi_form_element_filter_trim_interface_init (ZakCgiFormElementIFilterInterface *iface)
{
	iface->filter = zak_cgi_form_element_filter_trim_filter;
}

static void
zak_cgi_form_element_filter_trim_init (ZakCgiFormElementFilterTrim *zak_cgi_form_element)
{
	ZakCgiFormElementFilterTrimPrivate *priv = ZAK_CGI_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_cgi_form_element);
}

/**
 * zak_cgi_form_element_filter_trim_new:
 *
 * Returns: the newly created #ZakCgiFormElementFilterTrim object.
 */
ZakCgiFormElementFilterTrim
*zak_cgi_form_element_filter_trim_new ()
{
	ZakCgiFormElementFilterTrim *zak_cgi_form_element_filter_trim;

	zak_cgi_form_element_filter_trim = ZAK_CGI_FORM_ELEMENT_FILTER_TRIM (g_object_new (zak_cgi_form_element_filter_trim_get_type (), NULL));

	return zak_cgi_form_element_filter_trim;
}

/* PRIVATE */
static void
zak_cgi_form_element_filter_trim_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakCgiFormElementFilterTrim *zak_cgi_form_element = (ZakCgiFormElementFilterTrim *)object;
	ZakCgiFormElementFilterTrimPrivate *priv = ZAK_CGI_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_cgi_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_cgi_form_element_filter_trim_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakCgiFormElementFilterTrim *zak_cgi_form_element = (ZakCgiFormElementFilterTrim *)object;
	ZakCgiFormElementFilterTrimPrivate *priv = ZAK_CGI_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_cgi_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_cgi_form_element_filter_trim_dispose (GObject *gobject)
{
	ZakCgiFormElementFilterTrim *zak_cgi_form_element = (ZakCgiFormElementFilterTrim *)gobject;
	ZakCgiFormElementFilterTrimPrivate *priv = ZAK_CGI_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_cgi_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_cgi_form_element_filter_trim_finalize (GObject *gobject)
{
	ZakCgiFormElementFilterTrim *zak_cgi_form_element = (ZakCgiFormElementFilterTrim *)gobject;
	ZakCgiFormElementFilterTrimPrivate *priv = ZAK_CGI_FORM_ELEMENT_FILTER_TRIM_GET_PRIVATE (zak_cgi_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static GValue
*zak_cgi_form_element_filter_trim_filter (ZakCgiFormElementIFilter *filter_trim,
										  GValue *value)
{
	GValue *ret;
	gchar *_value;

	ret = g_new0 (GValue, 1);
	g_value_init (ret, G_TYPE_STRING);
	g_value_set_string (ret, g_strdup (""));

	g_return_val_if_fail (value != NULL, ret);

	_value = g_strdup (g_value_get_string (value));

	g_value_set_string (ret, g_strdup (g_strstrip (_value)));

	return ret;
}

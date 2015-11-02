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

#include "formelementfilter.h"

static void zak_form_element_filter_class_init (ZakFormElementFilterClass *class);
static void zak_form_element_filter_init (ZakFormElementFilter *zak_form_element_filter);

static void zak_form_element_filter_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_element_filter_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_element_filter_dispose (GObject *gobject);
static void zak_form_element_filter_finalize (GObject *gobject);

typedef struct
	{
	    gpointer nothing;
	} ZakFormElementFilterPrivate;

G_DEFINE_ABSTRACT_TYPE (ZakFormElementFilter, zak_form_element_filter, G_TYPE_OBJECT)

static void
zak_form_element_filter_class_init (ZakFormElementFilterClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_form_element_filter_set_property;
	object_class->get_property = zak_form_element_filter_get_property;
	object_class->dispose = zak_form_element_filter_dispose;
	object_class->finalize = zak_form_element_filter_finalize;
}

static void
zak_form_element_filter_init (ZakFormElementFilter *zak_form_element_filter)
{
	ZakFormElementFilterPrivate *priv = zak_form_element_filter_get_instance_private (zak_form_element_filter);
}

gchar
*zak_form_element_filter_filter (ZakFormElementFilter *self, const gchar *value)
{
	gchar *ret;

	g_return_val_if_fail (ZAK_FORM_IS_ELEMENT_FILTER (self), g_strdup (""));

	if (ZAK_FORM_ELEMENT_FILTER_GET_CLASS (self)->filter != NULL)
		{
			ret = ZAK_FORM_ELEMENT_FILTER_GET_CLASS (self)->filter (self, value);
		}
	else
		{
			ret = g_strdup ("");
		}
	return ret;
}

/* PRIVATE */
static void
zak_form_element_filter_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementFilter *zak_form_element_filter = (ZakFormElementFilter *)object;
	ZakFormElementFilterPrivate *priv = zak_form_element_filter_get_instance_private (zak_form_element_filter);

	switch (property_id)
		{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_filter_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementFilter *zak_form_element_filter = (ZakFormElementFilter *)object;
	ZakFormElementFilterPrivate *priv = zak_form_element_filter_get_instance_private (zak_form_element_filter);

	switch (property_id)
		{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_filter_dispose (GObject *gobject)
{
	ZakFormElementFilter *zak_form_element_filter = (ZakFormElementFilter *)gobject;
	ZakFormElementFilterPrivate *priv = zak_form_element_filter_get_instance_private (zak_form_element_filter);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_filter_finalize (GObject *gobject)
{
	ZakFormElementFilter *zak_form_element_filter = (ZakFormElementFilter *)gobject;
	ZakFormElementFilterPrivate *priv = zak_form_element_filter_get_instance_private (zak_form_element_filter);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

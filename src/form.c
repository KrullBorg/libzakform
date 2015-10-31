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

#include "form.h"

static void zak_form_form_class_init (ZakFormFormClass *class);
static void zak_form_form_init (ZakFormForm *zak_form_form);

static void zak_form_form_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_form_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_form_dispose (GObject *gobject);
static void zak_form_form_finalize (GObject *gobject);

static GPtrArray *zak_form_form_get_elements (ZakFormForm *zakform);

typedef struct
	{
		GPtrArray *ar_elements;
	} ZakFormFormPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (ZakFormForm, zak_form_form, G_TYPE_OBJECT)

static void
zak_form_form_class_init (ZakFormFormClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_form_form_set_property;
	object_class->get_property = zak_form_form_get_property;
	object_class->dispose = zak_form_form_dispose;
	object_class->finalize = zak_form_form_finalize;

	class->get_elements = zak_form_form_get_elements;
}

static void
zak_form_form_init (ZakFormForm *zak_form_form)
{
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);

	priv->ar_elements = g_ptr_array_new ();
}

/**
 * zak_form_form_add_element:
 * @zakform:
 * @element:
 *
 * Returns: #TRUE if @element is added; FALSE otherwise.
 */
gboolean
zak_form_form_add_element (ZakFormForm *zakform, ZakFormElement *element)
{
	gboolean ret;

	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	g_ptr_array_add (priv->ar_elements, g_object_ref (element));
	ret = TRUE;

	return ret;
}

/**
 * zak_form_form_is_valid:
 * @zakform:
 *
 * Returns:
 */
gboolean
zak_form_form_is_valid (ZakFormForm *zakform)
{
	guint i;

	gboolean ret;

	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	ret = TRUE;

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);
			if (!zak_form_element_is_valid (element))
				{
					ret = FALSE;
				}
		}

	return ret;
}


/* PRIVATE */
static void
zak_form_form_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)object;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_form_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)object;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_form_dispose (GObject *gobject)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)gobject;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_form_finalize (GObject *gobject)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)gobject;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static GPtrArray
*zak_form_form_get_elements (ZakFormForm *zakform)
{
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zakform);

	return priv->ar_elements;
}

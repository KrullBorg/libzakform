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

#include "formelement.h"

enum
{
	PROP_0
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

typedef struct
	{
		gchar *value;
		GPtrArray *pa_filters;
		GPtrArray *pa_validators;
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
}

static void
zak_form_element_init (ZakFormElement *zak_form_element)
{
	ZakFormElementPrivate *priv = zak_form_element_get_instance_private (zak_form_element);

	priv->value = NULL;
	priv->pa_filters = NULL;
	priv->pa_validators = NULL;
}

/**
 * zak_form_element_add_filter:
 * @element:
 * @filter:
 *
 */
void
zak_form_element_add_filter (ZakFormElement *element, ZakFormElementIFilter *filter)
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

			val = zak_form_element_ifilter_filter ((ZakFormElementIFilter *)g_ptr_array_index (priv->pa_filters, i),
													   value);
			zak_form_element_set_value (element, val);
		}
}

/**
 * zak_form_element_set_value:
 * @element:
 * @value:
 *
 */
void
zak_form_element_set_value (ZakFormElement *element, const gchar *value)
{
	ZakFormElementPrivate *priv;

	priv = zak_form_element_get_instance_private (element);

	priv->value = g_strdup (value);
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

	priv = zak_form_element_get_instance_private (element);

	return g_strdup (priv->value);
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

	if (priv->pa_validators != NULL)
		{
			value = zak_form_element_get_value (element);

			for (i = 0; i < priv->pa_validators->len; i++)
				{
					if (!zak_form_element_validator_validate ((ZakFormElementValidator *)g_ptr_array_index (priv->pa_validators, i),
																   value))
						{
							ret = FALSE;
						}
				}
		}

	return ret;
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

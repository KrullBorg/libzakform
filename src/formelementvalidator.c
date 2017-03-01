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

#include "formelementvalidator.h"

enum
	{
		PROP_0,
		PROP_ENABLED,
		PROP_MESSAGE
	};

static void zak_form_element_validator_class_init (ZakFormElementValidatorClass *class);
static void zak_form_element_validator_init (ZakFormElementValidator *zak_form_element_validator);

static void zak_form_element_validator_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_element_validator_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_element_validator_dispose (GObject *gobject);
static void zak_form_element_validator_finalize (GObject *gobject);

typedef struct
	{
		gboolean enabled;
		gchar *message;
	} ZakFormElementValidatorPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (ZakFormElementValidator, zak_form_element_validator, G_TYPE_OBJECT)

static void
zak_form_element_validator_class_init (ZakFormElementValidatorClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_form_element_validator_set_property;
	object_class->get_property = zak_form_element_validator_get_property;
	object_class->dispose = zak_form_element_validator_dispose;
	object_class->finalize = zak_form_element_validator_finalize;

	g_object_class_install_property (object_class, PROP_ENABLED,
									 g_param_spec_boolean ("enabled",
														   "Enabled",
														   "Enabled",
														   TRUE,
														   G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (object_class, PROP_MESSAGE,
									 g_param_spec_string ("message",
														  "Message",
														  "Message",
														  _("Invalid value"),
														  G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

static void
zak_form_element_validator_init (ZakFormElementValidator *zak_form_element_validator)
{
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (zak_form_element_validator);
}

gboolean
zak_form_element_validator_validate (ZakFormElementValidator *self, const gchar *value)
{
	gboolean ret;

	g_return_val_if_fail (ZAK_FORM_IS_ELEMENT_VALIDATOR (self), FALSE);

	ret = TRUE;
	if (zak_form_element_validator_get_enabled (self)
		&& ZAK_FORM_ELEMENT_VALIDATOR_GET_CLASS (self)->validate != NULL)
		{
			ret = ZAK_FORM_ELEMENT_VALIDATOR_GET_CLASS (self)->validate (self, value);
		}
	return ret;
}

/**
 * zak_form_element_validator_set_message:
 * @validator:
 * @message:
 *
 */
void
zak_form_element_validator_set_message (ZakFormElementValidator *validator,
										const gchar *message)
{
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (validator);

	priv->message = g_strdup (message);
}

/**
 * zak_form_element_validator_get_message:
 * @validator:
 *
 * Returns:
 */
gchar
*zak_form_element_validator_get_message (ZakFormElementValidator *validator)
{
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (validator);

    return g_strdup (priv->message);
}

/**
 * zak_form_element_validator_get_enabled:
 * @validator:
 *
 * Returns: whether @validator is enabled.
 */
gboolean
zak_form_element_validator_get_enabled (ZakFormElementValidator *validator)
{
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (validator);

	return priv->enabled;
}

/**
 * zak_form_element_validator_set_enabled:
 * @validator:
 * @enabled:
 *
 */
void
zak_form_element_validator_set_enabled (ZakFormElementValidator *validator, gboolean enabled)
{
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (validator);

	priv->enabled = enabled;
}

/* PRIVATE */
static void
zak_form_element_validator_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementValidator *zak_form_element_validator = (ZakFormElementValidator *)object;
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (zak_form_element_validator);

	switch (property_id)
		{
		case PROP_ENABLED:
			zak_form_element_validator_set_enabled (zak_form_element_validator, g_value_get_boolean (value));
			break;

		case PROP_MESSAGE:
			zak_form_element_validator_set_message (zak_form_element_validator, g_value_dup_string (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_validator_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementValidator *zak_form_element_validator = (ZakFormElementValidator *)object;
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (zak_form_element_validator);

	switch (property_id)
		{
		case PROP_ENABLED:
			g_value_set_boolean (value, zak_form_element_validator_get_enabled (zak_form_element_validator));
			break;

		case PROP_MESSAGE:
			g_value_set_string (value, zak_form_element_validator_get_message (zak_form_element_validator));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_validator_dispose (GObject *gobject)
{
	ZakFormElementValidator *zak_form_element_validator = (ZakFormElementValidator *)gobject;
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (zak_form_element_validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_validator_finalize (GObject *gobject)
{
	ZakFormElementValidator *zak_form_element_validator = (ZakFormElementValidator *)gobject;
	ZakFormElementValidatorPrivate *priv = zak_form_element_validator_get_instance_private (zak_form_element_validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

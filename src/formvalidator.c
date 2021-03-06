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

#include "form.h"
#include "formvalidator.h"

enum
	{
		PROP_0,
		PROP_ID,
		PROP_ENABLED,
		PROP_MESSAGE
	};

static void zak_form_validator_class_init (ZakFormValidatorClass *class);
static void zak_form_validator_init (ZakFormValidator *zak_form_validator);

static void zak_form_validator_set_property (GObject *object,
                                             guint property_id,
                                             const GValue *value,
                                             GParamSpec *pspec);
static void zak_form_validator_get_property (GObject *object,
                                             guint property_id,
                                             GValue *value,
                                             GParamSpec *pspec);

static void zak_form_validator_dispose (GObject *gobject);
static void zak_form_validator_finalize (GObject *gobject);

typedef struct
	{
		gchar *id;
		gboolean enabled;
		gchar *message;
	} ZakFormValidatorPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (ZakFormValidator, zak_form_validator, G_TYPE_OBJECT)

static void
zak_form_validator_class_init (ZakFormValidatorClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_form_validator_set_property;
	object_class->get_property = zak_form_validator_get_property;
	object_class->dispose = zak_form_validator_dispose;
	object_class->finalize = zak_form_validator_finalize;

	g_object_class_install_property (object_class, PROP_ID,
	                                 g_param_spec_string ("id",
	                                                      "Id",
	                                                      "Id",
	                                                      "",
	                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

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
zak_form_validator_init (ZakFormValidator *zak_form_validator)
{
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (zak_form_validator);
}

/**
 * zak_form_validator_xml_parsing:
 * @validator:
 * @xnode:
 * @form:
 *
 * Retuns:
 */
gboolean
zak_form_validator_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, gpointer form)
{
	gboolean ret;

	gchar *prop;

	g_return_val_if_fail (ZAK_FORM_IS_VALIDATOR (validator), FALSE);

	ret = TRUE;

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"id");
	if (prop != NULL)
		{
			zak_form_validator_set_id (validator, prop);
			g_free (prop);
		}

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"message");
	if (prop != NULL)
		{
			zak_form_validator_set_message (validator, prop);
			g_free (prop);
		}

	prop = (gchar *)xmlGetProp (xnode, (const xmlChar *)"disabled");
	if (prop != NULL)
		{
			zak_form_validator_set_enabled (validator, FALSE);
			g_free (prop);
		}

	if (ZAK_FORM_VALIDATOR_GET_CLASS (validator)->xml_parsing != NULL)
		{
			ret = ZAK_FORM_VALIDATOR_GET_CLASS (validator)->xml_parsing (validator, xnode, form);
		}

	return ret;
}

/**
 * zak_form_validator_set_id:
 * @validator:
 * @id:
 *
 */
void
zak_form_validator_set_id (ZakFormValidator *validator,
                           const gchar *id)
{
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (validator);

	if (id == NULL)
		{
			priv->id = g_strdup ("");
		}
	else
		{
			priv->id = g_strdup (id);
		}
}

/**
 * zak_form_validator_get_id:
 * @validator:
 *
 * Returns:
 */
gchar
*zak_form_validator_get_id (ZakFormValidator *validator)
{
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (validator);

	return g_strdup (priv->id);
}


gboolean
zak_form_validator_validate (ZakFormValidator *self)
{
	gboolean ret;

	g_return_val_if_fail (ZAK_FORM_IS_VALIDATOR (self), FALSE);

	ret = TRUE;
	if (zak_form_validator_get_enabled (self)
		&& ZAK_FORM_VALIDATOR_GET_CLASS (self)->validate != NULL)
		{
			ret = ZAK_FORM_VALIDATOR_GET_CLASS (self)->validate (self);
		}
	return ret;
}

/**
 * zak_form_validator_set_message:
 * @validator:
 * @message:
 *
 */
void
zak_form_validator_set_message (ZakFormValidator *validator,
                                const gchar *message)
{
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (validator);

	priv->message = g_strdup (message);
}

/**
 * zak_form_validator_get_message:
 * @validator:
 *
 * Returns:
 */
gchar
*zak_form_validator_get_message (ZakFormValidator *validator)
{
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (validator);

	return g_strdup (priv->message);
}

/**
 * zak_form_validator_get_enabled:
 * @validator:
 *
 * Returns: whether @validator is enabled.
 */
gboolean
zak_form_validator_get_enabled (ZakFormValidator *validator)
{
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (validator);

	return priv->enabled;
}

/**
 * zak_form_validator_set_enabled:
 * @validator:
 * @enabled:
 *
 */
void
zak_form_validator_set_enabled (ZakFormValidator *validator, gboolean enabled)
{
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (validator);

	priv->enabled = enabled;
}

/* PRIVATE */
static void
zak_form_validator_set_property (GObject *object,
                                 guint property_id,
                                 const GValue *value,
                                 GParamSpec *pspec)
{
	ZakFormValidator *zak_form_validator = (ZakFormValidator *)object;
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (zak_form_validator);

	switch (property_id)
		{
		case PROP_ID:
		    zak_form_validator_set_id (zak_form_validator, g_value_dup_string (value));
			break;

		case PROP_ENABLED:
		    zak_form_validator_set_enabled (zak_form_validator, g_value_get_boolean (value));
			break;

		case PROP_MESSAGE:
		    zak_form_validator_set_message (zak_form_validator, g_value_dup_string (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_validator_get_property (GObject *object,
                                 guint property_id,
                                 GValue *value,
                                 GParamSpec *pspec)
{
	ZakFormValidator *zak_form_validator = (ZakFormValidator *)object;
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (zak_form_validator);

	switch (property_id)
		{
		case PROP_ID:
			g_value_set_string (value, zak_form_validator_get_id (zak_form_validator));
			break;

		case PROP_ENABLED:
			g_value_set_boolean (value, zak_form_validator_get_enabled (zak_form_validator));
			break;

		case PROP_MESSAGE:
			g_value_set_string (value, zak_form_validator_get_message (zak_form_validator));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_validator_dispose (GObject *gobject)
{
	ZakFormValidator *zak_form_validator = (ZakFormValidator *)gobject;
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (zak_form_validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_validator_finalize (GObject *gobject)
{
	ZakFormValidator *zak_form_validator = (ZakFormValidator *)gobject;
	ZakFormValidatorPrivate *priv = zak_form_validator_get_instance_private (zak_form_validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

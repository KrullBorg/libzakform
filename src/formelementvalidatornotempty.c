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

#include "formelementvalidator.h"
#include "formelementvalidatornotempty.h"

enum
	{
		PROP_0,
		PROP_AS_EMPTY_STRING
	};

static void zak_form_element_validator_notempty_class_init (ZakFormElementValidatorNotemptyClass *class);
static void zak_form_element_validator_notempty_init (ZakFormElementValidatorNotempty *zak_form_element);

static void zak_form_element_validator_notempty_set_property (GObject *object,
                                                              guint property_id,
                                                              const GValue *value,
                                                              GParamSpec *pspec);
static void zak_form_element_validator_notempty_get_property (GObject *object,
                                                              guint property_id,
                                                              GValue *value,
                                                              GParamSpec *pspec);

static void zak_form_element_validator_notempty_dispose (GObject *gobject);
static void zak_form_element_validator_notempty_finalize (GObject *gobject);

static gboolean zak_form_element_validator_notempty_xml_parsing (ZakFormElementValidator *validator, xmlNode *xnode);
static gboolean zak_form_element_validator_notempty_validate (ZakFormElementValidator *validator_notempty, const gchar *value);

struct _ZakFormElementValidatorNotempty
{
	ZakFormElementValidator parent_instance;

	/* Other members, including private data. */
};

#define ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_TYPE_ELEMENT_VALIDATOR_NOTEMPTY, ZakFormElementValidatorNotemptyPrivate))

typedef struct _ZakFormElementValidatorNotemptyPrivate ZakFormElementValidatorNotemptyPrivate;
struct _ZakFormElementValidatorNotemptyPrivate
	{
		gchar *as_empty_string;
	};

G_DEFINE_TYPE (ZakFormElementValidatorNotempty, zak_form_element_validator_notempty, ZAK_FORM_TYPE_ELEMENT_VALIDATOR)

static void
zak_form_element_validator_notempty_class_init (ZakFormElementValidatorNotemptyClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	ZakFormElementValidatorClass *parent_class = ZAK_FORM_ELEMENT_VALIDATOR_CLASS (class);

	object_class->set_property = zak_form_element_validator_notempty_set_property;
	object_class->get_property = zak_form_element_validator_notempty_get_property;
	object_class->dispose = zak_form_element_validator_notempty_dispose;
	object_class->finalize = zak_form_element_validator_notempty_finalize;

	parent_class->xml_parsing = zak_form_element_validator_notempty_xml_parsing;
	parent_class->validate = zak_form_element_validator_notempty_validate;

	g_type_class_add_private (object_class, sizeof (ZakFormElementValidatorNotemptyPrivate));

	g_object_class_install_property (object_class, PROP_AS_EMPTY_STRING,
	                                 g_param_spec_string ("as-empty-string",
	                                                      "As empty string",
	                                                      "As empty string",
	                                                      "",
	                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

static void
zak_form_element_validator_notempty_init (ZakFormElementValidatorNotempty *zak_form_element)
{
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (zak_form_element);
}

/**
 * zak_form_element_validator_notempty_new:
 *
 * Returns: the newly created #ZakFormElementValidatorNotempty object.
 */
ZakFormElementValidatorNotempty
*zak_form_element_validator_notempty_new ()
{
	ZakFormElementValidatorNotempty *zak_form_element_validator_notempty;

	zak_form_element_validator_notempty = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY (g_object_new (zak_form_element_validator_notempty_get_type (), NULL));

	return zak_form_element_validator_notempty;
}

/**
 * zak_form_element_validator_notempty_xml_parsing:
 * @validator:
 * @xnode:
 *
 */
static gboolean
zak_form_element_validator_notempty_xml_parsing (ZakFormElementValidator *validator, xmlNode *xnode)
{
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);

	if (priv->as_empty_string != NULL)
		{
			g_free (priv->as_empty_string);
		}
	if (xmlGetProp (xnode, (xmlChar *)"empty_string") != NULL)
		{
			priv->as_empty_string = g_strdup ((gchar *)xmlGetProp (xnode, (xmlChar *)"empty_string"));
		}
	else
		{
			priv->as_empty_string = g_strdup ((gchar *)xmlNodeGetContent (xnode));
		}

	return TRUE;
}

/**
 * zak_form_element_validator_notempty_set_as_empty_string:
 * @validator:
 * @as_empty_string:
 *
 */
void
zak_form_element_validator_notempty_set_as_empty_string (ZakFormElementValidatorNotempty *validator, const gchar *as_empty_string)
{
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);

	if (priv->as_empty_string != NULL)
		{
			g_free (priv->as_empty_string);
		}
	if (as_empty_string == NULL)
		{
			priv->as_empty_string = g_strdup ("");
		}
	else
		{
			priv->as_empty_string = g_strdup (as_empty_string);
		}
}

/**
 * zak_form_element_validator_notempty_get_as_empty_string:
 * @validator:
 *
 * Returns:
 */
gchar
*zak_form_element_validator_notempty_get_as_empty_string (ZakFormElementValidatorNotempty *validator)
{
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);

	return g_strdup (priv->as_empty_string);
}

/* PRIVATE */
static void
zak_form_element_validator_notempty_set_property (GObject *object,
                                                  guint property_id,
                                                  const GValue *value,
                                                  GParamSpec *pspec)
{
	ZakFormElementValidatorNotempty *validator = (ZakFormElementValidatorNotempty *)object;
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);

	switch (property_id)
		{
		case PROP_AS_EMPTY_STRING:
			zak_form_element_validator_notempty_set_as_empty_string (validator, g_value_get_string (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_validator_notempty_get_property (GObject *object,
                                                  guint property_id,
                                                  GValue *value,
                                                  GParamSpec *pspec)
{
	ZakFormElementValidatorNotempty *validator = (ZakFormElementValidatorNotempty *)object;
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);

	switch (property_id)
		{
		case PROP_AS_EMPTY_STRING:
			g_value_set_string (value, zak_form_element_validator_notempty_get_as_empty_string (validator));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_validator_notempty_dispose (GObject *gobject)
{
	ZakFormElementValidatorNotempty *validator = (ZakFormElementValidatorNotempty *)gobject;
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_validator_notempty_finalize (GObject *gobject)
{
	ZakFormElementValidatorNotempty *validator = (ZakFormElementValidatorNotempty *)gobject;
	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
zak_form_element_validator_notempty_validate (ZakFormElementValidator *validator,
                                              const gchar *value)
{
	gboolean ret;

	ZakFormElementValidatorNotemptyPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (validator);

	g_return_val_if_fail (value != NULL, FALSE);

	ret = (g_strcmp0 (value, priv->as_empty_string) != 0);

	return ret;
}

/*
 * Copyright (C) 2015-2016 Andrea Zagli <azagli@libero.it>
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
#include "formelementvalidatorregex.h"

enum
{
	PROP_0,
	PROP_REGEX
};

static void zak_form_element_validator_regex_class_init (ZakFormElementValidatorRegexClass *class);
static void zak_form_element_validator_regex_init (ZakFormElementValidatorRegex *zak_form_element);

static void zak_form_element_validator_regex_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_element_validator_regex_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_element_validator_regex_dispose (GObject *gobject);
static void zak_form_element_validator_regex_finalize (GObject *gobject);

static gboolean zak_form_element_validator_regex_validate (ZakFormElementValidator *validator_regex, const gchar *value);

struct _ZakFormElementValidatorRegex
{
	ZakFormElementValidator parent_instance;

	/* Other members, including private data. */
};

#define ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_TYPE_FORM_ELEMENT_VALIDATOR_REGEX, ZakFormElementValidatorRegexPrivate))

typedef struct _ZakFormElementValidatorRegexPrivate ZakFormElementValidatorRegexPrivate;
struct _ZakFormElementValidatorRegexPrivate
	{
		gchar *regex;
	};

G_DEFINE_TYPE (ZakFormElementValidatorRegex, zak_form_element_validator_regex, ZAK_FORM_TYPE_ELEMENT_VALIDATOR)

static void
zak_form_element_validator_regex_class_init (ZakFormElementValidatorRegexClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	ZakFormElementValidatorClass *parent_class = ZAK_FORM_ELEMENT_VALIDATOR_CLASS (class);

	object_class->set_property = zak_form_element_validator_regex_set_property;
	object_class->get_property = zak_form_element_validator_regex_get_property;
	object_class->dispose = zak_form_element_validator_regex_dispose;
	object_class->finalize = zak_form_element_validator_regex_finalize;

	parent_class->validate = zak_form_element_validator_regex_validate;

	g_type_class_add_private (object_class, sizeof (ZakFormElementValidatorRegexPrivate));

	g_object_class_install_property (object_class, PROP_REGEX,
	                                 g_param_spec_string ("regex",
	                                                      "Regex",
	                                                      "Regex",
	                                                      "",
	                                                      G_PARAM_READWRITE));
}

static void
zak_form_element_validator_regex_init (ZakFormElementValidatorRegex *zak_form_element)
{
	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_form_element);

	priv->regex = NULL;
}

/**
 * zak_form_element_validator_regex_new:
 *
 * Returns: the newly created #ZakFormElementValidatorRegex object.
 */
ZakFormElementValidatorRegex
*zak_form_element_validator_regex_new ()
{
	ZakFormElementValidatorRegex *zak_form_element_validator_regex;

	zak_form_element_validator_regex = ZAK_FORM_ELEMENT_VALIDATOR_REGEX (g_object_new (zak_form_element_validator_regex_get_type (), NULL));

	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_form_element_validator_regex);

	return zak_form_element_validator_regex;
}

/**
 * zak_form_element_validator_regex_xml_parsing:
 * @validator:
 * @xnode:
 *
 */
gboolean
zak_form_element_validator_regex_xml_parsing (ZakFormElementValidator *validator, xmlNode *xnode)
{
	zak_form_element_validator_regex_set_regex (ZAK_FORM_ELEMENT_VALIDATOR_REGEX (validator), (gchar *)xmlNodeGetContent (xnode));

	return TRUE;
}

/**
 * zak_form_element_validator_regex_set_regex:
 * @validaotr:
 * @regex:
 *
 */
void
zak_form_element_validator_regex_set_regex (ZakFormElementValidatorRegex *validator, const gchar *regex)
{
	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (validator);

	if (priv->regex != NULL)
		{
			g_free (priv->regex);
		}
	priv->regex = g_strdup (regex);
}

/**
 * zak_form_element_validator_regex_get_regex:
 * @validator:
 *
 */
gchar
*zak_form_element_validator_regex_get_regex (ZakFormElementValidatorRegex *validator)
{
	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (validator);

	return g_strdup (priv->regex);
}

/* PRIVATE */
static void
zak_form_element_validator_regex_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementValidatorRegex *zak_form_element = (ZakFormElementValidatorRegex *)object;
	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
		case PROP_REGEX:
		    zak_form_element_validator_regex_set_regex (zak_form_element, g_value_dup_string (value));
			break;

			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_validator_regex_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementValidatorRegex *zak_form_element = (ZakFormElementValidatorRegex *)object;
	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_form_element);

	switch (property_id)
		{
		case PROP_REGEX:
			g_value_set_string (value, zak_form_element_validator_regex_get_regex (zak_form_element));
			break;

			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_validator_regex_dispose (GObject *gobject)
{
	ZakFormElementValidatorRegex *zak_form_element = (ZakFormElementValidatorRegex *)gobject;
	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_validator_regex_finalize (GObject *gobject)
{
	ZakFormElementValidatorRegex *zak_form_element = (ZakFormElementValidatorRegex *)gobject;
	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
zak_form_element_validator_regex_validate (ZakFormElementValidator *validator_regex,
											   const gchar *value)
{
	gboolean ret;

	GRegex *regex;
	GError *error;

	g_return_val_if_fail (value != NULL, FALSE);

	ZakFormElementValidatorRegexPrivate *priv = ZAK_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (validator_regex);

	if (priv->regex == NULL)
		{
			return TRUE;
		}

	error = NULL;
	regex = g_regex_new (priv->regex, 0, 0, &error);
	if (regex == NULL
		|| error != NULL)
		{
			g_warning ("Error on creating regex: %s.",
					   error->message != NULL ? error->message : "no details");
			return FALSE;
		}

	ret = g_regex_match ((const GRegex *)regex, value, 0, NULL);
	if (!ret)
		{
			g_warning ("Value «%s» not valid for regex «%s».", value, priv->regex);
		}

	return ret;
}

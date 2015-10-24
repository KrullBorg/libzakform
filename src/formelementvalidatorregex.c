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

#include <syslog.h>

#include "formelementivalidator.h"
#include "formelementvalidatorregex.h"

static void zak_cgi_form_element_validator_regex_class_init (ZakCgiFormElementValidatorRegexClass *class);
static void zak_cgi_form_element_validator_regex_interface_init (ZakCgiFormElementIValidatorInterface *iface);
static void zak_cgi_form_element_validator_regex_init (ZakCgiFormElementValidatorRegex *zak_cgi_form_element);

static void zak_cgi_form_element_validator_regex_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_cgi_form_element_validator_regex_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_cgi_form_element_validator_regex_dispose (GObject *gobject);
static void zak_cgi_form_element_validator_regex_finalize (GObject *gobject);

static gboolean zak_cgi_form_element_validator_regex_validate (ZakCgiFormElementIValidator *validator_regex, GValue *value);

struct _ZakCgiFormElementValidatorRegex
{
	GObject parent_instance;

	/* Other members, including private data. */
};

#define ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_CGI_TYPE_FORM_ELEMENT_VALIDATOR_REGEX, ZakCgiFormElementValidatorRegexPrivate))

typedef struct _ZakCgiFormElementValidatorRegexPrivate ZakCgiFormElementValidatorRegexPrivate;
struct _ZakCgiFormElementValidatorRegexPrivate
	{
		gchar *regex;
	};

G_DEFINE_TYPE_WITH_CODE (ZakCgiFormElementValidatorRegex, zak_cgi_form_element_validator_regex, G_TYPE_OBJECT,
						 G_IMPLEMENT_INTERFACE (ZAK_CGI_TYPE_FORM_ELEMENT_IVALIDATOR,
												zak_cgi_form_element_validator_regex_interface_init))

static void
zak_cgi_form_element_validator_regex_class_init (ZakCgiFormElementValidatorRegexClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_cgi_form_element_validator_regex_set_property;
	object_class->get_property = zak_cgi_form_element_validator_regex_get_property;
	object_class->dispose = zak_cgi_form_element_validator_regex_dispose;
	object_class->finalize = zak_cgi_form_element_validator_regex_finalize;

	g_type_class_add_private (object_class, sizeof (ZakCgiFormElementValidatorRegexPrivate));
}

static void
zak_cgi_form_element_validator_regex_interface_init (ZakCgiFormElementIValidatorInterface *iface)
{
	iface->validate = zak_cgi_form_element_validator_regex_validate;
}

static void
zak_cgi_form_element_validator_regex_init (ZakCgiFormElementValidatorRegex *zak_cgi_form_element)
{
	ZakCgiFormElementValidatorRegexPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_cgi_form_element);

	priv->regex = NULL;
}

/**
 * zak_cgi_form_element_validator_regex_new:
 * @regex:
 *
 * Returns: the newly created #ZakCgiFormElementValidatorRegex object.
 */
ZakCgiFormElementValidatorRegex
*zak_cgi_form_element_validator_regex_new (const gchar *regex)
{
	ZakCgiFormElementValidatorRegex *zak_cgi_form_element_validator_regex;

	zak_cgi_form_element_validator_regex = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX (g_object_new (zak_cgi_form_element_validator_regex_get_type (), NULL));

	ZakCgiFormElementValidatorRegexPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_cgi_form_element_validator_regex);

	if (regex != NULL)
		{
			priv->regex = g_strdup (regex);
		}

	return zak_cgi_form_element_validator_regex;
}

/* PRIVATE */
static void
zak_cgi_form_element_validator_regex_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakCgiFormElementValidatorRegex *zak_cgi_form_element = (ZakCgiFormElementValidatorRegex *)object;
	ZakCgiFormElementValidatorRegexPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_cgi_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_cgi_form_element_validator_regex_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakCgiFormElementValidatorRegex *zak_cgi_form_element = (ZakCgiFormElementValidatorRegex *)object;
	ZakCgiFormElementValidatorRegexPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_cgi_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_cgi_form_element_validator_regex_dispose (GObject *gobject)
{
	ZakCgiFormElementValidatorRegex *zak_cgi_form_element = (ZakCgiFormElementValidatorRegex *)gobject;
	ZakCgiFormElementValidatorRegexPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_cgi_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_cgi_form_element_validator_regex_finalize (GObject *gobject)
{
	ZakCgiFormElementValidatorRegex *zak_cgi_form_element = (ZakCgiFormElementValidatorRegex *)gobject;
	ZakCgiFormElementValidatorRegexPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (zak_cgi_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
zak_cgi_form_element_validator_regex_validate (ZakCgiFormElementIValidator *validator_regex,
											   GValue *value)
{
	gboolean ret;

	GRegex *regex;
	GError *error;

	ZakCgiFormElementValidatorRegexPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_REGEX_GET_PRIVATE (validator_regex);

	if (G_VALUE_HOLDS (value, G_TYPE_STRING))
		{
			error = NULL;
			regex = g_regex_new (priv->regex, 0, 0, &error);
			if (regex == NULL
				|| error != NULL)
				{
					syslog (LOG_MAKEPRI(LOG_SYSLOG, LOG_DEBUG), "Error on creating regex: %s.",
							error->message != NULL ? error->message : "no details");
					return FALSE;
				}

			ret = g_regex_match ((const GRegex *)regex, g_value_get_string (value), 0, NULL);
		}
	else
		{
			ret = FALSE;
		}

	return ret;
}

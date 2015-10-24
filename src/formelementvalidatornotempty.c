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

#include "formelementivalidator.h"
#include "formelementvalidatornotempty.h"

static void zak_cgi_form_element_validator_notempty_class_init (ZakCgiFormElementValidatorNotemptyClass *class);
static void zak_cgi_form_element_validator_notempty_interface_init (ZakCgiFormElementIValidatorInterface *iface);
static void zak_cgi_form_element_validator_notempty_init (ZakCgiFormElementValidatorNotempty *zak_cgi_form_element);

static void zak_cgi_form_element_validator_notempty_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_cgi_form_element_validator_notempty_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_cgi_form_element_validator_notempty_dispose (GObject *gobject);
static void zak_cgi_form_element_validator_notempty_finalize (GObject *gobject);

static gboolean zak_cgi_form_element_validator_notempty_validate (ZakCgiFormElementIValidator *validator_notempty, GValue *value);

struct _ZakCgiFormElementValidatorNotempty
{
	GObject parent_instance;

	/* Other members, including private data. */
};

#define ZAK_CGI_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_CGI_TYPE_FORM_ELEMENT_VALIDATOR_NOTEMPTY, ZakCgiFormElementValidatorNotemptyPrivate))

typedef struct _ZakCgiFormElementValidatorNotemptyPrivate ZakCgiFormElementValidatorNotemptyPrivate;
struct _ZakCgiFormElementValidatorNotemptyPrivate
	{
		gpointer nothing;
	};

G_DEFINE_TYPE_WITH_CODE (ZakCgiFormElementValidatorNotempty, zak_cgi_form_element_validator_notempty, G_TYPE_OBJECT,
						 G_IMPLEMENT_INTERFACE (ZAK_CGI_TYPE_FORM_ELEMENT_IVALIDATOR,
												zak_cgi_form_element_validator_notempty_interface_init))

static void
zak_cgi_form_element_validator_notempty_class_init (ZakCgiFormElementValidatorNotemptyClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_cgi_form_element_validator_notempty_set_property;
	object_class->get_property = zak_cgi_form_element_validator_notempty_get_property;
	object_class->dispose = zak_cgi_form_element_validator_notempty_dispose;
	object_class->finalize = zak_cgi_form_element_validator_notempty_finalize;

	g_type_class_add_private (object_class, sizeof (ZakCgiFormElementValidatorNotemptyPrivate));
}

static void
zak_cgi_form_element_validator_notempty_interface_init (ZakCgiFormElementIValidatorInterface *iface)
{
	iface->validate = zak_cgi_form_element_validator_notempty_validate;
}

static void
zak_cgi_form_element_validator_notempty_init (ZakCgiFormElementValidatorNotempty *zak_cgi_form_element)
{
	ZakCgiFormElementValidatorNotemptyPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (zak_cgi_form_element);
}

/**
 * zak_cgi_form_element_validator_notempty_new:
 *
 * Returns: the newly created #ZakCgiFormElementValidatorNotempty object.
 */
ZakCgiFormElementValidatorNotempty
*zak_cgi_form_element_validator_notempty_new ()
{
	ZakCgiFormElementValidatorNotempty *zak_cgi_form_element_validator_notempty;

	zak_cgi_form_element_validator_notempty = ZAK_CGI_FORM_ELEMENT_VALIDATOR_NOTEMPTY (g_object_new (zak_cgi_form_element_validator_notempty_get_type (), NULL));

	return zak_cgi_form_element_validator_notempty;
}

/* PRIVATE */
static void
zak_cgi_form_element_validator_notempty_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakCgiFormElementValidatorNotempty *zak_cgi_form_element = (ZakCgiFormElementValidatorNotempty *)object;
	ZakCgiFormElementValidatorNotemptyPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (zak_cgi_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_cgi_form_element_validator_notempty_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakCgiFormElementValidatorNotempty *zak_cgi_form_element = (ZakCgiFormElementValidatorNotempty *)object;
	ZakCgiFormElementValidatorNotemptyPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (zak_cgi_form_element);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_cgi_form_element_validator_notempty_dispose (GObject *gobject)
{
	ZakCgiFormElementValidatorNotempty *zak_cgi_form_element = (ZakCgiFormElementValidatorNotempty *)gobject;
	ZakCgiFormElementValidatorNotemptyPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (zak_cgi_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_cgi_form_element_validator_notempty_finalize (GObject *gobject)
{
	ZakCgiFormElementValidatorNotempty *zak_cgi_form_element = (ZakCgiFormElementValidatorNotempty *)gobject;
	ZakCgiFormElementValidatorNotemptyPrivate *priv = ZAK_CGI_FORM_ELEMENT_VALIDATOR_NOTEMPTY_GET_PRIVATE (zak_cgi_form_element);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
zak_cgi_form_element_validator_notempty_validate (ZakCgiFormElementIValidator *validator_notempty,
										  GValue *value)
{
	gboolean ret;

	g_return_val_if_fail (value != NULL, FALSE);

	ret = (g_strcmp0 (g_value_get_string (value), "") != 0);

	return ret;
}

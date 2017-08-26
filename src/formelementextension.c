/*
 * Copyright (C) 2017 Andrea Zagli <azagli@libero.it>
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

#include <locale.h>

#include <libzakutils/libzakutils.h>

#include "formelementextension.h"

enum
{
	PROP_0
};

static void zak_form_element_extension_class_init (ZakFormElementExtensionClass *class);
static void zak_form_element_extension_init (ZakFormElementExtension *zak_form_element);

static void zak_form_element_extension_construct (ZakFormElementExtension *element,
                                        const gchar *id,
                                        GHashTable *ht_attrs);

static void zak_form_element_extension_set_property (GObject *object,
                                           guint property_id,
                                           const GValue *value,
                                           GParamSpec *pspec);
static void zak_form_element_extension_get_property (GObject *object,
                                           guint property_id,
                                           GValue *value,
                                           GParamSpec *pspec);

static void zak_form_element_extension_dispose (GObject *gobject);
static void zak_form_element_extension_finalize (GObject *gobject);

typedef struct
	{
		gpointer useless;
	} ZakFormElementExtensionPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (ZakFormElementExtension, zak_form_element_extension, G_TYPE_OBJECT)

static void
zak_form_element_extension_class_init (ZakFormElementExtensionClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_form_element_extension_set_property;
	object_class->get_property = zak_form_element_extension_get_property;
	object_class->dispose = zak_form_element_extension_dispose;
	object_class->finalize = zak_form_element_extension_finalize;
}

static void
zak_form_element_extension_init (ZakFormElementExtension *zak_form_element)
{
	ZakFormElementExtensionPrivate *priv = zak_form_element_extension_get_instance_private (zak_form_element);

}

/**
 * zak_form_element_extension_xml_parsing:
 * @element:
 * @xmlnode:
 *
 */
void
zak_form_element_extension_xml_parsing (ZakFormElementExtension *element, xmlNode *xmlnode)
{
	if (ZAK_FORM_ELEMENT_EXTENSION_GET_CLASS (element)->xml_parsing != NULL)
		{
			ZAK_FORM_ELEMENT_EXTENSION_GET_CLASS (element)->xml_parsing (element, xmlnode);
		}
}

/* PRIVATE */
static void
zak_form_element_extension_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementExtension *zak_form_element_extension = (ZakFormElementExtension *)object;
	ZakFormElementExtensionPrivate *priv = zak_form_element_extension_get_instance_private (zak_form_element_extension);

	switch (property_id)
		{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_extension_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementExtension *zak_form_element_extension = (ZakFormElementExtension *)object;
	ZakFormElementExtensionPrivate *priv = zak_form_element_extension_get_instance_private (zak_form_element_extension);

	switch (property_id)
		{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
		}
}

static void
zak_form_element_extension_dispose (GObject *gobject)
{
	ZakFormElementExtension *zak_form_element_extension = (ZakFormElementExtension *)gobject;
	ZakFormElementExtensionPrivate *priv = zak_form_element_extension_get_instance_private (zak_form_element_extension);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_extension_finalize (GObject *gobject)
{
	ZakFormElementExtension *zak_form_element_extension = (ZakFormElementExtension *)gobject;
	ZakFormElementExtensionPrivate *priv = zak_form_element_extension_get_instance_private (zak_form_element_extension);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

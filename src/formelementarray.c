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

#include <glib/gi18n-lib.h>

#include <libzakutils/libzakutils.h>

#include "commons.h"
#include "formelementarray.h"

static void zak_form_element_array_class_init (ZakFormElementArrayClass *class);
static void zak_form_element_array_init (ZakFormElementArray *zak_form_element_array);

static void zak_form_element_array_set_property (GObject *object,
                                                          guint property_id,
                                                          const GValue *value,
                                                          GParamSpec *pspec);
static void zak_form_element_array_get_property (GObject *object,
                                                          guint property_id,
                                                          GValue *value,
                                                          GParamSpec *pspec);

static void zak_form_element_array_dispose (GObject *gobject);
static void zak_form_element_array_finalize (GObject *gobject);

static GValue *zak_form_element_array_get_value (ZakFormElementArray *element);
static gboolean zak_form_element_array_set_value (ZakFormElementArray *element, GValue *value);
static void zak_form_element_array_set_as_original_value (ZakFormElement *element);
static void zak_form_element_array_clear (ZakFormElement *element);

#define ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_TYPE_ELEMENT_ARRAY, ZakFormElementArrayPrivate))

typedef struct _ZakFormElementArrayPrivate ZakFormElementArrayPrivate;
struct _ZakFormElementArrayPrivate
	{
		GPtrArray *ar_elements;
	};

G_DEFINE_TYPE (ZakFormElementArray, zak_form_element_array, ZAK_FORM_TYPE_ELEMENT)

static void
zak_form_element_array_class_init (ZakFormElementArrayClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormElementClass *elem_class = ZAK_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_element_array_set_property;
	object_class->get_property = zak_form_element_array_get_property;
	object_class->dispose = zak_form_element_array_dispose;
	object_class->finalize = zak_form_element_array_finalize;

	elem_class->get_value = zak_form_element_array_get_value;
	elem_class->set_value = zak_form_element_array_set_value;
	elem_class->set_as_original_value = zak_form_element_array_set_as_original_value;
	elem_class->clear = zak_form_element_array_clear;

	elem_class->xml_parsing = zak_form_element_array_xml_parsing;

	g_type_class_add_private (object_class, sizeof (ZakFormElementArrayPrivate));

	/**
	 * ZakFormElementArray::element-add:
	 * @element:
	 *
	 */
	klass->element_added_signal_id  = g_signal_new ("element-added",
	                                                G_TYPE_FROM_CLASS (object_class),
	                                                G_SIGNAL_RUN_LAST,
	                                                0,
	                                                NULL,
	                                                NULL,
	                                                g_cclosure_marshal_VOID__OBJECT,
	                                                G_TYPE_NONE,
	                                                1, G_TYPE_OBJECT);
}

static void
zak_form_element_array_init (ZakFormElementArray *zak_form_element_array)
{
	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (zak_form_element_array);

	priv->ar_elements = g_ptr_array_new ();
}

/**
 * zak_form_element_array_new:
 *
 * Returns: the newly created #ZakFormElementArray object.
 */
ZakFormElement
*zak_form_element_array_new ()
{
	ZakFormElementArray *zak_form_element_array;

	zak_form_element_array = ZAK_FORM_ELEMENT_ARRAY (g_object_new (zak_form_element_array_get_type (), NULL));

	return ZAK_FORM_ELEMENT (zak_form_element_array);
}

/**
 * zak_form_element_array_xml_parsing:
 * @element:
 * @xmlnode:
 *
 */
void
zak_form_element_array_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	xmlNode *cur;
	xmlNode *cur_clean;

	gchar *type;

	ZakFormElement *element_to_add;

	ZakFormElementConstructorFunc element_constructor;

	cur = xmlnode->children;
	while (cur != NULL)
		{
			if (xmlStrEqual (cur->name, (const xmlChar *)"signal-element-added"))
				{
					GModule *module;
					GCallback func;
					gchar *handler_name;

					module = g_module_open (NULL, G_MODULE_BIND_LAZY);
					if (module != NULL)
						{
							handler_name = (gchar *)xmlNodeGetContent (cur);

							if (!g_module_symbol (module, handler_name, (gpointer)&func))
								{
									g_warning (_("Could not find signal handler '%s'."), handler_name);
								}
							else
								{
									g_signal_connect (element, "element-added", func, NULL);
								}

							g_free (handler_name);
						}
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"element"))
				{
					element_to_add = NULL;

					type = (gchar *)xmlGetProp (cur, (const xmlChar *)"type");

					element_constructor = zak_form_get_form_element (type);
					if (element_constructor != NULL)
						{
							element_to_add = element_constructor ();
							if (element_to_add != NULL)
								{
									zak_form_element_array_add_element (element, element_to_add);

									cur_clean = xmlCopyNode (cur, 1);
									zak_form_element_xml_parsing (element_to_add, cur_clean);

									xmlUnlinkNode (cur_clean);
									xmlFreeNode (cur_clean);
								}
						}
					else
						{
							g_warning (_("Unknown element type «%s»."), type);
						}
				}

			cur = cur->next;
		}
}

/**
 * zak_form_element_array_add_element:
 * @element:
 * @element_to_add:
 *
 */
void
zak_form_element_array_add_element (ZakFormElement *element, ZakFormElement *element_to_add)
{
	ZakFormElementArrayClass *klass = ZAK_FORM_ELEMENT_ARRAY_GET_CLASS (element);
	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (element);

	g_ptr_array_add (priv->ar_elements, element_to_add);

	g_signal_emit (G_OBJECT (element), klass->element_added_signal_id, 0, element_to_add);
}

/**
 * zak_form_element_array_get_elements:
 * @element:
 *
 * Returns:
 */
GPtrArray
*zak_form_element_array_get_elements (ZakFormElement *element)
{
	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (ZAK_FORM_ELEMENT_ARRAY (element));

	return priv->ar_elements;
}

/**
 * zak_form_element_array_get_element_by_id:
 * @element:
 * @id:
 *
 * Returns: the #ZakFormElement with @id.
 */
ZakFormElement
*zak_form_element_array_get_element_by_id (ZakFormElement *element, const gchar *id)
{
	ZakFormElement *ret;

	guint i;

	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (element);

	ret = NULL;
	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *form_element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			gchar *name = zak_form_element_get_name (form_element);
			if (g_strcmp0 (name, id) == 0)
				{
					ret = form_element;
					g_free (name);
					break;
				}
			g_free (name);
		}

	return ret;
}

/* PRIVATE */
static void
zak_form_element_array_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementArray *zak_form_element_array = (ZakFormElementArray *)object;
	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (zak_form_element_array);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_array_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormElementArray *zak_form_element_array = (ZakFormElementArray *)object;
	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (zak_form_element_array);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_element_array_dispose (GObject *gobject)
{
	ZakFormElementArray *zak_form_element_array = (ZakFormElementArray *)gobject;
	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (zak_form_element_array);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_element_array_finalize (GObject *gobject)
{
	ZakFormElementArray *zak_form_element_array = (ZakFormElementArray *)gobject;
	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (zak_form_element_array);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static GValue
*zak_form_element_array_get_value (ZakFormElementArray *element)
{
	GValue *ret;
	GString *str;
	guint i;

	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (element);

	str = g_string_new ("{");

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *form_element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			g_string_append_printf (str,
			                        "%s{\"%s\",\"%s\"}",
			                        str->len > 1 ? "," : "",
			                        zak_form_element_get_name (form_element),
			                        zak_form_element_get_value (form_element));
		}

	g_string_append (str, "}");

	ret = zak_utils_gvalue_new_string (str->str);

	g_string_free (str, TRUE);

	return ret;
}

static gboolean
zak_form_element_array_set_value (ZakFormElementArray *element, GValue *value)
{
	gchar **splitted;
	guint i;
	gchar *field;
	gchar **field_splitted;
	guint l;

	gchar *field_name;
	gchar *field_value;

	ZakFormElement *form_element;

	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (element);

	splitted = g_strsplit (g_value_get_string (value), "},{", -1);

	for (i = 0; i < g_strv_length (splitted); i++)
		{
			if (i == 0)
				{
					field = g_strdup (splitted[0] + 1);
				}
			else if (i == (g_strv_length (splitted) - 1))
				{
					field = g_strndup (splitted[i], strlen (splitted[i]) - 1);
				}
			else
				{
					field = g_strdup (splitted [i]);
				}

			field_name = NULL;
			field_value = NULL;

			field_splitted = g_strsplit_set (field, "{\",}", -1);
			for (l = 0; l < g_strv_length (field_splitted); l++)
				{
					if (strlen (field_splitted[l]) > 0)
						{
							if (field_name == NULL)
								{
									field_name = g_strdup (field_splitted[l]);
								}
							else
								{
									field_value = g_strdup (field_splitted[l]);
									break;
								}
						}
				}

			/* assign value to the right FormElement */
			if (field_name != NULL
				&& field_value != NULL)
				{
					form_element = zak_form_element_array_get_element_by_id (ZAK_FORM_ELEMENT (element), field_name);
					if (form_element != NULL)
						{
							zak_form_element_set_value (ZAK_FORM_ELEMENT (form_element), field_value);
						}
				}

			g_free (field_name);
			g_free (field_value);

			g_strfreev (field_splitted);

			g_free (field);
		}

	g_strfreev (splitted);

	return TRUE;
}

static void
zak_form_element_array_set_as_original_value (ZakFormElement *element)
{
	guint i;

	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (element);

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *form_element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			zak_form_element_set_as_original_value (form_element);
		}
}

static void
zak_form_element_array_clear (ZakFormElement *element)
{
	guint i;

	ZakFormElementArrayPrivate *priv = ZAK_FORM_ELEMENT_ARRAY_GET_PRIVATE (element);

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *form_element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			zak_form_element_clear (form_element);
		}
}

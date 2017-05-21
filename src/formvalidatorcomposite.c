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
#include "form.h"
#include "formelement.h"
#include "formelementvalidator.h"
#include "formvalidator.h"
#include "formvalidatorcomposite.h"

static void zak_form_validator_composite_class_init (ZakFormValidatorCompositeClass *class);
static void zak_form_validator_composite_init (ZakFormValidatorComposite *validator);

static void zak_form_validator_composite_set_property (GObject *object,
                                                       guint property_id,
                                                       const GValue *value,
                                                       GParamSpec *pspec);
static void zak_form_validator_composite_get_property (GObject *object,
                                                       guint property_id,
                                                       GValue *value,
                                                       GParamSpec *pspec);

static void zak_form_validator_composite_dispose (GObject *gobject);
static void zak_form_validator_composite_finalize (GObject *gobject);

static gboolean zak_form_validator_composite_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, gpointer form);
static gboolean zak_form_validator_composite_validate (ZakFormValidator *validator_notempty);

struct _ZakFormValidatorComposite
{
	ZakFormValidator parent_instance;

	/* Other members, including private data. */
};

#define ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_TYPE_VALIDATOR_COMPOSITE, ZakFormValidatorCompositePrivate))

typedef struct _ZakFormValidatorCompositePrivate ZakFormValidatorCompositePrivate;
struct _ZakFormValidatorCompositePrivate
	{
		GNode *tree;
	};

G_DEFINE_TYPE (ZakFormValidatorComposite, zak_form_validator_composite, ZAK_FORM_TYPE_VALIDATOR)

typedef enum
	{
		LOGIC_TYPE_AND = 1,
		LOGIC_TYPE_OR
	} LogicType;

typedef struct
	{
		gboolean condition;
		gboolean condition_type;
		LogicType logic_type;
		ZakFormElementValidator *validator;
		ZakFormElement *element;
		gchar *message;
	} Node;

static void
zak_form_validator_composite_class_init (ZakFormValidatorCompositeClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	ZakFormValidatorClass *parent_class = ZAK_FORM_VALIDATOR_CLASS (class);

	object_class->set_property = zak_form_validator_composite_set_property;
	object_class->get_property = zak_form_validator_composite_get_property;
	object_class->dispose = zak_form_validator_composite_dispose;
	object_class->finalize = zak_form_validator_composite_finalize;

	parent_class->xml_parsing = zak_form_validator_composite_xml_parsing;
	parent_class->validate = zak_form_validator_composite_validate;

	g_type_class_add_private (object_class, sizeof (ZakFormValidatorCompositePrivate));
}

static void
zak_form_validator_composite_init (ZakFormValidatorComposite *validator)
{
	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);

	priv->tree = NULL;
}

/**
 * zak_form_validator_composite_new:
 *
 * Returns: the newly created #ZakFormValidatorComposite object.
 */
ZakFormValidatorComposite
*zak_form_validator_composite_new ()
{
	ZakFormValidatorComposite *zak_form_validator_composite;

	zak_form_validator_composite = ZAK_FORM_VALIDATOR_COMPOSITE (g_object_new (zak_form_validator_composite_get_type (), NULL));

	return zak_form_validator_composite;
}

static void
_zak_form_validator_composite_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, ZakFormForm *form, GNode *tree)
{
	xmlNode *cur;

	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);

	cur = xnode->children;
	while (cur)
		{
			if (xmlStrEqual (cur->name, (xmlChar *)"condition"))
				{
					ZakFormElement *element;
					ZakFormElementValidatorConstructorFunc validator_constructor;
					GNode *gnode;
					Node *n;

					n = (Node *)g_new0 (Node, 1);

					n->condition = TRUE;

					n->condition_type= zak_utils_string_to_boolean ((gchar *)xmlGetProp (cur, (xmlChar *)"condition_type"));

					element = zak_form_form_get_element_by_id (form, (gchar *)xmlGetProp (cur, (xmlChar *)"element"));
					if (!ZAK_FORM_IS_ELEMENT (element))
						{
							g_warning ("Element «%s» not present in form.",
							           (gchar *)xmlGetProp (cur, (xmlChar *)"element"));
							cur = cur->next;
							continue;
						}

					validator_constructor = zak_form_get_form_element_validator (form, (gchar *)xmlGetProp (cur, (xmlChar *)"validator"));
					if (validator_constructor == NULL)
						{
							g_warning ("Validator «%s» not found.",
							           (gchar *)xmlGetProp (cur, (xmlChar *)"validator"));
							cur = cur->next;
							continue;
						}

					n->validator = validator_constructor ();
					zak_form_element_validator_xml_parsing (n->validator, cur);

					n->element = element;

					n->message = NULL;

					if (tree == NULL)
						{
							gnode = g_node_new (n);
							priv->tree = gnode;
						}
					else
						{
							n->logic_type = ((Node *)tree->data)->logic_type;

							gnode = g_node_append (tree, g_node_new (n));
						}

					_zak_form_validator_composite_xml_parsing (validator, cur, form, gnode);
				}
			else if (xmlStrEqual (cur->name, (xmlChar *)"logic"))
				{
					GNode *gnode;
					Node *n;

					n = (Node *)g_new0 (Node, 1);

					if (xmlStrEqual (xmlGetProp (cur, (xmlChar *)"type"), (xmlChar *)"and"))
						{
							n->logic_type = LOGIC_TYPE_AND;
						}
					else if (xmlStrEqual (xmlGetProp (cur, (xmlChar *)"type"), (xmlChar *)"or"))
						{
							n->logic_type = LOGIC_TYPE_OR;
						}
					else
						{
							g_warning ("Logic type «%s» not supported.",
							           xmlGetProp (cur, (xmlChar *)"type"));
							cur = cur->next;
							continue;
						}

					n->message = g_strdup ((gchar *)xmlGetProp (cur, (xmlChar *)"message"));

					if (tree == NULL)
						{
							gnode = g_node_new (n);
							priv->tree = gnode;
						}
					else
						{
							gnode = g_node_append (tree, g_node_new (n));
						}

					_zak_form_validator_composite_xml_parsing (validator, cur, form, gnode);
				}
			else if (xmlStrEqual (cur->name, (xmlChar *)"validator"))
				{
					ZakFormElement *element;
					ZakFormElementValidatorConstructorFunc validator_constructor;
					Node *n;

					element = zak_form_form_get_element_by_id (form, (gchar *)xmlGetProp (cur, (xmlChar *)"element"));
					if (!ZAK_FORM_IS_ELEMENT (element))
						{
							g_warning ("Element «%s» not present in form.",
							           (gchar *)xmlGetProp (cur, (xmlChar *)"element"));
							cur = cur->next;
							continue;
						}

					n = (Node *)g_new0 (Node, 1);

					validator_constructor = zak_form_get_form_element_validator (form, (gchar *)xmlGetProp (cur, (xmlChar *)"type"));
					if (validator_constructor == NULL)
						{
							g_warning ("Validator «%s» not found.",
							           (gchar *)xmlGetProp (cur, (xmlChar *)"type"));
							cur = cur->next;
							continue;
						}

					n->validator = validator_constructor ();
					zak_form_element_validator_xml_parsing (n->validator, cur);

					n->element = element;

					if (tree == NULL)
						{
							priv->tree = g_node_new (n);
						}
					else
						{
							g_node_append (tree, g_node_new (n));
						}
				}

			cur = cur->next;
		}
}

/**
 * zak_form_validator_composite_xml_parsing:
 * @validator:
 * @xnode:
 * @form:
 *
 */
static gboolean
zak_form_validator_composite_xml_parsing (ZakFormValidator *validator, xmlNode *xnode, gpointer form)
{

	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);

	_zak_form_validator_composite_xml_parsing (validator, xnode, (ZakFormForm *)form, NULL);

	return TRUE;
}

/* PRIVATE */
static void
zak_form_validator_composite_set_property (GObject *object,
                                           guint property_id,
                                           const GValue *value,
                                           GParamSpec *pspec)
{
	ZakFormValidatorComposite *validator = (ZakFormValidatorComposite *)object;
	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_validator_composite_get_property (GObject *object,
                                           guint property_id,
                                           GValue *value,
                                           GParamSpec *pspec)
{
	ZakFormValidatorComposite *validator = (ZakFormValidatorComposite *)object;
	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_validator_composite_dispose (GObject *gobject)
{
	ZakFormValidatorComposite *validator = (ZakFormValidatorComposite *)gobject;
	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_validator_composite_finalize (GObject *gobject)
{
	ZakFormValidatorComposite *validator = (ZakFormValidatorComposite *)gobject;
	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
_zak_form_validator_composite_validate (ZakFormValidator *validator, GNode *parent)
{
	gboolean ret;

	GNode *child;
	Node *n;
	Node *n_child;

	guint i;
	guint children;

	n = (Node *)parent->data;

	switch (n->logic_type)
		{
		case LOGIC_TYPE_AND:
			ret = TRUE;
			break;

		case LOGIC_TYPE_OR:
			ret = FALSE;
			break;
		}

	children = g_node_n_children (parent);
	for (i = 0; i < children; i++)
		{
			child = g_node_nth_child (parent, i);
			n_child = (Node *)child->data;

			if (n_child->condition)
				{
					if (zak_form_element_validator_validate (n_child->validator,
					                                         zak_form_element_get_value (n_child->element)) == n_child->condition_type)
						{
							switch (n->logic_type)
								{
								case LOGIC_TYPE_AND:
									ret = (ret && _zak_form_validator_composite_validate (validator, child));
									break;

								case LOGIC_TYPE_OR:
									ret = (ret || _zak_form_validator_composite_validate (validator, child));
									break;
								}
						}
				}
			else
				{
					switch (n->logic_type)
						{
						case LOGIC_TYPE_AND:
							if (n_child->logic_type > 0)
								{
									ret = (ret && _zak_form_validator_composite_validate (validator, child));
								}
							else
								{
									ret = (ret && zak_form_element_validator_validate (n_child->validator,
									                                                   zak_form_element_get_value (n_child->element)));
								}
							break;

						case LOGIC_TYPE_OR:
							if (n_child->logic_type > 0)
								{
									ret = (ret || _zak_form_validator_composite_validate (validator, child));
								}
							else
								{
									ret = (ret || zak_form_element_validator_validate (n_child->validator,
									                                                   zak_form_element_get_value (n_child->element)));
								}
							break;
						}
				}
		}

	if (!ret && n->message != NULL)
		{
			gchar *msg;

			msg = zak_form_validator_get_message (ZAK_FORM_VALIDATOR (validator));
			if (msg != NULL && g_strcmp0 (msg, "") != 0 && g_strcmp0 (msg, _("Invalid value")) != 0)
				{
					zak_form_validator_set_message (ZAK_FORM_VALIDATOR (validator), g_strdup_printf ("%s (%s)", n->message, msg));
				}
			else
				{
					zak_form_validator_set_message (ZAK_FORM_VALIDATOR (validator), n->message);
				}

			g_free (msg);
		}

	return ret;
}

static gboolean
zak_form_validator_composite_validate (ZakFormValidator *validator)
{
	gboolean ret;

	ZakFormValidatorCompositePrivate *priv = ZAK_FORM_VALIDATOR_COMPOSITE_GET_PRIVATE (validator);

	if (priv->tree != NULL)
		{
			ret = _zak_form_validator_composite_validate (validator, g_node_get_root (priv->tree));
		}
	else
		{
			ret = TRUE;
		}

	return ret;
}

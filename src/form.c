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

#include <libxml/xpath.h>

#ifdef G_OS_WIN32
	#include <windows.h>
#endif

#include "commons.h"
#include "form.h"
#include "formelementfilter.h"
#include "formelementvalidator.h"


typedef ZakFormValidator *(* FormValidatorConstructorFunc) (void);

static void zak_form_form_class_init (ZakFormFormClass *class);
static void zak_form_form_init (ZakFormForm *zak_form_form);

static void zak_form_form_set_property (GObject *object,
                                        guint property_id,
                                        const GValue *value,
                                        GParamSpec *pspec);
static void zak_form_form_get_property (GObject *object,
                                        guint property_id,
                                        GValue *value,
                                        GParamSpec *pspec);

static void zak_form_form_dispose (GObject *gobject);
static void zak_form_form_finalize (GObject *gobject);

typedef struct
	{
		GPtrArray *ar_elements;
		GPtrArray *ar_validators;
		GPtrArray *ar_messages;
	} ZakFormFormPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (ZakFormForm, zak_form_form, G_TYPE_OBJECT)

#ifdef G_OS_WIN32
static HMODULE backend_dll = NULL;

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

BOOL WINAPI
DllMain (HINSTANCE hinstDLL,
         DWORD     fdwReason,
         LPVOID    lpReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			backend_dll = (HMODULE) hinstDLL;
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}
	return TRUE;
}
#endif

static void
zak_form_form_class_init (ZakFormFormClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);

	object_class->set_property = zak_form_form_set_property;
	object_class->get_property = zak_form_form_get_property;
	object_class->dispose = zak_form_form_dispose;
	object_class->finalize = zak_form_form_finalize;

	class->get_elements = zak_form_form_get_elements;
}

static void
zak_form_form_init (ZakFormForm *zak_form_form)
{
	gchar *localedir;

	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);

	priv->ar_elements = g_ptr_array_new ();
	priv->ar_validators = g_ptr_array_new ();
	priv->ar_messages = g_ptr_array_new ();

#ifdef G_OS_WIN32

	gchar *moddir;
	gchar *p;

	moddir = g_win32_get_package_installation_directory_of_module (backend_dll);

	p = g_strrstr (moddir, g_strdup_printf ("%c", G_DIR_SEPARATOR));
	if (p != NULL
	    && (g_ascii_strcasecmp (p + 1, "src") == 0
	        || g_ascii_strcasecmp (p + 1, ".libs") == 0))
		{
			localedir = g_strdup (LOCALEDIR);
		}
	else
		{
			localedir = g_build_filename (moddir, "share", "locale", NULL);
		}

	g_free (moddir);

#else

	localedir = g_strdup (LOCALEDIR);

#endif

	bindtextdomain (GETTEXT_PACKAGE, localedir);
	textdomain (GETTEXT_PACKAGE);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

	zak_form_load_modules ();
}

/**
 * zak_form_form_load_from_xml:
 * @zakform:
 * @xmldoc:
 *
 */
gboolean
zak_form_form_load_from_xml (ZakFormForm *zakform, xmlDoc *xmldoc)
{
	ZakFormFormPrivate *priv;
	xmlNode *cur;
	xmlNode *cur_clean;

	gboolean ret;

	ZakFormElement *element;
	ZakFormValidator *validator;

	gchar *type;
	guint i;
	gint y;

	ZakFormElementConstructorFunc element_constructor;
	ZakFormValidatorConstructorFunc validator_constructor;

	xmlXPathContextPtr xpcontext;
	xmlXPathObjectPtr xpresult;
	xmlNodeSetPtr xnodeset;

	g_return_val_if_fail (ZAK_FORM_IS_FORM (zakform), FALSE);
	g_return_val_if_fail (xmldoc != NULL, FALSE);

	ret = FALSE;

	cur = xmlDocGetRootElement (xmldoc);
	if (cur != NULL)
		{
			if (xmlStrEqual (cur->name, (const xmlChar *)"zakform"))
				{
					ret = TRUE;

					cur = cur->children;
					while (cur)
						{
							if (xmlStrEqual (cur->name, (const xmlChar *)"element"))
								{
									element = NULL;

									type = (gchar *)xmlGetProp (cur, (const xmlChar *)"type");

									element_constructor = zak_form_get_form_element (type);
									if (element_constructor != NULL)
										{
											element = element_constructor ();
											if (element != NULL)
												{
													zak_form_form_add_element (zakform, element);

													cur_clean = xmlCopyNode (cur, 1);
													zak_form_element_xml_parsing (element, cur_clean);

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

					/* parsing of validators */
					xpcontext = xmlXPathNewContext (xmldoc);
					xpcontext->node = xmlDocGetRootElement (xmldoc);
					xpresult = xmlXPathEvalExpression ((const xmlChar *)"child::validator", xpcontext);
					if (!xmlXPathNodeSetIsEmpty (xpresult->nodesetval))
						{
							xnodeset = xpresult->nodesetval;
							for (y = 0; y < xnodeset->nodeNr; y++)
								{
									type = (gchar *)xmlGetProp (xnodeset->nodeTab[y], (const xmlChar *)"type");

									validator_constructor = (ZakFormValidatorConstructorFunc)zak_form_get_form_validator (type);
									if (validator_constructor != NULL)
										{
											validator = validator_constructor ();
											zak_form_form_add_validator (zakform, validator);

											zak_form_validator_xml_parsing (validator, xnodeset->nodeTab[y], zakform);
										}
									else
										{
											g_warning ("Validator «%s» not found.", type);
										}
								}
						}
				}
			else
				{
					g_warning (_("The file is not a valid ZakForm definition file."));
				}
		}

	return ret;
}

/**
 * zak_form_form_load_from_file:
 * @zakform:
 * @filename:
 *
 */
gboolean
zak_form_form_load_from_file (ZakFormForm *zakform, const gchar *filename)
{
	xmlDoc *xdoc;

	gboolean ret;

	g_return_val_if_fail (ZAK_FORM_IS_FORM (zakform), FALSE);
	g_return_val_if_fail (filename != NULL, FALSE);

	ret = FALSE;

	xdoc = xmlParseFile (filename);
	if (xdoc != NULL)
		{
			ret = zak_form_form_load_from_xml (zakform, xdoc);
		}

	return ret;
}

/**
 * zak_form_form_add_element:
 * @zakform:
 * @element:
 *
 * Returns: #TRUE if @element is added; FALSE otherwise.
 */
gboolean
zak_form_form_add_element (ZakFormForm *zakform, ZakFormElement *element)
{
	gboolean ret;

	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	g_ptr_array_add (priv->ar_elements, g_object_ref (element));
	ret = TRUE;

	if (ZAK_FORM_FORM_GET_CLASS (zakform)->element_added != NULL)
		{
			ZAK_FORM_FORM_GET_CLASS (zakform)->element_added (zakform, element);
		}

	return ret;
}

/**
 * zak_form_form_get_element_by_id:
 * @zakform:
 * @id:
 *
 * Returns: the #ZakFormElement with @id.
 */
ZakFormElement
*zak_form_form_get_element_by_id (ZakFormForm *zakform, const gchar *id)
{
	ZakFormElement *ret;

	ZakFormFormPrivate *priv;

	guint i;

	priv = zak_form_form_get_instance_private (zakform);

	ret = NULL;
	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			if (g_strcmp0 (zak_form_element_get_name (element), id) == 0)
				{
					ret = element;
					break;
				}
		}

	return ret;
}

/**
 * zak_form_form_get_elements:
 * @zakform:
 *
 * Returns: a #GPtrArray with the list of ZakFormElement's in the form.
 */
GPtrArray
*zak_form_form_get_elements (ZakFormForm *zakform)
{
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zakform);

	return priv->ar_elements;
}

/**
 * zak_form_form_get_elements_by_type:
 * @zakform:
 * @type:
 *
 * Returns: a #GPtrArray with the list of ZakFormElement's in the form filtered by @type.
 */
GPtrArray
*zak_form_form_get_elements_by_type (ZakFormForm *zakform, GType type)
{
	GPtrArray *ar;
	ZakFormElement *element;

	guint i;

	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zakform);

	ar = g_ptr_array_new ();
	for (i = 0; i < priv->ar_elements->len; i++)
		{
			element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);
			if (G_OBJECT_TYPE (element) == type)
				{
					g_ptr_array_add (ar, element);
				}
		}

	return ar;
}

/**
 * zak_form_form_add_validator:
 * @zakform:
 * @validator:
 *
 * Returns: #TRUE if @validator is added; FALSE otherwise.
 */
gboolean
zak_form_form_add_validator (ZakFormForm *zakform, ZakFormValidator *validator)
{
	gboolean ret;

	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	g_ptr_array_add (priv->ar_validators, g_object_ref (validator));
	ret = TRUE;

	return ret;
}

/**
 * zak_form_form_get_validator_by_id:
 * @zakform:
 * @id:
 *
 * Returns: a #ZakFormValidator.
 */
ZakFormValidator
*zak_form_form_get_validator_by_id (ZakFormForm *zakform, const gchar *id)
{
	ZakFormValidator *validator;

	guint i;

	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zakform);

	validator = NULL;

	for (i = 0; i < priv->ar_validators->len; i++)
		{
			if (g_strcmp0 (zak_form_validator_get_id ((ZakFormValidator *)g_ptr_array_index (priv->ar_validators, i)), id) == 0)
				{
					validator = (ZakFormValidator *)g_ptr_array_index (priv->ar_validators, i);
					break;
				}
		}

	return validator;
}

/**
 * zak_form_form_get_validators:
 * @zakform:
 *
 * Returns: a #GPtrArray with the list of ZakFormValidator's in the form.
 */
GPtrArray
*zak_form_form_get_validators (ZakFormForm *zakform)
{
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zakform);

	return priv->ar_validators;
}

/**
 * zak_form_form_get_validators_by_type:
 * @zakform:
 * @type:
 *
 * Returns: a #GPtrArray with the list of ZakFormValidator's in the form filtered by @type.
 */
GPtrArray
*zak_form_form_get_validators_by_type (ZakFormForm *zakform, GType type)
{
	GPtrArray *ar;
	ZakFormValidator *validator;

	guint i;

	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zakform);

	ar = g_ptr_array_new ();
	for (i = 0; i < priv->ar_validators->len; i++)
		{
			validator = (ZakFormValidator *)g_ptr_array_index (priv->ar_validators, i);
			if (G_OBJECT_TYPE (validator) == type)
				{
					g_ptr_array_add (ar, validator);
				}
		}

	return ar;
}

/**
 * zak_form_form_clear:
 * @zakform: a #ZakFormForm object.
 *
 */
void
zak_form_form_clear (ZakFormForm *zakform)
{
	guint i;

	ZakFormFormPrivate *priv;

	g_return_if_fail (ZAK_FORM_IS_FORM (zakform));

	priv = zak_form_form_get_instance_private (zakform);

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			zak_form_element_clear (element);
		}
}

/**
 * zak_form_form_set_as_original:
 * @zakform: a #ZakFormForm object.
 *
 */
void
zak_form_form_set_as_original (ZakFormForm *zakform)
{
	guint i;

	ZakFormFormPrivate *priv;

	g_return_if_fail (ZAK_FORM_IS_FORM (zakform));

	priv = zak_form_form_get_instance_private (zakform);

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			zak_form_element_set_as_original_value (element);
		}
}

/**
 * zak_form_form_set_editable:
 * @zakform:
 * @editable:
 *
 */
void
zak_form_form_set_editable (ZakFormForm *zakform, gboolean editable)
{
	guint i;

	ZakFormFormPrivate *priv;

	g_return_if_fail (ZAK_FORM_IS_FORM (zakform));

	priv = zak_form_form_get_instance_private (zakform);

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);

			zak_form_element_set_editable (element, editable);
		}
}

/**
 * zak_form_form_is_valid:
 * @zakform:
 *
 * Returns:
 */
gboolean
zak_form_form_is_valid (ZakFormForm *zakform)
{
	guint i;

	gboolean ret;

	ZakFormFormPrivate *priv;

	GPtrArray *ar_messages;

	priv = zak_form_form_get_instance_private (zakform);

	ret = TRUE;

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);
			if (!zak_form_element_is_valid (element))
				{
					ret = FALSE;
				}
		}

	g_ptr_array_set_size (priv->ar_messages, 0);
	for (i = 0; i < priv->ar_validators->len; i++)
		{
			ZakFormValidator *validator = (ZakFormValidator *)g_ptr_array_index (priv->ar_validators, i);
			if (!zak_form_validator_validate (validator))
				{
					g_ptr_array_add (priv->ar_messages, (gpointer)g_strdup (zak_form_validator_get_message (validator)));
					ret = FALSE;
				}
		}

	return ret;
}

/**
 * zak_form_form_get_messages:
 * @zakform:
 *
 */
GPtrArray
*zak_form_form_get_messages (ZakFormForm *zakform)
{
	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	return priv->ar_messages;
}

/**
 * zak_form_form_is_changed:
 * @zakform:
 *
 * Returns:
 */
gboolean
zak_form_form_is_changed (ZakFormForm *zakform)
{
	guint i;

	gboolean ret;

	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	ret = FALSE;

	for (i = 0; i < priv->ar_elements->len; i++)
		{
			ZakFormElement *element = (ZakFormElement *)g_ptr_array_index (priv->ar_elements, i);
			if (zak_form_element_is_changed (element))
				{
					ret = TRUE;
					break;
				}
		}

	return ret;
}

/**
 * zak_form_form_load:
 * @zakform:
 * @provider:
 *
 */
gboolean
zak_form_form_load (ZakFormForm *zakform, ZakFormIProvider *provider)
{
	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	return zak_form_iprovider_load (provider, priv->ar_elements);
}

/**
 * zak_form_form_insert:
 * @zakform:
 * @provider:
 *
 */
gboolean
zak_form_form_insert (ZakFormForm *zakform, ZakFormIProvider *provider)
{
	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	return zak_form_iprovider_insert (provider, priv->ar_elements);
}

/**
 * zak_form_form_update:
 * @zakform:
 * @provider:
 *
 */
gboolean
zak_form_form_update (ZakFormForm *zakform, ZakFormIProvider *provider)
{
	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	return zak_form_iprovider_update (provider, priv->ar_elements);
}

/**
 * zak_form_form_delete:
 * @zakform:
 * @provider:
 *
 */
gboolean
zak_form_form_delete (ZakFormForm *zakform, ZakFormIProvider *provider)
{
	ZakFormFormPrivate *priv;

	priv = zak_form_form_get_instance_private (zakform);

	return zak_form_iprovider_delete (provider, priv->ar_elements);
}

/* PRIVATE */
static void
zak_form_form_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)object;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_form_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)object;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_form_dispose (GObject *gobject)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)gobject;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_form_finalize (GObject *gobject)
{
	ZakFormForm *zak_form_form = (ZakFormForm *)gobject;
	ZakFormFormPrivate *priv = zak_form_form_get_instance_private (zak_form_form);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

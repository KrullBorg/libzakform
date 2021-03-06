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

#ifndef __ZAK_FORM_ELEMENT_H__
#define __ZAK_FORM_ELEMENT_H__


#include <glib-object.h>

#include <libxml/tree.h>

#include "formelementfilter.h"
#include "formelementvalidator.h"
#include "formelementextension.h"


G_BEGIN_DECLS


#define ZAK_FORM_TYPE_ELEMENT zak_form_element_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormElement, zak_form_element, ZAK_FORM, ELEMENT, GObject)

struct _ZakFormElementClass
	{
		GObjectClass parent_class;

		void (*xml_parsing) (ZakFormElement *element, xmlNode *xmlnode);

		gboolean (*set_value) (ZakFormElement *element, GValue *value);
		GValue *(*get_value) (ZakFormElement *element);

		void (*set_as_original_value) (ZakFormElement *element);
		gboolean (*is_changed) (ZakFormElement *element);

		void (*set_visible) (ZakFormElement *element, gboolean visible);
		gboolean (*get_visible) (ZakFormElement *element);

		void (*set_editable) (ZakFormElement *element, gboolean editable);
		gboolean (*get_editable) (ZakFormElement *element);

		void (*clear) (ZakFormElement *element);

		gboolean (*is_valid) (ZakFormElement *element);

		guint before_validating_signal_id;
		guint after_validating_signal_id;
	};

void zak_form_element_xml_parsing (ZakFormElement *element, xmlNode *xmlnode);

void zak_form_element_set_name (ZakFormElement *element, const gchar *name);
gchar *zak_form_element_get_name (ZakFormElement *element);

void zak_form_element_set_long_name (ZakFormElement *element, const gchar *long_name);
gchar *zak_form_element_get_long_name (ZakFormElement *element);

void zak_form_element_set_is_key (ZakFormElement *element, gboolean is_key);
gboolean zak_form_element_get_is_key (ZakFormElement *element);

void zak_form_element_set_provider_type (ZakFormElement *element, const gchar *type);
gchar *zak_form_element_get_provider_type (ZakFormElement *element);

GValue *zak_form_element_format_gvalue (ZakFormElement *element, GValue *value);
GValue *zak_form_element_unformat_gvalue (ZakFormElement *element, GValue *value);

gchar *zak_form_element_format (ZakFormElement *element, const gchar *value);
gchar *zak_form_element_unformat (ZakFormElement *element, const gchar *value);

gboolean zak_form_element_set_value_gvalue (ZakFormElement *element, GValue *value);
GValue *zak_form_element_get_value_gvalue (ZakFormElement *element);

gboolean zak_form_element_set_value (ZakFormElement *element, const gchar *value);
gchar *zak_form_element_get_value (ZakFormElement *element);

gboolean zak_form_element_set_default_value_gvalue (ZakFormElement *element, GValue *value);
GValue *zak_form_element_get_default_value_gvalue (ZakFormElement *element);

gboolean zak_form_element_set_default_value (ZakFormElement *element, const gchar *value);
gchar *zak_form_element_get_default_value (ZakFormElement *element);

gboolean zak_form_element_set_original_value_gvalue (ZakFormElement *element, GValue *value);
GValue *zak_form_element_get_original_value_gvalue (ZakFormElement *element);

gboolean zak_form_element_set_original_value (ZakFormElement *element, const gchar *value);
gchar *zak_form_element_get_original_value (ZakFormElement *element);

void zak_form_element_set_as_original_value (ZakFormElement *element);
gboolean zak_form_element_is_changed (ZakFormElement *element);

gboolean zak_form_element_set_format (ZakFormElement *element, GHashTable *format);
GHashTable *zak_form_element_get_format (ZakFormElement *element);

void zak_form_element_set_visible (ZakFormElement *element, gboolean visible);
gboolean zak_form_element_get_visible (ZakFormElement *element);

void zak_form_element_set_editable (ZakFormElement *element, gboolean editable);
gboolean zak_form_element_get_editable (ZakFormElement *element);

void zak_form_element_set_to_load (ZakFormElement *element, gboolean to_load);
gboolean zak_form_element_get_to_load (ZakFormElement *element);

void zak_form_element_set_to_save (ZakFormElement *element, gboolean to_save);
gboolean zak_form_element_get_to_save (ZakFormElement *element);

void zak_form_element_clear (ZakFormElement *element);

void zak_form_element_add_extension (ZakFormElement *element, ZakFormElementExtension *extension);

GPtrArray *zak_form_element_get_extensions (ZakFormElement *element);
GPtrArray *zak_form_element_get_extensions_by_type (ZakFormElement *element, GType type);

void zak_form_element_add_filter (ZakFormElement *element, ZakFormElementFilter *filter);
void zak_form_element_filter (ZakFormElement *element);

GPtrArray *zak_form_element_get_filters (ZakFormElement *element);
ZakFormElementFilter *zak_form_element_get_filter_by_id (ZakFormElement *element, const gchar *id);
GPtrArray *zak_form_element_get_filters_by_type (ZakFormElement *element, GType type);

void zak_form_element_add_validator (ZakFormElement *element, ZakFormElementValidator *validator);
gboolean zak_form_element_is_valid (ZakFormElement *element);
GPtrArray *zak_form_element_get_messages (ZakFormElement *element);

GPtrArray *zak_form_element_get_validators (ZakFormElement *element);
ZakFormElementValidator *zak_form_element_get_validator_by_id (ZakFormElement *element, const gchar *id);
GPtrArray *zak_form_element_get_validators_by_type (ZakFormElement *element, GType type);


G_END_DECLS


#endif /* __ZAK_FORM_ELEMENT_H__ */

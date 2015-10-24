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

#ifndef __ZAK_CGI_FORM_ELEMENT_IFILTER_H__
#define __ZAK_CGI_FORM_ELEMENT_IFILTER_H__

#include <glib-object.h>


G_BEGIN_DECLS


#define ZAK_CGI_TYPE_FORM_ELEMENT_IFILTER zak_cgi_form_element_ifilter_get_type ()
G_DECLARE_INTERFACE (ZakCgiFormElementIFilter, zak_cgi_form_element_ifilter, ZAK_CGI, FORM_ELEMENT_IFILTER, GObject)

struct _ZakCgiFormElementIFilterInterface
{
	GTypeInterface parent_iface;

	GValue *(*filter) (ZakCgiFormElementIFilter *self, GValue *value);
};

GValue *zak_cgi_form_element_ifilter_filter (ZakCgiFormElementIFilter *self, GValue *value);


G_END_DECLS


#endif /* __ZAK_CGI_FOR_ELEMENT_IFILTER_H__ */

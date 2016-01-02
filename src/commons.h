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

#ifndef _ZAK_FORM_COMMONS_H_
#define _ZAK_FORM_COMMONS_H_

#include <glib-object.h>


#include "formelement.h"


G_BEGIN_DECLS


ZakFormElement *zak_form_get_element_by_id (GPtrArray *ar_elements, const gchar *id);


G_END_DECLS


#endif /* _ZAK_FORM_COMMONS_H_ */

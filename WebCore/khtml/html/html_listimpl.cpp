/**
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * $Id$
 */
#include "html_listimpl.h"

using namespace DOM;

#include "css/cssproperties.h"
#include "css/cssvalues.h"
#include "rendering/render_list.h"
#include "misc/htmlhashes.h"
#include "xml/dom_docimpl.h"

using namespace khtml;

NodeImpl::Id HTMLUListElementImpl::id() const
{
    return ID_UL;
}

void HTMLUListElementImpl::parseAttribute(AttributeImpl *attr)
{
    switch(attr->id())
    {
    case ATTR_TYPE:
        addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, attr->value());
        break;
    default:
        HTMLElementImpl::parseAttribute(attr);
    }
}

// -------------------------------------------------------------------------

NodeImpl::Id HTMLDirectoryElementImpl::id() const
{
    return ID_DIR;
}

// -------------------------------------------------------------------------

NodeImpl::Id HTMLMenuElementImpl::id() const
{
    return ID_MENU;
}

// -------------------------------------------------------------------------

NodeImpl::Id HTMLOListElementImpl::id() const
{
    return ID_OL;
}

void HTMLOListElementImpl::parseAttribute(AttributeImpl *attr)
{
    switch(attr->id())
    {
    case ATTR_TYPE:
        if ( strcmp( attr->value(), "a" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_LOWER_ALPHA);
        else if ( strcmp( attr->value(), "A" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_UPPER_ALPHA);
        else if ( strcmp( attr->value(), "i" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_LOWER_ROMAN);
        else if ( strcmp( attr->value(), "I" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_UPPER_ROMAN);
        else if ( strcmp( attr->value(), "1" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_DECIMAL);
        break;
    case ATTR_START:
            _start = attr->val() ? attr->val()->toInt() : 1;
    default:
        HTMLUListElementImpl::parseAttribute(attr);
    }
}

// -------------------------------------------------------------------------

NodeImpl::Id HTMLLIElementImpl::id() const
{
    return ID_LI;
}


void HTMLLIElementImpl::parseAttribute(AttributeImpl *attr)
{
    switch(attr->id())
    {
    case ATTR_VALUE:
        isValued = true;
        requestedValue = attr->val() ? attr->val()->toInt() : 0;

        if(m_render && m_render->isListItem())
        {
            RenderListItem *list = static_cast<RenderListItem *>(m_render);
            // ### work out what to do when attribute removed - use default of some sort?

            list->setValue(requestedValue);
        }
        break;
    case ATTR_TYPE:
        if ( strcmp( attr->value(), "a" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_LOWER_ALPHA);
        else if ( strcmp( attr->value(), "A" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_UPPER_ALPHA);
        else if ( strcmp( attr->value(), "i" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_LOWER_ROMAN);
        else if ( strcmp( attr->value(), "I" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_UPPER_ROMAN);
        else if ( strcmp( attr->value(), "1" ) == 0 )
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, CSS_VAL_DECIMAL);
        else
            addCSSProperty(CSS_PROP_LIST_STYLE_TYPE, attr->value());
        break;
    default:
        HTMLElementImpl::parseAttribute(attr);
    }
}

void HTMLLIElementImpl::attach()
{
    assert(!attached());

    HTMLElementImpl::attach();

    // If we are first, and the OL has a start attr.
    if (parentNode() && parentNode()->id() == ID_OL)
    {
        HTMLOListElementImpl *ol = static_cast<HTMLOListElementImpl *>(parentNode());

        if(ol->firstChild() && ol->firstChild() == this &&  m_render)
           static_cast<RenderListItem*>(m_render)->setValue(ol->start());
    }

    // If we had a value attr.
    if (isValued && m_render)
        static_cast<RenderListItem*>(m_render)->setValue(requestedValue);

}


// -------------------------------------------------------------------------


NodeImpl::Id HTMLDListElementImpl::id() const
{
    return ID_DL;
}


/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014 Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include <openddlparser/DDLNode.h>

#include <algorithm>

BEGIN_ODDLPARSER_NS

DDLNode::DDLNode( const std::string &type, const std::string &name, DDLNode *parent )
: m_type( type )
, m_name( name )
, m_parent( parent )
, m_children()
, m_properties( nullptr ) {
    if( m_parent ) {
        m_parent->m_children.push_back( this );
    }
}

DDLNode::~DDLNode() {
    detachParent();
}

void DDLNode::attachParent( DDLNode *parent ) {
    if( m_parent == parent ) {
        return;
    }

    m_parent = parent;
    m_parent->m_children.push_back( this );
}

void DDLNode::detachParent() {
    if( m_parent ) {
        std::vector<DDLNode*>::iterator it;
        it = std::find( m_parent->m_children.begin(), m_parent->m_children.end(), this );
        if( m_parent->m_children.end() != it ) {
            m_parent->m_children.erase( it );
        }
        m_parent = nullptr;
    }
}

DDLNode *DDLNode::getParent() const {
    return m_parent;
}

const DDLNode::DllNodeList &DDLNode::getChildNodeList() const {
    return m_children;
}

void DDLNode::setType( const std::string &type ) {
    m_type = type;
}

const std::string &DDLNode::getType() const {
    return m_type;
}


void DDLNode::setName( const std::string &name ) {
    m_name = name;
}

const std::string &DDLNode::getName() const {
    return m_name;
}

void DDLNode::setProperties( Property *first ) {
    m_properties = first;
}

Property *DDLNode::getProperties() const {
    return m_properties;
}

END_ODDLPARSER_NS

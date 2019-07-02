#include "xmlbuilder.h"
#include <QTextCodec>


XMLBuilder::XMLBuilder(const QString& instr, const QString& docName) : m_doc( docName ) {
    QDomProcessingInstruction instructions =
        m_doc.createProcessingInstruction(docName, instr);
    if (docName != "")
      m_doc.appendChild( instructions );
}

XMLBuilder& XMLBuilder::begin( const QString& tagName ) noexcept(false) {
    if( tagName.isEmpty() ) {
        throw XMLException( EMPTY_TAG_NAME );
    }

    m_currentElement = m_doc.createElement(tagName);
    appendElement( m_currentElement );

    m_elementsStack.push( m_currentElement );

    return *this;
}

XMLBuilder& XMLBuilder::end() noexcept(false) {
    if( m_elementsStack.isEmpty() ) {
        throw XMLException( TAG_NOT_STARTED );
    }

    m_currentElement = m_elementsStack.pop();

    return *this;
}

XMLBuilder& XMLBuilder::attr( const QString& attrName, const QString& content ) noexcept(false) {
    if( attrName.isEmpty() ) {
        throw XMLException( EMPTY_ATTR_NAME );
    }
    if( m_currentElement.isNull() ) {
        throw XMLException( COULD_NOT_ADD_ATTR );
    }

    m_currentElement.setAttribute( attrName, content );

    return *this;
}

XMLBuilder &XMLBuilder::content(const QString &content)
{
  if( content.isEmpty() ) {
      throw XMLException( EMPTY_CONT );
  }
  if( m_currentElement.isNull() ) {
      throw XMLException( COULD_NOT_ADD_ATTR );
  }

  m_currentElement.appendChild(m_doc.createTextNode(content));

  return *this;
}

XMLBuilder &XMLBuilder::comment(const QString &comment)
{
  if( comment.isEmpty() ) {
      throw XMLException( EMPTY_COMM );
  }
  if( m_currentElement.isNull() ) {
      throw XMLException( COULD_NOT_ADD_ATTR );
  }

  m_currentElement.appendChild(m_doc.createComment(comment));

  return *this;
}

XMLBuilder& XMLBuilder::add( const QString& tagName, const QString& content ) noexcept(false) {
    if( tagName.isEmpty() ) {
        throw XMLException( EMPTY_TAG_NAME );
    }

    m_currentElement = m_doc.createElement( tagName );
    appendElement( m_currentElement );

    m_currentElement.appendChild( m_doc.createTextNode( content ) );

    return *this;
}

QString XMLBuilder::getXML() const {

    return m_doc.toString();
}

void XMLBuilder::appendElement( const QDomElement& element ) {
    if( m_elementsStack.isEmpty() ) {
        m_doc.appendChild( element );
    } else {
        QDomElement currentRoot = m_elementsStack.top();
        currentRoot.appendChild( element );
    }
}

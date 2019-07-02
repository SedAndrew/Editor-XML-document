#ifndef XMLBUILDER_H
#define XMLBUILDER_H

#include <QException>
#include <QDomDocument>
#include <QDomElement>
#include <QStack>


static const QString XML_INSTRUCTION_CONTENT = "version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"";
class XMLBuilder {
public:
    enum ErrorCode {
        EMPTY_TAG_NAME,
        EMPTY_ATTR_NAME,
        EMPTY_CONT,
        EMPTY_COMM,
        COULD_NOT_ADD_ATTR,
        TAG_NOT_STARTED
    };

    class XMLException : public std::exception {//QException {
    public:
        explicit XMLException( ErrorCode code ) : m_code( code ) { }
        ~XMLException() noexcept { }

        const char* what() const noexcept {
            switch( m_code ) {
            case EMPTY_TAG_NAME: return "Empty tag name!";
            case EMPTY_ATTR_NAME: return "Empty attribute name!";
            case EMPTY_CONT: return "Empty content!";
            case EMPTY_COMM: return "Empty comment!";
            case COULD_NOT_ADD_ATTR: return "Could not add attribute!";
            case TAG_NOT_STARTED: return "There are no started tags!";
            default: return "Unknown error!";
            }
        }

//        void raise() const;
//        QException *clone() const;

    private:
        ErrorCode m_code;


    };

public:
    explicit XMLBuilder(const QString& instr = XML_INSTRUCTION_CONTENT, const QString& docName = "");

    XMLBuilder &begin( const QString& tagName ) noexcept(false);
    XMLBuilder &add( const QString& tagName, const QString& content ) noexcept(false);
    XMLBuilder &attr( const QString& attrName, const QString& content ) noexcept(false);
    XMLBuilder &content( const QString& content ) noexcept(false);
    XMLBuilder &comment( const QString& comment ) noexcept(false);
    XMLBuilder &end() noexcept(false);

    QString getXML() const;

private:
    void appendElement( const QDomElement& element );

private:
    QDomDocument m_doc;
    QStack< QDomElement > m_elementsStack;
    QDomElement m_currentElement;
};



#endif // XMLBUILDER_H

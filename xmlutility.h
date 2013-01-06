#ifndef XMLUTILITY_H
#define XMLUTILITY_H

 #include <QString>
#include <QRegExp>

class QXmlStreamWriter;
class QXmlStreamReader;

//**************************************************************************
//! TODO:
/*!
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class XMLUtility
{
public:
    XMLUtility();
    static QString booleanToString(bool b);
    static bool stringToBoolean(const QString& s);

    static QString caseToString(Qt::CaseSensitivity sensitivity);
    static Qt::CaseSensitivity stringToCase(const QString& sensitivity);

    static QString patternSyntaxToString(QRegExp::PatternSyntax syntax);
    static QRegExp::PatternSyntax stringToPatternSyntax(const QString& syntax);

    static void write(QXmlStreamWriter& writer, const QRegExp& regexp, const QString& name);
    static QRegExp* readRegExp(QXmlStreamReader& read);

    static void writeElementAttribute(QXmlStreamWriter& writer, const QString& elementName, const QString& elementValue, const QString& attributeName, const QString& attributeValue);


};

#endif // XMLUTILITY_H

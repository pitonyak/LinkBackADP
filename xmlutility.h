#ifndef XMLUTILITY_H
#define XMLUTILITY_H

 #include <QString>
#include <QRegExp>

class QXmlStreamWriter;
class QXmlStreamReader;

//**************************************************************************
//! Utility methods used while reading and writing XML.
/*!
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 ***************************************************************************/

class XMLUtility
{
public:
    /*! \brief Default constructor; does nothing. */
    XMLUtility();

    //**************************************************************************
    /*! \brief Convert a bool to "True" or "False".
     *
     *  \param [in] b Boolean value to convert.
     *  \return String representation of a bool as "True" or "False".
     ***************************************************************************/
    static QString booleanToString(const bool b);

    //**************************************************************************
    /*! \brief Ignoring case, convert "true", "t", or "1" to True, everything else to False.
     *
     *  \param [in] s String to convert.
     *  \return Boolean value representated by the string.
     ***************************************************************************/
    static bool stringToBoolean(const QString& s);

    //**************************************************************************
    /*! \brief Convert a CaseSensitivity object to a string representation.
     *
     *  \param [in] sensitivity Value to convert to a string.
     *  \return String representation as "CaseInsensitive" or "CaseSensitive".
     ***************************************************************************/
    static QString caseToString(Qt::CaseSensitivity sensitivity);

    //**************************************************************************
    /*! \brief Ignoring case, convert "insensitive" to be CaseInsensitive, all other values to CaseSensitive.
     *
     *  \param [in] sensitivity String to convert
     *  \return Qt::CaseSensitivity valeu represented by the string.
     ***************************************************************************/
    static Qt::CaseSensitivity stringToCase(const QString& sensitivity);

    //**************************************************************************
    /*! \brief Write an XML representation of the regular expression.
     *
     * If the regular expression is invalid or empty, or, if the name is empty, nothing is written.
     * Writes the name with attributes "IsMinimal", "Pattern" (pattern syntax), and
     * "CaseSensitive". The regular expression is written as characters in the XML.
     *
     *  \param [in, out] writer XML Writer to which the regular expression is written.
     *  \param [in] regexp Regulra expression to write.
     *  \param [in] name Element name.
     ***************************************************************************/
    static void write(QXmlStreamWriter& writer, const QRegExp& regexp, const QString& name);

    //**************************************************************************
    /*! \brief Read a regular expression object into a newly allocated regular expression object. You own it, delete it when you are finished.
     *
     * Begins by looking at attributes, so it is assumed that the element is current.
     *
     *  \param [in, out] reader
     *  \return Pointer to the newly allocated regular expression object. You own it, delete it when you are finished.
     ***************************************************************************/
    static QRegExp* readRegExp(QXmlStreamReader& reader);

    //**************************************************************************
    /*! \brief Shortcut to quickly write an elment with one attribute.
     *
     *  \param [in, out] writer XML Writer to which the data is written.
     *  \param [in] elementName XML Element Name. If empty, nothing is written.
     *  \param [in] elementValue Text for the element. If zero length, written as <elementName/>.
     *  \param [in] attributeName If not empty, this attribute is added.
     *  \param [in] attributeValue If not empty, this attribute is added.
     ***************************************************************************/
    static void writeElementAttribute(QXmlStreamWriter& writer, const QString& elementName, const QString& elementValue, const QString& attributeName, const QString& attributeValue);

    //**************************************************************************
    /*! \brief Shortcut to quickly write an elment with no attribute.
     *
     *  \param [in, out] writer XML Writer to which the data is written.
     *  \param [in] elementName XML Element Name. If empty, nothing is written.
     *  \param [in] elementValue Text for the element. If zero length, written as <elementName/>.
     ***************************************************************************/
    static void writeElement(QXmlStreamWriter& writer, const QString& elementName, const QString& elementValue);
};

#endif // XMLUTILITY_H

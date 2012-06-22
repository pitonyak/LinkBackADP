#include "xmlutility.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QRegExp>

XMLUtility::XMLUtility()
{
}

QString XMLUtility::booleanToString(bool b)
{
  return b ? "True" : "False";
}

bool XMLUtility::stringToBoolean(const QString& s)
{
  return s.compare("true", Qt::CaseInsensitive) == 0 || s.compare("t", Qt::CaseInsensitive) == 0 || s.compare("1", Qt::CaseSensitive) == 0 ? true : false;
}

QString XMLUtility::caseToString(Qt::CaseSensitivity sensitivity)
{
  return sensitivity == Qt::CaseInsensitive ? "CaseInsensitive" : "CaseSensitive";
}

Qt::CaseSensitivity XMLUtility::stringToCase(const QString& sensitivity)
{
  return sensitivity.contains("insensitive", Qt::CaseInsensitive) ? Qt::CaseInsensitive : Qt::CaseSensitive;
}

QString XMLUtility::patternSyntaxToString(QRegExp::PatternSyntax syntax)
{
  switch (syntax)
  {
  case QRegExp::RegExp:
    return "RegExp";
    break;
  case QRegExp::RegExp2:
    return "RegExp2";
    break;
  case QRegExp::Wildcard:
    return "Wildcard";
    break;
  case QRegExp::WildcardUnix:
    return "WildcardUnix";
    break;
  case QRegExp::FixedString:
    return "FixedString";
    break;
  case QRegExp::W3CXmlSchema11:
    return "W3CXmlSchema11";
    break;
  default:
    break;
  }
  return "RegExp";
}

QRegExp::PatternSyntax XMLUtility::stringToPatternSyntax(const QString& syntax)
{
  if (syntax.compare("RegExp", Qt::CaseInsensitive))
  {
    return QRegExp::RegExp;
  }
  if (syntax.compare("RegExp2", Qt::CaseInsensitive))
  {
    return QRegExp::RegExp2;
  }
  if (syntax.compare("Wildcard", Qt::CaseInsensitive))
  {
    return QRegExp::Wildcard;
  }
  if (syntax.compare("WildcardUnix", Qt::CaseInsensitive))
  {
    return QRegExp::WildcardUnix;
  }
  if (syntax.compare("FixedString", Qt::CaseInsensitive))
  {
    return QRegExp::FixedString;
  }
  if (syntax.compare("W3CXmlSchema11", Qt::CaseInsensitive))
  {
    return QRegExp::W3CXmlSchema11;
  }
  return QRegExp::RegExp;
}

void XMLUtility::write(QXmlStreamWriter& writer, const QRegExp& regExp, const QString& name)
{
  if (regExp.isEmpty() || !regExp.isValid() || name.length() == 0)
  {
    return;
  }

  writer.writeStartElement(name);
  writer.writeAttribute("IsMinimal", booleanToString(regExp.isMinimal()));
  writer.writeAttribute("Pattern", patternSyntaxToString(regExp.patternSyntax()));
  writer.writeAttribute("CaseSensitive", caseToString(regExp.caseSensitivity()));
  writer.writeCharacters(regExp.pattern());
  writer.writeEndElement();
}

QRegExp* XMLUtility::readRegExp(QXmlStreamReader& reader)
{
  QRegExp* regExp = new QRegExp();
  if (reader.attributes().hasAttribute("IsMinimal"))
  {
    regExp->setMinimal(stringToBoolean(reader.attributes().value("IsMinimal").toString()));
  }
  if (reader.attributes().hasAttribute("Pattern"))
  {
    regExp->setPatternSyntax(stringToPatternSyntax(reader.attributes().value("Pattern").toString()));
  }
  if (reader.attributes().hasAttribute("CaseSensitive"))
  {
    regExp->setCaseSensitivity(stringToCase(reader.attributes().value("CaseSensitive").toString()));
  }
  while (!reader.isEndElement())
  {
    if (reader.isCharacters())
    {
      regExp->setPattern(reader.text().toString());
    }
    reader.readNext();
  }
  if (!regExp->isValid())
  {
    delete regExp;
    regExp = nullptr;
  }

  return regExp;
}

void XMLUtility::writeElementAttribute(QXmlStreamWriter& writer, const QString& elementName, const QString& elementValue, const QString& attributeName, const QString& attributeValue)
{
  if (elementName.length() == 0)
  {
    return;
  }
  if (elementValue.length() > 0)
  {
    writer.writeStartElement(elementName);
  }
  else
  {
    writer.writeEmptyElement(elementName);
  }
  if (attributeName.length() > 0 && attributeValue.length() > 0)
  {
    writer.writeAttribute(attributeName, attributeValue);
  }

  if (elementValue.length() > 0)
  {
    writer.writeCharacters(elementValue);
    writer.writeEndElement();
  }
}

#include "xmlutility.h"
#include "linkbackupglobals.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QRegExp>

XMLUtility::XMLUtility()
{
}

QString XMLUtility::booleanToString(const bool b)
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


void XMLUtility::write(QXmlStreamWriter& writer, const QRegExp& regExp, const QString& name)
{
  if (regExp.isEmpty() || !regExp.isValid() || name.length() == 0)
  {
    return;
  }

  writer.writeStartElement(name);
  writer.writeAttribute("IsMinimal", booleanToString(regExp.isMinimal()));
  writer.writeAttribute("CaseSensitive", caseToString(regExp.caseSensitivity()));
  writer.writeAttribute("PatternSyntax", getEnumMapper().PatternSyntaxToString(regExp.patternSyntax()));
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
  if (reader.attributes().hasAttribute("CaseSensitive"))
  {
    regExp->setCaseSensitivity(stringToCase(reader.attributes().value("CaseSensitive").toString()));
  }
  if (reader.attributes().hasAttribute("PatternSyntax"))
  {
    regExp->setPatternSyntax(getEnumMapper().stringToPatternSyntax(reader.attributes().value("PatternSyntax").toString()));
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

void XMLUtility::writeElement(QXmlStreamWriter& writer, const QString& elementName, const QString& elementValue)
{
  if (elementName.length() == 0)
  {
    return;
  }
  if (elementValue.length() > 0)
  {
    writer.writeStartElement(elementName);
    writer.writeCharacters(elementValue);
    writer.writeEndElement();
  }
  else
  {
    writer.writeEmptyElement(elementName);
  }
}

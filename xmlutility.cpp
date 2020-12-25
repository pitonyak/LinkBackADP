#include "xmlutility.h"
#include "linkbackupglobals.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QRegularExpression>

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


void XMLUtility::write(QXmlStreamWriter& writer, const QRegularExpression& regExp, const QString& name)
{
  if (regExp.pattern().isEmpty() || !regExp.isValid() || name.length() == 0)
  {
    return;
  }

  writer.writeStartElement(name);
  // Priory to QT6 there was RegExp class that had minimal (turn off greedy), case sensitive, and you could choose
  // the type of regular expression; for example, PERL, or File Spec. This is no longer supported.
  //writer.writeAttribute("IsMinimal", booleanToString(regExp.isMinimal()));
  //writer.writeAttribute("CaseSensitive", caseToString(regExp.caseSensitivity()));
  //writer.writeAttribute("PatternSyntax", getEnumMapper().PatternSyntaxToString(regExp.patternSyntax()));

  // Enumerate the flags and write them.
  // QRegularExpression does not seem to be a QObject so I cannot grab the staticMetaObject to enumerate the enums.
  if (regExp.patternOptions().testFlag(QRegularExpression::CaseInsensitiveOption)) {
    writer.writeAttribute("CaseInsensitive", "true");
  }
  if (regExp.patternOptions().testFlag(QRegularExpression::DotMatchesEverythingOption)) {
    writer.writeAttribute("DotMatchesEverything", "true");
  }
  if (regExp.patternOptions().testFlag(QRegularExpression::MultilineOption)) {
    writer.writeAttribute("Multiline", "true");
  }
  if (regExp.patternOptions().testFlag(QRegularExpression::ExtendedPatternSyntaxOption)) {
    writer.writeAttribute("ExtendedPatternSyntax", "true");
  }
  if (regExp.patternOptions().testFlag(QRegularExpression::InvertedGreedinessOption)) {
    writer.writeAttribute("InvertedGreediness", "true");
  }
  if (regExp.patternOptions().testFlag(QRegularExpression::DontCaptureOption)) {
    writer.writeAttribute("DontCapture", "true");
  }
  if (regExp.patternOptions().testFlag(QRegularExpression::UseUnicodePropertiesOption)) {
    writer.writeAttribute("UseUnicodeProperties", "true");
  }
  writer.writeCharacters(regExp.pattern());
  writer.writeEndElement();
}

QRegularExpression* XMLUtility::readRegExp(QXmlStreamReader& reader)
{
  QRegularExpression* regExp = new QRegularExpression();

  // Ignore IsMinimal and PatternSyntax because it is no longer supported.
  /**
  if (reader.attributes().hasAttribute("IsMinimal"))
  {
    // Deprecated, no longer supported.
    regExp->setMinimal(stringToBoolean(reader.attributes().value("IsMinimal").toString()));
  }
  if (reader.attributes().hasAttribute("PatternSyntax"))
  {
    regExp->setPatternSyntax(getEnumMapper().stringToPatternSyntax(reader.attributes().value("PatternSyntax").toString()));
  }
  **/

  QRegularExpression::PatternOptions po = regExp->patternOptions();

  if (reader.attributes().hasAttribute("CaseSensitive"))
  {
    Qt::CaseSensitivity cs = stringToCase(reader.attributes().value("CaseSensitive").toString());
    if (cs == Qt::CaseInsensitive) {
      po |= QRegularExpression::CaseInsensitiveOption;
    }
  }
  if (reader.attributes().hasAttribute("CaseInsensitive"))
  {
    if (reader.attributes().value("CaseInsensitive").toString().compare("true", Qt::CaseInsensitive)) {
      po |= QRegularExpression::CaseInsensitiveOption;
    }
  }
  if (reader.attributes().hasAttribute("DotMatchesEverything"))
  {
    if (reader.attributes().value("DotMatchesEverything").toString().compare("true", Qt::CaseInsensitive)) {
      po |= QRegularExpression::DotMatchesEverythingOption;
    }
  }
  if (reader.attributes().hasAttribute("Multiline"))
  {
    if (reader.attributes().value("Multiline").toString().compare("true", Qt::CaseInsensitive)) {
      po |= QRegularExpression::MultilineOption;
    }
  }
  if (reader.attributes().hasAttribute("ExtendedPatternSyntax"))
  {
    if (reader.attributes().value("ExtendedPatternSyntax").toString().compare("true", Qt::CaseInsensitive)) {
      po |= QRegularExpression::ExtendedPatternSyntaxOption;
    }
  }
  if (reader.attributes().hasAttribute("InvertedGreediness"))
  {
    if (reader.attributes().value("InvertedGreediness").toString().compare("true", Qt::CaseInsensitive)) {
      po |= QRegularExpression::InvertedGreedinessOption;
    }
  }
  if (reader.attributes().hasAttribute("DontCapture"))
  {
    if (reader.attributes().value("DontCapture").toString().compare("true", Qt::CaseInsensitive)) {
      po |= QRegularExpression::DontCaptureOption;
    }
  }
  if (reader.attributes().hasAttribute("UseUnicodeProperties"))
  {
    if (reader.attributes().value("UseUnicodeProperties").toString().compare("true", Qt::CaseInsensitive)) {
      po |= QRegularExpression::UseUnicodePropertiesOption;
    }
  }
  regExp->setPatternOptions(po);

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

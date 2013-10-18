#include "qtenummapper.h"


QtEnumMapper::QtEnumMapper()
{
  initialize();
}

QtEnumMapper::~QtEnumMapper()
{
  QMapIterator<QString, QMap<int, QString>* > i(m_valueToName);
  while (i.hasNext())
  {
    i.next();
    delete i.value();
  }
  m_valueToName.clear();
}

QList<QString> QtEnumMapper::getList(const QString& typeName) const
{
  const QMap<int, QString>* enumData = getEnumData(typeName);
  return (enumData != nullptr) ? enumData->values() : QList<QString>();
}

int  QtEnumMapper::stringToEnum(const QString& typeName, const QString& name, const int defaultValue) const
{
  const QMap<int, QString>* enumData = getEnumData(typeName);
  if (enumData != nullptr)
  {
    QMap<int, QString>::const_iterator i = enumData->constBegin();
    while (i != enumData->constEnd())
    {
      if (name.compare(i.value(), Qt::CaseInsensitive) == 0)
      {
        return i.key();
      }
    }
  }
  return defaultValue;
}

QString QtEnumMapper::enumToString(const QString& typeName, const int value, const QString& defaultName) const
{
  const QMap<int, QString>* enumData = getEnumData(typeName);
  return (enumData != nullptr && enumData->contains(value)) ? enumData->value(value) : defaultName;
}

const QMap<int, QString> *QtEnumMapper::getEnumData(const QString &typeName) const
{
  QString upperCaseName = typeName.toUpper();
  if (m_valueToName.contains(upperCaseName))
  {
    return m_valueToName.value(upperCaseName);
  }
  return nullptr;
}

void QtEnumMapper::initialize()
{
  QMap<int, QString>* regExpPatterns = new QMap<int, QString>();
  (*regExpPatterns)[QRegExp::RegExp] = "Perl-Like";
  (*regExpPatterns)[QRegExp::RegExp2] = "Greedy Perl-Like";
  (*regExpPatterns)[QRegExp::Wildcard] = "Wild Card";
  (*regExpPatterns)[QRegExp::WildcardUnix] = "Wild Card Unix";
  (*regExpPatterns)[QRegExp::FixedString] = "Fixed String";
  (*regExpPatterns)[QRegExp::W3CXmlSchema11] = "W3C XML Schema";
  m_valueToName["PatternSyntax"] = regExpPatterns;
}


#include "stringhelper.h"
#include <QDebug>

StringHelper::StringHelper()
{
}

//**************************************************************************
//! Facilitate checking flags such as "-s" and "-s-".
/*!
 * \param arg Argument to check
 * \param argCheck is it this argument or this argument with a trailing "-"?
 * \param  setValue This is set to true if it is argCheck and false if it is argCheck + "-".
 * \returns true if the flag was found and processed.
 *
 ***************************************************************************/
bool StringHelper::BooleanArgChecker(const QString& arg, const QString& argCheck, bool& setValue, Qt::CaseSensitivity cs)
{
  if (arg.compare(argCheck, cs))
  {
    setValue = true;
    return true;
  }
  if (arg.compare(argCheck + "-", cs))
  {
    setValue = false;
    return true;
  }
  return false;
}

//**************************************************************************
//! Facilitate checking flags such as "-s=value"
/*!
 * \param arg Argument to check
 * \param argCheck is it this argument with a trailing value?
 * \param  setValue This is set to the value.
 * \returns true if the flag was found and processed.
 *
 ***************************************************************************/
bool StringHelper::StringArgChecker(const QString& arg, const QString& argCheck, QString& setValue, Qt::CaseSensitivity cs)
{
  if (arg.startsWith(argCheck, cs))
  {
    setValue = arg.mid(argCheck.size());
    return true;
  }
  return false;
}

void StringHelper::ForceLastChar(QString& s, QChar c, Qt::CaseSensitivity cs)
{
  if (!s.endsWith(c, cs))
  {
    s += c;
  }
}

/**
 * str - Source string to verify
 * wild - Wild card string to verify against.
 * bCaseSensitive - If true, then the search is case sensitive.
 *
 * This string matches a string against a wildcard string.
 * The '?' character matches any single charcter.
 * The '*' character matches any string, including zero length strings.
 * This routine is Copyright (c) by Andrew Pitonyak, and may be used
 * without restrictions or renumerations. I make no statement of correctness.
 */
bool StringHelper::FNMatch(const QChar* str, const QChar* wild, Qt::CaseSensitivity cs)
{
if ( str == nullptr )
  {
    //throw ArgumentNullException("source string is null", FILE_LOC());
    qDebug() << "source string is null in StringHelper::FNMatch";
    return false;
  }

  if ( wild == nullptr )
  {
    //throw ArgumentNullException("wild card string is null", FILE_LOC());
    qDebug() << "wild is null in StringHelper::FNMatch";
    return false;
  }

  if ( str->isNull() )
  {
    while ( *wild == '*' )
      ++wild;
    return wild->isNull();
  }

  while ( !str->isNull() && !wild->isNull() )
  {
    if ( *str == *wild || *wild == '?' || (cs == Qt::CaseInsensitive && str->toLower() == wild->toLower()) )
    {
      ++str;
      ++wild;
    }
    else if ( *wild == '*' )
    {
      // Skip runs of wild card characters as one
      while ( *wild == '*' )
        ++wild;

      // Ended with a '*' so this must match.
      if ( wild->isNull() )
        return true;

      // wild card match
      // try a non-greedy match first because it is
      // easier to write.
      // First check a zero length match!
      //while (!str->isNull())
      //{
      //    if (FNMatch(str, wild, cs))
      //        return true;
      //    else
      //        ++str;
      //}
      //return false;

      // Try a greedy match. I do not need to check against an empty string.
      for ( int i = StringHelper::QStrLen(str) - 1; i>= 0; --i )
      {
        if ( FNMatch(str + i, wild, cs) )
        {
          return true;
        }
      }
      return false;
    }
    else
    {
      return false;
    }
  }
  // Ignore trailing wild cards
  while ( *wild == '*' )
    ++wild;

  // See if have ended for both.
  // Remember that if we are here, then at least one of the strings
  // is zero length, which means that they must both be zero length.
  return (str->isNull() && wild->isNull());
}

int StringHelper::QStrLen(const QChar* s)
{
  int i=0;
  if (s != nullptr)
  {
    while (!s->isNull())
    {
      ++i;
      ++s;
    }
  }
  return i;
}


QString StringHelper::RemoveEntities(const QString& sSource)
{
  QString s = sSource;
  const char* sEntities[] = {
    "&gt;", ">",
    "&lt;", "<",
    "&quot;", "\"",
    "&apos;", "'",
    "&amp;", "&",
  };

  int sx = sizeof(sEntities)/sizeof(*sEntities);

  for ( int i = 0; i < sx; i += 2 )
  {
    s.replace(sEntities[i], sEntities[i+1], Qt::CaseInsensitive);
  }
  return s;
}

QString StringHelper::RenderSafeForRegEx(const QString& dirtyString)
{
  QString s = dirtyString;
  QString specialCharacters = "\\[^$.|?*+()";
  for (QString::const_iterator i = specialCharacters.constBegin(); i!= specialCharacters.constEnd(); ++i)
  {
    s.replace((*i), QString("\\%1").arg(*i));
  }
  return s;
}

QStringList StringHelper::split(const QChar & sep, const QString& s, int max)
{
    QStringList list = s.split(sep);
    if (list.count() > max)
    {
        QString s = list.at(max - 1);
        list.removeAt(max-1);
        while (list.count() >= max)
        {
            s = s.append(sep);
            s = s.append(list.at(max-1));
            list.removeAt(max-1);
        }
        list.append(s);
    }
    return list;
}


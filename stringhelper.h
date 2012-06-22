#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <QString>
#include <QStringList>

//**************************************************************************
//! Functions that I frequently desire, but are missing in QString.
/*!
 * QString added some of the methods that I wanted that were missing
 * from std::string, but not all of them.
 **************************************************************************/
class StringHelper
{
public:
  //**************************************************************************
  //! Facilitate checking flags such as "-s" and "-s-".
  /*!
   * \param arg Argument to check
   * \param argCheck is it this argument or this argument with a trailing "-"?
   * \param  setValue This is set to true if it is argCheck and false if it is argCheck + "-".
   * \returns true if the flag was found and processed.
   *
   ***************************************************************************/
  static bool BooleanArgChecker(const QString& arg, const QString& argCheck, bool& setValue, Qt::CaseSensitivity cs = Qt::CaseSensitive);

  //**************************************************************************
  //! Facilitate checking flags such as "-s=value"
  /*!
   * \param arg Argument to check
   * \param argCheck is it this argument with a trailing value?
   * \param  setValue This is set to the value.
   * \returns true if the flag was found and processed.
   *
   ***************************************************************************/
  static bool StringArgChecker(const QString& arg, const QString& argCheck, QString& setValue, Qt::CaseSensitivity cs = Qt::CaseSensitive);

  //**************************************************************************
  //! Force a string to end with a specific character.
  /*!
   * Forcing a string to end with a specific character
   * is frequently used while dealing with file paths, for example.
   * \param s The string that is modified to end with the character.
   * \param c Will be the last character in the string.
   * \sa StringEndsWith()
   ***************************************************************************/
  static void ForceLastChar(QString& s, QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive);

  //**************************************************************************
  //! Perform wild card matching on strings.
  /*!
   * Perform wild card matching on the string.
   *   - This string matches a string against a wildcard string.
   *   - The '?' character matches any single charcter.
   *   - The '*' character matches any string, including zero length strings.
   *
   *  This routine is Copyright (c) by Andrew Pitonyak, and may be used
   *  without restrictions or renumerations. I make no statement of correctness.
   * \param sStr  Source string to verify
   * \param sWild Wild card string to verify against.
   * \param cs    If true, then the search is case sensitive.
   * \return True if the supplied string matches the wildcard, false otherwise.
   ***************************************************************************/
  static bool FNMatch(const QString& sStr, const QString sWild, Qt::CaseSensitivity cs = Qt::CaseInsensitive)
  {
    //QString s = sStr;
    //QString w = sWild;
    //return FNMatch(s.data(), w.data(), cs);
    return FNMatch(sStr.data(), sWild.data(), cs);
  }

  static int QStrLen(const QChar* s);

  //**************************************************************************
  //! Perform wild card matching on strings.
  /*!
   * Perform wild card matching on the string.
   *   - This string matches a string against a wildcard string.
   *   - The '?' character matches any single charcter.
   *   - The '*' character matches any string, including zero length strings.
   *
   *  This routine is Copyright (c) by Andrew Pitonyak, and may be used
   *  without restrictions or renumerations. I make no statement of correctness.
   * \param str   Source string to verify
   * \param wild  Wild card string to verify against.
   * \param cs    If true, then the search is case sensitive.
   * \return True if the supplied string matches the wildcard, false otherwise.
   ***************************************************************************/
  static bool FNMatch(const QChar* str, const QChar* wild, Qt::CaseSensitivity cs = Qt::CaseInsensitive);

  //**************************************************************************
  //! Make a copy of the argument that does not contain entities.
  /*!
   * A string is returned that is the same as the argument but with the entities replaced by their
   * standard form. For example, &gt; becomes >. Supported entities include:
   *    - \&gt; becomes >
   *    - \&lt; becomes <
   *    - \&quot; becomes "
   *    - \&apos; becomes '
   *    - \&amp; becomes &
   *
   * \param sSource String containg entities to be removed.
   * \return A copy of the string without the entities.
   ***************************************************************************/
  static QString RemoveEntities(const QString& sSource);

  //**************************************************************************
  //! Render special characters safe for use in a regular expression.
  /*!
   * \param dirtyString
   * \return A string with all special characters preceded by a backslash.
   *
   ***************************************************************************/
  static QString RenderSafeForRegEx(const QString& dirtyString);

  //**************************************************************************
  //! Perform a split but limit the number of returned strings.
  /*!
   * While spliting a file name containing a comma, there was a problem.
   * If there are more matches than required, then the extras are put back together
   * are returned as the last entry in the list.
   * \param sep Split on this string.
   * \param s String to split.
   * \param max Maximum number of strings to return.
   * \return A string with all special characters preceded by a backslash.
   *
   ***************************************************************************/
  static QStringList split(const QChar & sep, const QString& s, int max);


private:
    StringHelper();
};

#endif // STRINGHELPER_H

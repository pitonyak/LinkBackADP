#ifndef QTENUMMAPPER_H
#define QTENUMMAPPER_H

#include <QMap>
#include <QString>

#include <QRegularExpression>
#include <QList>

//**************************************************************************
/*! \class QtEnumMapper
 * \brief Provide a fast method to convert QT Enums to/from strings.
 *
 * Many of QT's enums are not set to include meta-data so I require custom methods to obtain this information.
 * I became tired of writing the same code over and over, so I created this class.
 *
 * Efficiency was ignored while designing this class, assuming that it will be used in low-volume situations.
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2020
 ***************************************************************************/
class QtEnumMapper
{
public:
  QtEnumMapper();
  ~QtEnumMapper();

  QList<QString> getList(const QString& typeName) const;
  int  stringToEnum(const QString& typeName, const QString& name, const int defaultValue = 0) const;
  QString enumToString(const QString& typeName, const int value, const QString& defaultName = "") const;

  inline QRegularExpression::PatternOption stringToPatternOption(const QString& name, const QRegularExpression::PatternOption defaultValue = QRegularExpression::NoPatternOption) const {
    return static_cast<QRegularExpression::PatternOption>(stringToEnum("PatternOption", name, defaultValue));
  }

  inline QRegularExpression::MatchOption stringToMatchOption(const QString& name, const QRegularExpression::MatchOption defaultValue = QRegularExpression::NoMatchOption) const {
    return static_cast<QRegularExpression::MatchOption>(stringToEnum("MatchOption", name, defaultValue));
  }

  inline QRegularExpression::WildcardConversionOption stringToWildcardConversionOption(const QString& name, const QRegularExpression::WildcardConversionOption defaultValue = QRegularExpression::DefaultWildcardConversion) const {
    return static_cast<QRegularExpression::WildcardConversionOption>(stringToEnum("WildcardConversionOption", name, defaultValue));
  }

  inline QString PatternSyntaxToString(const int value, const QString& defaultName = "Greedy Perl-Like") const {
    return enumToString("PatternSyntax", value, defaultName);
  }

  inline QString PatternOptionToString(const int value, const QString& defaultName = "NoPatternOption") const {
    return enumToString("PatternOption", value, defaultName);
  }

  inline QString MatchOptionToString(const int value, const QString& defaultName = "NoMatchOption") const {
    return enumToString("MatchOption", value, defaultName);
  }

  inline QString WildcardConversionOptionToString(const int value, const QString& defaultName = "DefaultWildcardConversion") const {
    return enumToString("WildcardConversionOption", value, defaultName);
  }

private:
  const QMap<int, QString> *getEnumData(const QString& typeName) const;
  void initialize();

  QMap<QString, QMap<int, QString>* > m_valueToName;
};

#endif // QTENUMMAPPER_H

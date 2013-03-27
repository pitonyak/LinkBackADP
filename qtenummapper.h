#ifndef QTENUMMAPPER_H
#define QTENUMMAPPER_H

#include <QMap>
#include <QString>

#include <QRegExp>
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
 * \date 2011-2013
 ***************************************************************************/
class QtEnumMapper
{
public:
  QtEnumMapper();
  ~QtEnumMapper();

  QList<QString> getList(const QString& typeName) const;
  int  stringToEnum(const QString& typeName, const QString& name, const int defaultValue = 0) const;
  QString enumToString(const QString& typeName, const int value, const QString& defaultName = "") const;

  inline QRegExp::PatternSyntax stringToPatternSyntax(const QString& name, const QRegExp::PatternSyntax defaultValue = QRegExp::RegExp2) const {
    return static_cast<QRegExp::PatternSyntax>(stringToEnum("PatternSyntax", name, defaultValue));
  }

  inline QString PatternSyntaxToString(const int value, const QString& defaultName = "Greedy Perl-Like") const {
    return enumToString("PatternSyntax", value, defaultName);
  }

private:
  const QMap<int, QString> *getEnumData(const QString& typeName) const;
  void initialize();

  QMap<QString, QMap<int, QString>* > m_valueToName;
};

#endif // QTENUMMAPPER_H

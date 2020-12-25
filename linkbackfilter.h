#ifndef LINKBACKFILTER_H
#define LINKBACKFILTER_H

#include <QObject>
#include <QVariant>
#include <QDate>
#include <QRegularExpression>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class QFileInfo;
class QRegularExpression;

//**************************************************************************
/*! \class LinkBackFilter
 * \brief Filter to decide if a specific file or directory matches some criteria.
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2020
 ***************************************************************************/

class LinkBackFilter : public QObject
{
  Q_OBJECT
  Q_ENUMS(CompareType)
  Q_ENUMS(CompareField)

  Q_PROPERTY(CompareType compareType READ getCompareType WRITE setCompareType NOTIFY compareTypeChanged RESET setCompareTypeDefault STORED true)
  Q_PROPERTY(CompareField compareField READ getCompareField WRITE setCompareField NOTIFY compareFieldChanged RESET setCompareFieldDefault STORED true)
  Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ getCaseSensitivity WRITE setCaseSensitivity NOTIFY caseSensitivityChanged RESET setCaseSensitivityDefault STORED true)
  Q_PROPERTY(bool invertFilterResult READ isInvertFilterResult WRITE setInvertFilterResult NOTIFY invertFilterResultChanged RESET setInvertFilterResultDefault STORED true)
  Q_PROPERTY(bool filterMeansAccept READ isFilterMeansAccept WRITE setFilterMeansAccept NOTIFY filterMeansAcceptChanged RESET setFilterMeansAcceptDefault STORED true)
  Q_PROPERTY(bool filterFiles READ isFilterFiles WRITE setFilterFiles NOTIFY filterFilesChanged RESET setFilterFilesDefault STORED true)
  Q_PROPERTY(bool filterDirs READ isFilterDirs WRITE setFilterDirs NOTIFY filterDirsChanged RESET setFilterDirsDefault STORED true)
  Q_PROPERTY(QVariant value READ getValue WRITE setValue NOTIFY valueChanged RESET setValueDefault STORED true)

public:
    //**************************************************************************
    /*! \brief Enumerate the supported comparisons such as Less and Less Equal.*/
    //**************************************************************************
    enum CompareType {Less, LessEqual, Equal, GreaterEqual, Greater, NotEqual, RegularExpression, FileSpec, Contains, RegExpPartial, RegExpFull};

    //**************************************************************************
    /*! \brief Enumerate the parts of a file that can be compared. */
    //**************************************************************************
    enum CompareField {Date, DateTime, FullPath, Name, Size, Time, PathOnly};

    //**************************************************************************
    /*! \brief Default constructor with the specified parent.
     *
     *  All objects owned by the parent are destroyed in the destructor.
     *  \param [in] parent The object's owner.
     ***************************************************************************/
    explicit LinkBackFilter(QObject *parent = 0);

    //**************************************************************************
    /*! \brief Constructor copied from the specified filter object.
     *
     *  \param [in] filter This filter object is copied into this object.
     ***************************************************************************/
    LinkBackFilter(const LinkBackFilter& filter);

    //**************************************************************************
    /*! \brief Constructor copied from the specified filter object.
     *
     *  \param [in] filter This filter object is copied into this object.
     *  \param [in] parent The object's owner.
     ***************************************************************************/
    explicit LinkBackFilter(const LinkBackFilter& filter, QObject *parent);

    //**************************************************************************
    /*! \brief Destructor in case I need any special clean-up. */
    //**************************************************************************
    ~LinkBackFilter();

    //**************************************************************************
    /*! \brief Copy anoter filter into this filter.
     *
     *  This object's parent object will not change.
     *  \param [in] filter Filter to copy into this one.
     *  \return Reference to this object.
     ***************************************************************************/
    LinkBackFilter& operator=(const LinkBackFilter& filter);

    //**************************************************************************
    /*! \brief Create a copy of this object that you then own and must delete.
     *
     *  \param [in] parent The cloned object's new owner; because that is not copied while cloning.
     *  \return Cloned copy of this object that you own.
     ***************************************************************************/
    LinkBackFilter* clone(QObject *parent = 0) const;

    //**************************************************************************
    /*! \brief Determine if the fileInfo object passes the filter.
     *
     *  \param [in] fileInfo Information about the file to compare.
     *  \return True if the object passes the filter. It is the callers problem to then understand that the filter is a "reject" filter, which means that the file is rejected.
     ***************************************************************************/
    bool passes(const QFileInfo& fileInfo) const;

    //**************************************************************************
    /*! \brief Determine if this filter is applicable to the fileInfo type. In other words, some filters apply to directories, and some to filenames.
     *
     *  Applicable ignores the inverted flag. Inverted only applies to the comparison.
     *  \param [in] fileInfo Information about the file to compare.
     *  \return True if the filter applies to files and QFileInfo represents a file, similarly for directories.
     ***************************************************************************/
    bool applicable(const QFileInfo& fileInfo) const;

    //**************************************************************************
    /*! \brief Get the compare type; equal, less than, greater than, etc...
     ***************************************************************************/
    CompareType getCompareType() const;
    void setCompareType(CompareType compareType=Equal);
    void setCompareTypeDefault();
    QString getCompareTypeAsString() const;

    //**************************************************************************
    /*! \brief Get the field that is compared; file name, full path, date, etc...
     ***************************************************************************/
    CompareField getCompareField() const;
    void setCompareField(CompareField compareField=Name);
    void setCompareFieldDefault();
    QString getCompareFieldAsString() const;

    //**************************************************************************
    /*! \brief Are comparisons case sensitive?
     ***************************************************************************/
    Qt::CaseSensitivity getCaseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity caseSensitivity=Qt::CaseInsensitive);
    void setCaseSensitivityDefault();
    QRegularExpression::PatternOptions getCaseSensitivytOption() const;

    //**************************************************************************
    /*! \brief Determine if the filter criteria is inverted before determining acceptance.
     *
     * Inverting a filter provides an easy way to expand the comparison criteria.
     * This allows me to say things such as the filter passes if a regular expression does not match.
     *
     *  \return True if the filter criteria is inverted before determining acceptance.
     ***************************************************************************/
    bool isInvertFilterResult() const;
    void setInvertFilterResult(bool invertFilterResult=false);
    void setInvertFilterResultDefault();

    //**************************************************************************
    /*! \brief Does passing the filter mean that the value should be accepted / backed up. If not, then passing means the file or directory is skipped.
     ***************************************************************************/
    bool isFilterMeansAccept() const;
    void setFilterMeansAccept(bool filterMeansAccept=true);
    void setFilterMeansAcceptDefault();

    //**************************************************************************
    /*! \brief Does this filter apply to directories?
     ***************************************************************************/
    bool isFilterDirs() const;
    void setFilterDirs(bool filterDirs=true);
    void setFilterDirsDefault();

    //**************************************************************************
    /*! \brief Does this filter apply to files?
     ***************************************************************************/
    bool isFilterFiles() const;
    void setFilterFiles(bool filterFiles=true);
    void setFilterFilesDefault();

    //**************************************************************************
    /*! \brief Is the "value" a comma delimited set of values where "passing" means match any one value.
     ***************************************************************************/
    bool isMultiValued() const;
    void setMultiValued(bool multiValued=false);
    void setMultiValuedDefault();

    //**************************************************************************
    /*! \brief Value against which the compare field is compared.
     ***************************************************************************/
    const QVariant& getValue() const;
    void setValue(const QVariant& value);
    void setValueDefault();

    //**************************************************************************
    /*! \brief Same as calling each set function with the default value.
     *
     * - Default Compare Type is Equal
     * - Default compare field is the name (which includes the file extension).
     * - Default for case comparisons is case insensitive compares.
     * - Default Filter acceptance is inverted is False
     * - Default Filter means accept is True
     * - Default Filter applies to directories is True
     * - Default Filter applies to files is True
     * - Default Multi-Valued is False
     ***************************************************************************/
    void setAllDefault();


    void setValue(const qlonglong);
    void setValue(const QDate&);
    void setValue(const QTime&);
    void setValue(const QDateTime&);
    void setValue(const QString&);
    void setValue(const QRegularExpression&);

    QString getMainValueAsString() const;

    QXmlStreamWriter& operator<<(QXmlStreamWriter& writer) const;
    QXmlStreamReader& operator>>(QXmlStreamReader& reader);


private:
    QXmlStreamReader& readFilter(QXmlStreamReader& reader);
    void readInternals(QXmlStreamReader& reader, const QString& version);
    void createRegularExpression(const QVariant& value);


signals:
    void compareTypeChanged(CompareType);
    void compareFieldChanged(CompareField);
    void caseSensitivityChanged(bool);
    void invertFilterResultChanged(bool);
    void filterMeansAcceptChanged(bool);
    void filterFilesChanged(bool);
    void filterDirsChanged(bool);
    void valueChanged(QVariant);
    void multiValuedChanged(bool);

public slots:

private:
    bool compareValues(const qlonglong aSize) const;
    bool compareValues(const QTime& aTime) const;
    bool compareValues(const QDate& aDate) const;
    bool compareValues(const QDateTime& aDateTime) const;
    bool compareValues(const QString& filePortion) const;

    /*! \brief Empty the lists, but do not delete them. */
    void clearLists(bool deleteLists, bool createIfDoNotExist);
    void createLists();
    void createRegularExpressions();

    /*! \brief Enumerate the supported comparisons such as Less and Less Equal. */
    enum CompareType m_compareType;

    /*! \brief Enumerate the parts of a file that can be compared. */
    enum CompareField m_compareField;

    Qt::CaseSensitivity m_caseSensitivity;
    bool m_invertFilterResult;
    bool m_filterMeansAccept;
    bool m_filterFiles;
    bool m_filterDirs;
    bool m_multiValued;

    /*! \brief Single value representation of this filter. */
    QVariant m_value;

    QList<QVariant>* m_values;
    QList<QRegularExpression*>* m_expressions;

};


inline LinkBackFilter::CompareType LinkBackFilter::getCompareType() const
{
  return m_compareType;
}

inline void LinkBackFilter::setCompareTypeDefault()
{
  setCompareType();
}

inline LinkBackFilter::CompareField LinkBackFilter::getCompareField() const
{
  return m_compareField;
}

inline void LinkBackFilter::setCompareFieldDefault()
{
  setCompareField();
}

inline Qt::CaseSensitivity LinkBackFilter::getCaseSensitivity() const
{
  return m_caseSensitivity;
}

inline void LinkBackFilter::setCaseSensitivityDefault()
{
  setCaseSensitivity();
}

inline bool LinkBackFilter::isInvertFilterResult() const
{
  return m_invertFilterResult;
}

inline void LinkBackFilter::setInvertFilterResultDefault()
{
  setInvertFilterResult();
}

inline bool LinkBackFilter::isFilterMeansAccept() const
{
  return m_filterMeansAccept;
}

inline void LinkBackFilter::setFilterMeansAcceptDefault()
{
  setFilterMeansAccept();
}

inline void LinkBackFilter::setValue(const qint64 x)
{
  setValue(QVariant(x));
}

inline void LinkBackFilter::setValue(const QDate& x)
{
  setValue(QVariant(x));
}

inline void LinkBackFilter::setValue(const QTime& x)
{
  setValue(QVariant(x));
}

inline void LinkBackFilter::setValue(const QDateTime& x)
{
  setValue(QVariant(x));
}

inline void LinkBackFilter::setValue(const QString& x)
{
  setValue(QVariant(x));
}

inline void LinkBackFilter::setValue(const QRegularExpression& x)
{
  setValue(QVariant(x));
}

inline QString LinkBackFilter::getMainValueAsString() const
{
  if (m_value.canConvert(QMetaType(QMetaType::QString)))
  {
    return m_value.toString();
  } else {
    return "";
  }
}

inline const QVariant& LinkBackFilter::getValue() const
{
  return m_value;
}

inline QXmlStreamWriter& operator<<(QXmlStreamWriter& os, const LinkBackFilter& filter)
{
    return filter.operator<<(os);
}

inline QXmlStreamReader& operator>>(QXmlStreamReader& is, LinkBackFilter& filter)
{
    return filter.operator>>(is);
}

#endif // LINKBACKFILTER_H

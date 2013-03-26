#ifndef CRITERIAFORFILEMATCH_H
#define CRITERIAFORFILEMATCH_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMap>

//**************************************************************************
/*! \class CriteriaForFileMatch
 * \brief Represents criteria used to determine if two files match.
 *
 * This object represents criteria such as compare the file name and file size of two files.
 * This object specifies the criteria, but does not perform the comparison.
 * Usually, more than one criteria is set; for example, file size and file hash
 *
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2011-2013
 *
 ***************************************************************************/

class CriteriaForFileMatch : public QObject
{
  Q_OBJECT
  Q_ENUMS(CriteriaField)

public:
  enum CriteriaField {FileName=0x01, FullPath=0x02, Time=0x04, Size=0x08, Hash=0x10};
  Q_DECLARE_FLAGS(CriteriaFields, CriteriaField)

  //**************************************************************************
  //! Default Constructor
  /*!
   * \param [in] parent This is a QObject, so you can set a parent that will control the lifetime of this object.
   *
   ***************************************************************************/
  explicit CriteriaForFileMatch(QObject *parent = 0);

  //**************************************************************************
  //! Copy Constructor
  /*!
   * \param [in] criteria Object to copy.
   * \param parent This is a QObject, so you can set a parent that will control the lifetime of this object.
   *
   ***************************************************************************/
  explicit CriteriaForFileMatch(const CriteriaForFileMatch& criteria, QObject *parent = 0);

  //**************************************************************************
  /*! \brief Destructor
   ***************************************************************************/
  ~CriteriaForFileMatch();

  //**************************************************************************
  //! Assignment operator
  /*!
   * \param [in] criteria Object to assign to this object.
   * \returns Reference to this object.
   *
   ***************************************************************************/
  CriteriaForFileMatch& operator=(const CriteriaForFileMatch& criteria);

  //**************************************************************************
  //! Copy the argument into this object.
  /*!
   * \param [in] criteria Object to assign to this object.
   * \returns Reference to this object.
   *
   ***************************************************************************/
  const CriteriaForFileMatch& copy(const CriteriaForFileMatch& criteria);

  //**************************************************************************
  //! Should the file name be used for comparison.
  /*!
   * \returns True if file name should be used for comparison.
   *
   ***************************************************************************/
  bool isFileName() const;

  //**************************************************************************
  //! Should the full path including file name be used for comparison.
  /*!
   * \returns True if the full path including file name should be used for comparison.
   *
   ***************************************************************************/
  bool isFullPath() const;

  //**************************************************************************
  //! Should the file date and time be used for comparison.
  /*!
   * \returns True if the file date and time should be used for comparison.
   *
   ***************************************************************************/
  bool isDateTime() const;

  //**************************************************************************
  //! Should the file size be used for comparison.
  /*!
   * \returns True if file size should be used for comparison.
   *
   ***************************************************************************/
  bool isFileSize() const;

  //**************************************************************************
  //! Should a hash of the file's contents be used for comparison.
  /*!
   * \returns True if a hash of the file's contents be used for comparison.
   *
   ***************************************************************************/
  bool isFileHash() const;

  //**************************************************************************
  //! Set if the file's name should be used for comparison.
  /*!
   * \param [in] value set to True if yes and False if not.
   *
   ***************************************************************************/
  void setFileName(bool value = false);

  //**************************************************************************
  //! Set if the file's full path (including the name) should be used for comparison.
  /*!
   * \param [in] value set to True if yes and False if not.
   *
   ***************************************************************************/
  void setFullPath(bool value = true);

  //**************************************************************************
  //! Set if the file's last changed date and time should be used for comparison.
  /*!
   * \param [in] value set to True if yes and False if not.
   *
   ***************************************************************************/
  void setDateTime(bool value = false);

  //**************************************************************************
  //! Set if the file's size should be used for comparison.
  /*!
   * \param [in] value set to True if yes and False if not.
   *
   ***************************************************************************/
  void setFileSize(bool value = true);

  //**************************************************************************
  //! Set if a hash of the file's contents should be used for comparison.
  /*!
   * \param [in] value set to True if yes and False if not.
   *
   ***************************************************************************/
  void setFileHash(bool value = true);

  //**************************************************************************
  //! Determine if the specified field should be used as a matching criteria.
  /*!
   * \param [in] field Is the field of interest.
   * \returns True if the field exists and is set to True, and False otherwise.
   *
   ***************************************************************************/
  bool getField(CriteriaField field) const;

  //**************************************************************************
  //! Set if the specified field should be used as a matching criteria.
  /*!
   * \param [in] field Is the field of interest.
   * \param [in] value Is True if the field should be used to match.
   *
   ***************************************************************************/
  void setField(CriteriaFields fields, bool value);

  //**************************************************************************
  //! Stream this criteria object as XML to the writer.
  /*!
   * An example of how a single Criteria object will appear as XML is shown below:
   * \code{.xml}
   * <Criteria>
   *   <FullPath>False</FullPath>
   *   <FileName>False</FileName>
   *   <Time>False</Time>
   *   <Size>True</Size>
   *   <Hash>True</Hash>
   * </Criteria>
   * \endcode
   *
   * \param [in,out] writer is the stream to which the XML is written.
   * \returns A reference to the XML Stream Writer.
   *
   ***************************************************************************/
  QXmlStreamWriter& operator<<(QXmlStreamWriter& writer) const;

  //**************************************************************************
  //! Stream XML into this criteria object.
  /*!
   *
   * \param [in,out] reader is the stream from which the XML is read.
   * \returns A reference to the XML Stream Reader.
   *
   ***************************************************************************/
  QXmlStreamReader& operator>>(QXmlStreamReader& reader) {
      return readCriteria(reader);
  }


private:
  //**************************************************************************
  //! Stream XML into this criteria object. Criteria must be current.
  /*!
   *
   * \param [in,out] reader is the stream from which the XML is read.
   * \returns A reference to the XML Stream Reader.
   *
   ***************************************************************************/
  QXmlStreamReader& readCriteria(QXmlStreamReader& reader);

  //**************************************************************************
  //! Stream XML into this criteria object. Criteria may be current.
  /*!
   *
   * \param [in,out] reader is the stream from which the XML is read.
   * \returns A reference to the XML Stream Reader.
   *
   ***************************************************************************/
  void readInternals(QXmlStreamReader& reader);

signals:

public slots:
  /*! \brief Set all values to default. */
  void setAllDefault();

private:
  /*! \brief holds the criteria values. */
  QMap<CriteriaField, bool>* m_criteria;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CriteriaForFileMatch::CriteriaFields)

inline QXmlStreamWriter& operator<<(QXmlStreamWriter& writer, const CriteriaForFileMatch& criteria)
{
    return criteria.operator<<(writer);
}

inline QXmlStreamReader& operator>>(QXmlStreamReader& reader, CriteriaForFileMatch& criteria)
{
    return criteria.operator>>(reader);
}


inline bool CriteriaForFileMatch::isFileName() const
{
    return getField(CriteriaField::FileName);
}
inline bool CriteriaForFileMatch::isFullPath() const
{
    return getField(CriteriaField::FullPath);
}
inline bool CriteriaForFileMatch::isDateTime() const
{
    return getField(CriteriaField::Time);
}
inline bool CriteriaForFileMatch::isFileSize() const
{
    return getField(CriteriaField::Size);
}
inline bool CriteriaForFileMatch::isFileHash() const
{
    return getField(CriteriaField::Hash);
}

inline void CriteriaForFileMatch::setFileName(bool value)
{
    setField(CriteriaField::FileName, value);
}
inline void CriteriaForFileMatch::setFullPath(bool value)
{
  setField(CriteriaField::FullPath, value);
}
inline void CriteriaForFileMatch::setDateTime(bool value)
{
  setField(CriteriaField::Time, value);
}
inline void CriteriaForFileMatch::setFileSize(bool value)
{
  setField(CriteriaField::Size, value);
}
inline void CriteriaForFileMatch::setFileHash(bool value)
{
  setField(CriteriaField::Hash, value);
}

#endif // CRITERIAFORFILEMATCH_H

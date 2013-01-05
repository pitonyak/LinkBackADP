#ifndef CRITERIAFORFILEMATCH_H
#define CRITERIAFORFILEMATCH_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

//**************************************************************************
//! Represents criteria used to determine if two files match.
/*!
 * This object represents criteria such as compare the file name and file size of two files.
 * This object specifies the criteria, but does not perform the comparison.
 * Usually, more than one criteria is set; for example, file size and file hash
 *
 *  This routine is Copyright (c) by Andrew Pitonyak, and may be used
 *  without restrictions or renumerations. I make no statement of correctness.
 ***************************************************************************/


class CriteriaForFileMatch : public QObject
{
  Q_OBJECT
  Q_ENUMS(CompareType)

public:
  enum CriteriaField {FileName=0, FullPath=1, DateTime=2, FileSize=3, FileHash=4};

  explicit CriteriaForFileMatch(QObject *parent = 0);
  explicit CriteriaForFileMatch(const CriteriaForFileMatch& criteria, QObject *parent = 0);

  const CriteriaForFileMatch& operator=(const CriteriaForFileMatch& criteria);
  const CriteriaForFileMatch& copy(const CriteriaForFileMatch& criteria);

  bool isFileName() const;
  bool isFullPath() const;
  bool isDateTime() const;
  bool isFileSize() const;
  bool isFileHash() const;

  void setFileName(bool fileName = false);
  void setFullPath(bool fullPath = true);
  void setDateTime(bool dateTime = false);
  void setFileSize(bool fileSize = true);
  void setFileHash(bool fileHash = true);

  bool getData(CriteriaField field) const;
  void setField(CriteriaField field, bool value);

  QXmlStreamWriter& operator<<(QXmlStreamWriter& writer) const;

  QXmlStreamReader& operator>>(QXmlStreamReader& reader) {
      return readCriteria(reader);
  }


private:
  QXmlStreamReader& readCriteria(QXmlStreamReader& reader);
  void readInternals(QXmlStreamReader& reader);

signals:

public slots:
  void setAllDefault();

private:
  bool m_fileName;
  bool m_fullPath;
  bool m_dateTime;
  bool m_fileSize;
  bool m_fileHash;
};

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
  return m_fileName;
}
inline bool CriteriaForFileMatch::isFullPath() const
{
  return m_fullPath;
}
inline bool CriteriaForFileMatch::isDateTime() const
{
  return m_dateTime;
}
inline bool CriteriaForFileMatch::isFileSize() const
{
  return m_fileSize;
}
inline bool CriteriaForFileMatch::isFileHash() const
{
  return m_fileHash;
}

inline void CriteriaForFileMatch::setFileName(bool fileName)
{
  m_fileName = fileName;
}
inline void CriteriaForFileMatch::setFullPath(bool fullPath)
{
  m_fullPath = fullPath;
}
inline void CriteriaForFileMatch::setDateTime(bool dateTime)
{
  m_dateTime = dateTime;
}
inline void CriteriaForFileMatch::setFileSize(bool fileSize)
{
  m_fileSize = fileSize;
}
inline void CriteriaForFileMatch::setFileHash(bool fileHash)
{
  m_fileHash = fileHash;
}

#endif // CRITERIAFORFILEMATCH_H

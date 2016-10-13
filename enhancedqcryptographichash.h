#ifndef ENHANCEDQCRYPTOGRAPHICHASH_H
#define ENHANCEDQCRYPTOGRAPHICHASH_H

#include <QCryptographicHash>
#include <QList>
#include <QStringList>

//**************************************************************************
//! Thin wrapper over the QCryptographicHash to remember algorithm and converting algorithm type to a string
/*!
 * The conversion functions were written to be easy to implement, not to be efficient.
 * The primary goal was to be able to ask the object what algorithm type is used,
 * get a list of supported algorithm types, and convert the algorithm type to and from a string.
 * Normally the meta-data infrastructure is used to do this, but the QCryptographicHash is not
 * derived from QObject so this is not possible.
 * \author Andrew Pitonyak
 * \copyright Andrew Pitonyak, but you may use without restriction.
 * \date 2012-2013
 **************************************************************************/
class EnhancedQCryptographicHash : public QCryptographicHash
{
public:
    //**************************************************************************
    //! Constructor
    /*!
     * \param [in] algorithm Algorithm to implement.
     *
     ***************************************************************************/
    explicit EnhancedQCryptographicHash(const QCryptographicHash::Algorithm algorithm);

    //**************************************************************************
    //! Get the algorithm used in the constructor.
    /*!
     * \returns The algorithm used in the constructor.
     *
     ***************************************************************************/
    QCryptographicHash::Algorithm getAlgorithm() const;

    //**************************************************************************
    //! Convert a string to an algorithm type; ignores case.
    /*!
     * \param [in] name Algorithm name.
     * \param [out] ok If not null, set to False if an error occurs, True otherwise.
     * \returns Algorithm type associated to the string.
     *
     ***************************************************************************/
    static QCryptographicHash::Algorithm toAlgorithm(const QString& name, bool* ok=nullptr);

    //**************************************************************************
    //! Get the string name for the algorithm.
    /*!
     * \param [in] algorithm convert this to a string.
     * \returns String name for the algorithm.
     *
     ***************************************************************************/
    static QString toAlgorithmString(const QCryptographicHash::Algorithm algorithm);

    //**************************************************************************
    //! Get a list of all supported algorithms.
    /*!
     * \returns List of all supported algorithms.
     *
     ***************************************************************************/
    static QList<QCryptographicHash::Algorithm> getAlgorithmList();

    inline static QCryptographicHash::Algorithm getDefaultAlgorithm() {return QCryptographicHash::Sha1;}

private:
    QCryptographicHash::Algorithm m_algorithm;
};

#endif // ENHANCEDQCRYPTOGRAPHICHASH_H

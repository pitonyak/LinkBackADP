#include "enhancedqcryptographichash.h"

EnhancedQCryptographicHash::EnhancedQCryptographicHash(const Algorithm algorithm) : QCryptographicHash(algorithm), m_algorithm(algorithm)
{
}

QCryptographicHash::Algorithm EnhancedQCryptographicHash::getAlgorithm() const
{
    return m_algorithm;
}

QCryptographicHash::Algorithm EnhancedQCryptographicHash::toAlgorithm(const QString& name, bool *ok)
{
    for (const QCryptographicHash::Algorithm algorithm : getAlgorithmList())
    {
        if (name.compare(toAlgorithmString(algorithm), Qt::CaseInsensitive) == 0)
        {
            if (ok != nullptr)
            {
                *ok = true;
            }
            return algorithm;
        }
    }
    if (ok != nullptr)
    {
        *ok = false;
    }
    return QCryptographicHash::Sha1;
}

QString EnhancedQCryptographicHash::toAlgorithmString(const QCryptographicHash::Algorithm algorithm)
{
    switch (algorithm)
    {
    case QCryptographicHash::Sha1:
        return "Sha1";
        break;
    case QCryptographicHash::Sha224:
        return "Sha224";
        break;
    case QCryptographicHash::Sha256:
        return "Sha256";
        break;
    case QCryptographicHash::Sha384:
        return "Sha384";
        break;
    case QCryptographicHash::Sha512:
        return "Sha512";
        break;
    case QCryptographicHash::Md4:
        return "Md4";
        break;
    case QCryptographicHash::Md5:
        return "Md5";
        break;
    default:
        return "";
    }
    return "";
}

QList<QCryptographicHash::Algorithm> EnhancedQCryptographicHash::getAlgorithmList()
{
    QList<QCryptographicHash::Algorithm> supportedAlgorithms;
    supportedAlgorithms << QCryptographicHash::Sha1   << QCryptographicHash::Sha224
                        << QCryptographicHash::Sha256 << QCryptographicHash::Sha384
                        << QCryptographicHash::Sha512
                        << QCryptographicHash::Md4 << QCryptographicHash::Md5;
    return supportedAlgorithms;
}

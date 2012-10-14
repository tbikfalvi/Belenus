
#ifndef CLICENCEMANAGER_H
#define CLICENCEMANAGER_H

#include <QObject>
#include <QDate>
#include <QStringList>

class cLicenceManager : public QObject
{
    Q_OBJECT

public:

    static const int EXPIRE_IN_DAYS = 7;
    static const int DEMO_LICENCE_KEY_ID = 1;
    static const int LICENCE_MAX_NUMBER = 30;

    enum licenceType {
        LTYPE_DEMO,
        LTYPE_REGISTERED,
        LTYPE_ACTIVATED,
        LTYPE_EXPIRED,
        LTYPE_INVALID
    };

    enum licenceError {
        ERR_NO_ERROR = 0,
        ERR_KEY_FORMAT_MISMATCH,    // Licence key format is not BLNSxx_ZZZZZZ where 'x' is a digit and 'ZZZZZZ' is a stored string
        ERR_KEY_NUMBER_INCORRECT,   // Licence key format is ok, but number after 'BLNS' is not ok
        ERR_KEY_NOT_EXISTS          // Licence key format is ok, but licence key is not official
    };

    cLicenceManager();
    ~cLicenceManager();

    void            initialize();
    bool            isDemo();
    int             daysRemain();
    int             validateLicence( const QString &p_qsLicenceString );
    QString         licenceKey() const;
    QString         validationKey() const;

private:

    QStringList     m_qslLicenceKeys;
    QStringList     m_qslCode;

    licenceType     m_LicenceType;
    QString         m_qsLicenceString;
    QString         m_qsCode;
    QDate           m_qdLastValidated;
    int             m_nLicenceId;

    void            _checkCode();
    void            _checkValidity();
    void            _EnCode( char *str, int size );
    void            _DeCode( char *str, int size );
};

#endif

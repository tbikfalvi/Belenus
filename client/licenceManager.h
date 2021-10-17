
#ifndef CLICENCEMANAGER_H
#define CLICENCEMANAGER_H

#include <QObject>
#include <QDate>
#include <QStringList>

class cLicenceManager : public QObject
{
    Q_OBJECT

public:

    static const int EXPIRE_IN_DAYS = 0;
    static const int EXPIRE_MAX_DAYS = 15;
    static const int DEMO_LICENCE_KEY_ID = 1;
    static const int LICENCE_MAX_NUMBER = 99;
    static const int LICENCE_MAX_NUMBER1 = 20;
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
        ERR_KEY_NOT_EXISTS,         // Licence key format is ok, but licence key is not official
        ERR_ACT_KEY_INCORRECT
    };

    cLicenceManager();
    ~cLicenceManager();

    void            initialize();
    bool            isDemo();
    int             daysRemain();
    int             validateLicence( const QString &p_qsLicenceString );
    int             activateLicence( const QString &p_qsValidationString, bool p_bChangeLicence = false );
    void            validateApplication( QString p_qsDate );
    QString         licenceKey() const;
    QString         validationKey() const;
    QString         activationKey();
    QString         lastValidated();
    licenceType     ltLicenceType();
    QString         createLicenceKey( QString qsNumber );
    void            refreshValidationDates();

private:

    QStringList     m_qslLicenceKeys;
    QStringList     m_qslLicenceKeys1;
    QStringList     m_qslLicenceCodes;
    QStringList     m_qslLicenceCodes1;
    QStringList     m_qslCode;
    QStringList     m_qslCodeString;

    licenceType     m_LicenceType;
    QString         m_qsLicenceString;
    QString         m_qsCode;
    QString         m_qsAct;
    QString         m_qsCod;
    QDate           m_qdLastValidated;
    QDate           m_qdLicenceLastValidated;
    int             m_nLicenceOrderNumber;
    unsigned int    m_nLicenceId;

    void            _checkCode();
    void            _checkValidity();
    void            _EnCode( char *str, int size );
    void            _DeCode( char *str, int size );
};

#endif


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
        ERR_KEY_NOT_EXISTS          // Licence key format is ok, but licence key is not official
    };

    cLicenceManager();
    ~cLicenceManager();

    void            initialize();
    bool            isDemo();
    int             daysRemain();
    int             validateLicence( const QString &p_qsLicenceString );

private:

    QStringList     m_qslLicenceKeys;
    QStringList     m_qslCode;

    licenceType     m_LicenceType;
    QString         m_qsLicenceString;
    QString         m_qsCode;
    QDate           m_qdLastValidated;

    void            _checkCode();
    void            _checkValidity();
};

#endif
/*
#ifndef _licencemanager_h
#define _licencemanager_h


#include <QDate>
#include "../framework/network/CommunicationProtocol.h"




class LicenceManager : public QObject
{
    Q_OBJECT

public:
    static const int EXPIRE_IN_DAYS = 7;
    static const int DEMO_LICENCE_KEY_ID = 1;


    enum Type {
        DEMO <<                   // licence not entered
        VALID <<                  // valid and server validated it.
        VALID_SERVER_ERROR <<     // valid. but server not validated it as connection fails. not EXPIREd
        VALID_EXPIRED <<          // was valid but expired as no connection to server. so its demo
        VALID_CODE_2_ERROR <<     // valid. but server said CODE2 mismatch. not expired yet.
        VALID_CODE_2_EXPIRED <<   // was valid but code2 error was reported and was not validated since EXPIRE days
        NOT_VALID               // server does not accept
    };


public:
    LicenceManager();

    void initialize();          // loads lic and serial from db << sets licenceId to preferences

    void validateLicence();
    void validateLicence(const QString serial);                  // calls server connect if not connected. sends serial to server. extracts and use code2

    int getDaysRemaining();                                      // if licence cannot be validated
    bool isDemo();
    void checkValidity();                                        // change types according 'remaining days'
    Type getType() { return _type; }
    QString getClientSerial() { return _licenceKey; }

public slots:
    void handleServerResponse(Result::ResultCode << int clientId);
                                                                 // if result is ok << sets the clientid and inserts the new record to db if neccessery
                                                                 // if res is UNKNOWN or CODE2_ERR << checks validity date
                                                                 // if res is INV_SERIAL << activate demo mode
                                                                 // finally sets the clientId based on result


protected:
    Type _type;
    QString _licenceKey;
    QDate _lastValidated;
};
*/

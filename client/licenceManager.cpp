
#include "licenceManager.h"

#include "preferences.h"
#include "bs_connection.h"
#include "../framework/qtmysqlconnection.h"
#include "../framework/qtlogger.h"


extern cQTMySQLConnection *g_poDB;
extern cPreferences *g_poPrefs;
extern BelenusServerConnection *g_poServer;
extern cQTLogger g_obLogger;



LicenceManager::LicenceManager()
    : _type(DEMO),
      _licenceKey("")
{
}



bool LicenceManager::isDemo()
{
    return _type==DEMO || _type==VALID_EXPIRED || _type==NOT_VALID || _type==VALID_CODE_2_ERROR;
}



void LicenceManager::initialize()
{
    int licenceId = 0;
    QSqlQuery *poQuery = NULL;

    try {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial, lastValidated FROM licences WHERE active=1 ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            _lastValidated = poQuery->value( 2 ).toDate();
            _licenceKey = poQuery->value( 1 ).toString();
            licenceId = poQuery->value( 0 ).toInt();
            if ( licenceId>1 ) {
                _type = VALID_SERVER_ERROR;
                checkValidity();
            }
            g_poPrefs->setLicenceId(licenceId);
            g_obLogger(cSeverity::DEBUG) << "[LicenceManager::initialize] initialized with "<< _licenceKey << " and " << licenceId << ". Last validated = " << _lastValidated.toString() << EOM;
        }
    } catch( cSevException &e ) {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}



void LicenceManager::validateLicence()
{
    validateLicence(_licenceKey);
}



void LicenceManager::validateLicence(const QString serial)
{
    if ( g_poServer->isConnected() ) {
        g_obLogger(cSeverity::ERROR) << "[LicenceManager::validateLicence] already connected to server. not allowed to validate new server" << EOM;
        return;
    }

    QFileInfo info;
    info.setFile("C:\\windows");
    if (!info.exists()) info.setFile("d:\\windows");
    if (!info.exists()) info.setFile("c:\\");
    if (!info.exists()) info.setFile("/etc");
    if (!info.exists()) info.setFile("/");
    if (!info.exists()) info.setFile(".");

    QString code2 = QCryptographicHash::hash(QString("SALT:%1").arg(info.created().toString()).toAscii(), QCryptographicHash::Sha1).toHex();

    connect(g_poServer, SIGNAL(licenceStatusChanged(Result::ResultCode, int)),
            this, SLOT(handleServerResponse(Result::ResultCode, int)) );

    g_poServer->setLoginKeys(serial, code2);
    g_poServer->connectTo();

    g_obLogger(cSeverity::DEBUG) << "[LicenceManager::validateLicence] connection called with "<< serial << " and " << code2 << EOM;
}



int LicenceManager::getDaysRemaining()
{
    if ( !_lastValidated.isValid() )
        return 0;

    int days = _lastValidated.daysTo(QDate::currentDate());
    // days<0 means currentdate is before lastvalidated - error
    if ( days < 0 )
        return 0;

    // itt lehet days 0..n .. EXP-days lehet -vegtelen..EXP
    days = EXPIRE_IN_DAYS - days;
    if ( days < 0 ) // ha ez mar tobb mint EXP_IN_DAYS akkor nincs tobb nap hatra
        days = 0;

    return days;
}



void LicenceManager::handleServerResponse(const Result::ResultCode res, const int licenceId)
{
    if ( res==Result::OK ) {        // server auth was successful
        _type = VALID;

        // see if there is already a record in db with the provided clientId
        QSqlQuery *query = NULL;
        try {
            query = g_poDB->executeQTQuery( QString( "SELECT * FROM licences WHERE licenceId=%1 ORDER BY licenceId DESC" ).arg(licenceId) );
            if( query->size()==1 )  // id exists. update validity date
            {
                QSqlQuery *a = g_poDB->executeQTQuery( QString("UPDATE licences SET lastValidated=NOW() WHERE licenceId=%1").arg(licenceId) );
                if ( !a->isActive() )
                    g_obLogger(cSeverity::ERROR) << "[LicenceManager::handleServerResponse] unable to update validity date" << EOM;
                delete a;
            } else {
                // if not, insert.
                QSqlQuery *a = g_poDB->executeQTQuery( QString("INSERT INTO licences(licenceId,lastValidated,active,serial, archive) VALUES(%1, NOW(), 1, \"%2\", \"NEW\");").arg(licenceId).arg(_licenceKey) );
                if ( !a->isActive() )
                    g_obLogger(cSeverity::ERROR) << "[LicenceManager::handleServerResponse] unable to insert new licenceId" << EOM;
                delete a;
            }
            delete query;
        } catch( cSevException &e ) {
            if( query ) delete query;
            g_obLogger(e.severity()) << e.what() << EOM;
        }

    } else if ( res==Result::INVALID_LICENSE_KEY ) {
        _type = NOT_VALID;
    } else if ( res==Result::INVALID_SECOND_ID ) { // server responded nothing or licence was invalid
        _type = VALID_CODE_2_ERROR;
    } else if ( res==Result::UNKNOWN ) {
        // do not change. it was set in initialize or a previous handleSResponse
    }

    checkValidity();
    if (isDemo())
        g_poPrefs->setLicenceId(DEMO_LICENCE_KEY_ID);
    else
        g_poPrefs->setLicenceId(licenceId);

    g_obLogger(cSeverity::DEBUG) << "[LicenceManager::handleServerResponse] type=" << _type << " daysremaining=" << getDaysRemaining() << " licenceId=" << licenceId << EOM;
}



void LicenceManager::checkValidity()
{
    if ( _type==DEMO || _type==VALID || getDaysRemaining()>0 )
        return;

    if ( _type==VALID_SERVER_ERROR )
        _type = VALID_EXPIRED;
    if ( _type==VALID_CODE_2_ERROR )
        _type = VALID_CODE_2_EXPIRED;
}
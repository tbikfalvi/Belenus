//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpatientcard.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpatientcard.h"
#include "dbvalidtimeperiods.h"
#include "dbpatientcardunits.h"
#include "dbpatientcardtype.h"

cDBPatientCard::cDBPatientCard()
{
    init();
}

cDBPatientCard::~cDBPatientCard()
{
}

void cDBPatientCard::init( const unsigned int p_uiId,
                           const unsigned int p_uiLicenceId,
                           const unsigned int p_uiPatientCardTypeId,
                           const unsigned int p_uiParentId,
                           const unsigned int p_uiPatientId,
                           const QString p_qsBarcode,
                           const QString p_qsComment,
                           const int p_nUnits,
                           const int p_nAmount,
                           const unsigned int p_uiTimeLeft,
                           const QString p_qsValidDateFrom,
                           const QString p_qsValidDateTo,
                           const QString p_qsPincode,
                           const QString &p_qsModified,
                           const bool p_bActive,
                           const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiPatientCardTypeId   = p_uiPatientCardTypeId;
    m_uiParentId            = p_uiParentId;
    m_uiPatientId           = p_uiPatientId;
    m_qsBarcode             = p_qsBarcode;
    m_qsComment             = p_qsComment;
    m_nUnits                = p_nUnits;
    m_nAmount               = p_nAmount;
    m_uiTimeLeft            = p_uiTimeLeft;
    m_qsValidDateFrom       = p_qsValidDateFrom;
    m_qsValidDateTo         = p_qsValidDateTo;
    m_qsPincode             = p_qsPincode;
    m_qsModified            = p_qsModified;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBPatientCard::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "patientCardId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPatientCardTypeIdIdx  = p_obRecord.indexOf( "patientCardTypeId" );
    int inParendIdIdx           = p_obRecord.indexOf( "parentCardId" );
    int inPatientIdIdx          = p_obRecord.indexOf( "patientId" );
    int inBarcodeIdx            = p_obRecord.indexOf( "barcode" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inUnitsIdx              = p_obRecord.indexOf( "units" );
    int inAmountIdx             = p_obRecord.indexOf( "amount" );
    int inTimeLeftIdx           = p_obRecord.indexOf( "timeLeft" );
    int inValidDateFromIdx      = p_obRecord.indexOf( "validDateFrom" );
    int inValidDateToIdx        = p_obRecord.indexOf( "validDateTo" );
    int inPincodeIdx            = p_obRecord.indexOf( "pincode" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardTypeIdIdx ).toUInt(),
          p_obRecord.value( inParendIdIdx ).toUInt(),
          p_obRecord.value( inPatientIdIdx ).toUInt(),
          p_obRecord.value( inBarcodeIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inUnitsIdx ).toInt(),
          p_obRecord.value( inAmountIdx ).toInt(),
          p_obRecord.value( inTimeLeftIdx ).toUInt(),
          p_obRecord.value( inValidDateFromIdx ).toString(),
          p_obRecord.value( inValidDateToIdx ).toString(),
          p_obRecord.value( inPincodeIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPatientCard::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE patientCardId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard id not found" );

    poQuery->first();
    init( poQuery->record() );
    synchronizeUnits();
}

void cDBPatientCard::load( const QString &p_qsBarcode ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::load", QString("name: \"%1\"").arg(p_qsBarcode) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patientCards WHERE barcode = \"" + p_qsBarcode + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard barcode not found" );

    poQuery->first();
    init( poQuery->record() );
    synchronizeUnits();
}

void cDBPatientCard::loadPatient( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::loadPatient", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE patientId = %1" ).arg( p_uiId ) );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Patient id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatientCard::save() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
        m_qsArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " patientCards SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
    qsQuery += QString( "parentCardId = \"%1\", " ).arg( m_uiParentId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "barcode = \"%1\", " ).arg( m_qsBarcode );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment.replace( QString("\""), QString("\\\"") ) );
    qsQuery += QString( "units = \"%1\", " ).arg( m_nUnits );
    qsQuery += QString( "amount = \"%1\", " ).arg( m_nAmount );
    qsQuery += QString( "timeLeft = \"%1\", " ).arg( m_uiTimeLeft );
    qsQuery += QString( "validDateFrom = \"%1\", " ).arg( m_qsValidDateFrom );
    qsQuery += QString( "validDateTo = \"%1\", " ).arg( m_qsValidDateTo );
    qsQuery += QString( "pincode = \"%1\", " ).arg( m_qsPincode );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();

    if( isAssignedCardExists() )
    {
        qsQuery = "UPDATE";
        qsQuery += " patientCards SET ";
        qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
        qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
        qsQuery += QString( "units = \"%1\", " ).arg( m_nUnits );
        qsQuery += QString( "amount = \"%1\", " ).arg( m_nAmount );
        qsQuery += QString( "timeLeft = \"%1\", " ).arg( m_uiTimeLeft );
        qsQuery += QString( "validDateFrom = \"%1\", " ).arg( m_qsValidDateFrom );
        qsQuery += QString( "validDateTo = \"%1\" " ).arg( m_qsValidDateTo );
        if( m_uiParentId > 0 )
        {
            // update parent
            qsQuery += QString( " WHERE patientCardId = %1" ).arg( m_uiParentId );
        }
        else
        {
            // update childs
            qsQuery += QString( " WHERE parentCardId = %1" ).arg( m_uiId );
        }
        poQuery = g_poDB->executeQTQuery( qsQuery );
    }

    if( poQuery ) delete poQuery;
}

void cDBPatientCard::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM patientCards ";
        }
        else
        {
            qsQuery = "UPDATE patientCards SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE patientCardId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPatientCard::deactivate() throw( cSevException )
{
    setPatientCardTypeId( 0 );
    setParentId( 0 );
    setPatientId( 0 );
    setUnits( 0 );
    setAmount( 0 );
    setTimeLeft( 0 );
    setValidDateFrom( "0000-00-00" );
    setValidDateTo( "0000-00-00" );
    setActive( false );
    save();

    cDBPatientcardUnit  obDBPatientcardUnit;

    obDBPatientcardUnit.deactivateUnits( m_uiId );
}

bool cDBPatientCard::isPatientCardTypeLinked( const unsigned int p_PCTId ) throw()
{
    cTracer obTrace( "cDBPatientCard::isPatientCardTypeLinked", QString( "id: %1" ).arg( p_PCTId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE patientCardTypeId = %1" ).arg( p_PCTId ) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

bool cDBPatientCard::isPatientCardCanBeReplaced() throw()
{
    if( m_bActive == 0 || m_nUnits < 1 || m_uiTimeLeft < 1 || patientCardTypeId() < 2 )
        return false;

    QDate   qdTo = QDate::fromString( m_qsValidDateTo, "yyyy-MM-dd" );

    if( QDate::currentDate() > qdTo )
        return false;

    return true;
}

bool cDBPatientCard::isPatientCardCanBeParent() throw()
{
    if( m_bActive == 0 || m_nUnits < 1 || m_uiTimeLeft < 1 || patientCardTypeId() < 2 || m_uiParentId > 0 || pincode().compare("LOST") == 0 )
        return false;

    QDate   qdTo = QDate::fromString( m_qsValidDateTo, "yyyy-MM-dd" );

    if( QDate::currentDate() > qdTo )
        return false;

    return true;
}

bool cDBPatientCard::isAssignedCardExists() throw()
{
    if( m_uiParentId > 0 )
        return true;

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE parentCardId = %1" ).arg( m_uiId ) );
    if( poQuery->size() > 0 )
        return true;

    return false;
}

void cDBPatientCard::synchronizeUnits() throw()
{
    QString qsQuery = "";

    qsQuery += QString( "SELECT COUNT(patientCardId) FROM patientcardunits WHERE " );
    qsQuery += QString( "( patientCardId=%1 " ).arg( m_uiId );
    if( parentId() > 0 )
    {
        qsQuery += QString( "OR patientCardId=%2 " ).arg( parentId() );
    }
    qsQuery += QString( " ) AND prepared=0 AND active=1 " );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

    poQuery->first();

    if( poQuery->size() > 0 )
        setUnits( poQuery->value(0).toInt() );
    else
        setUnits( 0 );
}

void cDBPatientCard::synchronizeTime() throw()
{
    QString qsQuery = "";

    qsQuery += QString( "SELECT SUM(unitTime) FROM patientcardunits WHERE " );
    qsQuery += QString( "( patientCardId=%1 " ).arg( m_uiId );
    if( parentId() > 0 )
    {
        qsQuery += QString( "OR patientCardId=%2 " ).arg( parentId() );
    }
    qsQuery += QString( " ) AND active=1 " );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

    poQuery->first();

    if( poQuery->size() > 0 )
        setTimeLeft( poQuery->value(0).toInt()*60 );
    else
        setUnits( 0 );
}

void cDBPatientCard::synchronizeUnitTime(int p_nUnitTime) throw()
{
    QString     qsQuery = "";
    QSqlQuery  *poQuery;

    qsQuery += QString( "UPDATE patientcardunits SET patientCardTypeId=%1 " ).arg( patientCardTypeId() );
    qsQuery += QString( "WHERE patientCardId=%1 " ).arg( m_uiId );
    qsQuery += QString( "AND patientCardTypeId=0 " );
//    qsQuery += QString( "AND active=1 " );

    poQuery = g_poDB->executeQTQuery( qsQuery );

    if( p_nUnitTime > 0 )
    {
        qsQuery  = "";
        qsQuery += QString( "UPDATE patientcardunits SET unitTime=%1 " ).arg( p_nUnitTime );
        qsQuery += QString( "WHERE patientCardId=%1 " ).arg( m_uiId );
        qsQuery += QString( "AND patientCardTypeId=%1 " ).arg( patientCardTypeId() );
        qsQuery += QString( "AND active=1 " );

        poQuery = g_poDB->executeQTQuery( qsQuery );
    }
}

void cDBPatientCard::updateActiveUnits(QDate p_qdNew) throw()
{
    QString qsQuery = QString( "UPDATE patientcardunits SET validDateTo='%1' WHERE patientCardId=%2 AND active=1" ).arg( p_qdNew.toString("yyyy-MM-dd") ).arg( m_uiId );

    g_poDB->executeQTQuery( qsQuery );
}

bool cDBPatientCard::isPatientCardCanBeUsed( unsigned int p_uiPatientCardTypeId, QString *p_qsValid ) throw()
{
    QStringList         qslDays;
    QDateTime           currDateTime( QDateTime::currentDateTime() );
    cDBValidTimePeriod  obDBValidTimePeriod;

    qslDays << QObject::tr("Mon") << QObject::tr("Tue") << QObject::tr("Wed") << QObject::tr("Thu") << QObject::tr("Fri") << QObject::tr("Sat") << QObject::tr("Sun");

    QStringList qslValidTimes;

    try
    {
        qslValidTimes = obDBValidTimePeriod.loadPeriods( p_uiPatientCardTypeId );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    *p_qsValid = ""; //QObject::tr( "Patientcard can be used:" );
    for( int i=0; i<qslValidTimes.count(); i++ )
    {
        QString qsValidTimeStr  = qslValidTimes.at(i);

        p_qsValid->append( QString("\n%1").arg(qsValidTimeStr) );

        if( qsValidTimeStr.contains( qslDays.at(currDateTime.date().dayOfWeek()-1) ) )
        {
            QString qsValidTime = qsValidTimeStr.left(14);
            QString qsStart     = qsValidTime.left(5);
            QString qsStop      = qsValidTime.right(5);
            QTime   qtStart     = QTime::fromString( qsStart, "hh:mm" );
            QTime   qtStop      = QTime::fromString( qsStop, "hh:mm" );

            if( qtStart < qtStop )
            {
                if( qtStart <= currDateTime.time() && currDateTime.time() <= qtStop )
                    return true;
            }
            else
            {
                if( (qtStart <= currDateTime.time() && currDateTime.time() <= QTime(23,59,59,0)) ||
                    (QTime(0,0,0,0) <= currDateTime.time() && currDateTime.time() <= qtStop) )
                    return true;
            }
        }
    }

    return false;
}

void cDBPatientCard::createNew() throw()
{
    init();
}

unsigned int cDBPatientCard::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPatientCard::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPatientCard::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPatientCard::patientCardTypeId() const throw()
{
    return m_uiPatientCardTypeId;
}

void cDBPatientCard::setPatientCardTypeId( const unsigned int p_uiPCardTypeId ) throw()
{
    m_uiPatientCardTypeId = p_uiPCardTypeId;
}

unsigned int cDBPatientCard::parentId() const throw()
{
    return m_uiParentId;
}

void cDBPatientCard::setParentId( const unsigned int p_uiParentId ) throw()
{
    m_uiParentId = p_uiParentId;
}

unsigned int cDBPatientCard::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBPatientCard::setPatientId( const unsigned int p_uiPatientId ) throw()
{
    m_uiPatientId = p_uiPatientId;
}

QString cDBPatientCard::barcode() const throw()
{
    return m_qsBarcode;
}

void cDBPatientCard::setBarcode( const QString &p_qsBarcode ) throw()
{
    m_qsBarcode = p_qsBarcode;
    m_qsBarcode = m_qsBarcode.replace( QString("\""), QString("\\\"") );
}

QString cDBPatientCard::comment() const throw()
{
    return m_qsComment;
}

void cDBPatientCard::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
}

int cDBPatientCard::units() const throw()
{
    return m_nUnits;
}

void cDBPatientCard::setUnits( const int p_nUnits ) throw()
{
    m_nUnits = p_nUnits;
}

int cDBPatientCard::amount() const throw()
{
    return m_nAmount;
}

void cDBPatientCard::setAmount( const int p_nAmount ) throw()
{
    m_nAmount = p_nAmount;
}

unsigned int cDBPatientCard::timeLeft() const throw()
{
    return m_uiTimeLeft;
}

void cDBPatientCard::setTimeLeft( const unsigned int p_uiTimeLeft ) throw()
{
    m_uiTimeLeft = p_uiTimeLeft;
}

QString cDBPatientCard::timeLeftStr() const throw()
{
    QTime   qtTemp( m_uiTimeLeft/3600, (m_uiTimeLeft%3600)/60, (m_uiTimeLeft%3600)%60, 0 );

    return qtTemp.toString( "hh:mm:ss" );
}

void cDBPatientCard::setTimeLeftStr( const QString &p_qsTimeLeft ) throw()
{
    QTime   qtTemp = QTime::fromString( p_qsTimeLeft, "hh:mm:ss" );

    m_uiTimeLeft = qtTemp.hour()*3600 + qtTemp.minute()*60 + qtTemp.second();
}

QString cDBPatientCard::validDateFrom() const throw()
{
    return m_qsValidDateFrom;
}

void cDBPatientCard::setValidDateFrom( const QString &p_qsValidDateFrom ) throw()
{
    m_qsValidDateFrom = p_qsValidDateFrom;
}

QString cDBPatientCard::validDateTo() const throw()
{
    return m_qsValidDateTo;
}

void cDBPatientCard::setValidDateTo( const QString &p_qsValidDateTo ) throw()
{
    m_qsValidDateTo = p_qsValidDateTo;
}

QString cDBPatientCard::pincode() const throw()
{
    return m_qsPincode;
}

void cDBPatientCard::setPincode( const QString &p_qsPincode ) throw()
{
    m_qsPincode = p_qsPincode;
    m_qsPincode = m_qsPincode.replace( QString("\""), QString("\\\"") );
}

QString cDBPatientCard::modified() const throw()
{
    return m_qsModified;
}

bool cDBPatientCard::active() const throw()
{
    return m_bActive;
}

void cDBPatientCard::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPatientCard::archive() const throw()
{
    return m_qsArchive;
}

void cDBPatientCard::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

void cDBPatientCard::sendDataToWeb( bool p_bSendNow ) throw()
{
    try
    {
        QString qsMessageData = "";

        synchronizeUnits();

        qsMessageData.append( "<div id='valid'><label>T201</label>" );
        qsMessageData.append( validDateTo() );
        qsMessageData.append( "</div><div id='unit'><label>T202</label>" );
        qsMessageData.append( QString::number(units()) );
        qsMessageData.append( "</div><label id='validT'>T203</label>" );

        QString qsQuery = QString( "SELECT patientCardUnitId, patientCardTypeId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                   "FROM patientcardunits "
                                   "WHERE patientCardId=%1 "
                                   "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                   "AND prepared=0 "
                                   "AND active=1 "
                                   "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" )
                                 .arg( id() );
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        while( poQuery->next() )
        {
            QString qsValid;
            unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

            if( uiPCTId == 0 )
            {
                uiPCTId = patientCardTypeId();
            }
            if( uiPCTId > 0 )
            {
                cDBPatientCardType obDBPatientCardType;

                obDBPatientCardType.load( uiPCTId );
                isPatientCardCanBeUsed( uiPCTId, &qsValid );
                qsValid.replace( QObject::tr("Mon"), "T205" );
                qsValid.replace( QObject::tr("Tue"), "T206" );
                qsValid.replace( QObject::tr("Wed"), "T207" );
                qsValid.replace( QObject::tr("Thu"), "T208" );
                qsValid.replace( QObject::tr("Fri"), "T209" );
                qsValid.replace( QObject::tr("Sat"), "T210" );
                qsValid.replace( QObject::tr("Sun"), "T211" );

                qsMessageData.append( "<div class='validType'><span class='cardName'>" );
                qsMessageData.append( poQuery->value( 5 ).toString() );
                qsMessageData.append( " T219 (" );
                qsMessageData.append( poQuery->value( 2 ).toString() );
                qsMessageData.append( "T204) (" );
                qsMessageData.append( obDBPatientCardType.name() );
                qsMessageData.append( ")</span><span class='cardDays'>" );
                qsMessageData.append( qsValid );
                qsMessageData.append( "</span></div>" );
            }
        }

        g_poBlnsHttp->sendPatientCardData( barcode(), qsMessageData, p_bSendNow );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

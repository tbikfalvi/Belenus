//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbproductactiontype.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbproductactiontype.h"

cDBProductActionType::cDBProductActionType()
{
    init();
}

cDBProductActionType::~cDBProductActionType()
{
}

void cDBProductActionType::init( const unsigned int p_uiId,
                           const unsigned int p_uiLicenceId,
                           const QString &p_qsName,
                           const bool p_bIncrease,
                           const bool p_bDecrease,
                           const QString &p_qsIndication,
                           const QString &p_qsModified,
                           const bool p_bActive,
                           const QString &p_qsArchive ) throw()
{
    m_uiId                      = p_uiId;
    m_uiLicenceId               = p_uiLicenceId;
    m_qsName                    = p_qsName;
    m_bIncreaseProductCount     = p_bIncrease;
    m_bDecreaseProductCount     = p_bDecrease;
    m_qsCassaActionIndication   = p_qsIndication;
    m_qsModified                = p_qsModified;
    m_bActive                   = p_bActive;
    m_qsArchive                 = p_qsArchive;
}

void cDBProductActionType::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "productActionTypeId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inIncreaseIdx       = p_obRecord.indexOf( "increaseProductCount" );
    int inDecreaseIdx       = p_obRecord.indexOf( "decreaseProductCount" );
    int inIndicationIdx     = p_obRecord.indexOf( "cassaActionIndication" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inIncreaseIdx ).toBool(),
          p_obRecord.value( inDecreaseIdx ).toBool(),
          p_obRecord.value( inIndicationIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBProductActionType::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBProductActionType::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productActionType WHERE productActionTypeId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Productaction type id not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

void cDBProductActionType::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBProductActionType::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM productActionType WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Productaction type name not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

void cDBProductActionType::save() throw( cSevException )
{
    cTracer obTrace( "cDBProductActionType::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_qsArchive != "NEW" )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " productActionType SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "increaseProductCount = %1, " ).arg( m_bIncreaseProductCount );
    qsQuery += QString( "decreaseProductCount = %1, " ).arg( m_bDecreaseProductCount );
    qsQuery += QString( "cassaActionIndication = \"%1\", " ).arg( m_qsCassaActionIndication );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE productActionTypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();

    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PATIENTCARD_TYPE );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PATIENTCARD_TYPE );
*/
}

void cDBProductActionType::remove() throw( cSevException )
{
    cTracer obTrace( "cDBProductActionType::remove" );

    if( m_uiId )
    {
        QString     qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM productActionType ";
        }
        else
        {
            qsQuery = "UPDATE productActionType SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE productActionTypeId = %1" ).arg( m_uiId );

        QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBProductActionType::createNew() throw()
{
    init();
}

unsigned int cDBProductActionType::id() const throw()
{
    return m_uiId;
}

unsigned int cDBProductActionType::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBProductActionType::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBProductActionType::name() const throw()
{
    return m_qsName;
}

void cDBProductActionType::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

bool cDBProductActionType::increaseProductCount() const throw()
{
    return m_bIncreaseProductCount;
}

void cDBProductActionType::setIncreaseProductCount( const bool p_bIncrease ) throw()
{
    m_bIncreaseProductCount = p_bIncrease;
}

bool cDBProductActionType::decreaseProductCount() const throw()
{
    return m_bDecreaseProductCount;
}

void cDBProductActionType::setDecreaseProductCount( const bool p_bDecrease ) throw()
{
    m_bDecreaseProductCount = p_bDecrease;
}

QString cDBProductActionType::cassaActionIndication() const throw()
{
    return m_qsCassaActionIndication;
}

void cDBProductActionType::setCassaActionIndication( const QString &p_qsIndication ) throw()
{
    m_qsCassaActionIndication = p_qsIndication;
}

QString cDBProductActionType::modified() const throw()
{
    return m_qsModified;
}

bool cDBProductActionType::active() const throw()
{
    return m_bActive;
}

void cDBProductActionType::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBProductActionType::archive() const throw()
{
    return m_qsArchive;
}

void cDBProductActionType::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}


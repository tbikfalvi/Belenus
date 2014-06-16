//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpaymentmethod.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpaymentmethod.h"

cDBPaymentMethod::cDBPaymentMethod()
{
    init();
}

cDBPaymentMethod::~cDBPaymentMethod()
{
}

void cDBPaymentMethod::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const QString &p_qsName,
                             const QString &p_qsModified,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_qsName            = p_qsName;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBPaymentMethod::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "paymentMethodId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPaymentMethod::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPaymentMethod::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM paymentmethods WHERE paymentMethodId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Payment method id not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

void cDBPaymentMethod::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBPaymentMethod::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM paymentmethods WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Payment method name not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

void cDBPaymentMethod::save() throw( cSevException )
{
    cTracer obTrace( "cDBPaymentMethod::save" );
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
    qsQuery += " paymentmethods SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE paymentMethodId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();

/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PAYMENTMETHOD_TYPE );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PAYMENTMETHOD_TYPE );
*/
}

void cDBPaymentMethod::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPaymentMethod::remove" );

    if( m_uiId )
    {
        QString     qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM paymentmethods ";
        }
        else
        {
            qsQuery = "UPDATE paymentmethods SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE paymentMethodId = %1" ).arg( m_uiId );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
}

void cDBPaymentMethod::createNew() throw()
{
    init();
}

unsigned int cDBPaymentMethod::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPaymentMethod::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPaymentMethod::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBPaymentMethod::name() const throw()
{
    return m_qsName;
}

void cDBPaymentMethod::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBPaymentMethod::modified() const throw()
{
    return m_qsModified;
}

bool cDBPaymentMethod::active() const throw()
{
    return m_bActive;
}

void cDBPaymentMethod::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPaymentMethod::archive() const throw()
{
    return m_qsArchive;
}

void cDBPaymentMethod::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}


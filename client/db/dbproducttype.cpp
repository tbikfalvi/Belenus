//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbproducttype.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbproducttype.h"

cDBProductType::cDBProductType()
{
    init();
}

cDBProductType::~cDBProductType()
{
}

void cDBProductType::init( const unsigned int p_uiId,
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

    m_qslProducts.clear();
}

void cDBProductType::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "productTypeId" );
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

    if( m_uiId > 0 )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectProductWithType WHERE productTypeId = %1" ).arg( m_uiId ) );
        while( poQuery->next() )
        {
            m_qslProducts.append( poQuery->value( 1 ).toString() );
        }

        if( poQuery ) delete poQuery;
    }
}

void cDBProductType::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBProductType::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productTypes WHERE productTypeId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Product type id not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

void cDBProductType::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBProductType::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM productTypes WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Product type name not found" );

    poQuery->first();
    init( poQuery->record() );
    if( poQuery ) delete poQuery;
}

void cDBProductType::save() throw( cSevException )
{
    cTracer obTrace( "cDBProductType::save" );
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
    qsQuery += " productTypes SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE productTypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();

    poQuery = g_poDB->executeQTQuery( QString("DELETE FROM connectProductWithType WHERE productTypeId = %1").arg(m_uiId) );
    if( poQuery ) delete poQuery;

    poQuery = NULL;
    for( int i=0; i<m_qslProducts.size(); i++ )
    {
        qsQuery  = "INSERT INTO connectProductWithType SET ";
        qsQuery += QString( "productTypeId = \"%1\", " ).arg(m_uiId);
        qsQuery += QString( "productId = \"%1\", " ).arg(m_qslProducts.at(i).toInt());
        qsQuery += QString( "licenceId = \"%1\" " ).arg( m_uiLicenceId );
        poQuery = g_poDB->executeQTQuery( qsQuery );
    }
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PATIENTCARD_TYPE );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PATIENTCARD_TYPE );
*/
}

void cDBProductType::remove() throw( cSevException )
{
    cTracer obTrace( "cDBProductType::remove" );

    if( m_uiId )
    {
        QSqlQuery   *poQuery = g_poDB->executeQTQuery( QString("DELETE FROM connectProductWithType WHERE productTypeId = %1").arg(m_uiId) );

        QString     qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM productTypes ";
        }
        else
        {
            qsQuery = "UPDATE productTypes SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE productTypeId = %1" ).arg( m_uiId );

        poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
}

void cDBProductType::createNew() throw()
{
    init();
}

unsigned int cDBProductType::id() const throw()
{
    return m_uiId;
}

unsigned int cDBProductType::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBProductType::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBProductType::name() const throw()
{
    return m_qsName;
}

void cDBProductType::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QStringList cDBProductType::products() const throw()
{
    return m_qslProducts;
}

void cDBProductType::setProducts( const QStringList &p_qslProducts ) throw()
{
    m_qslProducts = p_qslProducts;
}

QString cDBProductType::modified() const throw()
{
    return m_qsModified;
}

bool cDBProductType::active() const throw()
{
    return m_bActive;
}

void cDBProductType::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBProductType::archive() const throw()
{
    return m_qsArchive;
}

void cDBProductType::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

bool cDBProductType::isProductTypeLinkedToProduct() const throw()
{
    bool bRet = false;

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectProductWithType WHERE productTypeId = %1" ).arg( m_uiId ) );
    bRet = poQuery->first();
    if( poQuery ) delete poQuery;

    return bRet;
}

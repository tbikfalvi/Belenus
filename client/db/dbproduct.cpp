//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbproduct.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbproduct.h"
#include "dbdiscount.h"

cDBProduct::cDBProduct()
{
    init();
}

cDBProduct::~cDBProduct()
{
}

void cDBProduct::init( const unsigned int p_uiId,
                       const unsigned int p_uiLicenceId,
                       const QString &p_qsName,
                       const QString &p_qsBarcode,
                       const int p_nNetPriceBuy,
                       const int p_nVatPercentBuy,
                       const int p_nNetPriceSell,
                       const int p_nVatPercentSell,
                       const int p_nProductCount,
                       const QString &p_qsModified,
                       const bool p_bActive,
                       const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_qsName            = p_qsName;
    m_qsBarcode         = p_qsBarcode;
    m_nNetPriceBuy      = p_nNetPriceBuy;
    m_nVatPercentBuy    = p_nVatPercentBuy;
    m_nNetPriceSell     = p_nNetPriceSell;
    m_nVatPercentSell   = p_nVatPercentSell;
    m_nProductCount     = p_nProductCount;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;

    m_qslProductTypes.clear();
}

void cDBProduct::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "productId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inBarcodeIdx        = p_obRecord.indexOf( "barcode" );
    int inNetPriceBIdx      = p_obRecord.indexOf( "netPriceBuy" );
    int inVatPercentBIdx    = p_obRecord.indexOf( "vatpercentBuy" );
    int inNetPriceSIdx      = p_obRecord.indexOf( "netPriceSell" );
    int inVatPercentSIdx    = p_obRecord.indexOf( "vatpercentSell" );
    int inProductCountIdx   = p_obRecord.indexOf( "productCount" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inBarcodeIdx ).toString(),
          p_obRecord.value( inNetPriceBIdx ).toInt(),
          p_obRecord.value( inVatPercentBIdx ).toInt(),
          p_obRecord.value( inNetPriceSIdx ).toInt(),
          p_obRecord.value( inVatPercentSIdx ).toInt(),
          p_obRecord.value( inProductCountIdx ).toInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );

    if( m_uiId > 0 )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectProductWithType WHERE productId = %1" ).arg( m_uiId ) );
        while( poQuery->next() )
        {
            m_qslProductTypes.append( poQuery->value( 0 ).toString() );
        }

        if( poQuery ) delete poQuery;
    }
}

void cDBProduct::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBProduct::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM products WHERE productId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Product id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBProduct::loadName( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBProduct::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM products WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Product name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBProduct::loadBarcode( const QString &p_qsBarcode ) throw( cSevException )
{
    cTracer obTrace( "cDBProduct::load", QString("barcode: \"%1\"").arg(p_qsBarcode) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM products WHERE barcode = \"" + p_qsBarcode + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Product barcode not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBProduct::save() throw( cSevException )
{
    cTracer obTrace( "cDBProduct::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_qsArchive.compare("NEW") != 0 )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " products SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "barcode = \"%1\", " ).arg( m_qsBarcode );
    qsQuery += QString( "netPriceBuy = \"%1\", " ).arg( m_nNetPriceBuy );
    qsQuery += QString( "vatpercentBuy = \"%1\", " ).arg( m_nVatPercentBuy );
    qsQuery += QString( "netPriceSell = \"%1\", " ).arg( m_nNetPriceSell );
    qsQuery += QString( "vatpercentSell = \"%1\", " ).arg( m_nVatPercentSell );
    qsQuery += QString( "productCount = \"%1\", " ).arg( m_nProductCount );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE productId = %1" ).arg( m_uiId );
    }
    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery )
    {
        m_uiId = poQuery->lastInsertId().toUInt();
    }
    if( poQuery ) delete poQuery;

    poQuery = g_poDB->executeQTQuery( QString("DELETE FROM connectProductWithType WHERE productId = %1").arg(m_uiId) );
    if( poQuery ) delete poQuery;

    for( int i=0; i<m_qslProductTypes.size(); i++ )
    {
        qsQuery  = "INSERT INTO connectProductWithType SET ";
        qsQuery += QString( "productTypeId = \"%1\", " ).arg(m_qslProductTypes.at(i).toInt());
        qsQuery += QString( "productId = \"%1\", " ).arg(m_uiId);
        qsQuery += QString( "licenceId = \"%1\" " ).arg( m_uiLicenceId );
        poQuery = g_poDB->executeQTQuery( qsQuery );
    }
    if( poQuery ) delete poQuery;

/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_COMPANY );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_COMPANY );
*/
}

void cDBProduct::remove() throw( cSevException )
{
    cTracer obTrace( "cDBProduct::remove" );

    if( m_uiId )
    {
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( QString("DELETE FROM connectProductWithType WHERE productId = %1").arg(m_uiId) );

        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM products ";
        }
        else
        {
            qsQuery = "UPDATE products SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE productId = %1" ).arg( m_uiId );

        poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
}

void cDBProduct::createNew() throw()
{
    init();
}

unsigned int cDBProduct::id() const throw()
{
    return m_uiId;
}

unsigned int cDBProduct::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBProduct::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBProduct::name() const throw()
{
    return m_qsName;
}

void cDBProduct::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBProduct::barcode() const throw()
{
    return m_qsBarcode;
}

void cDBProduct::setBarcode( const QString &p_qsBarcode ) throw()
{
    m_qsBarcode = p_qsBarcode;
}

int cDBProduct::netPriceBuy() const throw()
{
    return m_nNetPriceBuy;
}

void cDBProduct::setNetPriceBuy( const int p_nNetPriceBuy ) throw()
{
    m_nNetPriceBuy = p_nNetPriceBuy;
}

int cDBProduct::vatPercentBuy() const throw()
{
    return m_nVatPercentBuy;
}

void cDBProduct::setVatPercentBuy( const int p_nVatPercentBuy ) throw()
{
    m_nVatPercentBuy = p_nVatPercentBuy;
}

int cDBProduct::netPriceSell() const throw()
{
    return m_nNetPriceSell;
}

void cDBProduct::setNetPriceSell( const int p_nNetPriceSell ) throw()
{
    m_nNetPriceSell = p_nNetPriceSell;
}

int cDBProduct::vatPercentSell() const throw()
{
    return m_nVatPercentSell;
}

void cDBProduct::setVatPercentSell( const int p_nVatPercentSell ) throw()
{
    m_nVatPercentSell = p_nVatPercentSell;
}

int cDBProduct::productCount() const throw()
{
    return m_nProductCount;
}

void cDBProduct::setProductCount( const int p_nProductCount ) throw()
{
    m_nProductCount = p_nProductCount;
}

void cDBProduct::increaseProductCount( const int p_nCount ) throw()
{
    m_nProductCount += p_nCount;
}

void cDBProduct::decreaseProductCount( const int p_nCount ) throw()
{
    m_nProductCount -= p_nCount;
}

QStringList cDBProduct::productTypes() const throw()
{
    return m_qslProductTypes;
}

void cDBProduct::setProductTypes( const QStringList &p_qslProductTypes ) throw()
{
    m_qslProductTypes = p_qslProductTypes;
}

QString cDBProduct::modified() const throw()
{
    return m_qsModified;
}

bool cDBProduct::active() const throw()
{
    return m_bActive;
}

void cDBProduct::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBProduct::archive() const throw()
{
    return m_qsArchive;
}

void cDBProduct::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

bool cDBProduct::isProductLinkedToProductType() const throw()
{
    bool bRet = false;

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectProductWithType WHERE productId = %1" ).arg( m_uiId ) );
    bRet = poQuery->first();
    if( poQuery ) delete poQuery;

    return bRet;
}

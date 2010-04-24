#include "belenus.h"
#include "dbproduct.h"

cDBProduct::cDBProduct()
{
    init();
}

cDBProduct::~cDBProduct()
{
}

void cDBProduct::init( const unsigned int p_uiId,
                       const string &p_stName,
                       const string &p_stBarCode,
                       const int p_inPriceIn,
                       const int p_inPriceOut,
                       const int p_inCount ) throw()
{
    m_uiId = p_uiId;
    m_stName = p_stName;
    m_stBarCode = p_stBarCode;
    m_inPriceIn = p_inPriceIn;
    m_inPriceOut = p_inPriceOut;
    m_inCount = p_inCount;
}

void cDBProduct::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "productid" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inBarCodeIdx        = p_obRecord.indexOf( "barcode" );
    int inPriceInIdx        = p_obRecord.indexOf( "pricein" );
    int inPriceOut          = p_obRecord.indexOf( "priceout" );
    int inCount             = p_obRecord.indexOf( "count" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inBarCodeIdx ).toString().toStdString(),
          p_obRecord.value( inPriceInIdx ).toString().toInt(),
          p_obRecord.value( inPriceOut ).toString().toInt(),
          p_obRecord.value( inCount ).toString().toInt() );
}

void cDBProduct::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBProduct::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM products WHERE productid = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Product id not found" );

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
    }
    else
    {
        qsQuery = "INSERT INTO";
    }
    qsQuery += " products SET ";
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "barcode = \"%1\", " ).arg( QString::fromStdString( m_stBarCode ) );
    qsQuery += QString( "priceIn = %1, " ).arg( m_inPriceIn );
    qsQuery += QString( "priceOut = %1, " ).arg( m_inPriceOut );
    qsQuery += QString( "count = %1" ).arg( m_inCount );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE producId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBProduct::createNew() throw()
{
    init( 0 );
}

unsigned int cDBProduct::id() throw()
{
    return m_uiId;
}

string cDBProduct::name() throw()
{
    return m_stName;
}

void cDBProduct::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

string cDBProduct::barcode() throw()
{
    return m_stBarCode;
}

void cDBProduct::setBarcode( const string &p_stBarCode ) throw()
{
    m_stBarCode = p_stBarCode;
}

int cDBProduct::priceIn() throw()
{
    return m_inPriceIn;
}

void cDBProduct::setPriceIn( const int &p_inPriceIn ) throw()
{
    m_inPriceIn = p_inPriceIn;
}

int cDBProduct::priceOut() throw()
{
    return m_inPriceOut;
}

void cDBProduct::setPriceOut( const int &p_inPriceOut ) throw()
{
    m_inPriceOut = p_inPriceOut;
}

int cDBProduct::count() throw()
{
    return m_inCount;
}

void cDBProduct::setCount( const int &p_inCount ) throw()
{
    m_inCount = p_inCount;
}


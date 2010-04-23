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

void cDBProduct::save() const throw( cSevException )
{
    cTracer obTrace( "cDBProduct::save" );
    stringstream  ssQuery;

    if( m_uiId )
    {
        ssQuery << "UPDATE products ";
    }
    else
    {
        ssQuery << "INSERT INTO products ";
    }
    ssQuery << "SET name = \"" << m_stName << "\", ";
    ssQuery << "barcode = \"" << m_stBarCode << "\", ";
    ssQuery << "pricein = " << m_inPriceIn << ", ";
    ssQuery << "priceout = " << m_inPriceOut << ", ";
    ssQuery << "count = " << m_inCount << " ";
    if( m_uiId )
    {
        ssQuery << "WHERE productid = " << m_uiId;
    }

    g_poDB->executeQuery( ssQuery.str(), true );
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


#include "belenus.h"
#include "dbproducttype.h"

cDBProductType::cDBProductType()
{
    init();
}

cDBProductType::~cDBProductType()
{
}

void cDBProductType::init( const unsigned int p_uiId, const string &p_stName ) throw()
{
    m_uiId = p_uiId;
    m_stName = p_stName;
}

void cDBProductType::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "producttypeid" );
    int inNameIdx           = p_obRecord.indexOf( "name" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString() );
}

void cDBProductType::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBProductType::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM producttypes WHERE producttypeid = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "ProductType id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBProductType::save() const throw( cSevException )
{
    cTracer obTrace( "cDBProductType::save" );
    stringstream  ssQuery;

    if( m_uiId )
    {
        ssQuery << "UPDATE producttypes ";
    }
    else
    {
        ssQuery << "INSERT INTO producttypes ";
    }
    ssQuery << "SET name = \"" << m_stName << "\" ";
    if( m_uiId )
    {
        ssQuery << "WHERE producttypeid = " << m_uiId;
    }

    g_poDB->executeQuery( ssQuery.str(), true );
}

void cDBProductType::createNew() throw()
{
    init( 0 );
}

unsigned int cDBProductType::id() throw()
{
    return m_uiId;
}

string cDBProductType::name() throw()
{
    return m_stName;
}

void cDBProductType::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

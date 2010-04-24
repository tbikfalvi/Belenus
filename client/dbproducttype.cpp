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

void cDBProductType::save() throw( cSevException )
{
    cTracer obTrace( "cDBProductType::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
    }
    else
    {
        qsQuery = "INSERT INTO";
    }
    qsQuery += " productTypes SET ";
    qsQuery += QString( "name = \"%1\"" ).arg( QString::fromStdString( m_stName ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE producTtypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
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

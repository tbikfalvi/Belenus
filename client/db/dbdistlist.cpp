//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbdistlist.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbdistlist.h"

cDBDistList::cDBDistList()
{
    init();
}

cDBDistList::~cDBDistList()
{
}

void cDBDistList::init(const unsigned int p_uiId,
                       const unsigned int p_uiLicenceId,
                       const QString &p_qsName,
                       const QString &p_qsDescription,
                       const bool p_bActive,
                       const QString &p_qsArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_qsName        = p_qsName;
    m_qsDescription = p_qsDescription;
    m_bActive       = p_bActive;
    m_qsArchive     = p_qsArchive;

    m_qslPatients.clear();
}

void cDBDistList::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "distlistId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inDescriptionIdx    = p_obRecord.indexOf( "description" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inDescriptionIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );

    if( m_uiId > 0 )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM connectPatientWithDistList WHERE distlistId = %1" ).arg( m_uiId ) );
        while( poQuery->next() )
        {
            m_qslPatients.append( poQuery->value( 1 ).toString() );
        }

        if( poQuery ) delete poQuery;
    }
}

void cDBDistList::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDistList::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM distlist WHERE distlistId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Distribution list id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDistList::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBDistList::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM distlist WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Distribution list name not found" );

    poQuery->first();
    init( poQuery->record() );
}

QString cDBDistList::recipients() throw()
{
    QStringList qslRecipients;

    for( int i=0; i<m_qslPatients.count(); i++ )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT email FROM patients WHERE patientId = \"" + m_qslPatients.at(i) + "\"" );

        poQuery->first();
        qslRecipients.append( poQuery->value( 0 ).toString() );
    }
    return qslRecipients.join( ";" );
}

void cDBDistList::save() throw( cSevException )
{
    cTracer obTrace( "cDBDistList::save" );
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
    qsQuery += " distlist SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "description = \"%1\", " ).arg( m_qsDescription );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE distlistId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;

    poQuery = NULL;
    poQuery = g_poDB->executeQTQuery( QString( "DELETE FROM connectPatientWithDistList WHERE distlistId = \"%1\" " ).arg(m_uiId) );
    if( poQuery ) delete poQuery;

    poQuery = NULL;
    for( int i=0; i<m_qslPatients.size(); i++ )
    {
        qsQuery  = "INSERT INTO connectPatientWithDistList SET ";
        qsQuery += QString( "distlistId = \"%1\", " ).arg(m_uiId);
        qsQuery += QString( "patientId = \"%1\", " ).arg(m_qslPatients.at(i).toInt());
        qsQuery += QString( "licenceId = \"%1\" " ).arg( m_uiLicenceId );
        poQuery = g_poDB->executeQTQuery( qsQuery );
    }
    if( poQuery ) delete poQuery;
}

void cDBDistList::remove() throw( cSevException )
{
    cTracer obTrace( "cDBDistList::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        qsQuery = "DELETE FROM distlist ";
        qsQuery += QString( " WHERE distlistId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;

        poQuery = NULL;
        poQuery = g_poDB->executeQTQuery( QString( "DELETE FROM connectPatientWithDistList WHERE distlistId = \"%1\" " ).arg(m_uiId) );
        if( poQuery ) delete poQuery;
    }
}

void cDBDistList::createNew() throw()
{
    init();
}

unsigned int cDBDistList::id() const throw()
{
    return m_uiId;
}

unsigned int cDBDistList::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBDistList::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBDistList::name() const throw()
{
    return m_qsName;
}

void cDBDistList::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBDistList::description() const throw()
{
    return m_qsDescription;
}

void cDBDistList::setDescription( const QString &p_qsDescription ) throw()
{
    m_qsDescription = p_qsDescription;
    m_qsDescription = m_qsDescription.replace( QString("\""), QString("\\\"") );
}

bool cDBDistList::active() const throw()
{
    return m_bActive;
}

void cDBDistList::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBDistList::archive() const throw()
{
    return m_qsArchive;
}

void cDBDistList::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

QStringList cDBDistList::patients() const throw()
{
    return m_qslPatients;
}

void cDBDistList::setPatients(const QStringList &p_qslPatients) throw()
{
    m_qslPatients = p_qslPatients;
}


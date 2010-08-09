#include "../belenus.h"
#include "dbuser.h"

cDBUser::cDBUser()
{
    init();
}

cDBUser::~cDBUser()
{
}

void cDBUser::init( const unsigned int p_uiId, const unsigned int p_uiLicenceId,
                    const QString &p_qsName, const QString &p_qsRealName,
                    const QString &p_qsPassword, const cAccessGroup::teAccessGroup p_enGroup,
                    const bool p_boActive, const QString &p_qsComment,
                    const QString &p_qsArchive ) throw()
{
    m_boLoggedIn  = false;
    m_uiId        = p_uiId;
    m_uiLicenceId = p_uiLicenceId;
    m_qsName      = p_qsName;
    if( p_qsRealName != "" ) m_qsRealName = p_qsRealName;
    else m_qsRealName = p_qsName;
    m_qsPassword  = p_qsPassword;
    m_enGroup     = p_enGroup;
    m_boActive    = p_boActive;
    m_qsComment   = p_qsComment;
    m_qsArchive   = p_qsArchive;
}

void cDBUser::init( const QSqlRecord &p_obRecord ) throw()
{
    int  inIdIdx        = p_obRecord.indexOf( "userId" );
    int  inLicenceIdIdx = p_obRecord.indexOf( "licenceId" );
    int  inNameIdx      = p_obRecord.indexOf( "name" );
    int  inRNameIdx     = p_obRecord.indexOf( "realName" );
    int  inPwdIdx       = p_obRecord.indexOf( "password" );
    int  inGrpIdx       = p_obRecord.indexOf( "accgroup" );
    int  inActIdx       = p_obRecord.indexOf( "active" );
    int  inCommIdx      = p_obRecord.indexOf( "comment" );
    int  inArchiveIdx   = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inRNameIdx ).toString(),
          p_obRecord.value( inPwdIdx ).toString(),
          (cAccessGroup::teAccessGroup)(p_obRecord.value( inGrpIdx ).toInt()),
          p_obRecord.value( inActIdx ).toBool(),
          p_obRecord.value( inCommIdx ).toString(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBUser::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBUser::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE userId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "User id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBUser::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBUser::load", QString("name: \"%1\"").arg(p_qsName) );

    if( p_qsName == "root" )
    {
        init( 0, 0, "root", "", "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922",
              cAccessGroup::ROOT, true, "Built-in root user", "" );
    }
    else
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM users WHERE name = \"" + p_qsName + "\"" );

        if( poQuery->size() != 1 )
            throw cSevException( cSeverity::ERROR, "User name not found" );

        poQuery->first();
        init( poQuery->record() );
    }
}

void cDBUser::save() throw( cSevException )
{
    cTracer obTrace( "cDBUser::save" );
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

    qsQuery += " users SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "password = \"%1\", " ).arg( m_qsPassword );
    qsQuery += QString( "realName = \"%1\", " ).arg( m_qsRealName );
    qsQuery += QString( "accgroup = %1, " ).arg( (int)m_enGroup );
    qsQuery += QString( "active = %1, " ).arg( m_boActive );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
    qsQuery += QString( "archive = \"%1\"" ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE userId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBUser::createNew() throw()
{
    init( 0, 0, "", "", "", cAccessGroup::USER, 1, "", "" );
}

void cDBUser::logIn( const QString &p_qsPassword ) throw( cSevException )
{
    cTracer obTrace( "cDBUser::logIn" );

    if( !m_boActive )
        throw cSevException( cSeverity::ERROR, "Inactive user" );
    if( p_qsPassword != m_qsPassword )
        throw cSevException( cSeverity::ERROR, "Incorrect Password" );

    m_boLoggedIn = true;
}

void cDBUser::logOut() throw()
{
    init();
}

bool cDBUser::isLoggedIn() const throw()
{
    return m_boLoggedIn;
}

unsigned int cDBUser::id() const throw()
{
    return m_uiId;
}

QString cDBUser::name() const throw()
{
    return m_qsName;
}

void cDBUser::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
}

QString cDBUser::password() const throw()
{
    return m_qsPassword;
}

void cDBUser::setPassword( const QString &p_qsPassword ) throw()
{
    m_qsPassword = p_qsPassword;
}

QString cDBUser::realName() const throw()
{
    return m_qsRealName;
}

void cDBUser::setRealName( const QString &p_qsRealName ) throw()
{
    m_qsRealName = p_qsRealName;
}

cAccessGroup::teAccessGroup cDBUser::group() const throw()
{
    return m_enGroup;
}

void cDBUser::setGroup( const cAccessGroup::teAccessGroup p_enGroup ) throw()
{
    m_enGroup = p_enGroup;
}

bool cDBUser::isInGroup( const cAccessGroup::teAccessGroup p_enGroup ) const throw()
{
    return ( p_enGroup <= m_enGroup );
}

bool cDBUser::active() const throw ()
{
    return m_boActive;
}

void cDBUser::setActive( const bool p_boActive ) throw ()
{
    m_boActive = p_boActive;
}

QString cDBUser::comment() const throw ()
{
    return m_qsComment;
}

void cDBUser::setComment( const QString &p_qsComment ) throw ()
{
    m_qsComment = p_qsComment;
}

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
                    const string &p_stName, const string &p_stRealName,
                    const string &p_stPassword, const cAccessGroup::teAccessGroup p_enGroup,
                    const bool p_boActive, const string &p_stComment,
                    const string &p_stArchive ) throw()
{
    m_boLoggedIn  = false;
    m_uiId        = p_uiId;
    m_uiLicenceId = p_uiLicenceId;
    m_stName      = p_stName;
    if( p_stRealName != "" ) m_stRealName = p_stRealName;
    else m_stRealName = p_stName;
    m_stPassword  = p_stPassword;
    m_enGroup     = p_enGroup;
    m_boActive    = p_boActive;
    m_stComment   = p_stComment;
    m_stArchive   = p_stArchive;
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
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inRNameIdx ).toString().toStdString(),
          p_obRecord.value( inPwdIdx ).toString().toStdString(),
          (cAccessGroup::teAccessGroup)(p_obRecord.value( inGrpIdx ).toInt()),
          p_obRecord.value( inActIdx ).toBool(),
          p_obRecord.value( inCommIdx ).toString().toStdString(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
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

void cDBUser::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBUser::load", QString("name: \"%1\"").arg(p_stName.c_str()) );

    if( p_stName == "root" )
    {
        init( 0, 0, "root", "", "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922",
              cAccessGroup::ROOT, true, "Built-in root user", "" );
    }
    else
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM users WHERE name = \"" + QString::fromStdString( p_stName ) + "\"" );

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

        if( m_stArchive.compare("NEW") != 0 )
        {
            m_stArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_stArchive = "NEW";
    }

    qsQuery += " users SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "password = \"%1\", " ).arg( QString::fromStdString( m_stPassword ) );
    qsQuery += QString( "realName = \"%1\", " ).arg( QString::fromStdString( m_stRealName ) );
    qsQuery += QString( "accgroup = %1, " ).arg( (int)m_enGroup );
    qsQuery += QString( "active = %1, " ).arg( m_boActive );
    qsQuery += QString( "comment = \"%1\", " ).arg( QString::fromStdString( m_stComment ) );
    qsQuery += QString( "archive = \"%1\"" ).arg( QString::fromStdString( m_stArchive ) );
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

void cDBUser::logIn( const string &p_stPassword ) throw( cSevException )
{
    cTracer obTrace( "cDBUser::logIn" );

    if( !m_boActive )
        throw cSevException( cSeverity::ERROR, "Inactive user" );
    if( p_stPassword != m_stPassword )
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

string cDBUser::name() const throw()
{
    return m_stName;
}

void cDBUser::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

string cDBUser::password() const throw()
{
    return m_stPassword;
}

void cDBUser::setPassword( const string &p_stPassword ) throw()
{
    m_stPassword = p_stPassword;
}

string cDBUser::realName() const throw()
{
    return m_stRealName;
}

void cDBUser::setRealName( const string &p_stRealName ) throw()
{
    m_stRealName = p_stRealName;
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

string cDBUser::comment() const throw ()
{
    return m_stComment;
}

void cDBUser::setComment( const string &p_stComment ) throw ()
{
    m_stComment = p_stComment;
}

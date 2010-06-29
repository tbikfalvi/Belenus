#include "../belenus.h"
#include "dbuser.h"

cDBUser::cDBUser()
{
    init();
}

cDBUser::~cDBUser()
{
}

void cDBUser::init( const unsigned int p_uiId, const string &p_stName,
                  const string &p_stRealName, const string &p_stPassword,
                  const string &p_stGroups, const bool p_boActive,
                  const string &p_stComment ) throw()
{
    m_boLoggedIn = false;
    m_uiId       = p_uiId;
    m_stName     = p_stName;
    if( p_stRealName != "" ) m_stRealName = p_stRealName;
    else m_stRealName = p_stName;
    m_stPassword = p_stPassword;
    m_stGroups   = p_stGroups;
    m_boActive   = p_boActive;
    m_stComment  = p_stComment;
}

void cDBUser::init( const QSqlRecord &p_obRecord ) throw()
{
    int  inIdIdx   = p_obRecord.indexOf( "userId" );
    int  inNameIdx = p_obRecord.indexOf( "name" );
    int  inRNameIdx = p_obRecord.indexOf( "realName" );
    int  inPwdIdx  = p_obRecord.indexOf( "password" );
    int  inGrpIdx  = p_obRecord.indexOf( "groups" );
    int  inActIdx  = p_obRecord.indexOf( "active" );
    int  inCommIdx = p_obRecord.indexOf( "comment" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inRNameIdx ).toString().toStdString(),
          p_obRecord.value( inPwdIdx ).toString().toStdString(),
          p_obRecord.value( inGrpIdx ).toString().toStdString(),
          p_obRecord.value( inActIdx ).toBool(),
          p_obRecord.value( inCommIdx ).toString().toStdString() );
}

void cDBUser::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBUser::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM users WHERE userId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "User id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBUser::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBUser::load", "name: \""  + p_stName + "\"" );

    if( p_stName == "root" )
    {
        init( 0, "root", "", "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922",
              "root,admin,user", true, "Built-in root user" );
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
    }
    else
    {
        qsQuery = "INSERT INTO";
    }
    qsQuery += " users SET ";
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "password = \"%1\", " ).arg( QString::fromStdString( m_stPassword ) );
    qsQuery += QString( "realName = \"%1\", " ).arg( QString::fromStdString( m_stRealName ) );
    qsQuery += QString( "groups = \"%1\", " ).arg( QString::fromStdString( m_stGroups ) );
    qsQuery += QString( "active = %1, " ).arg( m_boActive );
    qsQuery += QString( "comment = \"%1\"" ).arg( QString::fromStdString( m_stComment ) );
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
    init( 0, "", "", "", "user", 1, "" );
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

string cDBUser::groups() const throw()
{
    return m_stGroups;
}

void cDBUser::setGroups( const string &p_stGroups ) throw()
{
    m_stGroups = p_stGroups;
}

bool cDBUser::isInGroup( const string &p_stGroup ) const throw()
{
    //cTracer obTrace( "cDBUser::isInGroup", p_stGroup );

    string::size_type  uiGroupStartLoc = 0;

    while( uiGroupStartLoc < m_stGroups.size() )
    {
        //search for the next ',' in the string:
        string::size_type  uiSeparatorLoc = m_stGroups.find( ',', uiGroupStartLoc );

        //if there was no ',' left, point just behind the last character of the string
        if( uiSeparatorLoc == string::npos ) uiSeparatorLoc = m_stGroups.length();

        //the current group string length is SeparatorLoc - StartLoc
        string::size_type  uiGroupLength = uiSeparatorLoc - uiGroupStartLoc;

        if( m_stGroups.substr( uiGroupStartLoc, uiGroupLength ) == p_stGroup ) break;
        else uiGroupStartLoc += uiGroupLength + 1;
    }

    bool boFound = (uiGroupStartLoc < m_stGroups.size());
    //obTrace << boFound;
    return boFound;
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

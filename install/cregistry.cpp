//=======================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//=======================================================================================
//
// Filename    : cregistry.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//=======================================================================================

#include <QStringList>
#include <QMessageBox>

//=======================================================================================

#include "cregistry.h"

//=======================================================================================
cRegistry::cRegistry(QObject *parent) : QObject(parent)
{
}
//=======================================================================================
bool cRegistry::isRegPathExists( const QString &p_qsPath ) const
{
    QStringList qslPath = p_qsPath.split( QChar('\\') );
    QString     qsPath;
    int         nCount = 1;

    // path should contain at least HKEY_... and one subdir
    if( qslPath.size() < 2 )
        return false;

    qsPath = qslPath.at(0);

    while( nCount < qslPath.size() )
    {
        QSettings   obReg( qsPath, QSettings::NativeFormat );

        if( !obReg.childGroups().contains(qslPath.at(nCount)) )
            return false;

        qsPath.append( QString("\\%1").arg(qslPath.at(nCount)) );
        nCount++;
    }

    return true;
}
//=======================================================================================
bool cRegistry::isRegKeyExists( const QString &p_qsPath, const QString &p_qsKey ) const
{
    if( !isRegPathExists(p_qsPath) )
        return false;

    QSettings   obReg( p_qsPath, QSettings::NativeFormat );

    return obReg.contains( p_qsKey );
}
//=======================================================================================
void cRegistry::setKeyValueS( const QString &p_qsPath, const QString &p_qsKey, const QString &p_qsValue )
{
    QSettings   obReg( p_qsPath, QSettings::NativeFormat );

    obReg.setValue( p_qsKey, p_qsValue );
}
//=======================================================================================
void cRegistry::setKeyValueN( const QString &p_qsPath, const QString &p_qsKey, int p_nValue )
{
    QSettings   obReg( p_qsPath, QSettings::NativeFormat );

    obReg.setValue( p_qsKey, p_nValue );
}
//=======================================================================================
QString cRegistry::keyValueS( const QString &p_qsPath, const QString &p_qsKey, const QString &p_qsDefValue ) const
{
//    if( !isRegKeyExists(p_qsPath, p_qsKey) )
//        return "";

    QSettings   obReg( p_qsPath, QSettings::NativeFormat );

    return obReg.value( p_qsKey, p_qsDefValue ).toString();
}
//=======================================================================================
int cRegistry::keyValueN( const QString &p_qsPath, const QString &p_qsKey, int p_nDefValue )
{
    if( !isRegKeyExists(p_qsPath, p_qsKey) )
        return 0;

    QSettings   obReg( p_qsPath, QSettings::NativeFormat );

    return obReg.value( p_qsKey, p_nDefValue ).toInt();
}
//=======================================================================================
void cRegistry::deleteRegPath(const QString &p_qsPath)
{
    if( !isRegPathExists(p_qsPath) )
        return;

    QSettings   obReg( p_qsPath, QSettings::NativeFormat );

    obReg.clear();
}
//=======================================================================================

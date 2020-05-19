//====================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : cregistry.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef CREGISTRY_H
#define CREGISTRY_H

//====================================================================================

#include <QObject>
#include <QSettings>

//====================================================================================
class cRegistry : public QObject
{
    Q_OBJECT

public:
    explicit cRegistry(QObject *parent = 0);

    bool        isRegPathExists( const QString &p_qsPath ) const;
    bool        isRegKeyExists( const QString &p_qsPath, const QString &p_qsKey ) const;
    void        setKeyValueS( const QString &p_qsPath, const QString &p_qsKey, const QString &p_qsValue );
    void        setKeyValueN( const QString &p_qsPath, const QString &p_qsKey, int p_nValue );
    QString     keyValueS( const QString &p_qsPath, const QString &p_qsKey, const QString &p_qsDefValue = QString("") ) const;
    int         keyValueN( const QString &p_qsPath, const QString &p_qsKey, int p_nDefValue = 0 );
    void        deleteRegPath( const QString &p_qsPath );

signals:

public slots:

};
//====================================================================================

#endif // CREGISTRY_H

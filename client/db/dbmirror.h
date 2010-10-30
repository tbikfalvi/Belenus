//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbmirror.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef DBMIRROR_H
#define DBMIRROR_H

//====================================================================================

#include "../bs_connection.h"

//====================================================================================

#define     MIRROR_UPDATE_LICENCE_DATA      1

//====================================================================================
class cDBMirror : public QObject
{
    Q_OBJECT

public:
    cDBMirror();
    ~cDBMirror();

    void    initialize();
    void    start();

    void    updateLicenceData();

public slots:
    void    queryReady( int id, SqlResult* );

protected:
    int     _qId;

private:
    bool    m_bGetGlobalData;
    int     m_inProcessCount;

};
//====================================================================================

#endif // DBMIRROR_H

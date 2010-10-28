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

#include "../bs_connection.h"

//====================================================================================
class cDBMirror : public QObject
{
    Q_OBJECT

public:
    cDBMirror();
    ~cDBMirror();

    void initialize();
    void start();

public slots:
    void queryReady(int id, SqlResult*);

protected:
    int _qId;
};
//====================================================================================

#endif // DBMIRROR_H

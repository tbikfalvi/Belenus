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
// Process identifiers

#define     MIRROR_UPDATE_LICENCE_DATA      1
#define     MIRROR_SYNC_DB_USER             2
#define     MIRROR_SYNC_DB_PATIENTORIGIN    3

//====================================================================================
// Database modification levels

#define DB_USER                     0x00000001
#define DB_PATIENT_ORIGIN           0x00000002
#define DB_REASON_TO_VISIT          0x00000004
#define DB_ILLNESS_GROUP            0x00000008
#define DB_PUBLIC_PLACES            0x00000010
#define DB_HEALTH_INSURANCE         0x00000020
#define DB_COMPANY                  0x00000040
#define DB_DOCTOR_TYPE              0x00000080
#define DB_DOCTOR                   0x00000100
#define DB_DOCTOR_SCHEDULE          0x00000200
#define DB_PATIENT                  0x00000400
#define DB_ATTENDANCE               0x00000800
#define DB_PATIENTCARD_TYPE         0x00001000
#define DB_PATIENTCARD              0x00002000
#define DB_CONNECT_PATIENT_CARD     0x00004000
#define DB_PATIENTCARD_HISTORY      0x00008000
#define DB_PANEL_TYPE               0x00010000
#define DB_PANEL_STATUS             0x00020000
#define DB_PANEL                    0x00040000
#define DB_PANEL_USE                0x00080000
#define DB_ATTENDANCE_SCHEDULE      0x00100000
#define DB_DENOMINATION             0x00200000
#define DB_PRODUCT_TYPE             0x00400000
#define DB_PRODUCT                  0x00800000
#define DB_DISCOUNT                 0x01000000
#define DB_ZIP_REGION_CITY          0x02000000
#define DB_ADDRESS                  0x04000000
#define DB_CASSA                    0x08000000
#define DB_CASSA_HISTORY            0x10000000
#define DB_CASSA_DENOMINATION       0x20000000
#define DB_LEDGER_DEVICE            0x40000000
#define DB_LEDGER                   0x80000000

//====================================================================================
class cDBMirror : public QObject
{
    Q_OBJECT

public:
    cDBMirror();
    ~cDBMirror();

    void            initialize();
    bool            start();

    void            updateLicenceData();

    void            updateSynchronizationLevel( unsigned int p_uiSyncLevel );

    void            synchronizeUserTable();
    void            synchronizePatientOriginTable();
    void            synchronizeReasonToVisit();

public slots:
    void            queryReady( int id, SqlResult* );

protected:
    int             _qId;

private:
    bool            m_bGetGlobalData;               // Identifies when data with licenceId=0 requested from server
    int             m_inProcessCount;               // Identifies the current process, see process identifier defines
    bool            m_bSyncAllTable;                // Identifies whether all table has been synchronized or not
    unsigned int    m_uiDbModificationLevel;        // Identifies which table modified. Only table with higher level
                                                    // can be archived on server
    unsigned int    m_uiCurrentId;

    void            _tableSynchronized( unsigned int p_uiSyncLevel );

    void            _synchronizeUserTable( unsigned int p_uiSyncLevel = DB_USER );
    void            _recordUserSynchronized();
    void            _synchronizePatientOriginTable( unsigned int p_uiSyncLevel = DB_PATIENT_ORIGIN );
    void            _recordPatientOriginSynchronized();
};
//====================================================================================

#endif // DBMIRROR_H

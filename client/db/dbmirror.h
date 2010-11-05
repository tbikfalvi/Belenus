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

#define     MIRROR_UPDATE_LICENCE_DATA          1

#define     MIRROR_SYNC_DB_USER                 2
#define     MIRROR_SYNC_DB_PATIENTORIGIN        3
#define     MIRROR_SYNC_DB_REASONTOVISIT        4
#define     MIRROR_SYNC_DB_ILLNESSGROUP         5
#define     MIRROR_SYNC_DB_PUBLICPLACES         6
#define     MIRROR_SYNC_DB_HEALTHINSURANCE      7
#define     MIRROR_SYNC_DB_COMPANY              8
#define     MIRROR_SYNC_DB_DOCTORTYPE           9
#define     MIRROR_SYNC_DB_DOCTOR               10
#define     MIRROR_SYNC_DB_DOCTORSCHEDULE       11
#define     MIRROR_SYNC_DB_PATIENT              12
#define     MIRROR_SYNC_DB_ATTENDANCE           13
#define     MIRROR_SYNC_DB_PATIENTCARDTYPE      14
#define     MIRROR_SYNC_DB_PATIENTCARD          15
#define     MIRROR_SYNC_DB_PATIENTCARDCONNECT   16
#define     MIRROR_SYNC_DB_PATIENTCARDHISTORY   17
#define     MIRROR_SYNC_DB_PANELTYPE            18
#define     MIRROR_SYNC_DB_PANELSTATUS          19
#define     MIRROR_SYNC_DB_PANEL                20
#define     MIRROR_SYNC_DB_PANELUSE             21
#define     MIRROR_SYNC_DB_ATTENDANCESCHEDULE   22
#define     MIRROR_SYNC_DB_DENOMINATION         23
#define     MIRROR_SYNC_DB_PRODUCTTYPE          24
#define     MIRROR_SYNC_DB_PRODUCT              25
#define     MIRROR_SYNC_DB_DISCOUNT             26
#define     MIRROR_SYNC_DB_ZIPREGIONCITY        27
#define     MIRROR_SYNC_DB_ADDRESS              28
#define     MIRROR_SYNC_DB_CASSA                29
#define     MIRROR_SYNC_DB_CASSAHISTORY         30
#define     MIRROR_SYNC_DB_CASSADENOMINATION    31
#define     MIRROR_SYNC_DB_LEDGERDEVICE         32
#define     MIRROR_SYNC_DB_LEDGER               33

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
#define DB_CONNECT_PATIENTCARD      0x00004000
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
    void            updateGlobalSyncLevel( unsigned int p_uiSyncLevel );
    bool            checkSyncLevel( unsigned int p_uiSyncLevel );

    bool            checkSynchronizationFinished();
    bool            checkIsSynchronizationNeeded();

    void            synchronizeAllTable();
    void            synchronizeExit();

    void            synchronizeUserTable();
    void            synchronizePatientOriginTable();
    void            synchronizeReasonToVisit();
    void            synchronizeIllnessGroup();
    void            synchronizePublicPlaces();
    void            synchronizeHealthInsurance();
    void            synchronizeCompany();
    void            synchronizeDoctorType();
    void            synchronizeDoctor();
    void            synchronizeDoctorSchedule();
    void            synchronizePatient();
    void            synchronizeAttendance();
    void            synchronizePatientcardType();
    void            synchronizePatientcard();
    void            synchronizePatientcardConnect();
    void            synchronizePatientcardHistory();
    void            synchronizePanelType();
    void            synchronizePanelStatus();
    void            synchronizePanel();
    void            synchronizePanelUse();
    void            synchronizeAttendanceSchedule();
    void            synchronizeDenomination();
    void            synchronizeProductType();
    void            synchronizeProduct();
    void            synchronizeDiscount();
    void            synchronizeZipRegionCity();
    void            synchronizeAddress();
    void            synchronizeCassa();
    void            synchronizeCassaHistory();
    void            synchronizeCassaDenomination();
    void            synchronizeLedgerDevice();
    void            synchronizeLedger();

public slots:
    void            queryReady( int id, SqlResult* );

protected:
    int             _qId;

private:
    bool            m_bGetGlobalData;               // Identifies when data with licenceId=0 requested from server
    int             m_inProcessCount;               // Identifies the current process, see process identifier defines
    bool            m_bSyncAllTable;                // Identifies whether all table needs to be synchronized or not
    bool            m_bSyncExit;                    // Identifies whether tables modified during exit
                                                    // needs to be synchronized or not
    unsigned int    m_uiDbModificationLevel;        // Identifies which table modified. Only table with higher level
                                                    // can be archived on server
    unsigned int    m_uiGlobalSyncLevel;            // Identifies which table has modified global data.
    unsigned int    m_uiCurrentId;                  // Holds the actually archived record unique identifier

    void            _tableSynchronized( unsigned int p_uiSyncLevel );
    void            _globalDataSynchronized( unsigned int p_uiSyncLevel );

    void            _synchronizeUserTable( unsigned int p_uiSyncLevel = DB_USER );
    void            _recordUserSynchronized();
    void            _synchronizePatientOriginTable( unsigned int p_uiSyncLevel = DB_PATIENT_ORIGIN );
    void            _recordPatientOriginSynchronized();
    void            _synchronizeReasonToVisit( unsigned int p_uiSyncLevel = DB_REASON_TO_VISIT );
    void            _recordReasonToVisitSynchronized();
    void            _synchronizeIllnessGroup( unsigned int p_uiSyncLevel = DB_ILLNESS_GROUP );
    void            _recordIllnessSynchronized();
    void            _synchronizePublicPlaces( unsigned int p_uiSyncLevel = DB_PUBLIC_PLACES );
    void            _recordPublicPlacesSynchronized();
    void            _synchronizeHealthInsurance( unsigned int p_uiSyncLevel = DB_HEALTH_INSURANCE );
    void            _recordHealthInsuranceSynchronized();
    void            _synchronizeCompany( unsigned int p_uiSyncLevel = DB_COMPANY );
    void            _recordCompanySynchronized();
    void            _synchronizeDoctorType( unsigned int p_uiSyncLevel = DB_DOCTOR_TYPE );
    void            _recordDoctorTypeSynchronized();
    void            _synchronizeDoctor( unsigned int p_uiSyncLevel = DB_DOCTOR );
    void            _recordDoctorSynchronized();
    void            _synchronizeDoctorSchedule( unsigned int p_uiSyncLevel = DB_DOCTOR_SCHEDULE );
    void            _recordDoctorScheduleSynchronized();
    void            _synchronizePatient( unsigned int p_uiSyncLevel = DB_PATIENT );
    void            _recordPatientSynchronized();
    void            _synchronizeAttendance( unsigned int p_uiSyncLevel = DB_ATTENDANCE );
    void            _recordAttendanceSynchronized();
    void            _synchronizePatientcardType( unsigned int p_uiSyncLevel = DB_PATIENTCARD_TYPE );
    void            _recordPatientcardTypeSynchronized();
    void            _synchronizePatientcard( unsigned int p_uiSyncLevel = DB_PATIENTCARD );
    void            _recordPatientcardSynchronized();
    void            _synchronizePatientcardConnect( unsigned int p_uiSyncLevel = DB_CONNECT_PATIENTCARD );
    void            _recordPatientcardConnectSynchronized();
    void            _synchronizePatientcardHistory( unsigned int p_uiSyncLevel = DB_PATIENTCARD_HISTORY );
    void            _recordPatientcardHistorySynchronized();
    void            _synchronizePanelType( unsigned int p_uiSyncLevel = DB_PANEL_TYPE );
    void            _recordPanelTypeSynchronized();
    void            _synchronizePanelStatus( unsigned int p_uiSyncLevel = DB_PANEL_STATUS );
    void            _recordPanelStatusSynchronized();
    void            _synchronizePanel( unsigned int p_uiSyncLevel = DB_PANEL );
    void            _recordPanelSynchronized();
    void            _synchronizePanelUse( unsigned int p_uiSyncLevel = DB_PANEL_USE );
    void            _recordPanelUseSynchronized();
    void            _synchronizeAttendanceSchedule( unsigned int p_uiSyncLevel = DB_ATTENDANCE_SCHEDULE );
    void            _recordAttendanceScheduleSynchronized();
    void            _synchronizeDenomination( unsigned int p_uiSyncLevel = DB_DENOMINATION );
    void            _recordDenominationSynchronized();
    void            _synchronizeProductType( unsigned int p_uiSyncLevel = DB_PRODUCT_TYPE );
    void            _recordProductTypeSynchronized();
    void            _synchronizeProduct( unsigned int p_uiSyncLevel = DB_PRODUCT );
    void            _recordProductSynchronized();
    void            _synchronizeDiscount( unsigned int p_uiSyncLevel = DB_DISCOUNT );
    void            _recordDiscountSynchronized();
    void            _synchronizeZipRegionCity( unsigned int p_uiSyncLevel = DB_ZIP_REGION_CITY );
    void            _recordZipRegionCitySynchronized();
    void            _synchronizeAddress( unsigned int p_uiSyncLevel = DB_ADDRESS );
    void            _recordAddressSynchronized();
    void            _synchronizeCassa( unsigned int p_uiSyncLevel = DB_CASSA );
    void            _recordCassaSynchronized();
    void            _synchronizeCassaHistory( unsigned int p_uiSyncLevel = DB_CASSA_HISTORY );
    void            _recordCassaHistorySynchronized();
    void            _synchronizeCassaDenomination( unsigned int p_uiSyncLevel = DB_CASSA_DENOMINATION );
    void            _recordCassaDenominationSynchronized();
    void            _synchronizeLedgerDevice( unsigned int p_uiSyncLevel = DB_LEDGER_DEVICE );
    void            _recordLedgerDeviceSynchronized();
    void            _synchronizeLedger( unsigned int p_uiSyncLevel = DB_LEDGER );
    void            _recordLedgerSynchronized();
};
//====================================================================================

#endif // DBMIRROR_H

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

#define     MIRROR_GET_GLOBAL_TIMESTAMP             1
#define     MIRROR_SET_GLOBAL_TIMESTAMP_ON_SERVER   2
#define     MIRROR_UPDATE_LICENCE_DATA              3

#define     MIRROR_SYNC_DB_USER                     10
#define     MIRROR_SYNC_DB_PATIENTORIGIN            11
#define     MIRROR_SYNC_DB_REASONTOVISIT            12
#define     MIRROR_SYNC_DB_ILLNESSGROUP             13
#define     MIRROR_SYNC_DB_PUBLICPLACES             14
#define     MIRROR_SYNC_DB_HEALTHINSURANCE          15
#define     MIRROR_SYNC_DB_COMPANY                  16
#define     MIRROR_SYNC_DB_DOCTORTYPE               17
#define     MIRROR_SYNC_DB_DOCTOR                   18
#define     MIRROR_SYNC_DB_DOCTORSCHEDULE           19
#define     MIRROR_SYNC_DB_PATIENT                  20
#define     MIRROR_SYNC_DB_ATTENDANCE               21
#define     MIRROR_SYNC_DB_PATIENTCARDTYPE          22
#define     MIRROR_SYNC_DB_PATIENTCARD              23
#define     MIRROR_SYNC_DB_PATIENTCARDCONNECT       24
#define     MIRROR_SYNC_DB_PATIENTCARDHISTORY       25
#define     MIRROR_SYNC_DB_PANELTYPE                26
#define     MIRROR_SYNC_DB_PANELSTATUS              27
#define     MIRROR_SYNC_DB_PANEL                    28
#define     MIRROR_SYNC_DB_PANELUSE                 29
#define     MIRROR_SYNC_DB_ATTENDANCESCHEDULE       30
#define     MIRROR_SYNC_DB_DENOMINATION             31
#define     MIRROR_SYNC_DB_PRODUCTTYPE              32
#define     MIRROR_SYNC_DB_PRODUCT                  33
#define     MIRROR_SYNC_DB_DISCOUNT                 34
#define     MIRROR_SYNC_DB_ZIPREGIONCITY            35
#define     MIRROR_SYNC_DB_ADDRESS                  36
#define     MIRROR_SYNC_DB_CASSA                    37
#define     MIRROR_SYNC_DB_CASSAHISTORY             38
#define     MIRROR_SYNC_DB_CASSADENOMINATION        39
#define     MIRROR_SYNC_DB_LEDGERDEVICE             40
#define     MIRROR_SYNC_DB_LEDGER                   41

#define     MIRROR_GET_GLOBAL_USER                  50
#define     MIRROR_GET_GLOBAL_PATIENTORIGIN         51
#define     MIRROR_GET_GLOBAL_REASONTOVISIT         52
#define     MIRROR_GET_GLOBAL_ILLNESSGROUPS         53
#define     MIRROR_GET_GLOBAL_PUBLICPLACES          54
#define     MIRROR_GET_GLOBAL_HEALTHINSURANCES      55
#define     MIRROR_GET_GLOBAL_COMPANIES             56
#define     MIRROR_GET_GLOBAL_DOCTORTYPES           57
#define     MIRROR_GET_GLOBAL_DOCTORS               58
#define     MIRROR_GET_GLOBAL_PATIENTS              59
#define     MIRROR_GET_GLOBAL_PATIENTCARDTYPES      60
#define     MIRROR_GET_GLOBAL_PATIENTCARDS          61
#define     MIRROR_GET_GLOBAL_LEDGERTYPES           62
#define     MIRROR_GET_GLOBAL_PRODUCTTYPES          63
#define     MIRROR_GET_GLOBAL_PRODUCTS              64
#define     MIRROR_GET_GLOBAL_DISCOUNTS             65
#define     MIRROR_GET_GLOBAL_PAYMENTMETHODS        66

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

    void            requestGlobalDataTimestamp();
    void            updateLicenceData();

    void            updateSynchronizationLevel( unsigned int p_uiSyncLevel );
    void            updateGlobalSyncLevel( unsigned int p_uiSyncLevel );
    bool            checkSyncLevel( unsigned int p_uiSyncLevel );
    bool            checkGlobalData( unsigned int p_uiSyncLevel );

    bool            checkIsGlobalDataDownloadInProgress();
    bool            checkSynchronizationFinished();
    bool            checkIsSynchronizationNeeded();
    bool            checkIsGlobalDataModifiedOnServer();

    void            acquireGlobals();
    void            acquireUserGlobals();
    void            acquirePatientOriginGlobals();
    void            acquireReasonToVisitGlobals();
    void            acquireIllnessGroupsGlobals();
    void            acquirePublicPlacesGlobals();
    void            acquireHealthInsurancesGlobals();
    void            acquireCompaniesGlobals();
    void            acquireDoctorTypesGlobals();
    void            acquireDoctorsGlobals();
    void            acquirePatientsGlobals();
    void            acquirePatientCardTypesGlobals();
    void            acquirePatientCardsGlobals();
    void            acquireLedgerTypesGlobals();
    void            acquireProductTypesGlobals();
    void            acquireProductsGlobals();
    void            acquireDiscountsGlobals();
    void            acquirePaymentMethodsGlobals();

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
    int             m_inProcessCount;               // Identifies the current process, see process identifier defines
    bool            m_bAcquireGlobalData;           // Identifies when global data download is in progress
    bool            m_bGlobalDataSynchronize;       // Identifies if global data will be synchronized
    bool            m_bGlobalDataChanged;           // Identifies whether the global data changed on server
    bool            m_bSyncAllTable;                // Identifies whether all table needs to be synchronized or not
    bool            m_bSyncExit;                    // Identifies whether tables modified during exit
                                                    // needs to be synchronized or not
    unsigned int    m_uiDbModificationLevel;        // Identifies which table modified. Only table with higher level
                                                    // can be archived on server
    QString         m_qsServerTimestamp;            // Identifies date and time of global data modified on server
    unsigned int    m_uiGlobalSyncLevel;            // Identifies which table has modified global data.
    unsigned int    m_uiGlobalDataChanged;          // Identifies which table updated with data from server
    unsigned int    m_uiCurrentId;                  // Holds the actually archived record unique identifier
    int             m_inCountOfTries;               // Identifies how many times client tried to process sql query
                                                    // on belenus server
    bool            m_bProcessSucceeded;            // Identifies whether communication with server finished with success or not

    void            _compareGlobalDataTimestamp( const QString &p_qsGlobalTimestamp );

    void            _tableSynchronized( unsigned int p_uiSyncLevel );
    void            _globalDataSynchronized( unsigned int p_uiSyncLevel );
    void            _updateGlobalTimestampOnServer();

    void            _processUserGlobals( SqlResult *p_sqlResult );
    void            _processPatientOriginGlobals( SqlResult *p_sqlResult );
    void            _processReasonToVisitGlobals( SqlResult *p_sqlResult );
    void            _processIllnessGroupsGlobals( SqlResult *p_sqlResult );
    void            _processPublicPlacesGlobals( SqlResult *p_sqlResult );
    void            _processHealthInsurancesGlobals( SqlResult *p_sqlResult );
    void            _processCompaniesGlobals( SqlResult *p_sqlResult );
    void            _processDoctorTypesGlobals( SqlResult *p_sqlResult );
    void            _processDoctorsGlobals( SqlResult *p_sqlResult );
    void            _processPatientsGlobals( SqlResult *p_sqlResult );
    void            _processPatientCardTypesGlobals( SqlResult *p_sqlResult );
    void            _processPatientCardsGlobals( SqlResult *p_sqlResult );
    void            _processLedgerTypesGlobals( SqlResult *p_sqlResult );
    void            _processProductTypesGlobals( SqlResult *p_sqlResult );
    void            _processProductsGlobals( SqlResult *p_sqlResult );
    void            _processDiscountsGlobals( SqlResult *p_sqlResult );
    void            _processPaymentMethodsGlobals( SqlResult *p_sqlResult );
    void            _processGlobalsFinished();

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

    void            _synchronizeDatabaseFinished();
};
//====================================================================================

#endif // DBMIRROR_H

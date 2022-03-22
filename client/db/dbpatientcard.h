#ifndef DB_PATIENTCARD_H
#define DB_PATIENTCARD_H

#include <QSqlRecord>
#include <QDate>

#include "../../framework/sevexception.h"

class cDBPatientCard
{
public:
    cDBPatientCard();
    ~cDBPatientCard();

    void            load( const unsigned int p_uiId )                           throw( cSevException );
    void            load( const QString &p_qsBarcode )                          throw( cSevException );
    void            loadRFID( const QString &p_qsRFID )                         throw( cSevException );
    void            loadPatient( const unsigned int p_uiId )                    throw( cSevException );
    void            save()                                                      throw( cSevException );
    void            remove()                                                    throw( cSevException );
    void            deactivate()                                                throw( cSevException );
    bool            isServiceCard()                                             throw();
    bool            isPatientCardTypeLinked( const unsigned int p_PCTId )       throw();
    bool            isPatientCardCanBeReplaced()                                throw();
    bool            isPatientCardCanBeParent()                                  throw();
    bool            isPatientCardCanBeUsed(unsigned int p_uiPatientCardTypeId, QString *p_qsValid)                  throw();
    bool            isAssignedCardExists()                                      throw();
    bool            isRFIDCard()                                                throw();
    bool            isLedgerConnected()                                         throw();
    void            synchronizeUnits()                                          throw();
    void            synchronizeTime()                                           throw();
    void            synchronizeUnitTime( int p_nUnitTime )                      throw();
    void            updateActiveUnits( QDate p_qdNew, QString p_qsCondition )   throw();
    void            createNew()                                                 throw();
    unsigned int    id() const                                                  throw();
    unsigned int    licenceId() const                                           throw();
    void            setLicenceId( const unsigned int p_nLicenceId )             throw();
    unsigned int    patientCardTypeId() const                                   throw();
    void            setPatientCardTypeId( const unsigned int p_uiPCardTypeId )  throw();
    unsigned int    patientId() const                                           throw();
    void            setPatientId( const unsigned int p_uiPatientId )            throw();
    bool            servicecard() const                                         throw();
    void            setServiceCard( const bool p_bServiceCard )                 throw();
    unsigned int    parentId() const                                            throw();
    void            setParentId( const unsigned int p_uiParentId )              throw();
    QString         barcode() const                                             throw();
    void            setBarcode( const QString &p_qsBarcode )                    throw();
    QString         RFID() const                                                throw();
    void            setRFID( const QString &p_qsRFID )                          throw();
    QString         comment() const                                             throw();
    void            setComment( const QString &p_qsComment )                    throw();
    int             units()                                                     throw();
    void            setUnits( const int p_nUnits )                              throw();
    int             amount() const                                              throw();
    void            setAmount( const int p_nAmount )                            throw();
    unsigned int    timeLeft() const                                            throw();
    void            setTimeLeft( const unsigned int p_uiTimeLeft )              throw();
    QString         timeLeftStr() const                                         throw();
    void            setTimeLeftStr( const QString &p_qsTimeLeft )               throw();
    QString         validDateFrom() const                                       throw();
    void            setValidDateFrom( const QString &p_qsValidDateFrom )        throw();
    QString         validDateTo() const                                         throw();
    void            setValidDateTo( const QString &p_qsValidDateTo )            throw();
    QString         pincode() const                                             throw();
    void            setPincode( const QString &p_qsPincode )                    throw();
    QString         modified() const                                            throw();
    bool            active() const                                              throw();
    void            setActive( const bool p_bActive )                           throw();
    QString         archive() const                                             throw();
    void            setArchive( const QString &p_qsArchive )                    throw();
    void            sendDataToWeb()                                             throw();
    void            sendAutoMail( const int p_nMailType,
                                  const int p_nMailDestination,
                                  const QString &p_qsDate,
                                  const int p_nUnitCount,
                                  const QString &p_qsDateTime )                 throw();
    bool            isCardOwnerRegisteredOnCardy()                              throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientCardTypeId;
    unsigned int    m_uiParentId;
    unsigned int    m_uiPatientId;
    bool            m_bServiceCard;
    QString         m_qsBarcode;
    QString         m_qsRFID;
    QString         m_qsComment;
    int             m_nUnits;
    int             m_nAmount;
    unsigned int    m_uiTimeLeft;
    QString         m_qsValidDateFrom;
    QString         m_qsValidDateTo;
    QString         m_qsPincode;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientCardTypeId = 0,
               const unsigned int p_uiParentId = 0,
               const unsigned int p_uiPatientId = 0,
               const bool p_bServiceCard = false,
               const QString p_qsBarcode = "",
               const QString p_qsRFID = "",
               const QString p_qsComment = "",
               const int p_nUnits = 0,
               const int p_nAmount = 0,
               const unsigned int p_uiTimeLeft = 0,
               const QString p_qsValidDateFrom = "0000-00-00",
               const QString p_qsValidDateTo = "0000-00-00",
               const QString p_qsPincode = "",
               const QString &p_qsModified = "",
               const bool p_bActive = false,
               const QString &p_qsArchive = "NEW" )                             throw();
    void init( const QSqlRecord &p_obRecord )                                   throw();
};

#endif

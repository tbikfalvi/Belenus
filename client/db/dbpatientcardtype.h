#ifndef DB_PATIENTCARDTYPE_H
#define DB_PATIENTCARDTYPE_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPatientCardType
{
public:
    cDBPatientCardType();
    ~cDBPatientCardType();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            load( const QString &p_qsName )                     throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )     throw();
    QString         name() const                                        throw();
    void            setName( const QString &p_qsName )                  throw();
    float           price() const                                       throw();
    void            setPrice( const float p_fPrice )                    throw();
    int             vatpercent() const                                  throw();
    void            setVatpercent( const int p_nVatpercent )            throw();
    int             units() const                                       throw();
    void            setUnits( const int p_nUnits )                      throw();
    QString         validDateFrom() const                               throw();
    void            setValidDateFrom( const QString &p_qsVDFrom )       throw();
    QString         validDateTo() const                                 throw();
    void            setValidDateTo( const QString &p_qsVDTo )           throw();
    int             validDays() const                                   throw();
    void            setValidDays( const int p_nValidDays )              throw();
    int             unitTime() const                                    throw();
    void            setUnitTime( const int p_nUnitTime )                throw();
    QString         modified() const                                    throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_qsArchive )            throw();
    void            updatePatientCardUnits(int p_nUnitTime)             throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    float           m_fPrice;
    int             m_nVatpercent;
    int             m_nUnits;
    QString         m_qsValidDateFrom;
    QString         m_qsValidDateTo;
    int             m_nValidDays;
    int             m_nUnitTime;
    QString         m_qsModified;
    bool            m_bActive;
    QString          m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const float p_fPrice = 0,
               const int p_nVatpercent = 0,
               const int p_nUnits = 0,
               const QString &p_qsValidDateFrom = "",
               const QString &p_qsValidDateTo = "",
               const int p_nValidDays = 0,
               const int p_nUnitTime = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                     throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif

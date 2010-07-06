#ifndef DB_ATTENDANCE_H
#define DB_ATTENDANCE_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBAttendance
{
public:
    cDBAttendance();
    ~cDBAttendance();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )     throw();
    unsigned int    patientId() const                                   throw();
    void            setPatientId( const unsigned int p_nPatientId )     throw();
    QString         date() const                                        throw();
    void            setDate( const QString &p_qsDate )                  throw();
    QString         length() const                                      throw();
    void            setLength( const QString &p_qsLength )              throw();
    int             weight() const                                      throw();
    void            setWeight( const int p_nWeight )                    throw();
    int             height() const                                      throw();
    void            setHeight( const int p_nHeight )                    throw();
    float           bloodPressureStart()                                throw();
    void            setBloodPressureStart( const float p_fBPStart )     throw();
    float           pulseStart()                                        throw();
    void            setPulseStart( const float p_fPulseStart )          throw();
    float           bloodPressureStop()                                 throw();
    void            setBloodPressureStop( const float p_fBPStop )       throw();
    float           pulseStop()                                         throw();
    void            setPulseStop( const float p_fPulseStop )            throw();
    QString         medicineCurrent()                                   throw();
    void            setMedicineCurrent( const QString &p_qsMedicineC )  throw();
    QString         medicineAllergy()                                   throw();
    void            setMedicineAllergy( const QString &p_qsMedicineA )  throw();
    QString         comment() const                                     throw();
    void            setComment( const QString &p_qsComment )            throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_qsArchive )            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientId;
    QString         m_qsDate;
    QString         m_qsLength;
    int             m_nWeight;
    int             m_nHeight;
    float           m_fBloodPressureStart;
    float           m_fPulseStart;
    float           m_fBloodPressureStop;
    float           m_fPulseStop;
    QString         m_qsMedicineCurrent;
    QString         m_qsMedicineAllergy;
    QString         m_qsComment;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientId = 0,
               const QString &p_qsDate = "1900-01-01",
               const QString &p_qsLength = "00:00",
               const int p_nWeight = 0,
               const int p_nHeight = 0,
               const float p_fBloodPressureStart = 0,
               const float p_fPulseStart = 0,
               const float p_fBloodPressureStop = 0,
               const float p_fPulseStop = 0,
               const QString &p_qsMedicineCurrent = "",
               const QString &p_qsMedicineAllergy = "",
               const QString &p_qsComment = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                      throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif

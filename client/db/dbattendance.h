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
    string          date() const                                        throw();
    void            setDate( const string &p_stDate )                   throw();
    string          length() const                                      throw();
    void            setLength( const string &p_stLength )               throw();
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
    string          medicineCurrent()                                   throw();
    void            setMedicineCurrent( const string &p_stMedicineC )   throw();
    string          medicineAllergy()                                   throw();
    void            setMedicineAllergy( const string &p_stMedicineA )   throw();
    string          comment() const                                     throw();
    void            setComment( const string &p_stComment )             throw();
    string          archive() const                                     throw();
    void            setArchive( const string &p_stArchive )             throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientId;
    string          m_stDate;
    string          m_stLength;
    int             m_nWeight;
    int             m_nHeight;
    float           m_fBloodPressureStart;
    float           m_fPulseStart;
    float           m_fBloodPressureStop;
    float           m_fPulseStop;
    string          m_stMedicineCurrent;
    string          m_stMedicineAllergy;
    string          m_stComment;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientId = 0,
               const string &p_stDate = "1900-01-01",
               const string &p_stLength = "00:00",
               const int p_nWeight = 0,
               const int p_nHeight = 0,
               const float p_fBloodPressureStart = 0,
               const float p_fPulseStart = 0,
               const float p_fBloodPressureStop = 0,
               const float p_fPulseStop = 0,
               const string &p_stMedicineCurrent = "",
               const string &p_stMedicineAllergy = "",
               const string &p_stComment = "",
               const string &p_stArchive = "NEW" )                      throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif

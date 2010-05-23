#ifndef DB_POSTPONED_H
#define DB_POSTPONED_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPostponed
{
public:
    cDBPostponed();
    ~cDBPostponed();

    void            load( const unsigned int p_uiId )                       throw( cSevException );
    void            removePatient( const unsigned int p_uiPatientId )       throw( cSevException );
    void            removeAttendance( const unsigned int p_uiAttendanceId ) throw( cSevException );
    void            save()                                                  throw( cSevException );
    void            createNew()                                             throw();
    unsigned int    id() const                                              throw();
    unsigned int    patientId() const                                       throw();
    void            setPatientId( const unsigned int p_nPatientId )         throw();
    unsigned int    attendanceId() const                                    throw();
    void            setAttendanceId( const unsigned int p_nAttendanceId )   throw();
    unsigned int    countPostponedPatients() const                          throw();
    unsigned int    countPostponedAttendances() const                       throw();

private:
    unsigned int  m_uiId;
    unsigned int  m_uiPatientId;
    unsigned int  m_uiAttendanceId;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiPatientId = 0,
               const unsigned int p_uiAttendanceId = 0 )                    throw();
    void init( const QSqlRecord &p_obRecord )                               throw();
};

#endif

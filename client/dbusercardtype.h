#ifndef DBUSERCARDTYPE_H
#define DBUSERCARDTYPE_H

#include <QSqlRecord>

#include "../framework/sevexception.h"

class cDBUserCardType
{
public:
    cDBUserCardType();
    ~cDBUserCardType();

    void            load( const unsigned int p_uiId )                                   throw( cSevException );
    void            save()                                                              throw( cSevException );
    void            createNew()                                                         throw();
    unsigned int    id()                                                                throw();
    string          name()                                                              throw();
    void            setName( const string &p_stName )                                   throw();
    int             price()                                                             throw();
    void            setPrice( const int p_inPrice )                                     throw();
    unsigned int    units()                                                             throw();
    void            setUnits( const unsigned int puiUnits )                             throw();
    string          validDateFrom()                                                     throw();
    unsigned int    validDateFromYear()                                                 throw();
    void            setValidDateFromYear( const unsigned int p_uiValidDateFromYear )    throw();
    unsigned int    validDateFromMonth()                                                throw();
    void            setValidDateFromMonth( const unsigned int p_uiValidDateFromMonth )  throw();
    unsigned int    validDateFromDay()                                                  throw();
    void            setValidDateFromDay( const unsigned int p_uiValidDateFromDay )      throw();
    string          validDateTo()                                                       throw();
    unsigned int    validDateToYear()                                                   throw();
    void            setValidDateToYear( const unsigned int p_uiValidDateToYear )        throw();
    unsigned int    validDateToMonth()                                                  throw();
    void            setValidDateToMonth( const unsigned int p_uiValidDateToMonth )      throw();
    unsigned int    validDateToDay()                                                    throw();
    void            setValidDateToDay( const unsigned int p_uiValidDateToDay )          throw();
    unsigned int    validDays()                                                         throw();
    void            setValidDays( const unsigned int p_uiValidDays )                    throw();
    unsigned int    unitTime()                                                          throw();
    void            setUnitTime( const unsigned int p_uiUnitTime )                      throw();

private:
    unsigned int    m_uiId;
    string          m_stName;
    int             m_inPrice;
    unsigned int    m_uiUnits;
    unsigned int    m_uiValidDateFromYear;
    unsigned int    m_uiValidDateFromMonth;
    unsigned int    m_uiValidDateFromDay;
    unsigned int    m_uiValidDateToYear;
    unsigned int    m_uiValidDateToMonth;
    unsigned int    m_uiValidDateToDay;
    unsigned int    m_uiValidDays;
    unsigned int    m_uiUnitTime;

    void init( const unsigned int p_uiId = 0,
               const string &p_stName = "",
               const int p_inPrice = 0,
               const unsigned int p_uiUnits = 0,
               const unsigned int p_uiValidDateFromYear = 0,
               const unsigned int p_uiValidDateFromMonth = 0,
               const unsigned int p_uiValidDateFromDay = 0,
               const unsigned int p_uiValidDateToYear = 0,
               const unsigned int p_uiValidDateToMonth = 0,
               const unsigned int p_uiValidDateToDay = 0,
               const unsigned int p_uiValidDays = 365,
               const unsigned int p_uiUnitTime = 0 )   throw();
    void init( const QSqlRecord &p_obRecord )          throw();

};

#endif

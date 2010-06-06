#ifndef _sql_result_h_
#define _sql_result_h_

#include <QByteArray>
#include <QVariant>
#include <QSqlResult>



class SqlResult {
public:
    /** creates a byte-array from an SQL result to be sent over the network */
    static QByteArray * create(QSqlResult&);

    /** creates the object of SqlResult received from network */
    SqlResult() { /* be removed */ }
    SqlResult(QByteArray &b);
    ~SqlResult();

    /** returns the value of the i-th field */
    QVariant value(int i);
    /** returns the name of the i-th field */
    const char* fieldName(int i);
    /** returns the number of rows in the result */
    int size();
    /** returns true if dataset is valid (eg. complete) */
    bool isValid();


    bool seek(int i, bool relative = false);
    bool first();
    bool next();
    bool prev();
    bool last();
};


#endif

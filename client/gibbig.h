//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2014
//
//====================================================================================
//
// Filename    : gibbig.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Gibbig rendszerrel kapcsolatos reszek implementalasa
//====================================================================================

#ifndef GIBBIG_H
#define GIBBIG_H

//====================================================================================

#include <QNetworkAccessManager>
#include <QString>

//====================================================================================
class cGibbig
{
    //Q_OBJECT

public:

    cGibbig();
    ~cGibbig();

protected slots:
    void slotRestRequestFinished(QNetworkReply *);

private:

    QNetworkAccessManager *restManager;

};
//====================================================================================

#endif // GIBBIG_H

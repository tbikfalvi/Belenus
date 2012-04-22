//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dsppanel.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Panelok kezeleset vegzo osztaly
//====================================================================================

#ifndef DSPPANEL_H
#define DSPPANEL_H

//====================================================================================

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>

#include <vector>

//====================================================================================

//#include "db/dbpanelstatuses.h"

//====================================================================================

class cDspPanel : public QFrame
{
    Q_OBJECT

public:
    cDspPanel( const unsigned int p_uiPanelId );
    ~cDspPanel();

private:

    QVBoxLayout                 *verticalLayout;
    QLabel                      *lblTitle;
    QLabel                      *lblCurrStatus;
    QLabel                      *lblCurrTimer;
    QLabel                      *lblNextStatusLen;
    QLabel                      *lblInfo;
    QSpacerItem                 *spacer1;
    QSpacerItem                 *spacer2;
    QSpacerItem                 *spacer3;
    QSpacerItem                 *spacer4;
    QString                      m_qsStatus;
    QString                      m_qsTimer;
    QString                      m_qsTimerNextStatus;
    QString                      m_qsInfo;

    unsigned int                 m_uiId;

    void                         load();
/*
protected:
    void timerEvent ( QTimerEvent *p_poEvent );

private:

    QHBoxLayout                 *layoutIcons;
    QPushButton                 *icoShoppingCart;
    QSpacerItem                 *spacerIcons;
    QPushButton                 *icoPanelStart;
    QPushButton                 *icoPanelNext;
    QPushButton                 *icoPanelStop;
    QPushButton                 *icoPanelCassa;
    QPushButton                 *icoScheduledGuest;

    vector<cDBPanelStatuses*>    m_obStatuses;

    void            displayStatus();
    void            formatStatusString( QString p_qsStatusText );
    void            formatTimerString( QString p_qsTimerText );
    void            formatNextLengthString( QString p_qsNextLengthText );
    void            formatInfoString( QString p_qsInfoText );

    QString convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );

private slots:
*/
};

#endif // DSPPANEL_H

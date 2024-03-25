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

#include "db/dbpanelstatussettings.h"

//====================================================================================

class cDspPanel : public QFrame
{
    Q_OBJECT

public:
    cDspPanel( const unsigned int p_uiPanelId );
    ~cDspPanel();

    void                         refreshTitle();
    void                         setPanelStatus( const unsigned int p_uiPanelStatusId );
    void                         setPanelStatusText( const QString &p_qsStatus );
    void                         setPanelInfoText( const QString &p_qsInfo );
    void                         setCounterText( const QString &p_qsCounter );
    void                         setPanelWaitTime( const unsigned int p_uiWaitTime );
    void                         setImage( QString p_qsFilename );

private:

    QVBoxLayout                 *verticalLayout;
    QLabel                      *lblTitle;
    QSpacerItem                 *spacer1;
    QLabel                      *lblCurrStatus;
    QSpacerItem                 *spacer2;
    QLabel                      *lblCurrTimer;
    QSpacerItem                 *spacer3;
    QLabel                      *lblEstTimer;
    QSpacerItem                 *spacer4;
    QLabel                      *lblInfo;
    QSpacerItem                 *spacer5;
    QLabel                      *lblImage;

    unsigned int                 m_uiId;
    unsigned int                 m_uiWaitTime;
    int                          m_inTimerId;
    unsigned int                 m_uiPanelStatusId;

protected:

private:

    cDBPanelStatusSettings       m_obDBPanelStatusSettings;

    void                         _load();
    void                         _displayStatus();
    void                         _formatStatusString( QString p_qsStatusText );
    void                         _formatTimerString( QString p_qsTimerText );
    void                         _formatWaitTimeString( QString p_qsTimerText );
    void                         _formatInfoString( QString p_qsInfoText );

};

#endif // DSPPANEL_H

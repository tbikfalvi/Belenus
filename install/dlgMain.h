//====================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgMain.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef DLGMAIN_H
#define DLGMAIN_H

//====================================================================================

//#include <windows.h>
//#include <shlobj.h>

#include <QDialog>
#include <QFile>
#include <QSettings>

//====================================================================================

#include "ui_dlgMain.h"

//====================================================================================
class dlgMain : public QDialog, protected Ui::dlgMain
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();

private:
    QFile       *m_obFile;

    void processPage( int p_nPage );
    bool isRegKeyExists( QString p_qsKeyPath, QString p_qsKeyName );

private slots:
    void on_pbCancel_clicked();
    void on_pbPrev_clicked();
    void on_pbNext_clicked();
    void on_pbStartInstall_clicked();
};
//====================================================================================
#endif // DLGMAIN_H
//====================================================================================


/*
static HRESULT CreateShortCut(LPSTR pszTargetfile, LPSTR pszTargetargs,
                              LPSTR pszLinkfile, LPSTR pszDescription,
                              int iShowmode, LPSTR pszCurdir,
                              LPSTR pszIconfile, int iIconindex)
{
  HRESULT       hRes;
  IShellLink*   pShellLink;
  IPersistFile* pPersistFile;
  WORD          wszLinkfile[MAX_PATH];
  int           iWideCharsWritten;

  hRes = E_INVALIDARG;
  if (
       (pszTargetfile != NULL) && (strlen(pszTargetfile) > 0) &&
       (pszTargetargs != NULL) &&
       (pszLinkfile != NULL) && (strlen(pszLinkfile) > 0) &&
       (pszDescription != NULL) &&
       (iShowmode >= 0) &&
       (pszCurdir != NULL) &&
       (pszIconfile != NULL) &&
       (iIconindex >= 0)
     )
  {
    hRes = CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);

    if (SUCCEEDED(hRes))
    {
      hRes = pShellLink->SetPath((WCHAR*)pszTargetfile);
      hRes = pShellLink->SetArguments((WCHAR*)pszTargetargs);
      if (strlen(pszDescription) > 0)
      {
        hRes = pShellLink->SetDescription((WCHAR*)pszDescription);
      }
      if (iShowmode > 0)
      {
        hRes = pShellLink->SetShowCmd(iShowmode);
      }
      if (strlen(pszCurdir) > 0)
      {
        hRes = pShellLink->SetWorkingDirectory((WCHAR*)pszCurdir);
      }
      if (strlen(pszIconfile) > 0 && iIconindex >= 0)
      {
        hRes = pShellLink->SetIconLocation((WCHAR*)pszIconfile, iIconindex);
      }

      hRes = pShellLink->QueryInterface(IID_IPersistFile, &pPersistFile);

      if (SUCCEEDED(hRes))
      {
        iWideCharsWritten = MultiByteToWideChar(CP_ACP, 0, pszLinkfile, -1, wszLinkfile, MAX_PATH);
        hRes = pPersistFile->Save(pPersistFile, wszLinkfile, TRUE);
        pPersistFile->Release(pPersistFile);
      }
      pShellLink->Release(pShellLink);
    }

  }
  return (hRes);
}
*/
/*
bool createShortcut(LPCSTR lpszSrcFile,
                    LPCSTR lpszDstPath,
                    LPCSTR lpszName)
{
 IShellLink *pShl = NULL;
 IPersistFile *pPPF = NULL;
 HRESULT rc = CoCreateInstance(CLSID_ShellLink,
                               NULL,
                               CLSCTX_INPROC_SERVER,
                               IID_IShellLink,
                               (void**)(&pShl));
 if (FAILED(rc))
  return false;
 do
 {
  rc = pShl->SetPath(lpszSrcFile);
  if (FAILED(rc))
   break;
  rc = pShl->QueryInterface(IID_IPersistFile, (void**)&pPPF);
  if (FAILED(rc))
   break;
  WORD wsz[MAX_PATH];
  TCHAR path[MAX_PATH] = { 0 };
  lstrcat(path, lpszDstPath);
  lstrcat(path, "\\");
  lstrcat(path, lpszName);
  lstrcat(path, ".lnk");
  MultiByteToWideChar(CP_ACP, 0, buf, -1, wsz, MAX_PATH);
  rc = pPPF->Save(wsz, TRUE);
 } while(0);
 if (pPPF)
  pPPF->Release();
 if (pShl)
  pShl->Release();
 return SUCCEEDED(rc);
}
*/
// CreateLink - uses the Shell's IShellLink and IPersistFile interfaces
//              to create and store a shortcut to the specified object.
//
// Returns the result of calling the member functions of the interfaces.
//
// Parameters:
// lpszPathObj  - address of a buffer containing the path of the object.
// lpszPathLink - address of a buffer containing the path where the
//                Shell link is to be stored.
// lpszDesc     - address of a buffer containing the description of the
//                Shell link.
/*
HRESULT dlgMain::CreateShortcut(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc)
{
    HRESULT hres;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                            IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath((WCHAR*)lpszPathObj);
        psl->SetDescription((WCHAR*)lpszDesc);

        // Query IShellLink for the IPersistFile interface for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Add code here to check return value from MultiByteWideChar
            // for success.

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return hres;
}
*/

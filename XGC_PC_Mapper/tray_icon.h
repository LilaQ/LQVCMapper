#pragma once
#include <windows.h>
#include <shellapi.h>
#include <string>

#define WM_TRAYICON (WM_USER + 1)

class TrayIcon {
public:
    TrayIcon(HWND hWnd, UINT uID, const std::wstring& tooltip, HICON hIcon)
        : m_hWnd(hWnd), m_uID(uID)
    {
        NOTIFYICONDATA nid = { sizeof(nid) };
        nid.hWnd = hWnd;
        nid.uID = uID;
        nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        nid.uCallbackMessage = WM_TRAYICON;
        nid.hIcon = hIcon;
        wcsncpy_s(nid.szTip, tooltip.c_str(), tooltip.size());
        Shell_NotifyIcon(NIM_ADD, &nid);
    }
    ~TrayIcon() {
        NOTIFYICONDATA nid = { sizeof(nid) };
        nid.hWnd = m_hWnd;
        nid.uID = m_uID;
        Shell_NotifyIcon(NIM_DELETE, &nid);
    }
private:
    HWND m_hWnd;
    UINT m_uID;
};
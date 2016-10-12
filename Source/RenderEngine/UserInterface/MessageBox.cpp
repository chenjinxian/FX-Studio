#include "HumanView.h"
#include "MessageBox.h"
#include "../AppFramework/RenderEngineApp.h"
#include "../Graphics3D/D3DRenderer.h"

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#endif

void CALLBACK MessageBox::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void *pUserContext )
{
	PostMessage(g_pApp->GetHwnd(), g_MsgEndModal, 0, nControlID);
}

MessageBox::MessageBox(std::wstring msg, std::wstring title, int buttonFlags)
{
	m_UI.Init( &D3DRenderer::m_DialogResourceManager );
	m_UI.SetCallback( OnGUIEvent ); 

	RECT rc;
	SetRect( &rc, 0,0,0,0);
	m_UI.CalcTextRect( msg.c_str(), m_UI.GetDefaultElement(DXUT_CONTROL_STATIC,0), &rc );
	int msgWidth = rc.right - rc.left;
	int msgHeight = rc.bottom - rc.top;

	int numButtons = 2;
	if ( (buttonFlags == MB_ABORTRETRYIGNORE) ||
		(buttonFlags == MB_CANCELTRYCONTINUE) ||
		(buttonFlags == MB_CANCELTRYCONTINUE) )
	{
		numButtons = 3;
	}
	else if (buttonFlags == MB_OK)
	{
		numButtons = 1;
	}

	int btnWidth = (int)((float) g_pApp->GetScreenWidth() * 0.15f);
	int btnHeight = (int)((float) g_pApp->GetScreenHeight() * 0.037f);
	int border = (int)((float) g_pApp->GetScreenWidth() * 0.043f);

	m_Width = std::max(msgWidth + 2 * border, btnWidth + 2 * border);
	m_Height = msgHeight + (numButtons * (btnHeight+border) ) + (2 * border);

	m_PosX = (g_pApp->GetScreenWidth() - m_Width)/2;
	m_PosY = (g_pApp->GetScreenHeight() - m_Height)/2;
	m_UI.SetLocation( m_PosX, m_PosY );

	m_UI.SetSize( m_Width, m_Height );

	D3DCOLOR red = D3DCOLOR_ARGB(0xc0,0xff,0x00,0x00);
	m_UI.SetBackgroundColors(red);

	int iY = border; 
	int iX = (m_Width - msgWidth) / 2; 

	m_UI.AddStatic( 0, msg.c_str(), iX, iY, msgWidth, msgHeight);

	iX = (m_Width - btnWidth) / 2;
	iY = m_Height - btnHeight - border;

	buttonFlags &= 0xF;
	if ( (buttonFlags == MB_ABORTRETRYIGNORE) ||
		 (buttonFlags == MB_CANCELTRYCONTINUE) )

	{
		m_UI.AddButton( IDCONTINUE, g_pApp->GetString(_T("IDS_CONTINUE")).c_str(), iX, iY - (2*border), btnWidth, btnHeight );
		m_UI.AddButton( IDTRYAGAIN, g_pApp->GetString(_T("IDS_TRYAGAIN")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else if (buttonFlags == MB_OKCANCEL)
	{
		m_UI.AddButton( IDOK, g_pApp->GetString(_T("IDS_OK")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else if (buttonFlags == MB_RETRYCANCEL)
	{
		m_UI.AddButton( IDRETRY, g_pApp->GetString(_T("IDS_RETRY")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else if (buttonFlags == MB_YESNO)
	{
		m_UI.AddButton( IDYES, g_pApp->GetString(_T("IDS_YES")).c_str(), iX, iY - border, btnWidth, btnHeight, 0x59 );
		m_UI.AddButton( IDNO, g_pApp->GetString(_T("IDS_NO")).c_str(), iX, iY, btnWidth, btnHeight, 0x4E );
	}
	else if (buttonFlags == MB_YESNOCANCEL)
	{
		m_UI.AddButton( IDYES, g_pApp->GetString(_T("IDS_YES")).c_str(), iX, iY - (2*border), btnWidth, btnHeight );
		m_UI.AddButton( IDNO, g_pApp->GetString(_T("IDS_NO")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else if (buttonFlags & MB_OK)
	{
		m_UI.AddButton( IDOK, g_pApp->GetString(_T("IDS_OK")).c_str(), iX, iY, btnWidth, btnHeight );
	}
}

MessageBox::~MessageBox()
{
	GCC_ASSERT(1);
}

HRESULT MessageBox::VOnRestore()
{
	m_UI.SetLocation( m_PosX, m_PosY );
	m_UI.SetSize( m_Width, m_Height );
	return S_OK;
}

HRESULT MessageBox::VOnRender(float fTime, float fElapsedTime)
{
	HRESULT hr;
	V(m_UI.OnRender(fElapsedTime));
	return S_OK;
};

LRESULT CALLBACK MessageBox::VOnMsgProc( AppMsg msg )
{
	return m_UI.MsgProc( msg.m_hWnd, msg.m_uMsg, msg.m_wParam, msg.m_lParam );
}

int MessageBox::Ask(MessageBox_Questions question)
{
	std::wstring msg;
	std::wstring title;
	int buttonFlags;
	int defaultAnswer = IDOK;

	switch(question)
	{
		case QUESTION_WHERES_THE_CD:
		{
			msg = g_pApp->GetString(_T("IDS_QUESTION_WHERES_THE_CD"));
			title = g_pApp->GetString(_T("IDS_ALERT"));
			buttonFlags = MB_RETRYCANCEL|MB_ICONEXCLAMATION;
			defaultAnswer = IDCANCEL;
			break;
		}
		case QUESTION_QUIT_GAME:
		{
			msg = g_pApp->GetString(_T("IDS_QUESTION_QUIT_GAME"));
			title = g_pApp->GetString(_T("IDS_QUESTION"));
			buttonFlags = MB_YESNO|MB_ICONQUESTION;
			defaultAnswer = IDNO;
			break;
		}
		default:
			GCC_ASSERT(0 && _T("Undefined question in Game::Ask"));
			return IDCANCEL;
	}

	if (g_pApp && g_pApp->IsRunning())
	{
		ShowCursor(true);
		shared_ptr<MessageBox> pMessageBox(new MessageBox(msg, title, buttonFlags));
		int result = g_pApp->Modal(pMessageBox, defaultAnswer);
		ShowCursor(false);
		return result;
	}

	return ::MessageBox(g_pApp ? g_pApp->GetHwnd() : NULL, msg.c_str(), title.c_str(), buttonFlags);
}



#pragma once
#include <DXUTGui.h>

class MessageBox : public BaseUI
{
public:
	MessageBox(std::wstring msg, std::wstring title, int buttonFlags = MB_OK);
	~MessageBox();

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnRender(float fTime, float fElapsedTime);
	virtual int VGetZOrder() const { return 99; }
	virtual void VSetZOrder(int zOrder) { }

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	static void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void *pUserContext);
	static int Ask(MessageBox_Questions question);

protected:
	CDXUTDialog m_UI;
	int m_ButtonId;
};


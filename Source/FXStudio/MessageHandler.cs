using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace FXStudio
{
    public class MessageHandler : IMessageFilter
    {
        const int WM_LBUTTONDOWN = 0x0201;
        const int WM_LBUTTONUP = 0x0202;
        const int WM_LBUTTONDBLCLK = 0x0203;
        const int WM_RBUTTONDOWN = 0x0204;
        const int WM_RBUTTONUP = 0x0205;
        const int WM_RBUTTONDBLCLK = 0x0206;
        const int WM_MBUTTONDOWN = 0x0207;
        const int WM_MBUTTONUP = 0x0208;
        const int WM_MBUTTONDBLCLK = 0x0209;

        const int WM_MOUSEMOVE = 0x0200;
        const int WM_MOUSEHWHEEL = 0x020A;

        const int WM_CHAR = 0x0102;
        const int WM_KEYDOWN = 0x0100;
        const int WM_KEYUP = 0x0101;
        const int WM_SYSKEYDOWN = 0x0104;
        const int WM_SYSKEYUP = 0x0105;
        const int WM_CLOSE = 0x0010;

        FXStudioForm m_formMain;
        Panel m_renderPanel;
        bool m_fakeFocus;
        System.Drawing.Point m_mouseDownPosition;

        public MessageHandler(FXStudioForm formMain, Panel renderPanel)
        {
            m_formMain = formMain;
            m_renderPanel = renderPanel;
            m_fakeFocus = false;
            m_mouseDownPosition = new System.Drawing.Point(0, 0);
        }

        public void ResetRenderPanel(Panel renderPanel)
        {
            m_renderPanel = renderPanel;
        }

        void CheckFakeFocus()
        {
            System.Drawing.Point position = Cursor.Position;
            System.Drawing.Point relativeToForm = m_renderPanel.PointToClient(position);
            m_fakeFocus = (relativeToForm.X >= 0 && relativeToForm.Y >= 0 &&
                relativeToForm.X < m_renderPanel.Width && relativeToForm.Y < m_renderPanel.Width);
            if (m_fakeFocus)
            {
                m_mouseDownPosition = position;
            }
        }

        public bool PreFilterMessage(ref Message m)
        {
            if (m_renderPanel != null && m_renderPanel.IsHandleCreated)
            {
                if (m.Msg == WM_LBUTTONDOWN || m.Msg == WM_RBUTTONDOWN || m.Msg == WM_MBUTTONDOWN)
                    CheckFakeFocus();

                if (m.HWnd == m_renderPanel.Handle/* || (m_fakeFocus && (m.Msg == WM_KEYDOWN || m.Msg == WM_KEYUP))*/)
                {
                    switch (m.Msg)
                    {
                        case WM_LBUTTONDOWN:
                        case WM_RBUTTONDOWN:
                        case WM_MBUTTONDOWN:
                        case WM_KEYDOWN:
                        case WM_KEYUP:
                        case WM_SYSKEYDOWN:
                        case WM_SYSKEYUP:
                        case WM_LBUTTONUP:
                        case WM_LBUTTONDBLCLK:
                        case WM_RBUTTONUP:
                        case WM_RBUTTONDBLCLK:
                        case WM_MBUTTONUP:
                        case WM_MBUTTONDBLCLK:
                        case WM_MOUSEMOVE:
                        case WM_MOUSEHWHEEL:
                        case WM_CHAR:
                        case WM_CLOSE:
                            {
                                RenderMethods.WndProc(m_renderPanel.Handle, m.Msg, m.WParam, m.LParam);

                                if (m_fakeFocus && m.Msg == WM_LBUTTONUP)
                                {
                                    System.Drawing.Point position = Cursor.Position;
                                    double distance = Math.Round(Math.Sqrt(
                                        Math.Pow((position.X - m_mouseDownPosition.X), 2) +
                                        Math.Pow((position.Y - m_mouseDownPosition.Y), 2)), 1);
                                    if (distance < 3)
                                    {
                                        System.Drawing.Point relativeToForm = m_renderPanel.PointToClient(position);
                                        m_formMain.PickActor(relativeToForm);
                                    }
                                }
                                return true;
                            }
                    }
                }
            }
            return false;
        }

        public void Application_Idle(object sender, EventArgs e)
        {
            Debug.WriteLine("OnIdle " + new DateTime().Millisecond.ToString());
            if (m_formMain.WindowState != FormWindowState.Minimized)
            {
                try
                {
                    // Render the scene if we are idle
                    if (m_renderPanel.Handle != null)
                        RenderMethods.RenderFrame();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                if (m_renderPanel != null)
                    CheckFakeFocus();
                if (m_fakeFocus)
                    m_formMain.Invalidate();
            }
        }
    }
}

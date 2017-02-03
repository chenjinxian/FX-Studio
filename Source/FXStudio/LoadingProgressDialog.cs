using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace FXStudio
{
    public partial class LoadingProgressDialog : Form
    {
        private string m_SourceFileName;
        private bool m_Cancel = true;

        public LoadingProgressDialog()
        {
            InitializeComponent();
        }

        public string SourceFileName { set { m_SourceFileName = value; } }

        private void LoadingProgressDialog_Load(object sender, EventArgs e)
        {
            RenderMethods.ImportModel(m_SourceFileName, (percent, error) =>
            {
                Debug.WriteLine("percent: " + percent);
                progressBarLoading.Value = (int)percent  * 100;
//                 if (percent >= 1.0f)
//                 {
//                     this.DialogResult = DialogResult.OK;
//                     this.Close();
//                 }
                return m_Cancel;
            });
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            m_Cancel = false;
        }
    }
}

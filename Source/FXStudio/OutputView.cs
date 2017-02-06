using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FXStudio
{
    public partial class OutputView : ViewWindow
    {
        public OutputView()
        {
            InitializeComponent();
        }

        public void UpdateCompileInfo(string output, string error)
        {
            richTextBoxOutput.AppendText(error);
            richTextBoxOutput.AppendText(output);
            richTextBoxOutput.ScrollToCaret();
        }
    }
}

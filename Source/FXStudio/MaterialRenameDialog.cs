using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FXStudio
{
    public partial class MaterialRenameDialog : Form
    {
        private string m_MaterialName;

        public MaterialRenameDialog()
        {
            InitializeComponent();
        }

        public string MaterialName { get { return m_MaterialName; } set { m_MaterialName = value; } }

        private void textBoxName_TextChanged(object sender, EventArgs e)
        {
            string text = this.textBoxName.Text;
            this.buttonOK.Enabled = !string.IsNullOrEmpty(text) && (text != m_MaterialName);
            m_MaterialName = text;
        }

        private void MaterialRenameDialog_Load(object sender, EventArgs e)
        {
            this.textBoxName.Text = m_MaterialName;
            this.buttonOK.Enabled = false;
        }
    }
}

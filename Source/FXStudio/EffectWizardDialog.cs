using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CristiPotlog.WizardControl;

namespace FXStudio
{
    public partial class EffectWizardDialog : Form
    {
        private string m_FileName;
        private string m_EffectName;
        private string m_MaterialName;

        private string m_TemplatePath;

        public EffectWizardDialog()
        {
            InitializeComponent();

            m_TemplatePath = Directory.GetCurrentDirectory() + @"\Templates\";
        }

        public string FileName { get { return m_FileName; } set { m_FileName = value; } }

        public string EffectName { get { return m_EffectName; } set { m_EffectName = value; } }

        public string MaterialName { get { return m_MaterialName; } set { m_MaterialName = value; } }

        private void wizardForm_AfterSwitchPages(object sender, CristiPotlog.WizardControl.Wizard.AfterSwitchPagesEventArgs e)
        {
            WizardPage newPage = wizardForm.Pages[e.NewIndex];

            if (newPage == wizardPageFile)
            {
                wizardForm.NextEnabled = (textBoxFile.Text != string.Empty) && File.Exists(textBoxFile.Text);
            }
            else if (newPage == wizardPageTemplate)
            {
                wizardForm.NextEnabled = (listViewEffect.SelectedIndices.Count > 0) && Directory.Exists(textBoxLocation.Text);
            }
            else if (newPage == wizardPageName)
            {
                textBoxEffect.Text = m_EffectName;
                textBoxMaterial.Text = m_MaterialName;
                if (radioButtonFile.Checked)
                {
                    m_FileName = textBoxFile.Text;
                }
                else if (radioButtonHlsl.Checked)
                {
                    m_FileName = m_TemplatePath + textBoxName.Text;
                }
            }
        }

        private void wizardForm_BeforeSwitchPages(object sender, CristiPotlog.WizardControl.Wizard.BeforeSwitchPagesEventArgs e)
        {
            WizardPage oldPage = wizardForm.Pages[e.OldIndex];

            if (oldPage == wizardPageSelect && e.NewIndex > e.OldIndex)
            {
                if (!radioButtonFile.Checked)
                {
                    e.NewIndex++;
                }
            }
            else if (oldPage == wizardPageTemplate && e.NewIndex < e.OldIndex)
            {
                if (!radioButtonFile.Checked)
                {
                    e.NewIndex--;
                }
            }
            else if (oldPage == wizardPageFile && e.NewIndex > e.OldIndex)
            {
                e.NewIndex++;
            }
            else if (oldPage == wizardPageName && e.NewIndex < e.OldIndex)
            {
                if (radioButtonFile.Checked)
                {
                    e.NewIndex--;
                }
            }
        }

        private void radioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonEmpty.Checked)
            {
                wizardForm.NextEnabled = false;
                wizardForm.FinishEnabled = true;
            }
            else
            {
                wizardForm.NextEnabled = true;
                wizardForm.FinishEnabled = false;
            }
        }

        private void wizardForm_Load(object sender, EventArgs e)
        {
            wizardForm.SelectedPage = wizardPageSelect;
            wizardForm.NextEnabled = false;
            wizardForm.FinishEnabled = false;
        }

        private void textBoxFile_TextChanged(object sender, EventArgs e)
        {
            wizardForm.NextEnabled = (textBoxFile.Text != string.Empty) && File.Exists(textBoxFile.Text);
        }

        private void buttonFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.Filter = "HLSL Effect Files (*.fx)|*.fx";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                textBoxFile.Text = dialog.FileName;
                m_EffectName = Path.GetFileNameWithoutExtension(dialog.FileName);
                m_MaterialName = m_EffectName + "_Material";
            }
        }

        private void listViewEffect_SelectedIndexChanged(object sender, EventArgs e)
        {
            int select = listViewEffect.SelectedIndices[0];
        }

        private void buttonLocation_Click(object sender, EventArgs e)
        {

        }

        private void textBoxEffect_TextChanged(object sender, EventArgs e)
        {
            if (textBoxEffect.Text == string.Empty)
                wizardForm.FinishEnabled = false;
        }

        private void textBoxMaterial_TextChanged(object sender, EventArgs e)
        {
            if (textBoxMaterial.Text == string.Empty)
                wizardForm.FinishEnabled = false;
        }
    }
}

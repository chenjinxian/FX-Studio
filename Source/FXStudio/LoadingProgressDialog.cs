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

        public LoadingProgressDialog()
        {
            InitializeComponent();
        }

        public string SourceFileName { set { m_SourceFileName = value; } }

        private void LoadingProgressDialog_Load(object sender, EventArgs e)
        {
            if (!backgroundWorkerLoading.IsBusy)
            {
                backgroundWorkerLoading.RunWorkerAsync(m_SourceFileName);
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            if (backgroundWorkerLoading.WorkerSupportsCancellation)
            {
                backgroundWorkerLoading.CancelAsync();
            }
        }

        private void backgroundWorkerLoading_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            RenderMethods.ImportModel(e.Argument as string, (percent, error) =>
            {
                try
                {
                    worker.ReportProgress((int)(percent * 100));
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
                return !worker.CancellationPending;
            });
        }

        private void backgroundWorkerLoading_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBarLoading.Value = e.ProgressPercentage;
            labelProgress.Text = e.ProgressPercentage.ToString() + " %";
            Debug.WriteLine(labelProgress.Text);
        }

        private void backgroundWorkerLoading_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled)
            {

            }
            else if (e.Error != null)
            {

            }
            else
            {

            }
        }
    }
}

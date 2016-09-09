namespace FXStudio
{
    partial class EditorView
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panelTop = new System.Windows.Forms.Panel();
            this.panelBottom = new System.Windows.Forms.Panel();
            this.dockPanelEdit = new WeifenLuo.WinFormsUI.Docking.DockPanel();
            this.blueTheme = new WeifenLuo.WinFormsUI.Docking.VS2013BlueTheme();
            this.SuspendLayout();
            // 
            // panelTop
            // 
            this.panelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTop.Location = new System.Drawing.Point(0, 0);
            this.panelTop.Name = "panelTop";
            this.panelTop.Size = new System.Drawing.Size(738, 2);
            this.panelTop.TabIndex = 0;
            // 
            // panelBottom
            // 
            this.panelBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelBottom.Location = new System.Drawing.Point(0, 649);
            this.panelBottom.Name = "panelBottom";
            this.panelBottom.Size = new System.Drawing.Size(738, 2);
            this.panelBottom.TabIndex = 1;
            // 
            // dockPanelEdit
            // 
            this.dockPanelEdit.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dockPanelEdit.DocumentStyle = WeifenLuo.WinFormsUI.Docking.DocumentStyle.DockingWindow;
            this.dockPanelEdit.Location = new System.Drawing.Point(0, 0);
            this.dockPanelEdit.Name = "dockPanelEdit";
            this.dockPanelEdit.RightToLeftLayout = true;
            this.dockPanelEdit.Size = new System.Drawing.Size(738, 651);
            this.dockPanelEdit.TabIndex = 2;
            this.dockPanelEdit.Theme = this.blueTheme;
            // 
            // EditorView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(738, 651);
            this.Controls.Add(this.panelTop);
            this.Controls.Add(this.panelBottom);
            this.Controls.Add(this.dockPanelEdit);
            this.Name = "EditorView";
            this.TabText = "Editor";
            this.Text = "EditorView";
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel panelTop;
        private System.Windows.Forms.Panel panelBottom;
        private WeifenLuo.WinFormsUI.Docking.DockPanel dockPanelEdit;
        private WeifenLuo.WinFormsUI.Docking.VS2013BlueTheme blueTheme;
    }
}
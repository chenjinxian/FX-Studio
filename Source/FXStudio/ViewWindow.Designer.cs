namespace FXStudio
{
    partial class ViewWindow
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
            this.components = new System.ComponentModel.Container();
            this.contextMenuStripMode = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItemFloat = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemDock = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemAutoHide = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemHide = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripMode.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuStripMode
            // 
            this.contextMenuStripMode.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemFloat,
            this.toolStripMenuItemDock,
            this.toolStripMenuItemAutoHide,
            this.toolStripMenuItemHide});
            this.contextMenuStripMode.Name = "contextMenuStripMode";
            this.contextMenuStripMode.Size = new System.Drawing.Size(135, 92);
            // 
            // toolStripMenuItemFloat
            // 
            this.toolStripMenuItemFloat.Name = "toolStripMenuItemFloat";
            this.toolStripMenuItemFloat.Size = new System.Drawing.Size(134, 22);
            this.toolStripMenuItemFloat.Text = "Float";
            this.toolStripMenuItemFloat.Click += new System.EventHandler(this.toolStripMenuItemFloat_Click);
            // 
            // toolStripMenuItemDock
            // 
            this.toolStripMenuItemDock.Name = "toolStripMenuItemDock";
            this.toolStripMenuItemDock.Size = new System.Drawing.Size(134, 22);
            this.toolStripMenuItemDock.Text = "Dock";
            this.toolStripMenuItemDock.Click += new System.EventHandler(this.toolStripMenuItemDock_Click);
            // 
            // toolStripMenuItemAutoHide
            // 
            this.toolStripMenuItemAutoHide.Name = "toolStripMenuItemAutoHide";
            this.toolStripMenuItemAutoHide.Size = new System.Drawing.Size(134, 22);
            this.toolStripMenuItemAutoHide.Text = "Auto Hide";
            this.toolStripMenuItemAutoHide.Click += new System.EventHandler(this.toolStripMenuItemAutoHide_Click);
            // 
            // toolStripMenuItemHide
            // 
            this.toolStripMenuItemHide.Name = "toolStripMenuItemHide";
            this.toolStripMenuItemHide.Size = new System.Drawing.Size(134, 22);
            this.toolStripMenuItemHide.Text = "Hide";
            this.toolStripMenuItemHide.Click += new System.EventHandler(this.toolStripMenuItemHide_Click);
            // 
            // ViewWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Name = "ViewWindow";
            this.TabPageContextMenuStrip = this.contextMenuStripMode;
            this.TabText = "ViewWindow";
            this.Text = "ViewWindow";
            this.contextMenuStripMode.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip contextMenuStripMode;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemFloat;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemDock;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemAutoHide;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemHide;
    }
}
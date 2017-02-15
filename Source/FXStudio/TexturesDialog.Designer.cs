namespace FXStudio
{
    partial class TexturesDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TexturesDialog));
            this.listViewTextures = new System.Windows.Forms.ListView();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.imageListLarge = new System.Windows.Forms.ImageList(this.components);
            this.toolStripOperation = new System.Windows.Forms.ToolStrip();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewTextures
            // 
            this.listViewTextures.AllowDrop = true;
            this.listViewTextures.BackColor = System.Drawing.SystemColors.ControlDark;
            this.listViewTextures.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.listViewTextures.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewTextures.HideSelection = false;
            this.listViewTextures.LargeImageList = this.imageListLarge;
            this.listViewTextures.Location = new System.Drawing.Point(0, 0);
            this.listViewTextures.Name = "listViewTextures";
            this.listViewTextures.Size = new System.Drawing.Size(704, 380);
            this.listViewTextures.TabIndex = 0;
            this.listViewTextures.UseCompatibleStateImageBehavior = false;
            // 
            // buttonOK
            // 
            this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOK.Location = new System.Drawing.Point(541, 6);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 1;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCancel.Location = new System.Drawing.Point(622, 6);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.buttonOK);
            this.panel1.Controls.Add(this.buttonCancel);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 405);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(704, 36);
            this.panel1.TabIndex = 3;
            // 
            // imageListLarge
            // 
            this.imageListLarge.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListLarge.ImageStream")));
            this.imageListLarge.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListLarge.Images.SetKeyName(0, "default_texture.png");
            // 
            // toolStripOperation
            // 
            this.toolStripOperation.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripOperation.Location = new System.Drawing.Point(0, 0);
            this.toolStripOperation.Name = "toolStripOperation";
            this.toolStripOperation.Size = new System.Drawing.Size(704, 25);
            this.toolStripOperation.TabIndex = 4;
            this.toolStripOperation.Text = "toolStrip1";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.listViewTextures);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 25);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(704, 380);
            this.panel2.TabIndex = 5;
            // 
            // TexturesDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(704, 441);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.toolStripOperation);
            this.Controls.Add(this.panel1);
            this.Name = "TexturesDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Textures";
            this.Load += new System.EventHandler(this.ImageCollectionsDialog_Load);
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listViewTextures;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ImageList imageListLarge;
        private System.Windows.Forms.ToolStrip toolStripOperation;
        private System.Windows.Forms.Panel panel2;
    }
}
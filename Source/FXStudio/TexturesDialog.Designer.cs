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
            this.imageListLarge = new System.Windows.Forms.ImageList(this.components);
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.toolStripOperation = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonAdd = new System.Windows.Forms.ToolStripButton();
            this.panel2 = new System.Windows.Forms.Panel();
            this.openFileDialogAdd = new System.Windows.Forms.OpenFileDialog();
            this.panel1.SuspendLayout();
            this.toolStripOperation.SuspendLayout();
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
            this.listViewTextures.MultiSelect = false;
            this.listViewTextures.Name = "listViewTextures";
            this.listViewTextures.Size = new System.Drawing.Size(684, 503);
            this.listViewTextures.TabIndex = 0;
            this.listViewTextures.UseCompatibleStateImageBehavior = false;
            this.listViewTextures.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewTextures_MouseDoubleClick);
            // 
            // imageListLarge
            // 
            this.imageListLarge.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListLarge.ImageStream")));
            this.imageListLarge.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListLarge.Images.SetKeyName(0, "default_texture.png");
            // 
            // buttonOK
            // 
            this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOK.Location = new System.Drawing.Point(521, 7);
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
            this.buttonCancel.Location = new System.Drawing.Point(602, 7);
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
            this.panel1.Location = new System.Drawing.Point(0, 528);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(684, 36);
            this.panel1.TabIndex = 3;
            // 
            // toolStripOperation
            // 
            this.toolStripOperation.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripOperation.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonAdd});
            this.toolStripOperation.Location = new System.Drawing.Point(0, 0);
            this.toolStripOperation.Name = "toolStripOperation";
            this.toolStripOperation.Size = new System.Drawing.Size(684, 25);
            this.toolStripOperation.TabIndex = 4;
            this.toolStripOperation.Text = "toolStrip1";
            // 
            // toolStripButtonAdd
            // 
            this.toolStripButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonAdd.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonAdd.Image")));
            this.toolStripButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonAdd.Name = "toolStripButtonAdd";
            this.toolStripButtonAdd.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonAdd.Text = "Add Texture";
            this.toolStripButtonAdd.Click += new System.EventHandler(this.toolStripButtonAdd_Click);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.listViewTextures);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 25);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(684, 503);
            this.panel2.TabIndex = 5;
            // 
            // openFileDialogAdd
            // 
            this.openFileDialogAdd.Filter = "Image Files (*.dds; *.bmp; *.jpg; *.jpeg; *.png; *.gif; *.tif;)|*.dds; *.bmp; *.j" +
    "pg; *.jpeg; *.png; *.gif; *.tif;";
            // 
            // TexturesDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(684, 564);
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
            this.toolStripOperation.ResumeLayout(false);
            this.toolStripOperation.PerformLayout();
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
        private System.Windows.Forms.ToolStripButton toolStripButtonAdd;
        private System.Windows.Forms.OpenFileDialog openFileDialogAdd;
    }
}
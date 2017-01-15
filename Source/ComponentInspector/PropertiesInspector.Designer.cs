namespace Inspector
{
    partial class PropertiesInspector
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PropertiesInspector));
            this.pnlText = new System.Windows.Forms.Panel();
            this.toolSeparator01 = new System.Windows.Forms.ToolStripSeparator();
            this.btnAlphabetical = new System.Windows.Forms.ToolStripButton();
            this.btnCategorized = new System.Windows.Forms.ToolStripButton();
            this.topBar = new System.Windows.Forms.ToolStrip();
            this.btnShowHelp = new System.Windows.Forms.ToolStripButton();
            this.btnApply = new System.Windows.Forms.ToolStripButton();
            this.btnShowText = new System.Windows.Forms.ToolStripButton();
            this.splitBottom = new System.Windows.Forms.Splitter();
            this.pnlProp = new System.Windows.Forms.Panel();
            this.btnCancel = new System.Windows.Forms.Button();
            this.imgList = new System.Windows.Forms.ImageList(this.components);
            this.dtTmBox = new System.Windows.Forms.DateTimePicker();
            this.chkBox = new System.Windows.Forms.CheckBox();
            this.btnMore = new System.Windows.Forms.Button();
            this.numUpDn = new System.Windows.Forms.NumericUpDown();
            this.cmbBox = new System.Windows.Forms.ComboBox();
            this.txtBox = new System.Windows.Forms.TextBox();
            this.picProp = new System.Windows.Forms.PictureBox();
            this.dirDiag = new System.Windows.Forms.FolderBrowserDialog();
            this.fontDiag = new System.Windows.Forms.FontDialog();
            this.fileDiag = new System.Windows.Forms.OpenFileDialog();
            this.lblHelpCaption = new System.Windows.Forms.Label();
            this.lblHelpText = new System.Windows.Forms.Label();
            this.colDiag = new System.Windows.Forms.ColorDialog();
            this.iconList = new System.Windows.Forms.ImageList(this.components);
            this.pnlHelp = new System.Windows.Forms.Panel();
            this.topBar.SuspendLayout();
            this.pnlProp.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picProp)).BeginInit();
            this.pnlHelp.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnlText
            // 
            this.pnlText.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.pnlText.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlText.Location = new System.Drawing.Point(0, 0);
            this.pnlText.Name = "pnlText";
            this.pnlText.Size = new System.Drawing.Size(239, 25);
            this.pnlText.TabIndex = 0;
            this.pnlText.Visible = false;
            // 
            // toolSeparator01
            // 
            this.toolSeparator01.Name = "toolSeparator01";
            this.toolSeparator01.Size = new System.Drawing.Size(6, 25);
            // 
            // btnAlphabetical
            // 
            this.btnAlphabetical.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnAlphabetical.Image = ((System.Drawing.Image)(resources.GetObject("btnAlphabetical.Image")));
            this.btnAlphabetical.ImageTransparentColor = System.Drawing.Color.Red;
            this.btnAlphabetical.Name = "btnAlphabetical";
            this.btnAlphabetical.Size = new System.Drawing.Size(23, 22);
            this.btnAlphabetical.Text = "Alphabetical";
            this.btnAlphabetical.ToolTipText = "Order items alphabetically";
            this.btnAlphabetical.Click += new System.EventHandler(this.btnAlphabetical_Click);
            // 
            // btnCategorized
            // 
            this.btnCategorized.Checked = true;
            this.btnCategorized.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnCategorized.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnCategorized.Image = ((System.Drawing.Image)(resources.GetObject("btnCategorized.Image")));
            this.btnCategorized.ImageTransparentColor = System.Drawing.Color.Red;
            this.btnCategorized.Name = "btnCategorized";
            this.btnCategorized.Size = new System.Drawing.Size(23, 22);
            this.btnCategorized.Text = "Category";
            this.btnCategorized.ToolTipText = "Order items by category";
            this.btnCategorized.Click += new System.EventHandler(this.btnCategorized_Click);
            // 
            // topBar
            // 
            this.topBar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.topBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnCategorized,
            this.btnAlphabetical,
            this.toolSeparator01,
            this.btnShowHelp,
            this.btnApply,
            this.btnShowText});
            this.topBar.Location = new System.Drawing.Point(0, 25);
            this.topBar.Name = "topBar";
            this.topBar.Size = new System.Drawing.Size(239, 25);
            this.topBar.TabIndex = 1;
            // 
            // btnShowHelp
            // 
            this.btnShowHelp.Checked = true;
            this.btnShowHelp.CheckOnClick = true;
            this.btnShowHelp.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnShowHelp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnShowHelp.Image = ((System.Drawing.Image)(resources.GetObject("btnShowHelp.Image")));
            this.btnShowHelp.ImageTransparentColor = System.Drawing.Color.Red;
            this.btnShowHelp.Name = "btnShowHelp";
            this.btnShowHelp.Size = new System.Drawing.Size(23, 22);
            this.btnShowHelp.Text = "Help";
            this.btnShowHelp.ToolTipText = "Show-hide help box";
            this.btnShowHelp.Click += new System.EventHandler(this.btnShowHelp_Click);
            // 
            // btnApply
            // 
            this.btnApply.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnApply.Image = ((System.Drawing.Image)(resources.GetObject("btnApply.Image")));
            this.btnApply.ImageTransparentColor = System.Drawing.Color.Transparent;
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(23, 22);
            this.btnApply.Text = "Apply";
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // btnShowText
            // 
            this.btnShowText.Checked = true;
            this.btnShowText.CheckOnClick = true;
            this.btnShowText.CheckState = System.Windows.Forms.CheckState.Checked;
            this.btnShowText.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnShowText.Image = ((System.Drawing.Image)(resources.GetObject("btnShowText.Image")));
            this.btnShowText.ImageTransparentColor = System.Drawing.Color.Red;
            this.btnShowText.Name = "btnShowText";
            this.btnShowText.Size = new System.Drawing.Size(23, 22);
            this.btnShowText.Text = "Text";
            this.btnShowText.Visible = false;
            this.btnShowText.Click += new System.EventHandler(this.btnShowText_Click);
            // 
            // splitBottom
            // 
            this.splitBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.splitBottom.Location = new System.Drawing.Point(0, 256);
            this.splitBottom.Name = "splitBottom";
            this.splitBottom.Size = new System.Drawing.Size(239, 4);
            this.splitBottom.TabIndex = 8;
            this.splitBottom.TabStop = false;
            this.splitBottom.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitBottom_SplitterMoved);
            // 
            // pnlProp
            // 
            this.pnlProp.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlProp.BackColor = System.Drawing.Color.Silver;
            this.pnlProp.Controls.Add(this.btnCancel);
            this.pnlProp.Controls.Add(this.dtTmBox);
            this.pnlProp.Controls.Add(this.chkBox);
            this.pnlProp.Controls.Add(this.btnMore);
            this.pnlProp.Controls.Add(this.numUpDn);
            this.pnlProp.Controls.Add(this.cmbBox);
            this.pnlProp.Controls.Add(this.txtBox);
            this.pnlProp.Controls.Add(this.picProp);
            this.pnlProp.Location = new System.Drawing.Point(0, 50);
            this.pnlProp.Name = "pnlProp";
            this.pnlProp.Size = new System.Drawing.Size(239, 204);
            this.pnlProp.TabIndex = 7;
            // 
            // btnCancel
            // 
            this.btnCancel.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.ImageIndex = 5;
            this.btnCancel.ImageList = this.imgList;
            this.btnCancel.Location = new System.Drawing.Point(122, 69);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(20, 20);
            this.btnCancel.TabIndex = 8;
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Visible = false;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // imgList
            // 
            this.imgList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imgList.ImageStream")));
            this.imgList.TransparentColor = System.Drawing.Color.Purple;
            this.imgList.Images.SetKeyName(0, "Minus_13x13.bmp");
            this.imgList.Images.SetKeyName(1, "Plus_13x13.bmp");
            this.imgList.Images.SetKeyName(2, "");
            this.imgList.Images.SetKeyName(3, "");
            this.imgList.Images.SetKeyName(4, "");
            this.imgList.Images.SetKeyName(5, "");
            // 
            // dtTmBox
            // 
            this.dtTmBox.Location = new System.Drawing.Point(23, 116);
            this.dtTmBox.Name = "dtTmBox";
            this.dtTmBox.Size = new System.Drawing.Size(66, 20);
            this.dtTmBox.TabIndex = 6;
            this.dtTmBox.Visible = false;
            this.dtTmBox.ValueChanged += new System.EventHandler(this.dtTmBox_ValueChanged);
            // 
            // chkBox
            // 
            this.chkBox.BackColor = System.Drawing.Color.White;
            this.chkBox.Location = new System.Drawing.Point(22, 95);
            this.chkBox.Name = "chkBox";
            this.chkBox.Size = new System.Drawing.Size(62, 15);
            this.chkBox.TabIndex = 4;
            this.chkBox.Text = "chkBox";
            this.chkBox.UseVisualStyleBackColor = false;
            this.chkBox.Visible = false;
            this.chkBox.Click += new System.EventHandler(this.chkBox_Click);
            this.chkBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.chkBox_KeyPress);
            // 
            // btnMore
            // 
            this.btnMore.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnMore.ImageIndex = 4;
            this.btnMore.ImageList = this.imgList;
            this.btnMore.Location = new System.Drawing.Point(96, 69);
            this.btnMore.Name = "btnMore";
            this.btnMore.Size = new System.Drawing.Size(20, 20);
            this.btnMore.TabIndex = 5;
            this.btnMore.UseVisualStyleBackColor = true;
            this.btnMore.Visible = false;
            this.btnMore.Click += new System.EventHandler(this.btnMore_Click);
            // 
            // numUpDn
            // 
            this.numUpDn.Location = new System.Drawing.Point(22, 69);
            this.numUpDn.Name = "numUpDn";
            this.numUpDn.Size = new System.Drawing.Size(68, 20);
            this.numUpDn.TabIndex = 3;
            this.numUpDn.Visible = false;
            this.numUpDn.ValueChanged += new System.EventHandler(this.numUpDn_ValueChanged);
            this.numUpDn.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.numUpDn_KeyPress);
            // 
            // cmbBox
            // 
            this.cmbBox.Location = new System.Drawing.Point(22, 42);
            this.cmbBox.Name = "cmbBox";
            this.cmbBox.Size = new System.Drawing.Size(68, 21);
            this.cmbBox.TabIndex = 2;
            this.cmbBox.Visible = false;
            this.cmbBox.SelectedIndexChanged += new System.EventHandler(this.cmbBox_SelectedIndexChanged);
            this.cmbBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.cmbBox_KeyPress);
            this.cmbBox.TextChanged += new System.EventHandler(this.cmbBox_TextChanged);
            // 
            // txtBox
            // 
            this.txtBox.Location = new System.Drawing.Point(22, 16);
            this.txtBox.Name = "txtBox";
            this.txtBox.Size = new System.Drawing.Size(68, 20);
            this.txtBox.TabIndex = 1;
            this.txtBox.Visible = false;
            this.txtBox.DoubleClick += new System.EventHandler(this.txtBox_DoubleClick);
            this.txtBox.TextChanged += new System.EventHandler(this.txtBox_TextChanged);
            this.txtBox.Leave += new System.EventHandler(this.txtBox_Leave);
            this.txtBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtBox_KeyPress);
            // 
            // picProp
            // 
            this.picProp.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.picProp.BackColor = System.Drawing.Color.White;
            this.picProp.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.picProp.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picProp.Location = new System.Drawing.Point(0, 0);
            this.picProp.Name = "picProp";
            this.picProp.Size = new System.Drawing.Size(239, 147);
            this.picProp.TabIndex = 7;
            this.picProp.TabStop = false;
            this.picProp.DoubleClick += new System.EventHandler(this.picProp_DoubleClick);
            this.picProp.MouseMove += new System.Windows.Forms.MouseEventHandler(this.picProp_MouseMove);
            this.picProp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.picProp_MouseUp);
            // 
            // fontDiag
            // 
            this.fontDiag.ShowColor = true;
            // 
            // fileDiag
            // 
            this.fileDiag.DefaultExt = "*";
            this.fileDiag.Filter = "All pictures (*.bmp;*.png;*.jpg;*.tiff;*.gif)|*.bmp;*.png;*.jpg;*.tiff;*.gif|All " +
                "files (*.*)|*.*";
            // 
            // lblHelpCaption
            // 
            this.lblHelpCaption.AutoSize = true;
            this.lblHelpCaption.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblHelpCaption.Location = new System.Drawing.Point(3, 3);
            this.lblHelpCaption.Name = "lblHelpCaption";
            this.lblHelpCaption.Size = new System.Drawing.Size(33, 13);
            this.lblHelpCaption.TabIndex = 0;
            this.lblHelpCaption.Text = "Help";
            this.lblHelpCaption.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lblHelpCaption_MouseDoubleClick);
            // 
            // lblHelpText
            // 
            this.lblHelpText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblHelpText.Location = new System.Drawing.Point(3, 20);
            this.lblHelpText.Name = "lblHelpText";
            this.lblHelpText.Size = new System.Drawing.Size(232, 41);
            this.lblHelpText.TabIndex = 1;
            this.lblHelpText.Text = "Info...";
            this.lblHelpText.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lblHelpText_MouseDoubleClick);
            // 
            // colDiag
            // 
            this.colDiag.AnyColor = true;
            this.colDiag.ShowHelp = true;
            // 
            // iconList
            // 
            this.iconList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("iconList.ImageStream")));
            this.iconList.TransparentColor = System.Drawing.Color.Red;
            this.iconList.Images.SetKeyName(0, "Warning_16x16_TrueColor.bmp");
            // 
            // pnlHelp
            // 
            this.pnlHelp.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.pnlHelp.Controls.Add(this.lblHelpText);
            this.pnlHelp.Controls.Add(this.lblHelpCaption);
            this.pnlHelp.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pnlHelp.Location = new System.Drawing.Point(0, 260);
            this.pnlHelp.Name = "pnlHelp";
            this.pnlHelp.Size = new System.Drawing.Size(239, 66);
            this.pnlHelp.TabIndex = 9;
            this.pnlHelp.DoubleClick += new System.EventHandler(this.pnlHelp_DoubleClick);
            // 
            // PropertiesInspector
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitBottom);
            this.Controls.Add(this.pnlProp);
            this.Controls.Add(this.pnlHelp);
            this.Controls.Add(this.topBar);
            this.Controls.Add(this.pnlText);
            this.Name = "PropertiesInspector";
            this.Size = new System.Drawing.Size(239, 326);
            this.Load += new System.EventHandler(this.PropertyGrid_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.PropertyGrid_Paint);
            this.SizeChanged += new System.EventHandler(this.PropertyGrid_SizeChanged);
            this.topBar.ResumeLayout(false);
            this.topBar.PerformLayout();
            this.pnlProp.ResumeLayout(false);
            this.pnlProp.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picProp)).EndInit();
            this.pnlHelp.ResumeLayout(false);
            this.pnlHelp.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel pnlText;
        private System.Windows.Forms.ToolStripSeparator toolSeparator01;
        private System.Windows.Forms.ToolStripButton btnAlphabetical;
        private System.Windows.Forms.ToolStripButton btnCategorized;
        private System.Windows.Forms.ToolStrip topBar;
        private System.Windows.Forms.ToolStripButton btnShowHelp;
        private System.Windows.Forms.ToolStripButton btnShowText;
        private System.Windows.Forms.ToolStripButton btnApply;
        private System.Windows.Forms.Splitter splitBottom;
        private System.Windows.Forms.Panel pnlProp;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ImageList imgList;
        private System.Windows.Forms.DateTimePicker dtTmBox;
        private System.Windows.Forms.CheckBox chkBox;
        private System.Windows.Forms.Button btnMore;
        private System.Windows.Forms.NumericUpDown numUpDn;
        private System.Windows.Forms.ComboBox cmbBox;
        private System.Windows.Forms.TextBox txtBox;
        private System.Windows.Forms.PictureBox picProp;
        private System.Windows.Forms.FolderBrowserDialog dirDiag;
        private System.Windows.Forms.FontDialog fontDiag;
        private System.Windows.Forms.OpenFileDialog fileDiag;
        private System.Windows.Forms.Label lblHelpCaption;
        private System.Windows.Forms.Label lblHelpText;
        private System.Windows.Forms.ColorDialog colDiag;
        private System.Windows.Forms.ImageList iconList;
        private System.Windows.Forms.Panel pnlHelp;
    }
}

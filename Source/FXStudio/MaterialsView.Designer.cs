namespace FXStudio
{
    partial class MaterialsView
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.listViewMaterials = new System.Windows.Forms.ListView();
            this.imageListLarge = new System.Windows.Forms.ImageList(this.components);
            this.toolStripMaterial = new System.Windows.Forms.ToolStrip();
            this.toolStripTexture = new System.Windows.Forms.ToolStrip();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.listViewMaterials);
            this.splitContainer1.Panel1.Controls.Add(this.toolStripMaterial);
            this.splitContainer1.Panel1MinSize = 200;
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.toolStripTexture);
            this.splitContainer1.Panel2MinSize = 100;
            this.splitContainer1.Size = new System.Drawing.Size(366, 648);
            this.splitContainer1.SplitterDistance = 367;
            this.splitContainer1.TabIndex = 0;
            // 
            // listViewMaterials
            // 
            this.listViewMaterials.AllowDrop = true;
            this.listViewMaterials.BackColor = System.Drawing.SystemColors.Window;
            this.listViewMaterials.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.listViewMaterials.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewMaterials.HideSelection = false;
            this.listViewMaterials.LargeImageList = this.imageListLarge;
            this.listViewMaterials.Location = new System.Drawing.Point(0, 25);
            this.listViewMaterials.MultiSelect = false;
            this.listViewMaterials.Name = "listViewMaterials";
            this.listViewMaterials.Size = new System.Drawing.Size(366, 342);
            this.listViewMaterials.TabIndex = 1;
            this.listViewMaterials.UseCompatibleStateImageBehavior = false;
            this.listViewMaterials.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.listViewMaterials_ItemDrag);
            this.listViewMaterials.SelectedIndexChanged += new System.EventHandler(this.listViewMaterials_SelectedIndexChanged);
            // 
            // imageListLarge
            // 
            this.imageListLarge.ColorDepth = System.Windows.Forms.ColorDepth.Depth24Bit;
            this.imageListLarge.ImageSize = new System.Drawing.Size(100, 100);
            this.imageListLarge.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // toolStripMaterial
            // 
            this.toolStripMaterial.Location = new System.Drawing.Point(0, 0);
            this.toolStripMaterial.Name = "toolStripMaterial";
            this.toolStripMaterial.Size = new System.Drawing.Size(366, 25);
            this.toolStripMaterial.TabIndex = 0;
            this.toolStripMaterial.Text = "toolStrip1";
            // 
            // toolStripTexture
            // 
            this.toolStripTexture.Location = new System.Drawing.Point(0, 0);
            this.toolStripTexture.Name = "toolStripTexture";
            this.toolStripTexture.Size = new System.Drawing.Size(366, 25);
            this.toolStripTexture.TabIndex = 0;
            this.toolStripTexture.Text = "toolStrip2";
            // 
            // MaterialsView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(366, 648);
            this.Controls.Add(this.splitContainer1);
            this.Name = "MaterialsView";
            this.TabText = "Materials";
            this.Text = "     ";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ToolStrip toolStripMaterial;
        private System.Windows.Forms.ToolStrip toolStripTexture;
        private System.Windows.Forms.ListView listViewMaterials;
        private System.Windows.Forms.ImageList imageListLarge;
    }
}
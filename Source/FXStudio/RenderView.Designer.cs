namespace FXStudio
{
    partial class RenderView
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RenderView));
            this.toolStripRender = new System.Windows.Forms.ToolStrip();
            this.toolStripDropDownButtonNavigation = new System.Windows.Forms.ToolStripDropDownButton();
            this.toolStripMenuItemOrbit = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemFp = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonZoomObjects = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonReset = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonSelect = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonTranslate = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonRotate = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonScale = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.panelRender = new System.Windows.Forms.Panel();
            this.toolStripRender.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripRender
            // 
            this.toolStripRender.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStripRender.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripDropDownButtonNavigation,
            this.toolStripSeparator1,
            this.toolStripButtonZoomObjects,
            this.toolStripButtonReset,
            this.toolStripSeparator2,
            this.toolStripButtonSelect,
            this.toolStripButtonTranslate,
            this.toolStripButtonRotate,
            this.toolStripButtonScale,
            this.toolStripSeparator3});
            this.toolStripRender.Location = new System.Drawing.Point(0, 0);
            this.toolStripRender.Name = "toolStripRender";
            this.toolStripRender.Size = new System.Drawing.Size(756, 25);
            this.toolStripRender.TabIndex = 0;
            this.toolStripRender.Text = "toolStripRender";
            // 
            // toolStripDropDownButtonNavigation
            // 
            this.toolStripDropDownButtonNavigation.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButtonNavigation.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemOrbit,
            this.toolStripMenuItemFp});
            this.toolStripDropDownButtonNavigation.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButtonNavigation.Image")));
            this.toolStripDropDownButtonNavigation.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButtonNavigation.Name = "toolStripDropDownButtonNavigation";
            this.toolStripDropDownButtonNavigation.Size = new System.Drawing.Size(29, 22);
            this.toolStripDropDownButtonNavigation.Text = "Navigation Mode";
            // 
            // toolStripMenuItemOrbit
            // 
            this.toolStripMenuItemOrbit.Checked = true;
            this.toolStripMenuItemOrbit.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolStripMenuItemOrbit.Name = "toolStripMenuItemOrbit";
            this.toolStripMenuItemOrbit.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemOrbit.Text = "Orbit";
            this.toolStripMenuItemOrbit.Click += new System.EventHandler(this.toolStripMenuItemOrbit_Click);
            // 
            // toolStripMenuItemFp
            // 
            this.toolStripMenuItemFp.Name = "toolStripMenuItemFp";
            this.toolStripMenuItemFp.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemFp.Text = "First Person";
            this.toolStripMenuItemFp.Click += new System.EventHandler(this.toolStripMenuItemFp_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonZoomObjects
            // 
            this.toolStripButtonZoomObjects.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonZoomObjects.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonZoomObjects.Image")));
            this.toolStripButtonZoomObjects.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonZoomObjects.Name = "toolStripButtonZoomObjects";
            this.toolStripButtonZoomObjects.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonZoomObjects.Text = "Zoom Selected Object Extents";
            // 
            // toolStripButtonReset
            // 
            this.toolStripButtonReset.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonReset.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonReset.Image")));
            this.toolStripButtonReset.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonReset.Name = "toolStripButtonReset";
            this.toolStripButtonReset.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonReset.Text = "Reset Camera Rotation";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonSelect
            // 
            this.toolStripButtonSelect.Checked = true;
            this.toolStripButtonSelect.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolStripButtonSelect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSelect.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonSelect.Image")));
            this.toolStripButtonSelect.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripButtonSelect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSelect.Name = "toolStripButtonSelect";
            this.toolStripButtonSelect.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonSelect.Text = "Select Object";
            this.toolStripButtonSelect.Click += new System.EventHandler(this.toolStripButtonTransform_Click);
            // 
            // toolStripButtonTranslate
            // 
            this.toolStripButtonTranslate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonTranslate.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonTranslate.Image")));
            this.toolStripButtonTranslate.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.toolStripButtonTranslate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonTranslate.Name = "toolStripButtonTranslate";
            this.toolStripButtonTranslate.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonTranslate.Text = "Translate Object";
            this.toolStripButtonTranslate.Click += new System.EventHandler(this.toolStripButtonTransform_Click);
            // 
            // toolStripButtonRotate
            // 
            this.toolStripButtonRotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonRotate.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonRotate.Image")));
            this.toolStripButtonRotate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonRotate.Name = "toolStripButtonRotate";
            this.toolStripButtonRotate.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonRotate.Text = "Rotate Object";
            this.toolStripButtonRotate.Click += new System.EventHandler(this.toolStripButtonTransform_Click);
            // 
            // toolStripButtonScale
            // 
            this.toolStripButtonScale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonScale.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonScale.Image")));
            this.toolStripButtonScale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonScale.Name = "toolStripButtonScale";
            this.toolStripButtonScale.Size = new System.Drawing.Size(23, 22);
            this.toolStripButtonScale.Text = "Scale Object";
            this.toolStripButtonScale.Click += new System.EventHandler(this.toolStripButtonTransform_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // panelRender
            // 
            this.panelRender.AllowDrop = true;
            this.panelRender.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panelRender.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelRender.Location = new System.Drawing.Point(0, 25);
            this.panelRender.Name = "panelRender";
            this.panelRender.Size = new System.Drawing.Size(756, 523);
            this.panelRender.TabIndex = 1;
            this.panelRender.DragDrop += new System.Windows.Forms.DragEventHandler(this.panelRender_DragDrop);
            this.panelRender.DragEnter += new System.Windows.Forms.DragEventHandler(this.panelRender_DragEnter);
            this.panelRender.DragOver += new System.Windows.Forms.DragEventHandler(this.panelRender_DragOver);
            // 
            // RenderView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(756, 548);
            this.Controls.Add(this.panelRender);
            this.Controls.Add(this.toolStripRender);
            this.Name = "RenderView";
            this.TabText = "Render";
            this.Text = "RenderView";
            this.Load += new System.EventHandler(this.RenderView_Load);
            this.Resize += new System.EventHandler(this.RenderView_Resize);
            this.toolStripRender.ResumeLayout(false);
            this.toolStripRender.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStripRender;
        private System.Windows.Forms.Panel panelRender;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButtonZoomObjects;
        private System.Windows.Forms.ToolStripButton toolStripButtonReset;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton toolStripButtonSelect;
        private System.Windows.Forms.ToolStripButton toolStripButtonTranslate;
        private System.Windows.Forms.ToolStripButton toolStripButtonRotate;
        private System.Windows.Forms.ToolStripButton toolStripButtonScale;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButtonNavigation;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemOrbit;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemFp;
    }
}
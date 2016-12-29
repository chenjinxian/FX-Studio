namespace FXStudio
{
    partial class FXStudioForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FXStudioForm));
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonNew = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonOpen = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonImport = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonSaveAll = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonUndo = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonRedo = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonEffect = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonReBuild = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonCompile = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonTeapot = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonCube = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSphere = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonCylinder = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonPlane = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonDirectional = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonPoint = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSpot = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonPlay = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonStop = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonD3D = new System.Windows.Forms.ToolStripButton();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.panelTop = new System.Windows.Forms.Panel();
            this.panelBottom = new System.Windows.Forms.Panel();
            this.panelAllView = new WeifenLuo.WinFormsUI.Docking.DockPanel();
            this.blueTheme = new WeifenLuo.WinFormsUI.Docking.VS2013BlueTheme();
            this.toolStripEx = new FXStudio.ToolStripEx(this.components);
            this.menuStripMain.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(1219, 25);
            this.menuStripMain.TabIndex = 0;
            this.menuStripMain.Text = "menuStripMain";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
            this.newToolStripMenuItem.Text = "New";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
            this.openToolStripMenuItem.Text = "Open";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(108, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(42, 21);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // toolStripMain
            // 
            this.toolStripMain.ImageScalingSize = new System.Drawing.Size(40, 40);
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonNew,
            this.toolStripButtonOpen,
            this.toolStripButtonImport,
            this.toolStripSeparator1,
            this.toolStripButtonSaveAll,
            this.toolStripSeparator2,
            this.toolStripButtonUndo,
            this.toolStripButtonRedo,
            this.toolStripSeparator3,
            this.toolStripButtonEffect,
            this.toolStripButtonReBuild,
            this.toolStripButtonCompile,
            this.toolStripSeparator4,
            this.toolStripButtonTeapot,
            this.toolStripButtonCube,
            this.toolStripButtonSphere,
            this.toolStripButtonCylinder,
            this.toolStripButtonPlane,
            this.toolStripSeparator5,
            this.toolStripButtonDirectional,
            this.toolStripButtonPoint,
            this.toolStripButtonSpot,
            this.toolStripSeparator6,
            this.toolStripButtonPlay,
            this.toolStripButtonStop,
            this.toolStripSeparator7,
            this.toolStripButtonD3D});
            this.toolStripMain.Location = new System.Drawing.Point(0, 25);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(1219, 64);
            this.toolStripMain.TabIndex = 1;
            this.toolStripMain.Text = "toolStripMain";
            // 
            // toolStripButtonNew
            // 
            this.toolStripButtonNew.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonNew.Image")));
            this.toolStripButtonNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonNew.Margin = new System.Windows.Forms.Padding(0, 1, 6, 2);
            this.toolStripButtonNew.Name = "toolStripButtonNew";
            this.toolStripButtonNew.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonNew.Text = "New";
            this.toolStripButtonNew.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonNew.Click += new System.EventHandler(this.toolStripButtonNew_Click);
            // 
            // toolStripButtonOpen
            // 
            this.toolStripButtonOpen.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonOpen.Image")));
            this.toolStripButtonOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonOpen.Name = "toolStripButtonOpen";
            this.toolStripButtonOpen.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonOpen.Text = "Open";
            this.toolStripButtonOpen.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonOpen.Click += new System.EventHandler(this.toolStripButtonOpen_Click);
            // 
            // toolStripButtonImport
            // 
            this.toolStripButtonImport.Enabled = false;
            this.toolStripButtonImport.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonImport.Image")));
            this.toolStripButtonImport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonImport.Name = "toolStripButtonImport";
            this.toolStripButtonImport.Size = new System.Drawing.Size(52, 61);
            this.toolStripButtonImport.Text = "Import";
            this.toolStripButtonImport.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 64);
            // 
            // toolStripButtonSaveAll
            // 
            this.toolStripButtonSaveAll.Enabled = false;
            this.toolStripButtonSaveAll.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonSaveAll.Image")));
            this.toolStripButtonSaveAll.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSaveAll.Name = "toolStripButtonSaveAll";
            this.toolStripButtonSaveAll.Size = new System.Drawing.Size(57, 61);
            this.toolStripButtonSaveAll.Text = "Save All";
            this.toolStripButtonSaveAll.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 64);
            // 
            // toolStripButtonUndo
            // 
            this.toolStripButtonUndo.Enabled = false;
            this.toolStripButtonUndo.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonUndo.Image")));
            this.toolStripButtonUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonUndo.Name = "toolStripButtonUndo";
            this.toolStripButtonUndo.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonUndo.Text = "Undo";
            this.toolStripButtonUndo.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripButtonRedo
            // 
            this.toolStripButtonRedo.Enabled = false;
            this.toolStripButtonRedo.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonRedo.Image")));
            this.toolStripButtonRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonRedo.Name = "toolStripButtonRedo";
            this.toolStripButtonRedo.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonRedo.Text = "Redo";
            this.toolStripButtonRedo.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 64);
            // 
            // toolStripButtonEffect
            // 
            this.toolStripButtonEffect.Enabled = false;
            this.toolStripButtonEffect.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonEffect.Image")));
            this.toolStripButtonEffect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonEffect.Name = "toolStripButtonEffect";
            this.toolStripButtonEffect.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonEffect.Text = "Effect";
            this.toolStripButtonEffect.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripButtonReBuild
            // 
            this.toolStripButtonReBuild.Enabled = false;
            this.toolStripButtonReBuild.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonReBuild.Image")));
            this.toolStripButtonReBuild.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonReBuild.Name = "toolStripButtonReBuild";
            this.toolStripButtonReBuild.Size = new System.Drawing.Size(56, 61);
            this.toolStripButtonReBuild.Text = "ReBuild";
            this.toolStripButtonReBuild.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripButtonCompile
            // 
            this.toolStripButtonCompile.Enabled = false;
            this.toolStripButtonCompile.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonCompile.Image")));
            this.toolStripButtonCompile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonCompile.Name = "toolStripButtonCompile";
            this.toolStripButtonCompile.Size = new System.Drawing.Size(60, 61);
            this.toolStripButtonCompile.Text = "Compile";
            this.toolStripButtonCompile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 64);
            // 
            // toolStripButtonTeapot
            // 
            this.toolStripButtonTeapot.Enabled = false;
            this.toolStripButtonTeapot.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonTeapot.Image")));
            this.toolStripButtonTeapot.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonTeapot.Name = "toolStripButtonTeapot";
            this.toolStripButtonTeapot.Size = new System.Drawing.Size(53, 61);
            this.toolStripButtonTeapot.Text = "Teapot";
            this.toolStripButtonTeapot.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonTeapot.Click += new System.EventHandler(this.toolStripButtonTeapot_Click);
            // 
            // toolStripButtonCube
            // 
            this.toolStripButtonCube.Enabled = false;
            this.toolStripButtonCube.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonCube.Image")));
            this.toolStripButtonCube.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonCube.Name = "toolStripButtonCube";
            this.toolStripButtonCube.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonCube.Text = "Cube";
            this.toolStripButtonCube.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonCube.Click += new System.EventHandler(this.toolStripButtonCube_Click);
            // 
            // toolStripButtonSphere
            // 
            this.toolStripButtonSphere.Enabled = false;
            this.toolStripButtonSphere.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonSphere.Image")));
            this.toolStripButtonSphere.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSphere.Name = "toolStripButtonSphere";
            this.toolStripButtonSphere.Size = new System.Drawing.Size(53, 61);
            this.toolStripButtonSphere.Text = "Sphere";
            this.toolStripButtonSphere.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonSphere.Click += new System.EventHandler(this.toolStripButtonSphere_Click);
            // 
            // toolStripButtonCylinder
            // 
            this.toolStripButtonCylinder.Enabled = false;
            this.toolStripButtonCylinder.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonCylinder.Image")));
            this.toolStripButtonCylinder.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonCylinder.Name = "toolStripButtonCylinder";
            this.toolStripButtonCylinder.Size = new System.Drawing.Size(59, 61);
            this.toolStripButtonCylinder.Text = "Cylinder";
            this.toolStripButtonCylinder.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonCylinder.Click += new System.EventHandler(this.toolStripButtonCylinder_Click);
            // 
            // toolStripButtonPlane
            // 
            this.toolStripButtonPlane.Enabled = false;
            this.toolStripButtonPlane.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonPlane.Image")));
            this.toolStripButtonPlane.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonPlane.Name = "toolStripButtonPlane";
            this.toolStripButtonPlane.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonPlane.Text = "Plane";
            this.toolStripButtonPlane.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.toolStripButtonPlane.Click += new System.EventHandler(this.toolStripButtonPlane_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 64);
            // 
            // toolStripButtonDirectional
            // 
            this.toolStripButtonDirectional.Enabled = false;
            this.toolStripButtonDirectional.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonDirectional.Image")));
            this.toolStripButtonDirectional.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonDirectional.Name = "toolStripButtonDirectional";
            this.toolStripButtonDirectional.Size = new System.Drawing.Size(74, 61);
            this.toolStripButtonDirectional.Text = "Directional";
            this.toolStripButtonDirectional.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripButtonPoint
            // 
            this.toolStripButtonPoint.Enabled = false;
            this.toolStripButtonPoint.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonPoint.Image")));
            this.toolStripButtonPoint.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonPoint.Name = "toolStripButtonPoint";
            this.toolStripButtonPoint.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonPoint.Text = "Point";
            this.toolStripButtonPoint.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripButtonSpot
            // 
            this.toolStripButtonSpot.Enabled = false;
            this.toolStripButtonSpot.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonSpot.Image")));
            this.toolStripButtonSpot.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSpot.Name = "toolStripButtonSpot";
            this.toolStripButtonSpot.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonSpot.Text = "Spot";
            this.toolStripButtonSpot.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 64);
            // 
            // toolStripButtonPlay
            // 
            this.toolStripButtonPlay.Enabled = false;
            this.toolStripButtonPlay.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonPlay.Image")));
            this.toolStripButtonPlay.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonPlay.Name = "toolStripButtonPlay";
            this.toolStripButtonPlay.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonPlay.Text = "Play";
            this.toolStripButtonPlay.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripButtonStop
            // 
            this.toolStripButtonStop.Enabled = false;
            this.toolStripButtonStop.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonStop.Image")));
            this.toolStripButtonStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonStop.Name = "toolStripButtonStop";
            this.toolStripButtonStop.Size = new System.Drawing.Size(44, 61);
            this.toolStripButtonStop.Text = "Stop";
            this.toolStripButtonStop.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 64);
            // 
            // toolStripButtonD3D
            // 
            this.toolStripButtonD3D.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonD3D.Image")));
            this.toolStripButtonD3D.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonD3D.Name = "toolStripButtonD3D";
            this.toolStripButtonD3D.Size = new System.Drawing.Size(76, 61);
            this.toolStripButtonD3D.Text = "Direct3D11";
            this.toolStripButtonD3D.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            // 
            // statusStripMain
            // 
            this.statusStripMain.Location = new System.Drawing.Point(0, 753);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(1219, 22);
            this.statusStripMain.TabIndex = 2;
            this.statusStripMain.Text = "statusStripMain";
            // 
            // panelTop
            // 
            this.panelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTop.Location = new System.Drawing.Point(0, 89);
            this.panelTop.Name = "panelTop";
            this.panelTop.Size = new System.Drawing.Size(1219, 4);
            this.panelTop.TabIndex = 3;
            // 
            // panelBottom
            // 
            this.panelBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelBottom.Location = new System.Drawing.Point(0, 749);
            this.panelBottom.Name = "panelBottom";
            this.panelBottom.Size = new System.Drawing.Size(1219, 4);
            this.panelBottom.TabIndex = 4;
            // 
            // panelAllView
            // 
            this.panelAllView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelAllView.DockBottomPortion = 150D;
            this.panelAllView.DockLeftPortion = 300D;
            this.panelAllView.DockRightPortion = 300D;
            this.panelAllView.DockTopPortion = 150D;
            this.panelAllView.DocumentStyle = WeifenLuo.WinFormsUI.Docking.DocumentStyle.DockingWindow;
            this.panelAllView.Location = new System.Drawing.Point(0, 93);
            this.panelAllView.Name = "panelAllView";
            this.panelAllView.RightToLeftLayout = true;
            this.panelAllView.Size = new System.Drawing.Size(1219, 656);
            this.panelAllView.TabIndex = 5;
            this.panelAllView.Theme = this.blueTheme;
            // 
            // toolStripEx
            // 
            this.toolStripEx.DefaultRenderer = null;
            // 
            // FXStudioForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1219, 775);
            this.Controls.Add(this.panelAllView);
            this.Controls.Add(this.panelBottom);
            this.Controls.Add(this.panelTop);
            this.Controls.Add(this.statusStripMain);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.menuStripMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "FXStudioForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FX Studio 1.0";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FXStudioForm_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FXStudioForm_FormClosed);
            this.Load += new System.EventHandler(this.FXStudioForm_Load);
            this.Shown += new System.EventHandler(this.FXStudioForm_Shown);
            this.Resize += new System.EventHandler(this.FXStudioForm_Resize);
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.Panel panelTop;
        private System.Windows.Forms.Panel panelBottom;
        private WeifenLuo.WinFormsUI.Docking.DockPanel panelAllView;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButtonNew;
        private System.Windows.Forms.ToolStripButton toolStripButtonOpen;
        private System.Windows.Forms.ToolStripButton toolStripButtonImport;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButtonSaveAll;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton toolStripButtonUndo;
        private System.Windows.Forms.ToolStripButton toolStripButtonRedo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton toolStripButtonEffect;
        private System.Windows.Forms.ToolStripButton toolStripButtonReBuild;
        private System.Windows.Forms.ToolStripButton toolStripButtonCompile;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton toolStripButtonTeapot;
        private System.Windows.Forms.ToolStripButton toolStripButtonCube;
        private System.Windows.Forms.ToolStripButton toolStripButtonSphere;
        private System.Windows.Forms.ToolStripButton toolStripButtonPlane;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripButton toolStripButtonDirectional;
        private System.Windows.Forms.ToolStripButton toolStripButtonPoint;
        private System.Windows.Forms.ToolStripButton toolStripButtonSpot;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripButton toolStripButtonPlay;
        private System.Windows.Forms.ToolStripButton toolStripButtonStop;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripButton toolStripButtonD3D;
        private WeifenLuo.WinFormsUI.Docking.VS2013BlueTheme blueTheme;
        private ToolStripEx toolStripEx;
        private System.Windows.Forms.ToolStripButton toolStripButtonCylinder;
    }
}


namespace FXStudio
{
    partial class AssetsView
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
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Animations");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Effects");
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Materials");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Models");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Textures");
            this.contextMenuStripAnimation = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.playToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripEffect = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addEffectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addEffectFromFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripMaterial = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addMaterialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addMaterialFromFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addMaterialFromNewEffectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripModel = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.importToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripTexture = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addTextureFromFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.treeViewAssets = new System.Windows.Forms.TreeView();
            this.contextMenuStripAnimation.SuspendLayout();
            this.contextMenuStripEffect.SuspendLayout();
            this.contextMenuStripMaterial.SuspendLayout();
            this.contextMenuStripModel.SuspendLayout();
            this.contextMenuStripTexture.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuStripAnimation
            // 
            this.contextMenuStripAnimation.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.playToolStripMenuItem,
            this.stopToolStripMenuItem});
            this.contextMenuStripAnimation.Name = "contextMenuStripAnimation";
            this.contextMenuStripAnimation.Size = new System.Drawing.Size(99, 48);
            // 
            // playToolStripMenuItem
            // 
            this.playToolStripMenuItem.Name = "playToolStripMenuItem";
            this.playToolStripMenuItem.Size = new System.Drawing.Size(98, 22);
            this.playToolStripMenuItem.Text = "Play";
            // 
            // stopToolStripMenuItem
            // 
            this.stopToolStripMenuItem.Name = "stopToolStripMenuItem";
            this.stopToolStripMenuItem.Size = new System.Drawing.Size(98, 22);
            this.stopToolStripMenuItem.Text = "Stop";
            // 
            // contextMenuStripEffect
            // 
            this.contextMenuStripEffect.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addEffectToolStripMenuItem,
            this.addEffectFromFileToolStripMenuItem});
            this.contextMenuStripEffect.Name = "contextMenuStripEffect";
            this.contextMenuStripEffect.Size = new System.Drawing.Size(182, 48);
            // 
            // addEffectToolStripMenuItem
            // 
            this.addEffectToolStripMenuItem.Name = "addEffectToolStripMenuItem";
            this.addEffectToolStripMenuItem.Size = new System.Drawing.Size(181, 22);
            this.addEffectToolStripMenuItem.Text = "Add Effect...";
            // 
            // addEffectFromFileToolStripMenuItem
            // 
            this.addEffectFromFileToolStripMenuItem.Name = "addEffectFromFileToolStripMenuItem";
            this.addEffectFromFileToolStripMenuItem.Size = new System.Drawing.Size(181, 22);
            this.addEffectFromFileToolStripMenuItem.Text = "Add Effect From File";
            // 
            // contextMenuStripMaterial
            // 
            this.contextMenuStripMaterial.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addMaterialToolStripMenuItem,
            this.addMaterialFromFileToolStripMenuItem,
            this.addMaterialFromNewEffectToolStripMenuItem});
            this.contextMenuStripMaterial.Name = "contextMenuStripMaterial";
            this.contextMenuStripMaterial.Size = new System.Drawing.Size(243, 70);
            // 
            // addMaterialToolStripMenuItem
            // 
            this.addMaterialToolStripMenuItem.Name = "addMaterialToolStripMenuItem";
            this.addMaterialToolStripMenuItem.Size = new System.Drawing.Size(242, 22);
            this.addMaterialToolStripMenuItem.Text = "Add Material";
            // 
            // addMaterialFromFileToolStripMenuItem
            // 
            this.addMaterialFromFileToolStripMenuItem.Name = "addMaterialFromFileToolStripMenuItem";
            this.addMaterialFromFileToolStripMenuItem.Size = new System.Drawing.Size(242, 22);
            this.addMaterialFromFileToolStripMenuItem.Text = "Add Material From File";
            // 
            // addMaterialFromNewEffectToolStripMenuItem
            // 
            this.addMaterialFromNewEffectToolStripMenuItem.Name = "addMaterialFromNewEffectToolStripMenuItem";
            this.addMaterialFromNewEffectToolStripMenuItem.Size = new System.Drawing.Size(242, 22);
            this.addMaterialFromNewEffectToolStripMenuItem.Text = "Add Material From New Effect...";
            // 
            // contextMenuStripModel
            // 
            this.contextMenuStripModel.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.importToolStripMenuItem});
            this.contextMenuStripModel.Name = "contextMenuStripModel";
            this.contextMenuStripModel.Size = new System.Drawing.Size(120, 26);
            // 
            // importToolStripMenuItem
            // 
            this.importToolStripMenuItem.Name = "importToolStripMenuItem";
            this.importToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.importToolStripMenuItem.Text = "Import...";
            // 
            // contextMenuStripTexture
            // 
            this.contextMenuStripTexture.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addTextureFromFileToolStripMenuItem});
            this.contextMenuStripTexture.Name = "contextMenuStripTexture";
            this.contextMenuStripTexture.Size = new System.Drawing.Size(199, 26);
            // 
            // addTextureFromFileToolStripMenuItem
            // 
            this.addTextureFromFileToolStripMenuItem.Name = "addTextureFromFileToolStripMenuItem";
            this.addTextureFromFileToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.addTextureFromFileToolStripMenuItem.Text = "Add Texture From File...";
            // 
            // treeViewAssets
            // 
            this.treeViewAssets.AllowDrop = true;
            this.treeViewAssets.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewAssets.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.treeViewAssets.Location = new System.Drawing.Point(0, 0);
            this.treeViewAssets.Name = "treeViewAssets";
            treeNode1.ContextMenuStrip = this.contextMenuStripAnimation;
            treeNode1.Name = "Animations";
            treeNode1.Text = "Animations";
            treeNode2.ContextMenuStrip = this.contextMenuStripEffect;
            treeNode2.Name = "Effects";
            treeNode2.Text = "Effects";
            treeNode3.ContextMenuStrip = this.contextMenuStripMaterial;
            treeNode3.Name = "Materials";
            treeNode3.Text = "Materials";
            treeNode4.ContextMenuStrip = this.contextMenuStripModel;
            treeNode4.Name = "Models";
            treeNode4.Text = "Models";
            treeNode5.ContextMenuStrip = this.contextMenuStripTexture;
            treeNode5.Name = "Textures";
            treeNode5.Text = "Textures";
            this.treeViewAssets.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3,
            treeNode4,
            treeNode5});
            this.treeViewAssets.Size = new System.Drawing.Size(355, 765);
            this.treeViewAssets.TabIndex = 0;
            this.treeViewAssets.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.treeViewAssets_ItemDrag);
            this.treeViewAssets.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewAssets_AfterSelect);
            this.treeViewAssets.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeViewAssets_NodeMouseDoubleClick);
            this.treeViewAssets.VisibleChanged += new System.EventHandler(this.treeViewAssets_VisibleChanged);
            this.treeViewAssets.DragDrop += new System.Windows.Forms.DragEventHandler(this.treeViewAssets_DragDrop);
            this.treeViewAssets.DragEnter += new System.Windows.Forms.DragEventHandler(this.treeViewAssets_DragEnter);
            this.treeViewAssets.DragOver += new System.Windows.Forms.DragEventHandler(this.treeViewAssets_DragOver);
            this.treeViewAssets.MouseDown += new System.Windows.Forms.MouseEventHandler(this.treeViewAssets_MouseDown);
            this.treeViewAssets.MouseUp += new System.Windows.Forms.MouseEventHandler(this.treeViewAssets_MouseUp);
            // 
            // AssetsView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(355, 765);
            this.Controls.Add(this.treeViewAssets);
            this.Name = "AssetsView";
            this.TabText = "Assets";
            this.Text = "AssetsView";
            this.contextMenuStripAnimation.ResumeLayout(false);
            this.contextMenuStripEffect.ResumeLayout(false);
            this.contextMenuStripMaterial.ResumeLayout(false);
            this.contextMenuStripModel.ResumeLayout(false);
            this.contextMenuStripTexture.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeViewAssets;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripAnimation;
        private System.Windows.Forms.ToolStripMenuItem playToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem stopToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripEffect;
        private System.Windows.Forms.ToolStripMenuItem addEffectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addEffectFromFileToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripMaterial;
        private System.Windows.Forms.ToolStripMenuItem addMaterialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addMaterialFromFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addMaterialFromNewEffectToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripModel;
        private System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripTexture;
        private System.Windows.Forms.ToolStripMenuItem addTextureFromFileToolStripMenuItem;
    }
}
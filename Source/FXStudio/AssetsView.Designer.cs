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
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Animations");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Animation Clips");
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Cameras");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Effects");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Geometries");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("Lights");
            System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("Materials");
            System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("Models");
            System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("Particle Emitters");
            System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("Scene");
            System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode("Deformers");
            System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode("Textures");
            this.treeViewAssets = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // treeViewAssets
            // 
            this.treeViewAssets.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewAssets.Location = new System.Drawing.Point(0, 0);
            this.treeViewAssets.Name = "treeViewAssets";
            treeNode1.Name = "Animations";
            treeNode1.Text = "Animations";
            treeNode2.Name = "Animation Clips";
            treeNode2.Text = "Animation Clips";
            treeNode3.Name = "Cameras";
            treeNode3.Text = "Cameras";
            treeNode4.Name = "Effects";
            treeNode4.Text = "Effects";
            treeNode5.Name = "Geometries";
            treeNode5.Text = "Geometries";
            treeNode6.Name = "Lights";
            treeNode6.Text = "Lights";
            treeNode7.Name = "Materials";
            treeNode7.Text = "Materials";
            treeNode8.Name = "Models";
            treeNode8.Text = "Models";
            treeNode9.Name = "Particle Emitters";
            treeNode9.Text = "Particle Emitters";
            treeNode10.Name = "Scene";
            treeNode10.Text = "Scene";
            treeNode11.Name = "Deformers";
            treeNode11.Text = "Deformers";
            treeNode12.Name = "Textures";
            treeNode12.Text = "Textures";
            this.treeViewAssets.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3,
            treeNode4,
            treeNode5,
            treeNode6,
            treeNode7,
            treeNode8,
            treeNode9,
            treeNode10,
            treeNode11,
            treeNode12});
            this.treeViewAssets.Size = new System.Drawing.Size(355, 706);
            this.treeViewAssets.TabIndex = 0;
            // 
            // AssetsView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(355, 706);
            this.Controls.Add(this.treeViewAssets);
            this.Name = "AssetsView";
            this.TabText = "Assets";
            this.Text = "AssetsView";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeViewAssets;
    }
}
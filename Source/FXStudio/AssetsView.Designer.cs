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
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Effects");
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Materials");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Models");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Textures");
            this.treeViewAssets = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // treeViewAssets
            // 
            this.treeViewAssets.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewAssets.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.treeViewAssets.Location = new System.Drawing.Point(0, 0);
            this.treeViewAssets.Name = "treeViewAssets";
            treeNode1.Name = "Animations";
            treeNode1.Text = "Animations";
            treeNode2.Name = "Effects";
            treeNode2.Text = "Effects";
            treeNode3.Name = "Materials";
            treeNode3.Text = "Materials";
            treeNode4.Name = "Models";
            treeNode4.Text = "Models";
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
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeViewAssets;
    }
}
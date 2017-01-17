using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace Inspector
{

    public partial class PropertiesInspector : UserControl
    {

        #region Private internal var./properties

        // Properties
        private int mFirstColumnWidth = 100;
        private int mItemHeight = 24;
        private BaseItem mItemSelected = null;
        private BaseItem PreviousItemSelected = null;
        private Color mTextForeColor = Color.White;
        private CategoryCollection m_CategoryList = null;
        private ItemCollection m_ItemList = null;
        private bool resizeCursorOnScreen = false;
        private bool controlChangeEventLocked = true;

        #endregion

        #region Control constructor/destructor

        public PropertiesInspector()
        {
            m_CategoryList = new CategoryCollection();
            m_ItemList = new ItemCollection();

            InitializeComponent();
        }

        ~PropertiesInspector()
        {
            ItemsClear();
            m_CategoryList = null;
            m_ItemList = null;
        }

        #endregion

        #region Public properties

        public BaseItem this[string itemKey]
        {
            get
            {
                if (m_ItemList.Contains(itemKey))
                    return m_ItemList[itemKey];
                else
                    return null;
            }
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Visible),
        Browsable(true), EditorBrowsable(EditorBrowsableState.Always),
        Bindable(false)]
        public override string Text
        {
            get
            {
                return base.Text;
            }
            set
            {
                base.Text = value;
            }
        }

        public int ColumnWidth
        {
            get
            {
                return mFirstColumnWidth;
            }
            set
            {
                mFirstColumnWidth = value;
                if (mFirstColumnWidth < 40) mFirstColumnWidth = 40;
                if (mFirstColumnWidth > this.Width) mFirstColumnWidth = this.Width;
            }
        }

        public int RowsHeight
        {
            get
            {
                return mItemHeight;
            }
            set
            {
                if (value < 19)
                    mItemHeight = 19;
                else
                    mItemHeight = (value < 64) ? mItemHeight = value :
                                                 mItemHeight = 64;
            }
        }

        public BaseItem SelectedItem
        {
            get
            {
                return mItemSelected;
            }
            set
            {
                mItemSelected = value;
            }
        }


        public int ItemsCount
        {
            get
            {
                return m_ItemList.Count;
            }
        }

        public bool ItemExist(string itemKey)
        {
            // Add BaseItem to category list and BaseItem list
            return m_ItemList.Contains(itemKey);
        }


        public int CategoryCount
        {
            get
            {
                return m_CategoryList.Count;
            }
        }

        #endregion

        #region Private internal functions

        private string NowTimeString()
        {
            return System.DateTime.Now.ToLongTimeString() + ":" + DateTime.Now.Millisecond.ToString("000") + ") ";
        }

        private void PaintItem(BaseItem item, int x1, int x2, ref int y1, ref int y2)
        {
            System.Drawing.Font propFont = null;
            if (!item.Visible) return;

            item.Rect = new Rectangle(mFirstColumnWidth, y1, this.Width - mFirstColumnWidth, y2 - y1);
            //             if (item == mItemSelected)
            //                 g.FillRectangle(brush, x1 + 14, y1, mFirstColumnWidth - 14, mItemHeight);
            //             g.DrawLine(pen, x1, y2, x2, y2);
            // Property title (text)

            propFont = item.Changed ? new Font(this.Font, FontStyle.Bold) :
                                      new Font(this.Font, FontStyle.Regular);
            //             {
            //                 if (item.Enabled)
            //                     g.DrawString(item.ValueString, propFont, txtBrush, (float)mFirstColumnWidth, (float)y1 + 2);
            //                 else
            //                     g.DrawString(item.ValueString, propFont, txtDisabledBrush, (float)mFirstColumnWidth, (float)y1 + 2);
            //             }

            y1 = y2;
            y2 += mItemHeight;
        }

        private void PaintAsCategory(int x1, int x2, int y1, ref int y2)
        {
            CategoryItem category = null;
            BaseItem item = null;
            int k, j;

            for (k = 0; k < m_CategoryList.Count; k++)
            {
                category = m_CategoryList[k];

                Button btnExpand = new Button();
                btnExpand.Image = this.imageListExpand.Images[0];
                btnExpand.ImageAlign = ContentAlignment.MiddleCenter;
                btnExpand.Name = "button" + category.Name + "Expand";
                btnExpand.Text = "";
                btnExpand.Location = new Point(2, y1 + 2);
                btnExpand.Size = new Size(20, 20);
                if (category.Expanded)
                    btnExpand.Image = this.imageListExpand.Images[1];
                else
                    btnExpand.Image = this.imageListExpand.Images[0];

                Label labelCategory = new Label();
                labelCategory.AutoSize = true;
                labelCategory.Font = new Font("Microsoft YaHei UI", 9F, FontStyle.Regular, GraphicsUnit.Point, ((byte)(134)));
                labelCategory.ForeColor = Color.Black;
                labelCategory.Location = new Point(24, 3);
                labelCategory.Name = "label" + category.Name;
                labelCategory.AutoSize = true;
                labelCategory.Text = category.Name;

                Panel panelCategory = new Panel();
                panelCategory.BackColor = Color.FromArgb(240, 240, 240);
                panelCategory.Dock = DockStyle.Top;
                panelCategory.Location = new Point(0, 0);
                panelCategory.Name = "panel" + category.Name;
                panelCategory.Size = new Size(this.Width, mItemHeight);
                panelCategory.Controls.Add(btnExpand);
                panelCategory.Controls.Add(labelCategory);

                this.panelProperties.Controls.Add(panelCategory);

                category.Rect = new Rectangle(2, y1 + 2, 13, 13);

                y1 = y2;
                y2 += mItemHeight;

                if (category.Expanded)
                {
                    for (j = 0; j < category.ItemList.Count; j++)
                    {
                        item = category.ItemList[j];
                        PaintItem(item, x1, x2, ref y1, ref y2);
                    }
                }
            }
        }

        private void PaintControl()
        {
            int x1, x2, y1, y2;

            x1 = 0;
            x2 = this.Width;
            if (x2 < 32) x2 = 32;
            y2 = mItemHeight * (m_ItemList.Count + m_CategoryList.Count + 1);
            if (y2 <= 0) y2 = this.Height;
            if (y2 < 32) y2 = 32;

            y1 = 0;
            y2 = y1 + mItemHeight;

            PaintAsCategory(x1, x2, y1, ref y2);

            y2 -= mItemHeight;
            if (y2 < 32) y2 = 32;
        }

        private void RefreshCursor(int xMouse)
        {
            int x1;
            int x2;

            x1 = xMouse - 3;
            x2 = xMouse + 3;
            resizeCursorOnScreen = (mFirstColumnWidth >= x1 && mFirstColumnWidth <= x2);
            if (resizeCursorOnScreen)
            {
                if (this.Cursor != Cursors.VSplit)
                {
                    PaintControl();
                    this.Cursor = Cursors.VSplit;
                }
            }
            else
                this.Cursor = Cursors.Default;
        }

        private void VerifyMouseUpOnCategory(MouseEventArgs e)
        {
            //             Rectangle rect;
            //             CategoryItem category;
            //             BaseItem item;
            //             int k, j;
            //             int itemWidth;
            // 
            //             for (k = 0; k < m_CategoryList.Count; k++)
            //             {
            //                 category = m_CategoryList[k];
            //                 rect = category.Rect;
            //                 // - Category
            //                 if (e.X >= rect.X && e.X <= (rect.X + rect.Width) && e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
            //                 {
            //                     // Expand/Collapse category
            //                     category.Expanded = !category.Expanded;
            //                     PaintControl();
            //                     return;     // Exit
            //                 }
            //                 // - Property BaseItem (search for active selected item)
            //                 if (category.Expanded)
            //                     for (j = 0; j < category.ItemList.Count; j++)
            //                     {
            //                         item = category.ItemList[j];
            //                         rect = item.Rect;
            //                         if (e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
            //                         {
            //                             PreviousItemSelected = mItemSelected;
            //                             mItemSelected = item;
            //                             RefreshHelp(item);
            //                             PaintControl();
            //                             // Show input box (if BaseItem is enabled)
            //                             if (pnlProp.VerticalScroll.Visible)
            //                                 itemWidth = rect.Width - 20;
            //                             else
            //                                 itemWidth = rect.Width;
            //                             if (itemWidth < 8) itemWidth = 8;
            //                             if (item.Enabled)
            //                                 ShowInputControl(rect.X, rect.Y, itemWidth, mItemHeight, item);
            //                             return;     // Exit
            //                         }
            //                     }
            //             }
        }

        private void VerifyMouseUpOnItemList(MouseEventArgs e)
        {
            //             Rectangle rect;
            //             int j;
            // 
            //             for (j = 0; j < m_ItemList.Count; j++)
            //             {
            //                 rect = m_ItemList[j].Rect;
            //                 if (e.Y >= rect.Y && e.Y <= (rect.Y + rect.Height))
            //                 {
            //                     PreviousItemSelected = mItemSelected;
            //                     mItemSelected = m_ItemList[j];
            //                     RefreshHelp(m_ItemList[j]);
            //                     PaintControl();
            //                     // Show input box (if BaseItem is enabled)
            //                     if (m_ItemList[j].Enabled)
            //                         ShowInputControl(rect.X, rect.Y, rect.Width, mItemHeight, m_ItemList[j]);
            //                     return;     // Exit
            //                 }
            //             }
        }

        private void MoreButtonIsPressed()
        {
            //             DialogResult diagRet;
            //             ColorItem colItem = null;
            //             ImageItem imgItem = null;
            //             FileItem fileItem = null;
            //             StringItem StringItem = null;
            //             string oldValue = "";
            // 
            //             if (!btnMore.Visible) return;
            //             btnMore.Enabled = false;
            //             if (mItemSelected is ColorItem)
            //             {
            //                 colItem = (ColorItem)mItemSelected;
            //                 colDiag.Color = colItem.Value;
            //                 diagRet = colDiag.ShowDialog();
            //                 if (diagRet == DialogResult.OK)
            //                 {
            //                     colItem.Value = colDiag.Color;
            //                     PaintControl();
            //                 }
            //                 btnMore.Enabled = true;
            //                 return;
            //             }
            //             if (mItemSelected is ImageItem)
            //             {
            //                 imgItem = (ImageItem)mItemSelected;
            //                 fileDiag.FileName = imgItem.FileName;
            //                 fileDiag.Filter = "All pictures (*.bmp;*.png;*.jpg;*.tiff;*.gif)|*.bmp;*.png;*.jpg;*.tiff;*.gif|All files (*.*)|*.*";
            //                 fileDiag.FilterIndex = 1;
            //                 diagRet = fileDiag.ShowDialog();
            //                 if (diagRet == DialogResult.OK)
            //                 {
            //                     imgItem.FileName = fileDiag.FileName;
            //                     PaintControl();
            //                 }
            //                 btnMore.Enabled = true;
            //                 return;
            //             }
            //             if (mItemSelected is FileItem)
            //             {
            //                 fileItem = (FileItem)mItemSelected;
            //                 fileDiag.FileName = fileItem.Value;
            //                 fileDiag.Filter = fileItem.Filter;
            //                 fileDiag.FilterIndex = fileItem.FilterIndex;
            //                 diagRet = fileDiag.ShowDialog();
            //                 if (diagRet == DialogResult.OK)
            //                 {
            //                     fileItem.Value = fileDiag.FileName;
            //                     PaintControl();
            //                 }
            //                 btnMore.Enabled = true;
            //                 return;
            //             }
            //             if (mItemSelected is StringItem)
            //             {
            //                 // The more button is visible when the property
            //                 // .ShowExpandButton is true. Use this button to run 
            //                 // a user defined action like to open a dialog box.
            //                 // (The event ExpandButtonPressed will be raised; 
            //                 //  add your action into this event)
            //                 StringItem = (StringItem)mItemSelected;
            //                 oldValue = StringItem.Value;
            //                 RaiseExpandButtonPressed(StringItem);
            //                 btnMore.Enabled = true;
            //                 if (oldValue != StringItem.Value)
            //                     // Execute control refresh if the user chage a property value
            //                     RefreshControl(true);
            //                 return;
            //             }
            //             // Exit
            //             btnMore.Enabled = true;
        }

        /// <summary>
        /// Validate text
        /// </summary>
        private void txtBox_ValidateText(BaseItem item)
        {
            // Validate data!
            //             Int32Item int32Item = null;
            //             FloatItem dblItem = null;
            // 
            //             if (item == null) return;
            //             if (item is Int32Item)
            //             {
            //                 int32Item = (Int32Item)item;
            //                 int32Item.ValueString = txtBox.Text;
            //                 return;
            //             }
            //             if (item is FloatItem)
            //             {
            //                 dblItem = (FloatItem)item;
            //                 dblItem.ValueString = txtBox.Text;
            //                 if (dblItem.ValidationRangeCheck == ValidationRangeCheckType.Manual)
            //                 {
            //                     if (dblItem.Value < dblItem.Minimum || dblItem.Value > dblItem.Maximum)
            //                         System.Media.SystemSounds.Hand.Play();
            //                 }
            //                 return;
            //             }
        }

        #endregion

        #region Public methods

        public void RefreshControl(bool repaint)
        {
            PaintControl();
        }

        /// <summary>
        /// Set the Changed property of each BaseItem to false.
        /// </summary>
        public void ResetChanges()
        {
            int t;

            if (m_ItemList == null) return;
            for (t = 0; t < m_ItemList.Count; t++)
                m_ItemList[t].Changed = false;
            RefreshControl(true);
        }

        public void ItemsClear()
        {
            if (m_CategoryList != null)
            {
                for (int i = 0; i < m_CategoryList.Count; i++)
                {
                    if (m_CategoryList[i].ItemList != null)
                    {
                        m_CategoryList[i].ItemList.Clear();
                        m_CategoryList[i].ItemList = null;
                    }
                }
                m_CategoryList.Clear();
            }

            if (m_ItemList != null)
                m_ItemList.Clear();
            panelProperties.Controls.Clear();
            RefreshControl(true);
        }

        public void ItemAdd(string categoryKey, string itemKey, StringItem strItem)
        {
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, strItem);
                m_ItemList.Add(itemKey, strItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = strItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, Int32Item intItem)
        {
            // Add BaseItem to category list and BaseItem list
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, intItem);
                m_ItemList.Add(itemKey, intItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = intItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, DropDownItem cmbItem)
        {
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, cmbItem);
                m_ItemList.Add(itemKey, cmbItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = cmbItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, BooleanItem boolItem)
        {
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, boolItem);
                m_ItemList.Add(itemKey, boolItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = boolItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, FloatItem dblItem)
        {
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, dblItem);
                m_ItemList.Add(itemKey, dblItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = dblItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, ColorItem colItem)
        {
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, colItem);
                m_ItemList.Add(itemKey, colItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = colItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, ImageItem imgItem)
        {
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, imgItem);
                m_ItemList.Add(itemKey, imgItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = imgItem;
            }
        }

        public void ItemAdd(string categoryKey, string itemKey, FileItem fileItem)
        {
            if (m_CategoryList.Contains(categoryKey))
            {
                m_CategoryList[categoryKey].ItemList.Add(itemKey, fileItem);
                m_ItemList.Add(itemKey, fileItem);
                if (this.SelectedItem == null)
                    this.SelectedItem = fileItem;
            }
        }


        //TODO:Add any new BaseItem management here... (ItemAdd() function)


        public void CategoryAdd(string categoryKey, CategoryItem catItem)
        {
            m_CategoryList.Add(categoryKey, catItem);
        }

        public string GetItemValue(string itemKey)
        {
            if (m_ItemList.Contains(itemKey))
                return m_ItemList[itemKey].ValueString;
            else
                return null;
        }

        public double GetPropertyItemDoubleValue(string itemKey)
        {
            FloatItem dblItem = null;

            if (m_ItemList.Contains(itemKey) && m_ItemList[itemKey] is FloatItem)
            {
                dblItem = (FloatItem)m_ItemList[itemKey];
                return dblItem.Value;
            }
            else
                return 0.0;
        }

        public void SetItemValue(string itemKey, string value)
        {

            if (m_ItemList.Contains(itemKey) && (m_ItemList[itemKey] is StringItem ||
                m_ItemList[itemKey] is DropDownItem))
                m_ItemList[itemKey].ValueString = value;
        }

        public void SetItemValue(string itemKey, double value)
        {
            string format;

            if (m_ItemList.Contains(itemKey) && m_ItemList[itemKey] is FloatItem)
            {
                format = m_ItemList[itemKey].Format;
                m_ItemList[itemKey].ValueString = value.ToString(format);
            }
        }

        public void SetItemValue(string itemKey, int value)
        {
            if (m_ItemList.Contains(itemKey) && m_ItemList[itemKey] is Int32Item)
                m_ItemList[itemKey].ValueString = value.ToString();
        }

        public string GetItemText(string itemKey)
        {
            if (m_ItemList.Contains(itemKey))
                return m_ItemList[itemKey].Name;
            else
                return null;
        }

        public BaseItem GetItem(string itemKey)
        {
            if (m_ItemList.Contains(itemKey))
                return m_ItemList[itemKey];
            else
                return null;
        }

        public CategoryItem GetCategory(string key)
        {
            if (m_CategoryList.Contains(key))
                return m_CategoryList[key];
            else
                return null;
        }

        public void SetCategoryText(string key, string text)
        {
            if (m_CategoryList.Contains(key))
                m_CategoryList[key].Name = text;
        }

        #endregion

        #region Public events

        public delegate void ExpandButtonPressedHandle(object sender, StringItem item);

        public event ExpandButtonPressedHandle ExpandButtonPressed;

        private void RaiseExpandButtonPressed(StringItem item)
        {
            if (ExpandButtonPressed != null)
                ExpandButtonPressed(this, item);
        }

        public delegate void ApplyButtonPressedHandle(object sender, ref bool cancel);

        public event ApplyButtonPressedHandle ApplyButtonPressed;

        private void RaiseApplyButtonPressed()
        {
            bool cancel = false;

            if (ApplyButtonPressed != null)
            {
                // Raise event
                ApplyButtonPressed(this, ref cancel);
                if (!cancel)
                    ResetChanges();
            }
        }

        #endregion

        #region Private internal events generated by user interface

        private void txtBox_TextChanged(object sender, EventArgs e)
        {
            StringItem strItem = null;
            //Int32Item int32Item = null;
            //FloatItem dblItem = null;

            if (controlChangeEventLocked) return;
            if (mItemSelected == null) return;
            if (mItemSelected is StringItem)
            {
                strItem = (StringItem)mItemSelected;
                //                 strItem.Value = txtBox.Text;
                return;
            }
            /*
			if (mItemSelected is Int32Item)
			{
				int32Item = (Int32Item)mItemSelected;
				int32Item.ValueString = txtBox.Text;
				return;
			}
			if (mItemSelected is FloatItem)
			{
				dblItem = (FloatItem)mItemSelected;
				dblItem.ValueString = txtBox.Text;
				return;
			}
			*/
        }

        private void cmbBox_TextChanged(object sender, EventArgs e)
        {
            DropDownItem listItem = null;

            if (controlChangeEventLocked) return;
            if (mItemSelected == null) return;
            if (mItemSelected is DropDownItem)
            {
                listItem = (DropDownItem)mItemSelected;
                //                 listItem.Value = cmbBox.Text;
            }
        }

        private void cmbBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            DropDownItem listItem = null;

            if (controlChangeEventLocked) return;
            if (mItemSelected == null) return;
            if (mItemSelected is DropDownItem)
            {
                listItem = (DropDownItem)mItemSelected;
                //                 listItem.Value = cmbBox.Text;
            }
        }

        private void btnMore_Click(object sender, EventArgs e)
        {
            MoreButtonIsPressed();
        }

        private void txtBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            //             if (e.KeyChar == (Char)13)
            //             {
            //                 txtBox_ValidateText(mItemSelected);
            //                 HideInputControls(true);
            //             }
        }

        private void cmbBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            //             if (e.KeyChar == (Char)13)
            //                 HideInputControls();
        }

        private void picProp_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && !resizeCursorOnScreen)
            {
                VerifyMouseUpOnCategory(e);
            }
            else
            {
                controlChangeEventLocked = true;
            }
        }

        private void picProp_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && resizeCursorOnScreen)
            {
                if (mFirstColumnWidth != e.X)
                {
                    mFirstColumnWidth = e.X;
                    PaintControl();
                }
            }
            else
                RefreshCursor(e.X);
        }

        private void txtBox_DoubleClick(object sender, EventArgs e)
        {
            MoreButtonIsPressed();
        }

        private void picProp_DoubleClick(object sender, EventArgs e)
        {
            MoreButtonIsPressed();
        }

        private void txtBox_Leave(object sender, EventArgs e)
        {
            txtBox_ValidateText(PreviousItemSelected);
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            RaiseApplyButtonPressed();
        }

        #endregion

    }
}

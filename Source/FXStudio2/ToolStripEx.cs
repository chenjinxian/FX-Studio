using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FXStudio
{
    [ProvideProperty("EnableStyle", typeof(ToolStrip))]
    public partial class ToolStripEx : Component, IExtenderProvider
    {
        private readonly Dictionary<ToolStrip, ToolStripProperties> strips = new Dictionary<ToolStrip, ToolStripProperties>();

        public ToolStripEx()
        {
            InitializeComponent();
        }

        public ToolStripEx(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
        }

        public bool CanExtend(object extendee)
        {
            return extendee is ToolStrip;
        }

        public ToolStripRenderer DefaultRenderer { get; set; }

        [DefaultValue(false)]
        public void SetStyle(ToolStrip strip)
        {
            ToolStripProperties properties = null;

            if (!strips.ContainsKey(strip))
            {
                properties = new ToolStripProperties(strip);
                strips.Add(strip, properties);
            }
            else
            {
                properties = strips[strip];
            }

            strip.Renderer = DefaultRenderer;
        }

        private class ToolStripProperties
        {
            private readonly ToolStrip strip;
            private readonly Dictionary<ToolStripItem, string> menuText = new Dictionary<ToolStripItem, string>();


            public ToolStripProperties(ToolStrip toolstrip)
            {
                if (toolstrip == null) throw new ArgumentNullException("toolstrip");
                strip = toolstrip;

                if (strip is MenuStrip)
                    SaveMenuStripText();
            }

            private void SaveMenuStripText()
            {
                foreach (ToolStripItem item in strip.Items)
                    menuText.Add(item, item.Text);
            }

            public void UpdateMenuText(bool caps)
            {
                foreach (ToolStripItem item in menuText.Keys)
                {
                    var text = menuText[item];
                    item.Text = caps ? text.ToUpper() : text;
                }
            }
        }
    }
}

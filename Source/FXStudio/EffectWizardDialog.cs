using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.WizardFramework;

namespace FXStudio
{
    public partial class EffectWizardDialog : WizardForm
    {
        private EffectTypePage effectTypePage;

        public EffectWizardDialog()
        {
            InitializeComponent();

            effectTypePage = new EffectTypePage(this);

            AddPage(effectTypePage);
        }

        public override void OnCancel()
        {
            base.OnCancel();
        }

        public override void OnFinish()
        {
            base.OnFinish();
        }

        protected override void OnLoad(EventArgs e)
        {
            EnableButton(ButtonType.Finish, false);
            EnableButton(ButtonType.Previous, false);
            EnableButton(ButtonType.Next, true);
            base.OnLoad(e);
        }
    }
}

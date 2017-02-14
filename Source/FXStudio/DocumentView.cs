using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using ScintillaNET;

namespace FXStudio
{
    public partial class DocumentView : DockContent
    {
        private const int NumberMargin = 1;
        private const int FoldingMargin = 3;
        private const string HlslKeywords = @"AppendStructuredBuffer asm asm_fragment
BlendState bool break Buffer ByteAddressBuffer
case cbuffer centroid class column_major compile compile_fragment CompileShader const continue ComputeShader ConsumeStructuredBuffer
default DepthStencilState DepthStencilView discard do double DomainShader dword
else export extern
false float float1 float2 float3 float4 float1x1 float1x2 float1x3 float1x4 float2x1 float2x2 float2x3 float2x4 float3x1 float3x2 float3x3 float3x4 float4x1 float4x2 float4x3 float4x4 for fxgroup
GeometryShader groupshared
half Hullshader
if in inline inout InputPatch int interface
line lineadj linear LineStream
matrix min16float min10float min16int min12int min16uint
namespace nointerpolation noperspective NULL
out OutputPatch
packoffset pass pixelfragment PixelShader point PointStream precise
RasterizerState RenderTargetView return register row_major RWBuffer RWByteAddressBuffer RWStructuredBuffer RWTexture1D RWTexture1DArray RWTexture2D RWTexture2DArray RWTexture3D
sample sampler SamplerState SamplerComparisonState shared snorm stateblock stateblock_state static string struct switch StructuredBuffer
tbuffer technique technique10 technique11 texture Texture1D Texture1DArray Texture2D Texture2DArray Texture2DMS Texture2DMSArray Texture3D TextureCube TextureCubeArray true typedef triangle triangleadj TriangleStream
uint uniform unorm unsigned
vector vertexfragment VertexShader void volatile
while";

        public DocumentView()
        {
            InitializeComponent();

            scintillaBox.SetSelectionBackColor(true, Color.CornflowerBlue);
            InitSyntaxColoring();
            InitNumberMargin();
            InitCodeFolding();
        }

        public void SaveFile()
        {
            File.WriteAllText(this.ToolTipText, scintillaBox.Text);
        }

        private void DocumentView_Load(object sender, EventArgs e)
        {
            scintillaBox.Text = File.ReadAllText(this.ToolTipText);
            InitHotkeys();
        }

        private void InitHotkeys()
        {
//             HotKeyManager.AddHotKey(this, OpenSearch, Keys.F, true);
//             HotKeyManager.AddHotKey(this, OpenFindDialog, Keys.F, true, false, true);
//             HotKeyManager.AddHotKey(this, OpenReplaceDialog, Keys.R, true);
//             HotKeyManager.AddHotKey(this, OpenReplaceDialog, Keys.H, true);
//             HotKeyManager.AddHotKey(this, Uppercase, Keys.U, true);
//             HotKeyManager.AddHotKey(this, Lowercase, Keys.L, true);
//             HotKeyManager.AddHotKey(this, ZoomIn, Keys.Oemplus, true);
//             HotKeyManager.AddHotKey(this, ZoomOut, Keys.OemMinus, true);
//             HotKeyManager.AddHotKey(this, ZoomDefault, Keys.D0, true);
//             HotKeyManager.AddHotKey(this, CloseSearch, Keys.Escape);

            scintillaBox.ClearCmdKey(Keys.Control | Keys.F);
            scintillaBox.ClearCmdKey(Keys.Control | Keys.R);
            scintillaBox.ClearCmdKey(Keys.Control | Keys.H);
            scintillaBox.ClearCmdKey(Keys.Control | Keys.L);
            scintillaBox.ClearCmdKey(Keys.Control | Keys.U);

        }

        private void InitSyntaxColoring()
        {
            scintillaBox.StyleResetDefault();
            scintillaBox.Styles[Style.Default].Font = "Consolas";
            scintillaBox.Styles[Style.Default].Size = 10;
            scintillaBox.Styles[Style.Default].BackColor = Color.White;
            scintillaBox.Styles[Style.Default].ForeColor = Color.Black;
            scintillaBox.StyleClearAll();

            scintillaBox.Styles[Style.Cpp.Identifier].ForeColor = Color.DarkBlue;
            scintillaBox.Styles[Style.Cpp.Comment].ForeColor = Color.Green;
            scintillaBox.Styles[Style.Cpp.CommentLine].ForeColor = Color.Green;
            scintillaBox.Styles[Style.Cpp.CommentDoc].ForeColor = Color.Green;
            scintillaBox.Styles[Style.Cpp.Number].ForeColor = Color.Black;
            scintillaBox.Styles[Style.Cpp.String].ForeColor = Color.DarkRed;
            scintillaBox.Styles[Style.Cpp.Character].ForeColor = Color.DarkRed;
            scintillaBox.Styles[Style.Cpp.Preprocessor].ForeColor = Color.DarkGray;
            scintillaBox.Styles[Style.Cpp.Operator].ForeColor = Color.Black;
            scintillaBox.Styles[Style.Cpp.Word].ForeColor = Color.Blue;

            scintillaBox.Lexer = Lexer.Cpp;

            scintillaBox.SetKeywords(0, HlslKeywords);
        }

        private void InitNumberMargin()
        {

            scintillaBox.Styles[Style.LineNumber].BackColor = Color.White;
            scintillaBox.Styles[Style.LineNumber].ForeColor = Color.DarkGray;
            scintillaBox.Styles[Style.IndentGuide].ForeColor = Color.Green;
            scintillaBox.Styles[Style.IndentGuide].BackColor = Color.Green;
            scintillaBox.Styles[Style.IndentGuide].FillLine = true;

            var nums = scintillaBox.Margins[NumberMargin];
            nums.Width = 30;
            nums.Type = MarginType.Number;
            nums.Sensitive = true;
            nums.Mask = 0;
        }

        private void InitCodeFolding()
        {

            scintillaBox.SetFoldMarginColor(true, Color.White);
            scintillaBox.SetFoldMarginHighlightColor(true, Color.White);

            scintillaBox.SetProperty("fold", "1");
            scintillaBox.SetProperty("fold.compact", "1");

            scintillaBox.Margins[FoldingMargin].Type = MarginType.Symbol;
            scintillaBox.Margins[FoldingMargin].Mask = Marker.MaskFolders;
            scintillaBox.Margins[FoldingMargin].Sensitive = true;
            scintillaBox.Margins[FoldingMargin].Width = 20;

            for (int i = 25; i <= 31; i++)
            {
                scintillaBox.Markers[i].SetForeColor(Color.White);
                scintillaBox.Markers[i].SetBackColor(Color.Gray);
            }

            scintillaBox.Markers[Marker.Folder].Symbol = MarkerSymbol.BoxPlus;
            scintillaBox.Markers[Marker.FolderOpen].Symbol = MarkerSymbol.BoxMinus;
            scintillaBox.Markers[Marker.FolderEnd].Symbol = MarkerSymbol.BoxPlusConnected;
            scintillaBox.Markers[Marker.FolderMidTail].Symbol = MarkerSymbol.TCorner;
            scintillaBox.Markers[Marker.FolderOpenMid].Symbol = MarkerSymbol.BoxMinusConnected;
            scintillaBox.Markers[Marker.FolderSub].Symbol = MarkerSymbol.VLine;
            scintillaBox.Markers[Marker.FolderTail].Symbol = MarkerSymbol.LCorner;

            scintillaBox.AutomaticFold = (AutomaticFold.Show | AutomaticFold.Click | AutomaticFold.Change);
        }
    }
}

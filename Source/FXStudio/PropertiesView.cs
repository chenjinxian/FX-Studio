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
using System.Xml;

namespace FXStudio
{
    public partial class PropertiesView : ViewWindow
    {
        private MoveActorDelegate m_MoveDelegate = null;
        private ModifyMaterialDelegate m_ModifyDelegate = null;
        private XmlNode m_LastSelectedNode = null;
        private string m_ProjectLocation;

        public PropertiesView()
        {
            InitializeComponent();
            try
            {
                DevILMethods.ilInit();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }

        public string ProjectLocation { get { return m_ProjectLocation; } set { m_ProjectLocation = value; } }

        public void SetMoveActorDelegate(MoveActorDelegate moveDelegate, ModifyMaterialDelegate modifyDelegate)
        {
            m_MoveDelegate = moveDelegate;
            m_ModifyDelegate = modifyDelegate;
        }

        public void UpdateAssetProperties(XmlNode selectedNode)
        {
            bool reset = (m_LastSelectedNode != selectedNode);
            if (reset)
                inspectorComponent.ItemsClear();

            if (selectedNode != null)
            {
                string nodeName = selectedNode.Name;
                if (nodeName == "Effect")
                {
                    AddEffectProperties(selectedNode);
                }
                else if (nodeName == "Material")
                {
                    AddMaterialProperties(selectedNode);
                }
                else if (nodeName == "Model")
                {

                }
                else if (nodeName == "Texture")
                {
                    //                 Inspector.CategoryItem category = new Inspector.CategoryItem("Image Properties");
                    //                 inspectorComponent.CategoryAdd("ImageProperties", category);
                    // 
                    //                 string textureName = selectedNode.Attributes["name"].Value;
                    //                 inspectorComponent.ItemAdd(new Inspector.StringItem("ImageProperties", textureName, "", selectedNode.InnerText));
                }
            }

            inspectorComponent.UpdateControl(reset);
            m_LastSelectedNode = selectedNode;
        }

        public bool UpdateProjectProperties(XmlNode selectedNode)
        {
            bool reset = (m_LastSelectedNode != selectedNode);
            if (reset)
                inspectorComponent.ItemsClear();

            bool isActorNode = false;

            string nodeType = selectedNode.Attributes["type"].Value;
            if (nodeType == "Project")
            {
                AddProjectProperties(selectedNode);
            }
            else if (nodeType == "Scene")
            {
                AddSceneProperties(selectedNode);
            }
            else if (nodeType == "EditorCamera")
            {
                AddEditorCameraProperties(selectedNode);
            }
            else
            {
                isActorNode = true;
                foreach (XmlNode component in selectedNode.ChildNodes)
                {
                    if (component.Name == "TransformComponent")
                    {
                        AddTransformProperties(component);
                    }
                    else if (component.Name == "SkyboxRenderComponent")
                    {
                        AddSkyboxProperties(component);
                    }
                    else if (component.Name == "GridRenderComponent")
                    {
                        AddGridProperties(component);
                    }
                    else if (component.Name == "PlaneRenderComponent")
                    {
                        AddCubeProperties(component);
                    }
                    else if (component.Name == "SphereRenderComponent")
                    {
                        AddSphereProperties(component);
                    }
                    else if (component.Name == "TorusRenderComponent")
                    {
                        AddTorusProperties(component);
                    }
                    else if (component.Name == "TeapotRenderComponent")
                    {
                        AddTeapotProperties(component);
                    }
                    else if (component.Name == "ModelRenderComponent")
                    {
                        AddModelProperties(component);
                    }
                }
            }

            inspectorComponent.UpdateControl(reset);
            m_LastSelectedNode = selectedNode;
            return isActorNode;
        }

        private void AddEffectProperties(XmlNode effectNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("Effect Properties");
            inspectorComponent.CategoryAdd("EffectProperties", category);

            string effectName = effectNode.Attributes["name"].Value;
            inspectorComponent.ItemAdd(new Inspector.StringItem("EffectProperties", "Name", "", effectName));
            string effectPath = effectNode.InnerText;
            inspectorComponent.ItemAdd(new Inspector.StringItem("EffectProperties", "Full Path", "", effectNode.InnerText));
        }

        private void AddMaterialProperties(XmlNode materialNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("Material Properties");
            inspectorComponent.CategoryAdd("MaterialProperties", category);

            XmlNode variablesRoot = materialNode.SelectSingleNode("Variables");
            foreach (XmlNode child in variablesRoot)
            {
                string varName = child.Name;
                string varValue = child.InnerText;
                string uiName = varName;

                XmlAttribute nameNode = child.Attributes["uiname"];
                if (nameNode != null)
                    uiName = nameNode.Value;

                XmlAttribute widgetNode = child.Attributes["uiwidget"];
                if (widgetNode != null)
                {
                    string uiWidget = widgetNode.Value;
                    if (string.Equals(uiWidget, "None", StringComparison.OrdinalIgnoreCase))
                    {
                        // hide property
                    }
                    else if (string.Equals(uiWidget, "Color", StringComparison.OrdinalIgnoreCase))
                    {
                        AddColorItem("MaterialProperties", uiName, varName, varValue);
                    }
                    else if (string.Equals(uiWidget, "slider", StringComparison.OrdinalIgnoreCase))
                    {
                        XmlNode minNode = child.Attributes["uimin"];
                        XmlNode maxNode = child.Attributes["uimax"];
                        XmlNode stepNode = child.Attributes["uistep"];
                        if (minNode != null && maxNode != null && stepNode != null)
                        {
                            AddFloatItem("MaterialProperties", uiName, varName,
                                varValue, minNode.InnerText, maxNode.InnerText, stepNode.InnerText);
                        }
                        else
                        {
                            inspectorComponent.ItemAdd(new Inspector.StringItem("MaterialProperties", uiName, varName, varValue));
                        }
                    }
                    continue;
                }

                XmlNode objectNode = child.Attributes["bbject"];
                if (objectNode != null)
                {
                    inspectorComponent.ItemAdd(new Inspector.StringItem("MaterialProperties", uiName, varName, varValue));
                    continue;
                }

                XmlNode textureNode = child.Attributes["resourcetype"];
                if (textureNode != null)
                {
                    XmlNode valueNode = child.Attributes["resourcename"];
                    if (!string.IsNullOrEmpty(valueNode.InnerText))
                    {
                        AddTextureItem("MaterialProperties", uiName, varName,
                            m_ProjectLocation + @"\Textures\" + Path.GetFileName(valueNode.InnerText));
                    }
                    else
                    {
                        inspectorComponent.ItemAdd(new Inspector.StringItem("MaterialProperties", uiName, varName, varValue));
                    }
                    continue;
                }

                if (!string.IsNullOrEmpty(varValue))
                    inspectorComponent.ItemAdd(new Inspector.StringItem("MaterialProperties", uiName, varName, varValue));
            }
        }

        private void AddProjectProperties(XmlNode projectNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(projectNode.Name);
            inspectorComponent.CategoryAdd(projectNode.Name, category);

            XmlNode nameNode = projectNode.SelectSingleNode("Name");
            inspectorComponent.ItemAdd(new Inspector.StringItem(projectNode.Name, nameNode.Name, "", nameNode.InnerText));

            XmlNode locationNode = projectNode.SelectSingleNode("Location");
            inspectorComponent.ItemAdd(new Inspector.StringItem(projectNode.Name, locationNode.Name, "", locationNode.InnerText));
        }

        private void AddSceneProperties(XmlNode sceneNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("Scene");
            inspectorComponent.CategoryAdd("Scene", category);

            inspectorComponent.ItemAdd(new Inspector.StringItem("Scene", "Name", "", sceneNode.Name));
        }

        private void AddEditorCameraProperties(XmlNode cameraNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(cameraNode.Name);
            inspectorComponent.CategoryAdd(cameraNode.Name, category);

            AddTransformItem(cameraNode, false);
        }

        private void AddSkyboxProperties(XmlNode skyboxNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(skyboxNode.Name);
            inspectorComponent.CategoryAdd(skyboxNode.Name, category);

            AddTextureItem(skyboxNode);
        }

        private void AddGridProperties(XmlNode gridNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(gridNode.Name);
            inspectorComponent.CategoryAdd(gridNode.Name, category);

            AddTextureItem(gridNode);
        }

        private void AddTransformProperties(XmlNode transformNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(transformNode.Name);
            inspectorComponent.CategoryAdd(transformNode.Name, category);

            AddTransformItem(transformNode);
        }

        private void AddGeometryMaterial(XmlNode actorNode)
        {
            XmlNode material = actorNode.SelectSingleNode("Material");
            if (material != null)
            {
                AddMaterialItem(actorNode.Name, material.Name, material.InnerText);
            }
        }

        private void AddCubeProperties(XmlNode cubeNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(cubeNode.Name);
            inspectorComponent.CategoryAdd(cubeNode.Name, category);

            AddGeometryMaterial(cubeNode);
        }

        private void AddSphereProperties(XmlNode sphereNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(sphereNode.Name);
            inspectorComponent.CategoryAdd(sphereNode.Name, category);

            AddGeometryMaterial(sphereNode);
        }

        private void AddTorusProperties(XmlNode torusNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(torusNode.Name);
            inspectorComponent.CategoryAdd(torusNode.Name, category);

            AddGeometryMaterial(torusNode);
        }

        private void AddTeapotProperties(XmlNode teapotNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(teapotNode.Name);
            inspectorComponent.CategoryAdd(teapotNode.Name, category);

            AddGeometryMaterial(teapotNode);
        }

        private void AddModelProperties(XmlNode modelNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(modelNode.Name);
            inspectorComponent.CategoryAdd(modelNode.Name, category);

            XmlNode materials = modelNode.SelectSingleNode("Materials");
            if (materials != null)
            {
                foreach (XmlNode child in materials)
                {
                    string name = child.Attributes["name"].Value;
                    AddMaterialItem(modelNode.Name, name, child.InnerText);
                }
            }
        }

        private void AddTransformItem(XmlNode transformNode, bool enable = true)
        {
            foreach (XmlNode vector3Node in transformNode)
            {
                float x = Convert.ToSingle(vector3Node.Attributes["x"].Value);
                float y = Convert.ToSingle(vector3Node.Attributes["y"].Value);
                float z = Convert.ToSingle(vector3Node.Attributes["z"].Value);

                Inspector.Vector3Item vector3Item = new Inspector.Vector3Item(
                    transformNode.Name, vector3Node.Name, "", new Inspector.Vector3(x, y, z));
                vector3Item.Enabled = enable;
                vector3Item.ValueChanged += Vector3Item_ValueChanged;
                inspectorComponent.ItemAdd(vector3Item);
            }
        }

        private void Vector3Item_ValueChanged(object sender, Inspector.Vector3 value)
        {
            Inspector.Vector3Item vector3Item = sender as Inspector.Vector3Item;
            m_MoveDelegate?.Invoke(vector3Item.CategoryName, vector3Item.UIName, value);
        }

        private void AddTextureItem(XmlNode actorNode)
        {
            XmlNode textureNode = actorNode.SelectSingleNode("Texture");
            if (textureNode != null)
            {
                AddTextureItem(actorNode.Name, textureNode.Name, textureNode.Name, textureNode.InnerText);
            }
        }

        private void AddMaterialItem(string category, string itemName, string materialName)
        {
            Inspector.StringItem stringItem = new Inspector.StringItem(category, itemName, itemName, materialName);
            stringItem.Enabled = false;
            inspectorComponent.ItemAdd(stringItem);
        }

        private void AddColorItem(string category, string itemName, string variableName, string colorValue)
        {
            string[] elements = colorValue.Split(' ');
            float r = Convert.ToSingle(elements[0]);
            float g = Convert.ToSingle(elements[1]);
            float b = Convert.ToSingle(elements[2]);
            float a = 1.0F;
            if (elements.Count() > 3)
                a = Convert.ToSingle(elements[3]);
            Color color = Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0));

            Inspector.ColorItem colorItem = new Inspector.ColorItem(category, itemName, variableName, color);
            colorItem.ValueChanged += ColorItem_ValueChanged;
            inspectorComponent.ItemAdd(colorItem);
        }

        private void ColorItem_ValueChanged(object sender, Color value)
        {
            Inspector.ColorItem vector3Item = sender as Inspector.ColorItem;
            m_ModifyDelegate?.Invoke(vector3Item.VariableName, vector3Item.ValueString);
        }

        private void AddTextureItem(string category, string itemName, string variableName, string fileName)
        {
            Image original = DevILMethods.ImageFromFile(fileName);
            if (original != null)
                original = DevILMethods.FixedSize(original, 20, 20);
            Inspector.ImageItem imageItem = new Inspector.ImageItem(category, itemName, variableName, fileName, original);
            imageItem.ValueChanging += ImageItem_ValueChanging;
            inspectorComponent.ItemAdd(imageItem);
        }

        private void ImageItem_ValueChanging(object sender, string value)
        {
            TexturesDialog dialog = new TexturesDialog();
            dialog.FileName = value;
            dialog.TextureDirectory = m_ProjectLocation + @"\Textures";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                Inspector.ImageItem imageItem = sender as Inspector.ImageItem;
                m_ModifyDelegate?.Invoke(imageItem.VariableName, Path.GetFileName(dialog.FileName));
                imageItem.FileName = dialog.FileName;
                Image original = DevILMethods.ImageFromFile(dialog.FileName);
                if (original != null)
                    original = DevILMethods.FixedSize(original, 20, 20);
                imageItem.Value = original;
            }
        }

        private void AddFloatItem(string category, string itemName, string variableName,
            string floatValue, string minValue, string maxValue, string stepValue)
        {
            float value = Convert.ToSingle(floatValue);
            float min = Convert.ToSingle(minValue);
            float max = Convert.ToSingle(maxValue);
            float step = Convert.ToSingle(stepValue);
            Inspector.FloatItem floatItem = new Inspector.FloatItem(category, itemName, variableName, value, min, max, step);
            floatItem.ValueChanged += FloatItem_ValueChanged;
            inspectorComponent.ItemAdd(floatItem);
        }

        private void FloatItem_ValueChanged(object sender, float value)
        {
            Inspector.FloatItem floatItem = sender as Inspector.FloatItem;
            m_ModifyDelegate?.Invoke(floatItem.VariableName, floatItem.ValueString);
        }
    }
}
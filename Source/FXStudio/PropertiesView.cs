using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.Reflection;
using System.Globalization;

namespace FXStudio
{
    public partial class PropertiesView : ViewWindow
    {
        private MoveActorDelegate m_MoveDelegate = null;
        private XmlNode m_LastSelectedNode = null;

        public PropertiesView()
        {
            InitializeComponent();
        }

        public void SetMoveActorDelegate(MoveActorDelegate moveDelegate)
        {
            m_MoveDelegate = moveDelegate;
        }

        public void UpdateAssetProperties(XmlNode selectedNode)
        {
            bool reset = (m_LastSelectedNode != selectedNode);
            if (reset)
                inspectorComponent.ItemsClear();

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
                Inspector.CategoryItem category = new Inspector.CategoryItem("Image Properties");
                inspectorComponent.CategoryAdd("ImageProperties", category);

                string textureName = selectedNode.Attributes["name"].Value;
                inspectorComponent.ItemAdd(new Inspector.StringItem("ImageProperties", textureName, selectedNode.InnerText));
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
                    else if (component.Name == "CubeRenderComponent")
                    {
                        AddCubeProperties(component);
                    }
                    else if (component.Name == "SphereRenderComponent")
                    {
                        AddSphereProperties(component);
                    }
                    else if (component.Name == "CylinderRenderComponent")
                    {
                        AddCylinderProperties(component);
                    }
                    else if (component.Name == "TeapotRenderComponent")
                    {
                        AddTeapotProperties(component);
                    }
                    else if (component.Name == "ModelRenderComponent")
                    {

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
            inspectorComponent.ItemAdd(new Inspector.StringItem("EffectProperties", "Name", effectName));
            string effectPath = effectNode.InnerText;
            inspectorComponent.ItemAdd(new Inspector.StringItem("EffectProperties", "Full Path", effectNode.InnerText));
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

                XmlAttribute nameNode = child.Attributes["UIName"];
                if (nameNode != null)
                    varName = nameNode.Value;

                XmlAttribute widgetNode = child.Attributes["UIWidget"];
                if (widgetNode != null)
                {
                    string uiWidget = widgetNode.Value;
                    if (string.Equals(uiWidget, "None", StringComparison.OrdinalIgnoreCase))
                    {
                        // hide property
                    }
                    else if (string.Equals(uiWidget, "Color", StringComparison.OrdinalIgnoreCase))
                    {
                        AddColorItem("MaterialProperties", varName, varValue);
                    }
                    else if (string.Equals(uiWidget, "slider", StringComparison.OrdinalIgnoreCase))
                    {
                        XmlNode minNode = child.Attributes["UIMin"];
                        XmlNode maxNode = child.Attributes["UIMax"];
                        XmlNode stepNode = child.Attributes["UIStep"];
                        if (minNode != null && maxNode != null && stepNode != null)
                        {
                            AddFloatItem("MaterialProperties", varName,
                                varValue, minNode.InnerText, maxNode.InnerText, stepNode.InnerText);
                        }
                        else
                        {
                            inspectorComponent.ItemAdd(new Inspector.StringItem("MaterialProperties", varName, varValue));
                        }
                    }
                    continue;
                }

                XmlNode objectNode = child.Attributes["Object"];
                if (objectNode != null)
                {
                    inspectorComponent.ItemAdd(new Inspector.StringItem("MaterialProperties", varName, varValue));
                    continue;
                }

                XmlNode textureNode = child.Attributes["ResourceType"];
                if (textureNode != null)
                {
                    XmlNode valueNode = child.Attributes["ResourceName"];
                    inspectorComponent.ItemAdd(new Inspector.StringItem(
                        "MaterialProperties", varName, valueNode != null ? valueNode.InnerText : varValue));
                    continue;
                }

                if (!string.IsNullOrEmpty(varValue))
                    inspectorComponent.ItemAdd(new Inspector.StringItem("MaterialProperties", varName, varValue));
            }
        }

        private void AddProjectProperties(XmlNode projectNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(projectNode.Name);
            inspectorComponent.CategoryAdd(projectNode.Name, category);

            XmlNode nameNode = projectNode.SelectSingleNode("Name");
            inspectorComponent.ItemAdd(new Inspector.StringItem(projectNode.Name, nameNode.Name, nameNode.InnerText));

            XmlNode locationNode = projectNode.SelectSingleNode("Location");
            inspectorComponent.ItemAdd(new Inspector.StringItem(projectNode.Name, locationNode.Name, locationNode.InnerText));
        }

        private void AddSceneProperties(XmlNode sceneNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("Scene");
            inspectorComponent.CategoryAdd("Scene", category);

            inspectorComponent.ItemAdd(new Inspector.StringItem("Scene", "Name", sceneNode.Name));
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

            AddColorItem(skyboxNode);
            AddTextureItem(skyboxNode);
        }

        private void AddGridProperties(XmlNode gridNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(gridNode.Name);
            inspectorComponent.CategoryAdd(gridNode.Name, category);

            AddColorItem(gridNode);
            AddTextureItem(gridNode);
        }

        private void AddTransformProperties(XmlNode transformNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(transformNode.Name);
            inspectorComponent.CategoryAdd(transformNode.Name, category);

            AddTransformItem(transformNode);
        }

        private void AddCubeProperties(XmlNode cubeNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(cubeNode.Name);
            inspectorComponent.CategoryAdd(cubeNode.Name, category);

            AddColorItem(cubeNode);
            AddTextureItem(cubeNode);
            AddEffectItem(cubeNode);
        }

        private void AddSphereProperties(XmlNode sphereNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(sphereNode.Name);
            inspectorComponent.CategoryAdd(sphereNode.Name, category);

            AddColorItem(sphereNode);
            AddTextureItem(sphereNode);
            AddEffectItem(sphereNode);
        }

        private void AddCylinderProperties(XmlNode cylinderNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(cylinderNode.Name);
            inspectorComponent.CategoryAdd(cylinderNode.Name, category);

            AddColorItem(cylinderNode);
            AddTextureItem(cylinderNode);
            AddEffectItem(cylinderNode);
        }

        private void AddTeapotProperties(XmlNode teapotNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(teapotNode.Name);
            inspectorComponent.CategoryAdd(teapotNode.Name, category);

            AddColorItem(teapotNode);
            AddTextureItem(teapotNode);
            AddEffectItem(teapotNode);
        }

        private void AddTransformItem(XmlNode transformNode, bool enable = true)
        {
            foreach (XmlNode vector3Node in transformNode)
            {
                float x = Convert.ToSingle(vector3Node.Attributes["x"].Value);
                float y = Convert.ToSingle(vector3Node.Attributes["y"].Value);
                float z = Convert.ToSingle(vector3Node.Attributes["z"].Value);

                Inspector.Vector3Item vector3Item = new Inspector.Vector3Item(
                    transformNode.Name, vector3Node.Name, new Inspector.Vector3(x, y, z));
                vector3Item.Enabled = enable;
                vector3Item.ValueChanged += Vector3Item_ValueChanged;
                inspectorComponent.ItemAdd(vector3Item);
            }
        }

        private void Vector3Item_ValueChanged(object sender, Inspector.Vector3 value)
        {
            Inspector.Vector3Item vector3Item = sender as Inspector.Vector3Item;
            m_MoveDelegate?.Invoke(vector3Item.CategoryName, vector3Item.ItemName, value);
        }

        private void AddColorItem(XmlNode actorNode)
        {
            XmlNode colorNode = actorNode.SelectSingleNode("Color");
            if (colorNode != null)
            {
                float r = Convert.ToSingle(colorNode.Attributes["r"].Value);
                float g = Convert.ToSingle(colorNode.Attributes["g"].Value);
                float b = Convert.ToSingle(colorNode.Attributes["b"].Value);
                float a = Convert.ToSingle(colorNode.Attributes["a"].Value);
                inspectorComponent.ItemAdd(new Inspector.ColorItem(actorNode.Name, colorNode.Name,
                    Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0))));
            }
        }

        private void AddTextureItem(XmlNode actorNode)
        {
            XmlNode textureNode = actorNode.SelectSingleNode("Texture");
            if (textureNode != null)
            {
                inspectorComponent.ItemAdd(new Inspector.ImageItem(actorNode.Name, textureNode.Name, textureNode.InnerText));
            }
        }

        private void AddEffectItem(XmlNode actorNode)
        {
            XmlNode effectNode = actorNode.SelectSingleNode("Effect");
            if (effectNode != null)
            {
                inspectorComponent.ItemAdd(new Inspector.ImageItem(actorNode.Name, effectNode.Name, effectNode.InnerText));
            }
        }

        private void AddColorItem(string category, string itemName, string colorValue)
        {
            string[] elements = colorValue.Split(' ');
            float r = Convert.ToSingle(elements[0]);
            float g = Convert.ToSingle(elements[1]);
            float b = Convert.ToSingle(elements[2]);
            float a = 1.0F;
            if (elements.Count() > 3)
                a = Convert.ToSingle(elements[3]);
            Color color = Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0));

            Inspector.ColorItem colorItem = new Inspector.ColorItem(category, itemName, color);
            inspectorComponent.ItemAdd(colorItem);
        }

        private void AddFloatItem(string category, string itemName,
            string floatValue, string minValue, string maxValue, string stepValue)
        {
            float value = Convert.ToSingle(floatValue);
            float min = Convert.ToSingle(minValue);
            float max = Convert.ToSingle(maxValue);
            float step = Convert.ToSingle(stepValue);
            Inspector.FloatItem floatItem = new Inspector.FloatItem(category, itemName, value, min, max, step);
            inspectorComponent.ItemAdd(floatItem);
        }
    }
}
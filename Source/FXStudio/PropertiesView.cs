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
        public PropertiesView()
        {
            InitializeComponent();
        }

        public void UpdateAssetProperties(XmlNode selectedNode)
        {
            inspectorComponent.ItemsClear();

            string nodeName = selectedNode.Name;
            if (nodeName == "Effect")
            {

            }
            else if (nodeName == "Model")
            {

            }
            else if (nodeName == "Texture")
            {
                Inspector.CategoryItem category = new Inspector.CategoryItem("Image Properties");
                inspectorComponent.CategoryAdd("ImageProperties", category);

                string textureName = selectedNode.Attributes["name"].Value;
                Inspector.StringItem nameItem = new Inspector.StringItem(textureName, selectedNode.InnerText);
                inspectorComponent.ItemAdd("ImageProperties", textureName, nameItem);
            }

            inspectorComponent.UpdateControl();
        }

        public bool UpdateProjectProperties(XmlNode selectedNode)
        {
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

            inspectorComponent.UpdateControl();
            return isActorNode;
        }

        private void AddProjectProperties(XmlNode projectNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(projectNode.Name);
            inspectorComponent.CategoryAdd(projectNode.Name, category);

            XmlNode nameNode = projectNode.SelectSingleNode("Name");
            Inspector.StringItem nameItem = new Inspector.StringItem(nameNode.Name, nameNode.InnerText);
            inspectorComponent.ItemAdd(projectNode.Name, nameNode.Name, nameItem);

            XmlNode locationNode = projectNode.SelectSingleNode("Location");
            Inspector.StringItem locationItem = new Inspector.StringItem(locationNode.Name, locationNode.InnerText);
            inspectorComponent.ItemAdd(projectNode.Name, locationNode.Name, locationItem);
        }

        private void AddSceneProperties(XmlNode sceneNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("Scene");
            inspectorComponent.CategoryAdd("Scene", category);

            Inspector.StringItem nameItem = new Inspector.StringItem("Name", sceneNode.Name);
            inspectorComponent.ItemAdd("Scene", sceneNode.Name, nameItem);
        }

        private void AddEditorCameraProperties(XmlNode cameraNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(cameraNode.Name);
            inspectorComponent.CategoryAdd(cameraNode.Name, category);

            AddTransformItem(cameraNode);
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

        private void AddTransformItem(XmlNode transformNode)
        {
            foreach (XmlNode vector3Node in transformNode)
            {
                float x = Convert.ToSingle(vector3Node.Attributes["x"].Value);
                float y = Convert.ToSingle(vector3Node.Attributes["y"].Value);
                float z = Convert.ToSingle(vector3Node.Attributes["z"].Value);

                inspectorComponent.ItemAdd(transformNode.Name, vector3Node.Name,
                    new Inspector.Vector3Item(vector3Node.Name, new Inspector.Vector3(x, y, z)));
            }
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
                inspectorComponent.ItemAdd(actorNode.Name, colorNode.Name, new Inspector.ColorItem(colorNode.Name,
                    Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0))));
            }
        }

        private void AddTextureItem(XmlNode actorNode)
        {
            XmlNode textureNode = actorNode.SelectSingleNode("Texture");
            if (textureNode != null)
            {
                Inspector.ImageItem textureItem = new Inspector.ImageItem(textureNode.Name, textureNode.InnerText);
                inspectorComponent.ItemAdd(actorNode.Name, textureNode.Name, textureItem);
            }
        }

        private void AddEffectItem(XmlNode actorNode)
        {
            XmlNode effectNode = actorNode.SelectSingleNode("Effect");
            if (effectNode != null)
            {
                Inspector.ImageItem effectItem = new Inspector.ImageItem(effectNode.Name, effectNode.InnerText);
                inspectorComponent.ItemAdd(actorNode.Name, effectNode.Name, effectItem);
            }
        }
    }
}
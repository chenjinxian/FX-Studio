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
        int m_SelectedActorId;

        public PropertiesView()
        {
            InitializeComponent();
        }

        private void AddProjectProperties(XmlNode projectNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(projectNode.Name);
            inspectorComponent.CategoryAdd("project", category);

            XmlNode nameNode = projectNode.SelectSingleNode("Name");
            Inspector.StringItem nameItem = new Inspector.StringItem(nameNode.Name, nameNode.InnerText);
            inspectorComponent.ItemAdd("project", nameNode.Name, nameItem);

            XmlNode locationNode = projectNode.SelectSingleNode("Location");
            Inspector.StringItem locationItem = new Inspector.StringItem(locationNode.Name, locationNode.InnerText);
            inspectorComponent.ItemAdd("project", locationNode.Name, locationItem);
        }

        private void AddSceneProperties(XmlNode sceneNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("Scene");
            inspectorComponent.CategoryAdd("scene", category);

            Inspector.StringItem nameItem = new Inspector.StringItem("Name", sceneNode.Name);
            inspectorComponent.ItemAdd("scene", sceneNode.Name, nameItem);
        }

        private void AddEditorCameraProperties(XmlNode cameraNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(cameraNode.Name);
            inspectorComponent.CategoryAdd("camera", category);

            XmlNode translation = cameraNode.SelectSingleNode("Translation");
            if (translation != null)
                inspectorComponent.ItemAdd("camera", translation.Name, CreateVector3Item(translation));

            XmlNode rotation = cameraNode.SelectSingleNode("Rotation");
            if (rotation != null)
                inspectorComponent.ItemAdd("camera", rotation.Name, CreateVector3Item(rotation));
        }

        private void AddTransformProperties()
        {

        }

        private void AddSkyboxProperties(XmlNode skyboxNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(skyboxNode.Name);
            inspectorComponent.CategoryAdd("skybox", category);

            XmlNode colorNode = skyboxNode.SelectSingleNode("Color");
            if (colorNode != null)
                inspectorComponent.ItemAdd("skybox", colorNode.Name, CreateColorItem(colorNode));

            XmlNode textureNode = skyboxNode.SelectSingleNode("Texture");
            Inspector.ImageItem textureItem = new Inspector.ImageItem(textureNode.Name, textureNode.InnerText);
            inspectorComponent.ItemAdd("skybox", textureNode.Name, textureItem);
        }

        private void AddGridProperties(XmlNode gridNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem(gridNode.Name);
            inspectorComponent.CategoryAdd("grid", category);

            XmlNode colorNode = gridNode.SelectSingleNode("Color");
            if (colorNode != null)
                inspectorComponent.ItemAdd("grid", colorNode.Name, CreateColorItem(colorNode));

            XmlNode textureNode = gridNode.SelectSingleNode("Texture");
            Inspector.ImageItem textureItem = new Inspector.ImageItem(textureNode.Name, textureNode.InnerText);
            inspectorComponent.ItemAdd("grid", textureNode.Name, textureItem);
        }

        private Inspector.Vector3Item CreateVector3Item(XmlNode vector3Node)
        {
            float x = Convert.ToSingle(vector3Node.Attributes["x"].Value);
            float y = Convert.ToSingle(vector3Node.Attributes["y"].Value);
            float z = Convert.ToSingle(vector3Node.Attributes["z"].Value);
            return new Inspector.Vector3Item(vector3Node.Name, new Inspector.Vector3(x, y, z));
        }

        private Inspector.ColorItem CreateColorItem(XmlNode colorNode)
        {
            float r = Convert.ToSingle(colorNode.Attributes["r"].Value);
            float g = Convert.ToSingle(colorNode.Attributes["g"].Value);
            float b = Convert.ToSingle(colorNode.Attributes["b"].Value);
            float a = Convert.ToSingle(colorNode.Attributes["a"].Value);

            return new Inspector.ColorItem(colorNode.Name,
                Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0)));
        }

        public void UpdateProperties(XmlNode selectedNode)
        {
            inspectorComponent.ItemsClear();

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
                foreach (XmlNode component in selectedNode.ChildNodes)
                {
                    if (component.Name == "TransformComponent")
                    {
                        AddTransformProperties();
                    }
                    else if (component.Name == "SkyboxRenderComponent")
                    {
                        AddSkyboxProperties(component);
                    }
                    else if (component.Name == "GridRenderComponent")
                    {
                        AddGridProperties(component);
                    }
                }
            }

            inspectorComponent.UpdateControl();
        }

        private void AddCompoentProperties(XmlNode actorComponentValues, XmlNode editorComponentValues)
        {
            //             string componentName = actorComponentValues.Name.ToString();
            //             string componentXpath = XmlUtility.GetXPathToNode(actorComponentValues);
            //             try
            //             {
            //                 int elementNum = 0;
            //                 foreach (XmlNode inputField in editorComponentValues)
            //                 {
            //                     string xpath = XmlUtility.GetXPathToNode(inputField);
            //                     string elementName = inputField.Attributes["name"].Value;
            //                     string elementType = inputField.Attributes["type"].Value;
            // 
            //                     XmlNode actorValues = actorComponentValues.ChildNodes[elementNum];
            // 
            //                     if (elementType == typeof(int).ToString())
            //                     {
            //                         int x = Convert.ToInt32(actorValues.FirstChild.Value);
            // 
            //                         m_PropertyInstance.Properties.Add(
            //                             new PropertyElement(elementName, typeof(int),
            //                             x, false, new CategoryAttribute(componentName)));
            //                     }
            //                     else if (elementType == typeof(float).ToString())
            //                     {
            //                         float x = Convert.ToSingle(actorValues.FirstChild.Value);
            // 
            //                         m_PropertyInstance.Properties.Add(
            //                             new PropertyElement(elementName, typeof(float),
            //                             x, false, new CategoryAttribute(componentName)));
            //                     }
            //                     else if (elementType == typeof(Size).ToString())
            //                     {
            //                         int x = Convert.ToInt32(actorValues.Attributes["x"].Value);
            //                         int y = Convert.ToInt32(actorValues.Attributes["y"].Value);
            // 
            //                         m_PropertyInstance.Properties.Add(
            //                             new PropertyElement(elementName, typeof(Size),
            //                             new Size(x, y), false, new CategoryAttribute(componentName)));
            //                     }
            //                     else if (elementType == typeof(Vector3).ToString())
            //                     {
            //                         float x = Convert.ToSingle(actorValues.Attributes["x"].Value);
            //                         float y = Convert.ToSingle(actorValues.Attributes["y"].Value);
            //                         float z = Convert.ToSingle(actorValues.Attributes["z"].Value);
            // 
            //                         m_PropertyInstance.Properties.Add(
            //                             new PropertyElement(elementName, typeof(Vector3),
            //                             new Vector3(x, y, z), false, new CategoryAttribute(componentName)));
            //                     }
            //                     else if (elementType == typeof(Color).ToString())
            //                     {
            //                         float r = Convert.ToSingle(actorValues.Attributes["r"].Value);
            //                         float g = Convert.ToSingle(actorValues.Attributes["g"].Value);
            //                         float b = Convert.ToSingle(actorValues.Attributes["b"].Value);
            //                         float a = Convert.ToSingle(actorValues.Attributes["a"].Value);
            // 
            //                         m_PropertyInstance.Properties.Add(
            //                             new PropertyElement(elementName, typeof(Color),
            //                             Color.FromArgb((int)(a * 255.0), (int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0)),
            //                             false, new CategoryAttribute(componentName)));
            //                     }
            //                     else if (elementType == typeof(Image).ToString())
            //                     {
            // 
            //                     }
            //                     else if (elementType == typeof(bool).ToString())
            //                     {
            // 
            //                     }
            // 
            //                     //                     Assembly[] AssembliesLoaded = AppDomain.CurrentDomain.GetAssemblies();
            //                     //                     Type target = AssembliesLoaded.Select(assembly => assembly.GetType(elementType))
            //                     //                         .Where(type => type != null)
            //                     //                         .FirstOrDefault();
            // 
            // 
            //                     ++elementNum;
            //                 }
            //             }
            //             catch (Exception e)
            //             {
            //                 MessageBox.Show("Error in ComponentName " + componentName + "\n" + e.ToString());
            //             }
        }

    }
}
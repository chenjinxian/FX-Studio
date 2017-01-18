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
        private static string m_TranslationProperty = "Translation";
        private static string m_ScaleProperty = "Scale";
        private static string m_RotationProperty = "Rotation";
        private static string m_NameDescription = "The name of this graph item";
        private static string m_LocationDescription = "The location of this graph item";
        private static string m_TranslationDescription = "The translation of this node";
        private static string m_ScaleDescription = "The uniform scale of this node";
        private static string m_RotationDescription = "The rotation of this node";

        private Dictionary<string, XmlNode> m_ComponentsByName;
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

            inspectorComponent.UpdateControl();
        }

        private void AddSceneProperties(XmlNode sceneNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("Scene");
            inspectorComponent.CategoryAdd("scene", category);

            Inspector.StringItem nameItem = new Inspector.StringItem("Name", sceneNode.Name);
            inspectorComponent.ItemAdd("scene", sceneNode.Name, nameItem);

            inspectorComponent.UpdateControl();
        }

        private void AddEditorCameraProperties(XmlNode cameraNode)
        {
            Inspector.CategoryItem category = new Inspector.CategoryItem("EditorCamera");
            inspectorComponent.CategoryAdd("camera", category);

            XmlNode translation = cameraNode.SelectSingleNode("Translation");
            float x = Convert.ToSingle(translation.Attributes["x"].Value);
            float y = Convert.ToSingle(translation.Attributes["y"].Value);
            float z = Convert.ToSingle(translation.Attributes["z"].Value);
            Inspector.Vector3Item translateItem = new Inspector.Vector3Item(translation.Name, new Inspector.Vector3(x, y, z));
            inspectorComponent.ItemAdd("camera", translation.Name, translateItem);

            XmlNode rotation = cameraNode.SelectSingleNode("Rotation");
            x = Convert.ToSingle(rotation.Attributes["x"].Value);
            y = Convert.ToSingle(rotation.Attributes["y"].Value);
            z = Convert.ToSingle(rotation.Attributes["z"].Value);
            Inspector.Vector3Item rotateItem = new Inspector.Vector3Item(rotation.Name, new Inspector.Vector3(x, y, z));
            inspectorComponent.ItemAdd("camera", rotation.Name, rotateItem);

            inspectorComponent.UpdateControl();
        }

        private void AddTransformProperties()
        {

        }

        private void AddSkyboxProperties()
        {

        }

        private void AddGridProperties()
        {

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

            }
            //             m_PropertyInstance.Properties.Clear();
            // 
            //             m_ActorXml = selectedNode;
            //             m_SelectedComponents = new XmlDocument();
            //             XmlNode editorComponents = m_SelectedComponents.CreateElement("Actor");
            //             m_SelectedComponents.AppendChild(editorComponents);
            // 
            //             foreach (XmlNode component in m_ActorXml.ChildNodes)
            //             {
            //                 XmlNode sourceEditorComponent;
            //                 if (m_ComponentsByName.TryGetValue(component.Name, out sourceEditorComponent))
            //                 {
            //                     XmlDocument ownerDoc = editorComponents.OwnerDocument;
            //                     XmlNode editorComponent = ownerDoc.ImportNode(sourceEditorComponent, true);
            //                     editorComponents.AppendChild(editorComponent);
            //                     AddCompoentProperties(component, editorComponent);
            //                 }
            //             }
            // 
            //             if (m_PropertyInstance.Properties.Count == 0)
            //             {
            //                 m_PropertyInstance.Properties.Add(new PropertyElement(m_NameProperty, typeof(string), selectedNode.Name, false,
            //                     new CategoryAttribute("Properties"), new DescriptionAttribute(m_NameDescription)));
            //                 m_PropertyInstance.Properties.Add(new PropertyElement(m_HiddenProperty, typeof(bool), false, true,
            //                     new CategoryAttribute("Properties"), new DescriptionAttribute(m_HiddenDescription)));
            //             }
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
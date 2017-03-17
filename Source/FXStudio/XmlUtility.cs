using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;

namespace FXStudio
{
    class XmlUtility
    {
        public static XmlAttribute CreateAttribute(XmlDocument xmlDoc, string name, string value)
        {
            XmlAttribute attribute = xmlDoc.CreateAttribute(name);
            attribute.Value = value;
            return attribute;
        }

        public static XmlElement CreateTransformComponent(XmlDocument xmlDoc, string tx = "0", string ty = "0", string tz = "0")
        {
            XmlElement transformElement = xmlDoc.CreateElement("TransformComponent");

            XmlElement translation = xmlDoc.CreateElement("Translation");
            XmlElement scale = xmlDoc.CreateElement("Scale");
            XmlElement rotation = xmlDoc.CreateElement("Rotation");
            transformElement.AppendChild(translation);
            transformElement.AppendChild(scale);
            transformElement.AppendChild(rotation);

            translation.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "x", tx));
            translation.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "y", ty));
            translation.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "z", tz));

            scale.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "x", "1"));
            scale.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "y", "1"));
            scale.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "z", "1"));

            rotation.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "x", "0"));
            rotation.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "y", "0"));
            rotation.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "z", "0"));

            return transformElement;
        }

        public static XmlElement CreateSphereRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement sphereElement = xmlDoc.CreateElement("SphereRenderComponent");

            XmlElement sphere = xmlDoc.CreateElement("Sphere");
            XmlElement effect = xmlDoc.CreateElement("Material");
            sphereElement.AppendChild(sphere);
            sphereElement.AppendChild(effect);

            effect.InnerText = @"Materials\DefaultMaterial.mat";

            sphere.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "diameter", "1.0"));
            sphere.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "tessellation", "4"));
            sphere.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return sphereElement;
        }

        public static XmlElement CreateTorusRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement torusElement = xmlDoc.CreateElement("TorusRenderComponent");

            XmlElement torus = xmlDoc.CreateElement("Torus");
            XmlElement effect = xmlDoc.CreateElement("Material");
            torusElement.AppendChild(torus);
            torusElement.AppendChild(effect);

            effect.InnerText = @"Materials\DefaultMaterial.mat";

            torus.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "diameter", "1.0"));
            torus.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "thickness", "0.33"));
            torus.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "tessellation", "32"));
            torus.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return torusElement;
        }

        public static XmlElement CreateTeapotRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement teapotElement = xmlDoc.CreateElement("TeapotRenderComponent");

            XmlElement teapot = xmlDoc.CreateElement("Teapot");
            XmlElement effect = xmlDoc.CreateElement("Material");
            teapotElement.AppendChild(teapot);
            teapotElement.AppendChild(effect);

            effect.InnerText = @"Materials\DefaultMaterial.mat";

            teapot.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "size", "1.0"));
            teapot.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "tessellation", "8"));
            teapot.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return teapotElement;
        }

        public static XmlElement CreatePlaneRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement cubeElement = xmlDoc.CreateElement("PlaneRenderComponent");

            XmlElement cube = xmlDoc.CreateElement("Cube");
            XmlElement effect = xmlDoc.CreateElement("Material");
            cubeElement.AppendChild(cube);
            cubeElement.AppendChild(effect);

            effect.InnerText = @"Materials\DefaultMaterial.mat";

            cube.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "size", "1.0"));
            cube.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return cubeElement;
        }

        public static XmlElement CreateModelRenderComponent(XmlDocument xmlDoc, string fileName)
        {
            XmlElement modelElement = xmlDoc.CreateElement("ModelRenderComponent");

            XmlElement model = xmlDoc.CreateElement("Model");
            modelElement.AppendChild(model);
            model.InnerText = @"Models\" + Path.GetFileName(fileName);

            XmlElement materials = xmlDoc.CreateElement("Materials");
            modelElement.AppendChild(materials);

            XmlDocument xmlModel = new XmlDocument();
            xmlModel.Load(fileName);
            XmlNode nodeRoot = xmlModel.DocumentElement;
            if (nodeRoot != null)
            {
                int index = 0;
                XmlNode meshList = nodeRoot.SelectSingleNode("MeshList");
                foreach (XmlNode mesh in meshList.ChildNodes)
                {
                    string name = mesh.Attributes["name"].Value;
                    if (string.IsNullOrEmpty(name))
                        name = "Material" + index.ToString();
                    XmlElement child = xmlDoc.CreateElement("Material");
                    materials.AppendChild(child);
                    child.Attributes.Append(CreateAttribute(xmlDoc, "name", name));
                    child.Attributes.Append(CreateAttribute(xmlDoc, "index", index.ToString()));
                    child.InnerText = @"Materials\DefaultMaterial.mat";
                    index++;
                }
            }

            return modelElement;
        }

        public static readonly string DefaultProjectXml =
            @"<?xml version=""1.0"" encoding=""UTF-8"" ?>
<Project type = ""Project"">
    <EditorCamera type = ""EditorCamera"" >
        <Translation x = ""2.5"" y=""3.535533"" z=""2.5"" />
        <Rotation x = ""45"" y=""45"" z=""0"" />
    </EditorCamera>
    <DefaultScene type = ""Scene"">
        <Skybox type = ""Skybox"">
            <SkyboxRenderComponent>
                <Texture>Textures\Skybox.dds</Texture>
            </SkyboxRenderComponent>
        </Skybox>
        <Grid type = ""Grid"" >
            <GridRenderComponent >
                <GridSize>100.0</GridSize>
                <TicksInterval>1.0</TicksInterval>
            </GridRenderComponent>
        </Grid>
    </DefaultScene>
</Project>
";

        public static readonly string DefaultAssetXml =
            @"<Assets>
    <Effects>
        <Effect name=""DefaultEffect"" object=""Effects\DefaultEffect.fx"">Effects\DefaultEffect.fx</Effect>
    </Effects>
    <Materials>
        <Material name=""DefaultMaterial"">Materials\DefaultMaterial.mat</Material>
    </Materials>
    <Models/>
    <Textures>
        <Texture name=""DefaultColor"">Textures\DefaultColor.dds</Texture>
    </Textures>
</Assets>
";
    }
}

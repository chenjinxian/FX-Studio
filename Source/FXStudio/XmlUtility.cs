using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace FXStudio
{
    class XmlUtility
    {
        static int GetNodePosition(XmlNode child)
        {
            int count = 1;
            for (int i = 0; i < child.ParentNode.ChildNodes.Count; i++)
            {
                if (child.ParentNode.ChildNodes[i] == child)
                {
                    return count;
                }
                if (child.ParentNode.ChildNodes[i].Name == child.Name)
                {
                    ++count;
                }
            }
            throw new InvalidOperationException("Child node somehow not found in its parent's ChildNodes property.");
        }

        public static string GetXPathToNode(XmlNode node)
        {
            if (node.NodeType == XmlNodeType.Attribute)
            {
                return String.Format(
                    "{0}/@{1}",
                    GetXPathToNode(((XmlAttribute)node).OwnerElement),
                    "*"
                    );
            }

            if (node.ParentNode == null)
            {
                return "";
            }
            return String.Format(
                "{0}/{1}[{2}]",
                GetXPathToNode(node.ParentNode),
                "*",
                GetNodePosition(node)
                );
        }

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

        static XmlElement CreateColorElement(XmlDocument xmlDoc)
        {
            XmlElement color = xmlDoc.CreateElement("Color");
            color.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "r", "1"));
            color.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "g", "1"));
            color.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "b", "1"));
            color.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "a", "1"));

            return color;
        }

        public static XmlElement CreateCubeRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement cubeElement = xmlDoc.CreateElement("CubeRenderComponent");
            cubeElement.AppendChild(CreateColorElement(xmlDoc));

            XmlElement texture = xmlDoc.CreateElement("Texture");
            XmlElement effect = xmlDoc.CreateElement("Effect");
            XmlElement cube = xmlDoc.CreateElement("Cube");
            cubeElement.AppendChild(texture);
            cubeElement.AppendChild(effect);
            cubeElement.AppendChild(cube);


            texture.InnerText = @"Textures\DefaultColor.dds";

            effect.InnerText = @"Effects\DefaultEffect.fx";
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "technique", "main11"));
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "pass", "p0"));

            cube.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "size", "1.0"));
            cube.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return cubeElement;
        }

        public static XmlElement CreateSphereRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement sphereElement = xmlDoc.CreateElement("SphereRenderComponent");
            sphereElement.AppendChild(CreateColorElement(xmlDoc));

            XmlElement texture = xmlDoc.CreateElement("Texture");
            XmlElement effect = xmlDoc.CreateElement("Effect");
            XmlElement sphere = xmlDoc.CreateElement("Sphere");
            sphereElement.AppendChild(texture);
            sphereElement.AppendChild(effect);
            sphereElement.AppendChild(sphere);


            texture.InnerText = @"Textures\DefaultColor.dds";

            effect.InnerText = @"Effects\DefaultEffect.fx";
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "technique", "main11"));
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "pass", "p0"));

            sphere.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "diameter", "1.0"));
            sphere.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "tessellation", "4"));
            sphere.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return sphereElement;
        }

        public static XmlElement CreateCylinderRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement cylinderElement = xmlDoc.CreateElement("CylinderRenderComponent");
            cylinderElement.AppendChild(CreateColorElement(xmlDoc));

            XmlElement texture = xmlDoc.CreateElement("Texture");
            XmlElement effect = xmlDoc.CreateElement("Effect");
            XmlElement cylinder = xmlDoc.CreateElement("Cylinder");
            cylinderElement.AppendChild(texture);
            cylinderElement.AppendChild(effect);
            cylinderElement.AppendChild(cylinder);


            texture.InnerText = @"Textures\DefaultColor.dds";

            effect.InnerText = @"Effects\DefaultEffect.fx";
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "technique", "main11"));
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "pass", "p0"));

            cylinder.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "height", "1.0"));
            cylinder.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "diameter", "1.0"));
            cylinder.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "tessellation", "32"));
            cylinder.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return cylinderElement;
        }

        public static XmlElement CreateTeapotRenderComponent(XmlDocument xmlDoc)
        {
            XmlElement teapotElement = xmlDoc.CreateElement("TeapotRenderComponent");
            teapotElement.AppendChild(CreateColorElement(xmlDoc));

            XmlElement texture = xmlDoc.CreateElement("Texture");
            XmlElement effect = xmlDoc.CreateElement("Effect");
            XmlElement teapot = xmlDoc.CreateElement("Teapot");
            teapotElement.AppendChild(texture);
            teapotElement.AppendChild(effect);
            teapotElement.AppendChild(teapot);


            texture.InnerText = @"Textures\DefaultColor.dds";

            effect.InnerText = @"Effects\DefaultEffect.fx";
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "technique", "main11"));
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "pass", "p0"));

            teapot.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "size", "1.0"));
            teapot.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "tessellation", "8"));
            teapot.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "rhcoords", "1"));

            return teapotElement;
        }

        public static XmlElement CreateModelRenderComponent(XmlDocument xmlDoc, string fileName)
        {
            XmlElement modelElement = xmlDoc.CreateElement("ModelRenderComponent");
            modelElement.AppendChild(CreateColorElement(xmlDoc));

            XmlElement model = xmlDoc.CreateElement("Model");
            XmlElement effect = xmlDoc.CreateElement("Effect");
            XmlElement textures = xmlDoc.CreateElement("Textures");
            modelElement.AppendChild(model);
            modelElement.AppendChild(effect);
            modelElement.AppendChild(textures);

            model.InnerText = @"Models\" + fileName;

            effect.InnerText = @"Effects\DefaultEffect.fx";
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "technique", "main11"));
            effect.Attributes.Append(XmlUtility.CreateAttribute(xmlDoc, "pass", "p0"));

            XmlElement textureChild = xmlDoc.CreateElement("Texture");
            textureChild.InnerText = @"Textures\DefaultColor.dds";

            textures.AppendChild(textureChild);

            return modelElement;
        }

        public static readonly string DefaultProjectXml =
            @"<?xml version=""1.0"" encoding=""UTF-8"" ?>
<Project type = ""Project"">
    <EditorCamera type = ""EditorCamera"" >
        <Translation x = ""0"" y=""0"" z=""5"" />
        <Rotation x = ""45"" y=""45"" z=""0"" />
    </EditorCamera>
    <DefaultScene type = ""Scene"">
        <Skybox type = ""Skybox"">
            <SkyboxRenderComponent>
                <Color r = ""1"" g=""1"" b=""1"" a=""1"" />
                <Texture>Textures\Skybox.dds</Texture>
            </SkyboxRenderComponent>
        </Skybox>
        <Grid type = ""Grid"" >
            <GridRenderComponent >
                <Color r = ""1"" g=""1"" b=""1"" a=""1"" />
                <Texture>Textures\Grid.dds</Texture>
                <GridSize x = ""10"" y=""10"" />
                <TicksInterval>1.0</TicksInterval>
            </GridRenderComponent>
        </Grid>
    </DefaultScene>
</Project>
";

        public static readonly string DefaultAssetXml =
            @"<Assets>
    <Effects>
        <Effect name=""DefaultEffect"" object=""Effects\DefaultEffect.fx"" source=""Effects\DefaultEffect.fx"">
            <Techniques>
                <Technique name=""main11"" checked=""true"">
                    <Pass>p0</Pass>
                </Technique>
            </Techniques>
            <Variables>
                <Variable name=""WorldITXf"" value=""0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"">
                    <UIWidget>None</UIWidget>
                </Variable>
                <Variable name=""WvpXf"" value=""0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"">
                    <UIWidget>None</UIWidget>
                </Variable>
                <Variable name=""WorldXf"" value=""0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"">
                    <UIWidget>None</UIWidget>
                </Variable>
                <Variable name=""ViewIXf"" value=""0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"">
                    <UIWidget>None</UIWidget>
                </Variable>
                <Variable name=""LightPos"" value=""10 10 -10"">
                    <Object>PointLight0</Object>
                    <Space>World</Space>
                </Variable>
                <Variable name=""AmbiColor"" value=""0.1 0.1 0.1"">
                    <UIName>Ambient Lighting</UIName>
                    <UIWidget>Color</UIWidget>
                </Variable>
                <Variable name=""SurfColor"" value=""0.8 0.8 1"">
                    <UIName>Surface Color</UIName>
                    <UIWidget>Color</UIWidget>
                </Variable>
                <Variable name=""Ks"" value=""0.5"">
                    <UIName>Specular Intensity</UIName>
                    <UIWidget>slider</UIWidget>
                    <UIMin>0</UIMin>
                    <UIMax>1</UIMax>
                    <UIStep>0.01</UIStep>
                </Variable>
                <Variable name=""SpecExpon"" value=""30"">
                    <UIName>Specular Power</UIName>
                    <UIWidget>slider</UIWidget>
                    <UIMin>1</UIMin>
                    <UIMax>128</UIMax>
                    <UIStep>1</UIStep>
                </Variable>
            </Variables>
        </Effect>
    </Effects>
    <Models/>
    <Textures>
        <Texture name=""DefaultColor"">Textures\DefaultColor.dds</Texture>
    </Textures>
</Assets>
";
    }
}

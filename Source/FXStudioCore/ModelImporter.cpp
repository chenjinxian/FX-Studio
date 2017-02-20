#include "stdafx.h"
#include "ModelImporter.h"
#include "../TinyEngine/TinyEngine.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <fstream>
#include <thread>

#ifdef _WIN64
#pragma comment(lib, "assimp-vc140-mt-x64.lib")
#else
#pragma comment(lib, "assimp-vc140-mt-x86.lib")
#endif

ModelImporter& ModelImporter::GetImporter()
{
	static ModelImporter g_Importer;
	return g_Importer;
}

ModelImporter::ModelImporter()
	: m_Callback(nullptr),
	m_AssimpImporter(new Assimp::Importer())
{
	m_AssimpImporter->SetProgressHandler(this);
}

ModelImporter::~ModelImporter()
{
	m_AssimpImporter->SetProgressHandler(nullptr);
}

bool ModelImporter::Update(float percentage /*= -1.f*/)
{
	if (m_Callback != nullptr)
	{
		return m_Callback(percentage * 0.2f, nullptr);
	}

	return true;
}

void ModelImporter::LoadModel(const std::string& importPath, const std::string& exportPath, ProgressCallback callback)
{
	m_Callback = callback;

// 	std::thread workThread([=]() {
// 	});
// 
// 	workThread.join();

	uint32_t flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs | aiProcess_FlipWindingOrder;
	const aiScene* scene = m_AssimpImporter->ReadFile(importPath, flags);

	if (scene == nullptr)
	{
		if (callback != nullptr)
		{
			callback(-1.f, m_AssimpImporter->GetErrorString());
		}
		return;
	}

	ExportModel(exportPath, scene);
}

void ModelImporter::ExportModel(const std::string& exportPath, const aiScene* pScene)
{
	std::ofstream fs(exportPath.c_str(), std::ofstream::out | std::ofstream::trunc);
	if (fs.bad())
		return;
	
	// write header
	std::string header(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<!-- XML Model dump produced by assimp dump\n"
		"\t%s"
		"-->"
		"\n\n"
		"<Model flags=\"%d\">\n"
	);

	time_t tt = ::time(NULL);
	tm buf;
	localtime_s(&buf, &tt);
	char curtime[26];
	asctime_s(curtime, &buf);
	FStreamPrintf(fs, header.c_str(), curtime, pScene->mFlags);

	WriteNode(pScene->mRootNode, fs, 1);
	if (pScene->HasAnimations())
	{
		WriteAnimation(pScene, fs);
	}
	if (pScene->HasMeshes())
	{
		WriteMesh(pScene, fs);
	}

	FStreamPrintf(fs, "</Model>");
	fs.close();
}

void ModelImporter::WriteNode(const aiNode* node, std::ofstream& fs, uint32_t depth)
{
	char prefix[512];
	for (unsigned int i = 0; i < depth; ++i)
		prefix[i] = '\t';
	prefix[depth] = '\0';

	const aiMatrix4x4& m = node->mTransformation;

	aiString name;
	ConvertName(name, node->mName);
	FStreamPrintf(fs, "%s<Node name=\"%s\">\n"
		"%s\t<Matrix4>"
		"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
		"</Matrix4>\n",
		prefix, name.data, prefix,
		m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4, m.c1, m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4);

	if (node->mNumMeshes)
	{
		FStreamPrintf(fs, "%s\t<MeshRefs num=\"%i\">", prefix, node->mNumMeshes);

		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			FStreamPrintf(fs, "%i ", node->mMeshes[i]);
		}

		FStreamPrintf(fs, "</MeshRefs>\n");
	}

	if (node->mNumChildren)
	{
		FStreamPrintf(fs, "%s\t<NodeList num=\"%i\">\n", prefix, node->mNumChildren);

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			WriteNode(node->mChildren[i], fs, depth + 2);
		}

		FStreamPrintf(fs, "%s\t</NodeList>\n", prefix);
	}

	FStreamPrintf(fs, "%s</Node>\n", prefix);
}

void ModelImporter::WriteAnimation(const aiScene* pScene, std::ofstream& fs)
{
	FStreamPrintf(fs, "\t<AnimationList num=\"%i\">\n", pScene->mNumAnimations);

	aiString name;
	for (unsigned int i = 0; i < pScene->mNumAnimations; ++i)
	{
		aiAnimation* anim = pScene->mAnimations[i];

		// anim header
		ConvertName(name, anim->mName);
		FStreamPrintf(fs, "\t\t<Animation name=\"%s\" duration=\"%f\" ticks=\"%f\">\n",
			name.data, anim->mDuration, anim->mTicksPerSecond);

		// write bone animation channels
		if (anim->mNumChannels)
		{
			FStreamPrintf(fs, "\t\t\t<NodeAnimList num=\"%i\">\n", anim->mNumChannels);
			for (unsigned int n = 0; n < anim->mNumChannels; ++n)
			{
				aiNodeAnim* nd = anim->mChannels[n];

				// node anim header
				ConvertName(name, nd->mNodeName);
				FStreamPrintf(fs, "\t\t\t\t<NodeAnim node=\"%s\">\n", name.data);

				// write position keys
				if (nd->mNumPositionKeys)
				{
					FStreamPrintf(fs, "\t\t\t\t\t<PositionKeyList num=\"%i\">\n", nd->mNumPositionKeys);
					for (unsigned int a = 0; a < nd->mNumPositionKeys; ++a) {
						aiVectorKey* vc = nd->mPositionKeys + a;
						FStreamPrintf(fs, "\t\t\t\t\t\t<PositionKey time=\"%f\">%f %f %f</PositionKey>\n",
							vc->mTime, vc->mValue.x, vc->mValue.y, vc->mValue.z);
					}
					FStreamPrintf(fs, "\t\t\t\t\t</PositionKeyList>\n");
				}

				// write scaling keys
				if (nd->mNumScalingKeys)
				{
					FStreamPrintf(fs, "\t\t\t\t\t<ScalingKeyList num=\"%i\">\n", nd->mNumScalingKeys);
					for (unsigned int a = 0; a < nd->mNumScalingKeys; ++a) {
						aiVectorKey* vc = nd->mScalingKeys + a;
						FStreamPrintf(fs, "\t\t\t\t\t\t<ScalingKey time=\"%f\">%f %f %f</ScalingKey>\n",
							vc->mTime, vc->mValue.x, vc->mValue.y, vc->mValue.z);
					}
					FStreamPrintf(fs, "\t\t\t\t\t</ScalingKeyList>\n");
				}

				// write rotation keys
				if (nd->mNumRotationKeys)
				{
					FStreamPrintf(fs, "\t\t\t\t\t<RotationKeyList num=\"%i\">\n", nd->mNumRotationKeys);
					for (unsigned int a = 0; a < nd->mNumRotationKeys; ++a) {
						aiQuatKey* vc = nd->mRotationKeys + a;
						FStreamPrintf(fs, "\t\t\t\t\t\t<RotationKey time=\"%f\">%f %f %f</RotationKey>\n",
							vc->mTime, vc->mValue.x, vc->mValue.y, vc->mValue.z, vc->mValue.w);
					}
					FStreamPrintf(fs, "\t\t\t\t\t</RotationKeyList>\n");
				}

				FStreamPrintf(fs, "\t\t\t\t</NodeAnim>\n");
			}
			FStreamPrintf(fs, "\t\t\t</NodeAnimList>\n");
		}
		FStreamPrintf(fs, "\t\t</Animation>\n");
	}
	FStreamPrintf(fs, "\t</AnimationList>\n");
}

void ModelImporter::WriteMesh(const aiScene* pScene, std::ofstream& fs)
{
	FStreamPrintf(fs, "\t<MeshList num=\"%i\">\n", pScene->mNumMeshes);
	aiString name;
	for (unsigned int i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh* mesh = pScene->mMeshes[i];

		// mesh header
		ConvertName(name, mesh->mName);
		FStreamPrintf(fs, "\t\t<Mesh types=\"%s%s%s%s\" name=\"%s\">\n",
			(mesh->mPrimitiveTypes & aiPrimitiveType_POINT ? "points" : ""),
			(mesh->mPrimitiveTypes & aiPrimitiveType_LINE ? "lines" : ""),
			(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE ? "triangles" : ""),
			(mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON ? "polygons" : ""),
			name.data);

		// bones
		if (mesh->mNumBones)
		{
			FStreamPrintf(fs, "\t\t\t<BoneList num=\"%i\">\n", mesh->mNumBones);

			for (unsigned int n = 0; n < mesh->mNumBones; ++n)
			{
				aiBone* bone = mesh->mBones[n];

				ConvertName(name, bone->mName);
				// bone header
				FStreamPrintf(fs, "\t\t\t\t<Bone name=\"%s\">\n"
					"\t\t\t\t<Matrix4>"
					"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
					"</Matrix4>\n",
					name.data,
					bone->mOffsetMatrix.a1, bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3, bone->mOffsetMatrix.a4,
					bone->mOffsetMatrix.b1, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.b4,
					bone->mOffsetMatrix.c1, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.c4,
					bone->mOffsetMatrix.d1, bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3, bone->mOffsetMatrix.d4);

				if (bone->mNumWeights)
				{
					FStreamPrintf(fs, "\t\t\t\t\t<WeightList num=\"%i\">\n", bone->mNumWeights);

					// bone weights
					for (unsigned int a = 0; a < bone->mNumWeights; ++a)
					{
						aiVertexWeight* wght = bone->mWeights + a;

						FStreamPrintf(fs, "\t\t\t\t\t\t<Weight index=\"%i\">%f</Weight>\n",
							wght->mVertexId, wght->mWeight);
					}

					FStreamPrintf(fs, "\t\t\t\t\t</WeightList>\n");
				}
				FStreamPrintf(fs, "\t\t\t\t</Bone>\n");
			}
			FStreamPrintf(fs, "\t\t\t</BoneList>\n");
		}

		// faces
		if (mesh->mNumFaces)
		{
			FStreamPrintf(fs, "\t\t\t<FaceList num=\"%i\">\n", mesh->mNumFaces);
			for (unsigned int n = 0; n < mesh->mNumFaces; ++n) {
				aiFace& f = mesh->mFaces[n];
				FStreamPrintf(fs, "\t\t\t\t<Face num=\"%i\">", f.mNumIndices);

				for (unsigned int j = 0; j < f.mNumIndices; ++j)
					FStreamPrintf(fs, "%i ", f.mIndices[j]);

				FStreamPrintf(fs, "</Face>\n");
			}
			FStreamPrintf(fs, "\t\t\t</FaceList>\n");
		}

		// vertex positions
		if (mesh->HasPositions())
		{
			FStreamPrintf(fs, "\t\t\t<Positions num=\"%i\" set=\"0\" num_components=\"3\"> \n", mesh->mNumVertices);
			for (unsigned int n = 0; n < mesh->mNumVertices; ++n)
			{
				FStreamPrintf(fs, "\t\t\t\t<Position>%f %f %f</Position>\n",
					mesh->mVertices[n].x,
					mesh->mVertices[n].y,
					mesh->mVertices[n].z);
			}
			FStreamPrintf(fs, "\t\t\t</Positions>\n");
		}

		// vertex normals
		if (mesh->HasNormals())
		{
			FStreamPrintf(fs, "\t\t\t<Normals num=\"%i\" set=\"0\" num_components=\"3\"> \n", mesh->mNumVertices);
			for (unsigned int n = 0; n < mesh->mNumVertices; ++n)
			{
				FStreamPrintf(fs, "\t\t\t\t<Normal>%f %f %f</Normal>\n",
					mesh->mNormals[n].x,
					mesh->mNormals[n].y,
					mesh->mNormals[n].z);
			}
			FStreamPrintf(fs, "\t\t\t</Normals>\n");
		}

		// vertex tangents and bitangents
		if (mesh->HasTangentsAndBitangents())
		{
			FStreamPrintf(fs, "\t\t\t<Tangents num=\"%i\" set=\"0\" num_components=\"3\"> \n", mesh->mNumVertices);
			for (unsigned int n = 0; n < mesh->mNumVertices; ++n)
			{
				FStreamPrintf(fs, "\t\t\t\t<Tangent>%f %f %f</Tangent>\n",
					mesh->mTangents[n].x,
					mesh->mTangents[n].y,
					mesh->mTangents[n].z);
			}
			FStreamPrintf(fs, "\t\t\t</Tangents>\n");

			FStreamPrintf(fs, "\t\t\t<Bitangents num=\"%i\" set=\"0\" num_components=\"3\"> \n", mesh->mNumVertices);
			for (unsigned int n = 0; n < mesh->mNumVertices; ++n)
			{
				FStreamPrintf(fs, "\t\t\t\t<Bitangent>%f %f %f</Bitangent>\n",
					mesh->mBitangents[n].x,
					mesh->mBitangents[n].y,
					mesh->mBitangents[n].z);
			}
			FStreamPrintf(fs, "\t\t\t</Bitangents>\n");
		}

		// texture coordinates
		for (unsigned int a = 0; a < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++a)
		{
			if (!mesh->mTextureCoords[a])
				break;

			FStreamPrintf(fs, "\t\t\t<TextureCoords num=\"%i\" set=\"%i\" num_components=\"%i\"> \n", mesh->mNumVertices,
				a, mesh->mNumUVComponents[a]);

			if (mesh->mNumUVComponents[a] == 3)
			{
				for (unsigned int n = 0; n < mesh->mNumVertices; ++n)
				{
					FStreamPrintf(fs, "\t\t\t\t<UV>%f %f %f</UV>\n",
						mesh->mTextureCoords[a][n].x,
						mesh->mTextureCoords[a][n].y,
						mesh->mTextureCoords[a][n].z);
				}
			}
			else
			{
				for (unsigned int n = 0; n < mesh->mNumVertices; ++n)
				{
					FStreamPrintf(fs, "\t\t\t\t<UV>%f %f</UV>\n",
						mesh->mTextureCoords[a][n].x,
						mesh->mTextureCoords[a][n].y);
				}
			}
			FStreamPrintf(fs, "\t\t\t</TextureCoords>\n");
		}

		// vertex colors
		for (unsigned int a = 0; a < AI_MAX_NUMBER_OF_COLOR_SETS; ++a)
		{
			if (!mesh->mColors[a])
				break;
			FStreamPrintf(fs, "\t\t\t<Colors num=\"%i\" set=\"%i\" num_components=\"4\"> \n", mesh->mNumVertices, a);
			for (unsigned int n = 0; n < mesh->mNumVertices; ++n)
			{
				FStreamPrintf(fs, "\t\t\t\t<Color>%f %f %f</Color>\n",
					mesh->mColors[a][n].r,
					mesh->mColors[a][n].g,
					mesh->mColors[a][n].b,
					mesh->mColors[a][n].a);
			}
			FStreamPrintf(fs, "\t\t\t</Colors>\n");
		}

		FStreamPrintf(fs, "\t\t</Mesh>\n");

		m_Callback(0.3f + 0.7f * (i + 1) / pScene->mNumMeshes, nullptr);
	}

	FStreamPrintf(fs, "\t</MeshList>\n");
}

int ModelImporter::FStreamPrintf(std::ofstream& fs, const char* format, ...)
{
	if (fs.bad())
		return -1;

	char buffer[BufferSize];
	size_t len(strlen(format));

	va_list va;
	va_start(va, format);
	int size = vsnprintf(buffer, BufferSize - 1, format, va);
	va_end(va);

	fs.write(buffer, size);
	return size;
}

void ModelImporter::ConvertName(aiString& out, const aiString& in)
{
	out.length = 0;
	for (unsigned int i = 0; i < in.length; ++i)
	{
		switch (in.data[i])
		{
		case '<':
			out.Append("&lt;"); break;
		case '>':
			out.Append("&gt;"); break;
		case '&':
			out.Append("&amp;"); break;
		case '\"':
			out.Append("&quot;"); break;
		case '\'':
			out.Append("&apos;"); break;
		default:
			out.data[out.length++] = in.data[i];
		}
	}
	out.data[out.length] = 0;
}

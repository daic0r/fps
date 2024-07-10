#include <cassert>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "packedVertex.h"
#include <math/vector.h>

#include "ModelImporterOBJ.h"

namespace fps::importers {

namespace fs = std::filesystem;

/*!
* \brief
* [ModelImporterOBJ::ModelImporterOBJ]
*
* \author Matthias.Gruen
* \date 2020/09/23
* [9/23/2020 Matthias.Gruen]
*/
ModelImporterOBJ::ModelImporterOBJ(const std::string& strFile)
{
	if (!fs::exists(strFile))
		throw std::runtime_error("File not found");
	m_strFile = strFile;
}

/*!
* \brief
* [ModelImporterOBJ::import]
*
* \author Matthias.Gruen
* \date 2020/09/23
* [9/23/2020 Matthias.Gruen]
*/
bool ModelImporterOBJ::import(std::map<std::string, rendering::model>& outMeshData) noexcept
{
   using namespace fps::math;
   using namespace fps::rendering;
   //m_mMaterials.clear();
   outMeshData.clear();

   std::ifstream f{ m_strFile };
   if (!f.good())
      return false;

   std::string strLine;
   std::string strType;
   float fValue;

   std::vector<vec3f> v, n;
   std::vector<vec2f> t;
   vec3f v3;
   vec2f v2;

   size_t nNumVerts{ 0 };
   std::map<packedVertex, size_t> mVertices;

   std::string strCurrentMaterial;
   std::string strCurrentObject;

   model* pMeshDataCurrentObject{ nullptr };
   std::vector<std::size_t>* pvIndicesCurrentMaterial{ nullptr };

   while (std::getline(f, strLine)) {
      std::stringstream ss{ strLine };
      ss >> strType;
      if (strType == "#")
         continue;

      if (strType == "o") {
         ss >> strType;
         strCurrentObject = strType;
         strCurrentMaterial = "";
         // Clear vertex buffer for each model since our indices of course have to restart for each new model
         mVertices.clear();
         nNumVerts = 0;
         pMeshDataCurrentObject = nullptr;
      }
      else if (strType == "mtllib") {
         ss >> strType;
         //loadMTL(strType);
      }
      else if (strType == "usemtl") {
         ss >> strType;
         strCurrentMaterial = strType;
         pvIndicesCurrentMaterial = nullptr;
      }
      else if (strType == "v") {

         int nIndex = 0;
         while (nIndex < 3 && (ss >> fValue)) {
            v3[nIndex++] = fValue;
         }
         v.emplace_back(v3);
      }
      else if (strType == "vt") {
         int nIndex = 0;
         while (nIndex < 2 && (ss >> fValue)) {
            v2[nIndex++] = fValue;
         }
         t.emplace_back(v2);
      }
      else if (strType == "vn") {
         int nIndex = 0;
         while (nIndex < 3 && (ss >> fValue)) {
            v3[nIndex++] = fValue;
         }
         n.emplace_back(v3);
      }
      else if (strType == "f") {
         if (pMeshDataCurrentObject == nullptr)
            pMeshDataCurrentObject = &outMeshData[strCurrentObject];

         if (pvIndicesCurrentMaterial == nullptr)
            pvIndicesCurrentMaterial = &pMeshDataCurrentObject->indices_;

         std::string strVertexInfo;
         int nIndex = 0;
         packedVertex pv;
         std::string strIndex;
         //std::cout << "Doing face " << nFace << std::endl;
         while (nIndex < 4 && (ss >> strVertexInfo)) {

            int chIndex = 0;

            for (int i = 0; i < 3; ++i) {
               while (chIndex < strVertexInfo.length() && strVertexInfo[chIndex] != '/')
                  strIndex += strVertexInfo[chIndex++];

               ++chIndex;

               if (strIndex == "")
                  continue;

               int nBufIndex = std::stoi(strIndex);
               switch (i) {
                  case 0:
                     pv.vertex = v[nBufIndex - 1];
                     break;
                  case 1:
                     pv.texCoord = t[nBufIndex - 1];
                     break;
                  case 2:
                     pv.normal = n[nBufIndex - 1];
                     break;
                  default:
                     break;
               }
               strIndex = "";
            }

            // Handling for quads: if this is a quad, we must turn it into 2 triangles
            if (nIndex == 3) {
               pvIndicesCurrentMaterial->emplace_back(pvIndicesCurrentMaterial->back());
            }
            auto iter = mVertices.find(pv);
            if (iter != mVertices.end()) {
               pvIndicesCurrentMaterial->emplace_back(iter->second);
            }
            else
            {
               //vVertices.emplace_back(pv);
               vec3f outVertex;
               for (int i = 0; i < 3; ++i) {
                  outVertex[i] = pv.vertex[i];

                  // if (pv.vertex[i] > pMeshDataCurrentObject->extents().maxPoint[i])
                  //    pMeshDataCurrentObject->extents().maxPoint[i] = pv.vertex[i];
                  // if (pv.vertex[i] < pMeshDataCurrentObject->extents().minPoint[i])
                  //    pMeshDataCurrentObject->extents().minPoint[i] = pv.vertex[i];
               }
               pMeshDataCurrentObject->vertices_.push_back(vertex{ outVertex[0], outVertex[1], outVertex[2], 1.0f });
               pMeshDataCurrentObject->texCoords_.emplace_back(pv.texCoord[0], pv.texCoord[1]);
               pMeshDataCurrentObject->normals_.emplace_back(pv.normal[0], pv.normal[1], pv.normal[2]);
               pvIndicesCurrentMaterial->emplace_back(nNumVerts);
               mVertices[pv] = nNumVerts;
               ++nNumVerts;
            }
            // Handling for quads: if this is a quad, we must turn it into 2 triangles
            if (nIndex == 3) {
               pvIndicesCurrentMaterial->emplace_back(*(pvIndicesCurrentMaterial->rbegin() + 4));
            }

            ++nIndex;
         }
      }
      // else if (strType == "l") {
      //
      //    auto& pathMap = m_mPaths[strCurrentObject];
      //    int nIndex1{ 0 }, nIndex2{ 0 };
      //    ss >> nIndex1 >> nIndex2;
      //
      //    const auto& vert1 = v[nIndex1 - 1];
      //    const auto& vert2 = v[nIndex2 - 1];
      //    glm::vec3 p1{ vert1.data[0], vert1.data[1], vert1.data[2] };
      //    glm::vec3 p2{ vert2.data[0], vert2.data[1], vert2.data[2] };
      //
      //    //if (pathMap.empty() || p1 != pathMap.back())
      //    pathMap.emplace_back(p1);
      //    //if (pathMap.empty() || p2 != pathMap.back())
      //    pathMap.emplace_back(p2);
      //
      // }
   }

   f.close();

   assert(outMeshData.begin()->second.vertices_.size() > 1);

   return true;
}

/*!
* \brief
* [ModelImporterOBJ::loadMTL]
*
* \author Matthias.Gruen
* \date 2020/09/23
* [9/23/2020 Matthias.Gruen]
*/
// bool ModelImporterOBJ::loadMTL(const std::string& strFile)
// {
// 	fs::path p(m_strFile);
// 	std::string strFullFilePath;
// 	strFullFilePath = p.parent_path().string();
// 	strFullFilePath.append(std::string{ fs::path::preferred_separator });
// 	strFullFilePath.append(strFile);
//
// 	std::ifstream f{ strFullFilePath };
// 	if (!f.good())
// 		return false;
//
// 	std::string strLine;
// 	std::string strType;
// 	float fValue;
// 	std::string strValue;
//
// 	glm::vec3 v3;
//
// 	RenderMaterial mat{};
// 	bool bFirstMaterial{ true };
//
// 	while (std::getline(f, strLine)) {
// 		std::stringstream ss{ strLine };
//
// 		ss >> strType;
//         if (strLine.empty() || strType == "#")
//             continue;
//         
// 		if (strType == "newmtl") {
// 			if (!bFirstMaterial) {
// 				m_mMaterials.emplace(mat.name(), mat);
// 				mat = RenderMaterial{};
// 			}
// 			ss >> strValue;
// 			mat.setName(strValue);
// 			bFirstMaterial = false;
// 		} else
// 		if (strType == "Ka") {
// 			int nIndex = 0;
// 			while (nIndex < 3 && (ss >> fValue)) {
// 				v3[nIndex++] = fValue;
// 			}
// 			mat.setAmbient(v3);
// 		} else
// 		if (strType == "Kd") {
// 			int nIndex = 0;
// 			while (nIndex < 3 && (ss >> fValue)) {
// 				v3[nIndex++] = fValue;
// 			}
// 			mat.setDiffuse(v3);
// 		} else
// 		if (strType == "Ks") {
// 			int nIndex = 0;
// 			while (nIndex < 3 && (ss >> fValue)) {
// 				v3[nIndex++] = fValue;
// 			}
// 			mat.setSpecular(v3);
// 		} else
// 		if (strType == "Ns") {
// 			ss >> fValue;
// 			mat.setSpecularExponent(fValue);
// 		} else
// 		if (strType == "d") {
// 			ss >> fValue;
// 			mat.setAlpha(fValue);
// 		} else		
// 		if (strType == "Tr") {
// 			ss >> fValue;
// 			mat.setAlpha(1.0f - fValue);
// 		} else
// 		if (strType == "Ni") {
// 			ss >> fValue;
// 			mat.setRefractionIndex(fValue);
// 		} else
//         if (strType == "map_Kd") {
//             std::string strFile;
//             while (ss >> strValue) {
//                 strFile.append(strValue);
//                 strFile.append(" ");
//             }
//             strFile = strFile.substr(0, strFile.length() - 1);
//             mat.setTextureFile(strFile);
//         }
// 	}
// 	if (!bFirstMaterial)
// 		m_mMaterials.emplace(mat.name(), mat);
//
// 	f.close();
//
// 	return true;
// }

}

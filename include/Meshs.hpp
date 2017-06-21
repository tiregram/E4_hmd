#include <vector>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <sstream>
using namespace Assimp;


struct Texture {
  GLuint id;
  std::string type;
  aiString path;
};

struct Mesh
{
public:
  std::vector<unsigned int> indices;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;

  GLuint indiceBuffID;
  GLuint positionBuffID;
  GLuint normalBuffID;
  GLuint uvBuffID;

  std::vector<Texture> textures;


  Mesh(std::vector<unsigned int> indices,
       std::vector<glm::vec3> positions,
       std::vector<glm::vec3> normals,
       std::vector<glm::vec2> uvs,
       std::vector<Texture> textures):indices(indices),
                                      positions(positions),
                                      normals(normals),
                                      uvs(uvs),
                                      textures(textures)

  {
    std::cout<<"mesh load"<<"\n";
    initOpenglBuffer();

  }

  void initOpenglBuffer()
  {
    std::cout <<"opengl buffer"  << "\n";
    // vertex ///////////////////////////////////////////////////////////////////
    glGenBuffers(1, &positionBuffID);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffID);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);

    // uv //////////////////////////////////////////////////////////////////////
    glGenBuffers(1, &uvBuffID);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffID);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    // normal ///////////////////////////////////////////////////////////////////
    glGenBuffers(1, &normalBuffID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffID);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // element vertex id ////////////////////////////////////////////////////////
    glGenBuffers(1, &indiceBuffID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
  }

  void updateOpenglBuffer()
  {
    // vertex ///////////////////////////////////////////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffID);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);

    // uv //////////////////////////////////////////////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffID);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    // normal ///////////////////////////////////////////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffID);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // element vertex id ////////////////////////////////////////////////////////
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
  }

  void draw(GLuint shaderID)
  {
    // unsigned int diffuseNr = 1;
    // unsigned int specularNr = 1;

    // for(unsigned int i = 0; i < textures.size(); i++)
    //   {
    //     glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    //     // retrieve texture number (the N in diffuse_textureN)
    //     std::stringstream ss;
    //     std::string number;
    //     std::string name = textures[i].type;
    //     if(name == "texture_diffuse")
    //       ss << diffuseNr++; // transfer unsigned int to stream
    //     else if(name == "texture_specular")
    //       ss << specularNr++; // transfer unsigned int to stream
    //     number = ss.str();

    //     GLuint TextureID = glGetUniformLocation(shaderID, std::string("material." + name + number).c_str());
    //     glUniform1i(TextureID, i);
    //     glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //   }
    // glActiveTexture(GL_TEXTURE0);

    /////////////////////////////////////////////////////////////////////////////
    //                             load obj                             //
    /////////////////////////////////////////////////////////////////////////////
    // std::cout<<"load"<<indices.size() <<"\n";

    // std::cout << shaderID<<positionBuffID<<","<<uvBuffID<<","<<indiceBuffID<<","<<normalBuffID << "\n";
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, uvBuffID);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // glEnableVertexAttribArray(2);
    // glBindBuffer(GL_ARRAY_BUFFER, normalBuffID);
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffID);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);
    // glDisableVertexAttribArray(2);

  }

private:
};



struct Meshs
{

  Meshs(const char * path)
  {
    std::cout << "load mesh:" << path <<"\n";
    std::string p(path);
    this->directory = p.substr(0, p.find_last_of('/'));

    loadModel(path);
  }


  void loadModel(const char * path)
  {
    Assimp::Importer import;

    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
      {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
        return;
      }

    processNode(scene->mRootNode, scene);
  }


  void processNode(aiNode *node, const aiScene *scene)
  {
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
      {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshs.push_back(processMesh(mesh, scene));
      }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
      {
        processNode(node->mChildren[i], scene);
      }

  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene)
  {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<unsigned int> indices;



    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
      {

        // positions
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        positions.push_back(position);

        // normals
        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        positions.push_back(normal);

        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
          {
            glm::vec2 uv;
            uv.x = mesh->mTextureCoords[0][i].x; 
            uv.y = mesh->mTextureCoords[0][i].y;
            uvs.push_back(uv);
          }
        else
          {
            uvs.push_back(glm::vec2(0.0f, 0.0f));
          }
      }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
      {
        aiFace face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
          indices.push_back(face.mIndices[j]);
      }

    // process materials
    //    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> textures;

    // 1. diffuse maps
    // std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // // 2. specular maps
    // std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // // 3. normal maps
    // std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // // 4. height maps
    // std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(indices,positions,normals,uvs,textures);
  }

  // std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
  // {
  //   std::vector<Texture> textures;

  //   for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  //     {
  //       aiString str;
  //       mat->GetTexture(type, i, &str);
  //       // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
  //       bool skip = false;
  //       for(unsigned int j = 0; j < load.size(); j++)
  //         {
  //           if(std::strcmp(load[j].path.C_Str(), str.C_Str()) == 0)
  //             {
  //               textures.push_back(load[j]);
  //               skip = true;
  //               break;
  //             }
  //         }

  //       if(!skip)
  //         {   // if texture hasn't been loaded already, load it
  //           Texture texture;
  //           texture.id = TextureFromFile(str.C_Str());
  //           texture.type = typeName;
  //           texture.path = str;
  //           textures.push_back(texture);
  //           load.push_back(texture);
  //         }
  //     }
  //   return textures;
  // }


  // unsigned int TextureFromFile(const char *path)
  // {
  //   std::string filename = this->directory + '/' + path;

  //   unsigned int textureID;
  //   glGenTextures(1, &textureID);

  //   int width, height, nrComponents;
  //   unsigned char *data = SOIL_load_image(
  //                                         filename.c_str(),
  //                                         &width, &height, &nrComponents,
  //                                         SOIL_LOAD_L
  //                                         );

  //   if (data)
  //     {
  //       GLenum format;
  //       if (nrComponents == 1)
  //         format = GL_RED;
  //       else if (nrComponents == 3)
  //         format = GL_RGB;
  //       else if (nrComponents == 4)
  //         format = GL_RGBA;

  //       glBindTexture(GL_TEXTURE_2D, textureID);
  //       glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  //       glGenerateMipmap(GL_TEXTURE_2D);

  //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //       SOIL_free_image_data( data );
  //     }
  //   else
  //     {
  //       std::cout << "Texture failed to load at path: " << path << std::endl;
  //       SOIL_free_image_data( data );
  //     }

  //   return textureID;
  // }

  std::vector<Mesh> meshs;
private:
  //  std::vector<Texture> load;


  std::string directory;
};

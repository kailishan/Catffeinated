#define NOMINMAX

#include "Shape.h"
#include <iostream>
#include <math.h>

#include "GLSL.h"
#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "stb_image.h"

using namespace std;

vector<tinyobj::material_t> mats;

void Shape::loadMesh(const string &meshName, string *mtlpath,
                     unsigned char *(loadimage)(char const *, int *, int *,
                                                int *, int)) {
  // Load geometry
  vector<tinyobj::shape_t> shapes;
  vector<tinyobj::material_t> objMaterials;
  string errStr;
  bool rc = false;
  if (mtlpath)
    rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str(),
                          mtlpath->c_str());
  else
    rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());

  
  for (int i = 0; i < objMaterials.size(); i++)
  {
      mats.push_back(objMaterials[i]);
  }
  

  if (!rc) {
    cerr << errStr << endl;
  } else if (shapes.size()) {
    obj_count = shapes.size();
    cout << shapes.size() << endl;
    posBuf = new std::vector<float>[shapes.size()];
    norBuf = new std::vector<float>[shapes.size()];
    texBuf = new std::vector<float>[shapes.size()];
    eleBuf = new std::vector<unsigned int>[shapes.size()];

    eleBufID = new unsigned int[shapes.size()];
    posBufID = new unsigned int[shapes.size()];
    norBufID = new unsigned int[shapes.size()];
    texBufID = new unsigned int[shapes.size()];
    vaoID = new unsigned int[shapes.size()];
    materialIDs = new unsigned int[shapes.size()];

    textureIDs = new unsigned int[shapes.size()];

    for (int i = 0; i < obj_count; i++) {
      // load textures
      textureIDs[i] = 0;
      posBuf[i] = shapes[i].mesh.positions;
      norBuf[i] = shapes[i].mesh.normals;
      texBuf[i] = shapes[i].mesh.texcoords;
      eleBuf[i] = shapes[i].mesh.indices;
      if (shapes[i].mesh.material_ids.size() > 0)
        materialIDs[i] = shapes[i].mesh.material_ids[0];
      else
        materialIDs[i] = -1;
    }
  }
  // material:
  for (int i = 0; i < objMaterials.size(); i++) {
      if (objMaterials[i].diffuse_texname.size() > 0) {
          char filepath[1000];
          int width, height, channels;
          string filename = objMaterials[i].diffuse_texname;
          int subdir = filename.rfind("\\");
          if (subdir > 0)
              filename = filename.substr(subdir + 1);
          string str = *mtlpath + filename;
          strcpy(filepath, str.c_str());
          // stbi_load(char const *filename, int *x, int *y, int *comp, int
          // req_comp)

          //unsigned char* data = loadimage(filepath, &width, &height, &channels, 4);
          unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
          glGenTextures(1, &textureIDs[i]);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
              GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);
          // delete[] data;
      }
  }

  int z;
  z = 0;
}

void Shape::measure() {
  float minX, minY, minZ;
  float maxX, maxY, maxZ;

  minX = minY = minZ = std::numeric_limits<float>::max();
  maxX = maxY = maxZ = -std::numeric_limits<float>::max();

  // Go through all vertices to determine min and max of each dimension
  for (int i = 0; i < obj_count; i++) {
    for (size_t v = 0; v < posBuf[i].size() / 3; v++) {
      if (posBuf[i][3 * v + 0] < minX)
        minX = posBuf[i][3 * v + 0];
      if (posBuf[i][3 * v + 0] > maxX)
        maxX = posBuf[i][3 * v + 0];

      if (posBuf[i][3 * v + 1] < minY)
        minY = posBuf[i][3 * v + 1];
      if (posBuf[i][3 * v + 1] > maxY)
        maxY = posBuf[i][3 * v + 1];

      if (posBuf[i][3 * v + 2] < minZ)
        minZ = posBuf[i][3 * v + 2];
      if (posBuf[i][3 * v + 2] > maxZ)
        maxZ = posBuf[i][3 * v + 2];
    }
  }

  min.x = minX;
  min.y = minY;
  min.z = minZ;
  max.x = maxX;
  max.y = maxY;
  max.z = maxZ;
}

void Shape::resize() {
  float minX, minY, minZ;
  float maxX, maxY, maxZ;
  float scaleX, scaleY, scaleZ;
  float shiftX, shiftY, shiftZ;
  float epsilon = 0.001f;

  minX = minY = minZ = 1.1754E+38F;
  maxX = maxY = maxZ = -1.1754E+38F;

  // Go through all vertices to determine min and max of each dimension
  for (int i = 0; i < obj_count; i++)
    for (size_t v = 0; v < posBuf[i].size() / 3; v++) {
      if (posBuf[i][3 * v + 0] < minX)
        minX = posBuf[i][3 * v + 0];
      if (posBuf[i][3 * v + 0] > maxX)
        maxX = posBuf[i][3 * v + 0];

      if (posBuf[i][3 * v + 1] < minY)
        minY = posBuf[i][3 * v + 1];
      if (posBuf[i][3 * v + 1] > maxY)
        maxY = posBuf[i][3 * v + 1];

      if (posBuf[i][3 * v + 2] < minZ)
        minZ = posBuf[i][3 * v + 2];
      if (posBuf[i][3 * v + 2] > maxZ)
        maxZ = posBuf[i][3 * v + 2];
    }

  // From min and max compute necessary scale and shift for each dimension
  float maxExtent, xExtent, yExtent, zExtent;
  xExtent = maxX - minX;
  yExtent = maxY - minY;
  zExtent = maxZ - minZ;
  if (xExtent >= yExtent && xExtent >= zExtent) {
    maxExtent = xExtent;
  }
  if (yExtent >= xExtent && yExtent >= zExtent) {
    maxExtent = yExtent;
  }
  if (zExtent >= xExtent && zExtent >= yExtent) {
    maxExtent = zExtent;
  }
  scaleX = 2.0f / maxExtent;
  shiftX = minX + (xExtent / 2.0f);
  scaleY = 2.0f / maxExtent;
  shiftY = minY + (yExtent / 2.0f);
  scaleZ = 2.0f / maxExtent;
  shiftZ = minZ + (zExtent / 2.0f);

  // Go through all verticies shift and scale them
  for (int i = 0; i < obj_count; i++)
    for (size_t v = 0; v < posBuf[i].size() / 3; v++) {
      posBuf[i][3 * v + 0] = (posBuf[i][3 * v + 0] - shiftX) * scaleX;
      assert(posBuf[i][3 * v + 0] >= -1.0f - epsilon);
      assert(posBuf[i][3 * v + 0] <= 1.0f + epsilon);
      posBuf[i][3 * v + 1] = (posBuf[i][3 * v + 1] - shiftY) * scaleY;
      assert(posBuf[i][3 * v + 1] >= -1.0f - epsilon);
      assert(posBuf[i][3 * v + 1] <= 1.0f + epsilon);
      posBuf[i][3 * v + 2] = (posBuf[i][3 * v + 2] - shiftZ) * scaleZ;
      assert(posBuf[i][3 * v + 2] >= -1.0f - epsilon);
      assert(posBuf[i][3 * v + 2] <= 1.0f + epsilon);
    }
}

void Shape::init() {
  for (int i = 0; i < obj_count; i++)

  {

    // Initialize the vertex array object
    glGenVertexArrays(1, &vaoID[i]);
    glBindVertexArray(vaoID[i]);

    // Send the position array to the GPU
    glGenBuffers(1, &posBufID[i]);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID[i]);
    glBufferData(GL_ARRAY_BUFFER, posBuf[i].size() * sizeof(float),
                 posBuf[i].data(), GL_STATIC_DRAW);

    // Send the normal array to the GPU
    if (norBuf[i].empty()) {
      norBufID[i] = 0;
    } else {
      glGenBuffers(1, &norBufID[i]);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID[i]);
      glBufferData(GL_ARRAY_BUFFER, norBuf[i].size() * sizeof(float),
                   norBuf[i].data(), GL_STATIC_DRAW);
    }

    // Send the texture array to the GPU
    if (texBuf[i].empty()) {
      texBufID[i] = 0;
    } else {
      glGenBuffers(1, &texBufID[i]);
      glBindBuffer(GL_ARRAY_BUFFER, texBufID[i]);
      glBufferData(GL_ARRAY_BUFFER, texBuf[i].size() * sizeof(float),
                   texBuf[i].data(), GL_STATIC_DRAW);
    }

    // Send the element array to the GPU
    glGenBuffers(1, &eleBufID[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 eleBuf[i].size() * sizeof(unsigned int), eleBuf[i].data(),
                 GL_STATIC_DRAW);

    // Unbind the arrays
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    assert(glGetError() == GL_NO_ERROR);
  }
}
void Shape::draw(const shared_ptr<Program> prog,
                 bool use_extern_texures) const {

  //prog->bind();
  for (int i = 0; i < obj_count; i++)

  {
    //cout << i << endl;
    int h_pos, h_nor, h_tex, h_col;
    h_pos = h_nor = h_tex = h_col = -1;

    glBindVertexArray(vaoID[i]);
    // Bind position buffer
    h_pos = prog->getAttribute("vertPos");
    GLSL::enableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID[i]);
    glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);

    // Bind normal buffer
    h_nor = prog->getAttribute("vertNor");
    if (h_nor != -1 && norBufID[i] != 0) {
      GLSL::enableVertexAttribArray(h_nor);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID[i]);
      glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    }

    if (texBufID[i] != 0) {
      // Bind texcoords buffer
      h_tex = prog->getAttribute("vertTex");
      if (h_tex != -1 && texBufID[i] != 0) {
        GLSL::enableVertexAttribArray(h_tex);
        glBindBuffer(GL_ARRAY_BUFFER, texBufID[i]);
        glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
      }
    }

    // Bind element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID[i]);

    // texture
    if (!use_extern_texures) {

      //cout << mats.size() << endl;
      int textureindex = materialIDs[i];
      if (textureindex >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIDs[textureindex]);
      }
      if (mats.size() > i) {
          glm::vec4 diffuse = glm::vec4(mats[i].diffuse[0], mats[i].diffuse[1], mats[i].diffuse[2], 1);
          glUniform4fv(prog->getUniform("objColor"), 1, &diffuse[0]);
          /*
          if (i == 0)
            glUniform3fv(prog->getUniform("objColor"), 1, &mats[mats.size() - 1].diffuse[0]);
          else
            glUniform3fv(prog->getUniform("objColor"), 1, &mats[i-1].diffuse[0]);
          */
      }
    }

    // color
    // Draw
    glDrawElements(GL_TRIANGLES, (int)eleBuf[i].size(), GL_UNSIGNED_INT,
                   (const void *)0);

    // Disable and unbind
    if (h_tex != -1) {
      GLSL::disableVertexAttribArray(h_tex);
    }
    if (h_nor != -1) {
      GLSL::disableVertexAttribArray(h_nor);
    }
    GLSL::disableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //prog->unbind();
  }
}

void Shape::draw(const std::shared_ptr<Program> prog, int object, bool use_extern_textures) const {
    //cout << i << endl;
    int h_pos, h_nor, h_tex;
    h_pos = h_nor = h_tex = -1;

    glBindVertexArray(vaoID[object]);
    // Bind position buffer
    h_pos = prog->getAttribute("vertPos");
    GLSL::enableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID[object]);
    glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);

    // Bind normal buffer
    h_nor = prog->getAttribute("vertNor");
    if (h_nor != -1 && norBufID[object] != 0) {
      GLSL::enableVertexAttribArray(h_nor);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID[object]);
      glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    }

    if (texBufID[object] != 0) {
      // Bind texcoords buffer
      h_tex = prog->getAttribute("vertTex");
      if (h_tex != -1 && texBufID[object] != 0) {
        GLSL::enableVertexAttribArray(h_tex);
        glBindBuffer(GL_ARRAY_BUFFER, texBufID[object]);
        glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
      }
    }

    // Bind element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID[object]);

    // texture

    if (!use_extern_textures) {
      int textureindex = materialIDs[object];
      if (textureindex >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIDs[textureindex]);
      }
    }

    // color
    //glUniform3fv(prog->getUniform("objColor"), 1, &objMaterials[i].ambient[0]);
    // Draw
    glDrawElements(GL_TRIANGLES, (int)eleBuf[object].size(), GL_UNSIGNED_INT,
                   (const void *)0);

    // Disable and unbind
    if (h_tex != -1) {
      GLSL::disableVertexAttribArray(h_tex);
    }
    if (h_nor != -1) {
      GLSL::disableVertexAttribArray(h_nor);
    }
    GLSL::disableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

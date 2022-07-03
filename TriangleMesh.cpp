/* -------------------------------------------------------------
   name:           TriangleMesh.cpp
   purpose:        cg1_ex1 triangle mesh for OpenGL rendering
   version:	   STARTER CODE
   author:         katrin lang
                   computer graphics
                   htw berlin
   ------------------------------------------------------------- */

#include "TriangleMesh.hpp"

// use this with care
// might cause name collisions
using namespace glm;

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <limits>

#include "GLError.hpp"

using namespace std;

// NVIDIA wants it like this
// see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
const GLuint TriangleMesh::attribPosition= 0;
const GLuint TriangleMesh::attribNormal= 2;
const GLuint TriangleMesh::attribColor= 3;
const GLuint TriangleMesh::attribTexCoord= 8;

TriangleMesh::TriangleMesh() : winding(CW){
  
  vbo[0]=0;
  vbo[1]=0;
  vbo[2]=0;
}

TriangleMesh::~TriangleMesh(){
 
    clearBuffers();
}

void TriangleMesh::clearBuffers(void){

  if(vbo[0] && glIsBuffer(vbo[0])) glDeleteBuffers(1, &vbo[0]);
  vbo[0]= 0;
  if(vbo[1] && glIsBuffer(vbo[1])) glDeleteBuffers(1, &vbo[1]);
  vbo[1]= 0; 
  if(vbo[2] && glIsBuffer(vbo[2])) glDeleteBuffers(1, &vbo[2]);
  vbo[2]= 0;  
}

TriangleMesh::Segment::~Segment(){
  
  //  if(vao && glIsVertexArray(vao)) glDeleteVertexArrays(1, &vao);
  //if(ibo && glIsBuffer(ibo)) glDeleteBuffers(1, &ibo);
}

float TriangleMesh::getBoundingSphereRadius(void){
  return boundingSphereRadius;
}

pair<vec3, vec3> TriangleMesh::getBoundingBox(void){
  return pair<vec3, vec3> (boundingBoxMin, boundingBoxMax);
}

Material TriangleMesh::getMaterial(string name){
  if(name == "default"){
    return {name, vec4(1), vec4(1), vec4(1), 1};
  }
  return materials[name];
}

void TriangleMesh::load(const std::string& fileName, bool unitize, bool center){
  
  this->name= fileName;
  
  clearBuffers();

  positions.clear();
  normals.clear();
  texCoords.clear();
  faces.clear();

  groups.clear();
  materials.clear();
  
  if(fileName.size() > 3 && fileName.compare(fileName.size()-4, 4, ".off")==0)
    loadOff(fileName);
  else if(fileName.size() > 3 && fileName.compare(fileName.size()-4, 4, ".obj")==0)
    loadObj(fileName);
  else{
    cerr << "Unknown file format for triangle mesh. Must be .off or .obj." << endl;
    return;
  }

  if(center) this->center();
  if(unitize) this->unitize();
  computeBoundingSphere();
  computeBoundingBox();
  if(normals.size()==0) computeNormals();
  if(texCoords.size()==0) computeSphereUVs();
  uploadVertexBuffers();

  vector<Group>::iterator group= begin();
  while(group != end()){
    vector<Segment>::iterator segment= group->begin();
    while(segment != group->end()){
      segment->uploadIndexBuffer();
      segment->createVAO(vbo);
      segment++;
    }
    group++;
  }
}

void TriangleMesh::setWinding(PolygonWinding winding){
  this->winding= winding;
}

// load a model	
void TriangleMesh::loadObj(const string &filename){

	    /** 
	     * .obj file format
	     * 
	     * '#'  -  comment
	     * 'v'  -  vertex position (3 floats separated by whitespace)
	     * 'vt' -  texture coordinates (2 floats separated by whitespace)
	     * 'vn' -  vertex normal (3 floats separated by whitespace)
	     * 'f'  -  faces, n times 3 v/vt/vn indices separated by /  and whitespace: v/t/n v/t/n ... v/t/n
	     * 'g'  -  group, 
	     */
	 
	    const string POSITION= "v";
	    const string TEX_COORD= "vt";
	    const string NORMAL= "vn";
	    const string FACE= "f";
	    const string GROUP= "g";
	    const string USE_MTL= "usemtl";
	    const string MTL_GROUP= "group";
	    const string MTL_LIB= "mtllib";

	    groups.clear();
	    materials.clear();
	    
	    string material= "default";
	    string texture= "off";

	    groups.push_back(Group(0, "default", material, texture));
	    std::vector<Group>::iterator group= groups.begin();
	    
	    boundingSphereRadius= 0;

	    ifstream file;
	    file.open(filename.c_str());
	    assert(file.is_open());
	    
	    string l;

	    while(getline(file, l)){  

	      stringstream line(l);
	      string type;
	      line >> type;

	      if(type == POSITION){

		float x, y, z;
		line >> x >> y >> z;
		positions.push_back(vec3(x,y,z));
	      }
	      else if(type == TEX_COORD){

		float u, v;
		line >> u >> v;
		texCoordsRAW.push_back(vec2(u,v));
	      }
	      else if(type == NORMAL){

		float nx, ny, nz;
		line >> nx >> ny >> nz;
		normalsRAW.push_back(vec3(nx, ny, nz));
	      }
	      else if(type == FACE){

		vector<uint> positionIndices;
		vector<uint> texCoordIndices;
		vector<uint> normalIndices;
		
		GLuint positionIndex, texCoordIndex, normalIndex;
		char slash;

		while(line.good()){

		  // v v v
		  if(line.str().find('/') == string::npos){
		    line >> positionIndex;
		    positionIndices.push_back(positionIndex-1);
		  }
		  // v//n v//n v//n
		  else if(line.str().find("//") != string::npos){

		    line >> positionIndex >> slash >> slash >> normalIndex;
		    positionIndices.push_back(positionIndex-1);
		    normalIndices.push_back(normalIndex-1);
		    }
		  else{
		    // v/t v/t v/t
		    line >> positionIndex >> slash >> texCoordIndex;
		    positionIndices.push_back(positionIndex-1);
		    texCoordIndices.push_back(texCoordIndex-1);
		    // v/t/n v/t/n v/t/n
		    if(line.peek() == '/'){
		      line >> slash >> normalIndex;
		      normalIndices.push_back(normalIndex-1);
		    }
		  }
		} // line end
		triangulate(positionIndices, texCoordIndices, normalIndices);
	      } // end face
	      else if(type == GROUP){
		
		if(faces.size()==0) groups.pop_back();
		else group->end(faces.size());
		string name;
		line >> name;
		group= find_if(groups.begin(), groups.end(), [&] (Group g){ return g.name == name; });
		if(group== groups.end()){
		  groups.push_back(Group(faces.size(), name, material, texture));
		  group= --groups.end();
		}
		else group->begin(faces.size());
		
	      } // end group
	      else if(type == MTL_GROUP){ // materials separately specified

		group->end(faces.size());
		string name;
	      	line >> name;
		group= find_if(groups.begin(), groups.end(), [&] (Group g){ return g.name == name; });
		assert(group!=groups.end());
	      }
	      else if(type == USE_MTL){

	      	line >> material;
	        group->addSegment(faces.size(), material, texture);
	      }
	      else if(type == MTL_LIB){
		
	      	string dir, library;
		stringstream name(filename);
		getline(name, dir, '/');
	      	line >> library;
		name << dir << '/' << library;
		
	      	parseMaterials(name.str());
	      }
	    } // eof

	    group->end(faces.size());
	    
	    file.close();
	    
	    cout << "loaded " << filename << ": " 
		 << positions.size() << " vertices, " << texCoordsRAW.size() << " texture coordinates, " << normalsRAW.size() << " normals, " 
		 << faces.size() << " faces, " << groups.size() << " groups, " << materials.size() << " materials" << endl;	    
	    
	    // bring to format opengl eats
	    // this means possible duplication of normals 
		 // and / or texture coordinates
		 clean();

	    for(int i= 0; i<groups.size(); i++){
	      groups[i].computeElements(faces);
	    }	    	    
}
	    
// parse a material library	
void TriangleMesh::parseMaterials(string filename){
	 
  const string NEW_MATERIAL= "newmtl";
  const string MATERIAL_AMBIENT= "Ka";
  const string MATERIAL_DIFFUSE= "Kd";
  const string MATERIAL_SPECULAR= "Ks";
  const string SPECULAR_EXPONENT= "Ns";

	    ifstream file;
	    file.open(filename.c_str());
	    
	    string l, current;

	    while(getline(file, l)){  

	      stringstream line(l);
	      string type;
	      line >> type;

	      if(type == NEW_MATERIAL){
		
		string name;
		line >> name;
		Material material;
		material.name= name;
		materials[name]= material;
		current= name;
	      }
	      else if(type == MATERIAL_AMBIENT){

		float r, g, b;
		line >> r >> g >> b;
		vec4 ambient= vec4(r,g,b,1.0f);
		materials[current].ambient= ambient;
	      }
	      else if(type == MATERIAL_DIFFUSE){

		float r, g, b;
		line >> r >> g >> b;
		vec4 diffuse= vec4(r,g,b,1.0f);
		materials[current].diffuse= diffuse;
	      }
	      else if(type == MATERIAL_SPECULAR){

		float r, g, b;
		line >> r >> g >> b;
		vec4 specular= vec4(r,g,b,1.0f);
		materials[current].specular= specular;
	      }
	      else if(type == SPECULAR_EXPONENT){

		float exponent;
		line >> exponent;
		materials[current].shininess= exponent;
	      }
	    } // eof
	    file.close();
}

// TODO: load triangle mesh in OFF format
void TriangleMesh::loadOff(const string& fileName){
  
  // Open the file
  std::ifstream file(fileName.c_str());
  assert(file.is_open());

  groups.push_back(Group(0, "default", "default", "off"));
 
  std::string magic;

  // Read the first line, must be "OFF"
  file >> magic;
  assert(magic == "OFF");

  // number of vertices, faces and edges (unused)
  int V, F, E;
  file >> V >> F >> E;

  assert(file.good());

  // read all the vertices positions
  positions.resize(V);
  for (int i = 0; i < V; i++){

    float x,y,z;

    file >> x >> y >> z;

    assert(file.good());

    positions[i]= vec3(x, y, z);
  }

  // read number of vertices in the faces and the indices of the vertices
  faces.resize(F);
  for (int i = 0; i < F; i++){
    
    int n, a, b, c;
    
    file >> n >> a >> b >> c;
    
    assert(n == 3);
    assert(file.good());

    if(winding==CW)
      faces[i]= uvec3(a, b, c);
    else
      faces[i]= uvec3(a, c, b);
  }
  
  // close file
  file.close();

  groups.back().end(faces.size());

  for(int i= 0; i<groups.size(); i++){
    groups[i].computeElements(faces);
  }
  
  cout << "Mesh: reading off file done. |V|=" << positions.size() << " |F|=" << faces.size() << endl;
}


// center the mesh at its origin
void TriangleMesh::center(void){

  computeBoundingBox();

  vec3 center= (boundingBoxMin + boundingBoxMax) * vec3(0.5);
  
  for(unsigned int i= 0; i<positions.size(); i++){  
    positions[i]-= center;
  }
  boundingBoxMin-= center;
  boundingBoxMax-= center;
}

// scale to size 1
void TriangleMesh::unitize(void){

  computeBoundingSphere();

  for(unsigned int i= 0; i<positions.size(); ++i){
    positions[i]/= boundingSphereRadius;
  }
  boundingSphereRadius= 1;
  boundingBoxMin= vec3(-1);
  boundingBoxMax= vec3(1);
}

// compute bounding sphere
void TriangleMesh::computeBoundingSphere(void){

  boundingSphereRadius= 0;
  for(unsigned int i= 0; i<positions.size(); i++){
    vec3 v= positions[i];
    if(length(v) > boundingSphereRadius) boundingSphereRadius= length(v);
  }
}

// compute bounding box
void TriangleMesh::computeBoundingBox(void){

  boundingBoxMin= vec3(numeric_limits<float>::max());
  boundingBoxMax= vec3(numeric_limits<float>::min());
  for(unsigned int i= 0; i<positions.size(); i++){
    if(positions[i].x < boundingBoxMin.x) boundingBoxMin.x= positions[i].x;
    if(positions[i].x > boundingBoxMax.x) boundingBoxMax.x= positions[i].x;
    if(positions[i].y < boundingBoxMin.y) boundingBoxMin.y= positions[i].y;
    if(positions[i].y > boundingBoxMax.y) boundingBoxMax.y= positions[i].y;
    if(positions[i].z < boundingBoxMin.z) boundingBoxMin.z= positions[i].z;
    if(positions[i].z > boundingBoxMax.z) boundingBoxMax.z= positions[i].z;
  }
}

 // triangulate a polygon
void TriangleMesh::triangulate(vector<uint> pi, vector<uint> ti, vector<uint> ni){

  for(unsigned int v= 2; v<pi.size(); v++){
    if(winding==CW)
      faces.push_back(uvec3(pi[0], pi[v-1], pi[v]));
    else
      faces.push_back(uvec3(pi[0], pi[v], pi[v-1]));
  }

  for(size_t v= 2; v<ti.size(); v++){
    if(winding==CW)
      texCoordIndices.push_back(uvec3(ti[0], ti[v-1], ti[v]));
    else
      texCoordIndices.push_back(uvec3(ti[0], ti[v], ti[v-1]));
  }

  for(unsigned int v= 2; v<ni.size(); v++){
    if(winding==CW)
      normalIndices.push_back(uvec3(ni[0], ni[v-1], ni[v]));
    else
      normalIndices.push_back(uvec3(ni[0], ni[v], ni[v-1]));
      
  }
}

// bring to format opengl eats
// this means possible duplication of normals 
   // and / or texture coordinates
void TriangleMesh::clean(void){
  
  if(normalIndices.size()>0){
    normals.clear();
    normals.resize(positions.size());
    for(unsigned int i= 0; i<normalIndices.size(); i++){    
      normals[faces[i][0]]= normalsRAW[normalIndices[i][0]];
      normals[faces[i][1]]= normalsRAW[normalIndices[i][1]];
      normals[faces[i][2]]= normalsRAW[normalIndices[i][2]];
    }
    normalsRAW.clear();
    normalIndices.clear();
  }

  if(texCoordIndices.size()>0){
    texCoords.clear();
    texCoords.resize(positions.size());
    for(unsigned int i= 0; i<texCoordIndices.size(); i++){
      texCoords[faces[i][0]]= texCoordsRAW[texCoordIndices[i][0]];
      texCoords[faces[i][1]]= texCoordsRAW[texCoordIndices[i][1]];
      texCoords[faces[i][2]]= texCoordsRAW[texCoordIndices[i][2]];
    }
    texCoordsRAW.clear();
    texCoordIndices.clear();
  }
}


/**
 * compute smooth per-vertex normals
 * by means of simple averaging
 **/
void TriangleMesh::computeNormals(void){
 
  normals.clear();
  normals.resize(positions.size());

  for(size_t f= 0; f<faces.size(); f++){

    // face vertices
    vec3 v0= positions[faces[f].x];
    vec3 v1= positions[faces[f].y];
    vec3 v2= positions[faces[f].z];

    // edges
    vec3 edge0= normalize(v0 - v1);
    vec3 edge1= normalize(v1 - v2);

    // face normal
    vec3 n= normalize(cross(edge0, edge1));

    // push out on vertices
    normals[faces[f][0]]+= n;
    normals[faces[f][1]]+= n;   
    normals[faces[f][2]]+= n;
  }

  //normalize
  for(size_t i= 0; i< normals.size(); i++){
    normals[i]= normalize(normals[i]);
  }
}

// Compute uv coordinates with a spherical mapping
// (vertices are projected on a sphere along the normal and classical sphere uv unwrap is used)
void TriangleMesh::computeSphereUVs(void){

  texCoords.resize(positions.size());
  
  // for each vertex
  for(unsigned int i = 0; i < positions.size(); i++){

    float x= positions[i].z;
    float y= positions[i].x;
    float z= -positions[i].y;

    float radius= glm::sqrt(x*x + y*y + z*z);
    float theta= acos(z/radius);
    float phi= atan2(y, x);
    
    texCoords[i]= vec2((pi<float>()+phi)/(2*pi<float>()), theta/pi<float>());
  }

  // texture coordinate correction

  vec3 u;

  for(unsigned int f = 0; f < faces.size(); ++f){

    uvec3 face= faces[f];

    u[0]= texCoords[face[0]].x;
    u[1]= texCoords[face[1]].x;
    u[2]= texCoords[face[2]].x;

    float threshold = 0.7f;

    if (fabs(u[0] - u[1]) > threshold || fabs(u[0] - u[2]) > threshold || fabs(u[1] - u[2]) > threshold){

      float middle = 0.5f;

      if (u[0] > middle && u[1] > middle && u[2] > middle)
	continue;

      for(unsigned int i= 0; i<3; i++){

	if (u[i] > middle){
	  positions.push_back(positions[face[i]]);
	  normals.push_back(normals[face[i]]);
	  texCoords.push_back(vec2(u[i] - 1, texCoords[face[i]].y));
	  faces[f][i] = positions.size() - 1;
	}
      }
    }
  }
}

// called whenever a new mesh is loaded
void TriangleMesh::uploadVertexBuffers(void){
  
  // vertex buffers 
  glGenBuffers(3, &vbo[0]);
  assert(vbo[0]);
  assert(vbo[1]);
  assert(vbo[2]);
  
  glBindVertexArray(0);
  
  // bind buffer on GPU which is to receive position data
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);         
  
  // specify from where to read the position data and how many bytes to read
  glBufferData(GL_ARRAY_BUFFER,
	       positions.size() * sizeof(vec3),  // how many bytes?
	       &positions[0],                    // location of position data on host
	       GL_STATIC_DRAW);                  // no animation (hint)

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);            

  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, texCoords.size()*sizeof(vec2), &texCoords[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}  

void TriangleMesh::Segment::uploadIndexBuffer(){
  
  // create index buffer
  glGenBuffers(1, &ibo);
  assert(ibo);

  // bind buffer on GPU which is to receive index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  // specify buffer data for triangles
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	       elements.size() * sizeof(uvec3),     // how many bytes?  
	       &elements[0],                        // location of index data on host
	       GL_STATIC_DRAW);                     // no animation (hint)
}

void TriangleMesh::Segment::createVAO(unsigned int vbo[]){
  
  // create vertex array object
  // this is just a unique id
  glGenVertexArrays(1, &vao);
  assert(vao);

  // bind vertex array object for first time
  // and begin record
  glBindVertexArray(vao);
  
  // activate vertex attribute position
  // -> values in the vertex attribute array associated with vertex attribute "attribPosition" will be used for rendering
  // this call modifies the state of the currently bound vertex array object
  glEnableVertexAttribArray(attribPosition);
  
  // bind vertex position buffer
  // vao will "record" this call and repeat it whenever the vao is bound
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  
  // how is the buffer data to be interpreted?
  glVertexAttribPointer(attribPosition,          // vertex attribute to associate position data with
			3,                       // size of n-tuple (here: vec3) making up a position
			GL_FLOAT,                // data type of elements in n-tuple
			GL_FALSE,                // normalize?
			0,                       // stride
			(GLvoid*) 0);            // a value of 0 tells opengl to use data from currently bound buffer

  
  // enable vertex attribute array associated with vertex attribute normal
  glEnableVertexAttribArray(attribNormal);
  // bind vertex normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  // how is the buffer data to be interpreted?
  glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_TRUE, 0, (GLvoid*) 0);

  // enable vertex attribute array associated with vertex attribute texture
  glEnableVertexAttribArray(attribTexCoord);
  // bind vertex texture buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  // how is the buffer data to be interpreted?
  glVertexAttribPointer(attribTexCoord, 2, GL_FLOAT, GL_FALSE, 0,  (GLvoid*) 0);

  // bind triangle index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  
  // unbind vertex array object and end record
  glBindVertexArray(0);
}

void TriangleMesh::Segment::draw() {
  
  glBindVertexArray(vao);
  
  glDrawElements(GL_TRIANGLES, elements.size() * 3, GL_UNSIGNED_INT, (GLvoid*) 0);
  
  glBindVertexArray(0);
}

// called every frame
void TriangleMesh::draw(){
  
  vector<Group>::iterator group= begin();
  while(group != end()){
    vector<Segment>::iterator segment= group->begin();
    while(segment != group->end()){
      segment->draw();
      segment++;
    }
    group++;
  }
}

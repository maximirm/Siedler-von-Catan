/* ----------------------------------------------------------------
   name:           GLSL.cpp
   purpose:        cg1_ex3 OpenGL shading language wrapper
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include "GLSL.hpp"

#define DEBUG // (enable/disable logging)
#include "Log.hpp"

#include "glm/gtc/matrix_transform.hpp"

using namespace std;

glsl::Shader::Shader(){
  program= 0;
}

glsl::Shader::~Shader(){

  for(unsigned int i= 0; i<vertexShaders.size(); i++)
    glDeleteShader(vertexShaders[i]);
  for(unsigned int i= 0; i<geometryShaders.size(); i++)
    glDeleteShader(geometryShaders[i]);
  for(unsigned int i= 0; i<fragmentShaders.size(); i++)
    glDeleteShader(fragmentShaders[i]);
  glDeleteProgram(program);
}

// load vertex shader from file
void glsl::Shader::loadVertexShader(const string& fileName){

  LOG << "loading " << fileName << "...";
  vertexCode.push_back(load(fileName));
  LOG << " done." << endl;
}

// load geometry shader from file
void glsl::Shader::loadGeometryShader(const string& fileName){
  
  LOG << "loading " << fileName << "...";
  geometryCode.push_back(load(fileName));
  LOG << " done." << endl;
}
// load fragment shader from file
void glsl::Shader::loadFragmentShader(const string& fileName){

  LOG << "loading " << fileName << "...";
  fragmentCode.push_back(load(fileName));
  LOG << " done." << endl;
}

// add vertex shader from string
void glsl::Shader::addVertexShader(const string& code){

  vertexCode.push_back(code);
}

// add geometry shader from string
void glsl::Shader::addGeometryShader(const string& code){
  
  geometryCode.push_back(code);
}
// add fragment shader from string
void glsl::Shader::addFragmentShader(const string& code){

  fragmentCode.push_back(code);
}

// compile vertex shader
void glsl::Shader::compileVertexShader(void){
  LOG << "compiling vertex shader... ";
  if(!program) program = glCreateProgram();
  GLuint id= compile(GL_VERTEX_SHADER, vertexCode);
  vertexShaders.push_back(id);
  if(id){
    glAttachShader(program, id);
    LOG << "done." << endl;
  }
  vertexCode.clear();
}

// compile geometry shader
void glsl::Shader::compileGeometryShader(void){
  LOG << "compiling geometry shader... ";
  if(!program) program = glCreateProgram();
  GLuint id= compile(GL_GEOMETRY_SHADER, geometryCode);
  geometryShaders.push_back(id);
  if(id){
    glAttachShader(program, id);
    LOG << "done." << endl;
  }
  geometryCode.clear();
}

// load fragment shader
void glsl::Shader::compileFragmentShader(void){
  LOG << "compiling fragment shader... ";
  if(!program) program = glCreateProgram();
  GLuint id= compile(GL_FRAGMENT_SHADER, fragmentCode);
  fragmentShaders.push_back(id);
  if(id){
    glAttachShader(program, id);
    LOG << "done." << endl;
  }
  fragmentCode.clear();
}

void glsl::Shader::setGeometryShaderInOut(GLuint in, GLuint out, int count){
  if(!program) program = glCreateProgram();
  glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, in);
  glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, out);
  glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, count);
}

void glsl::Shader::link(void){
  LOG << "linking... ";
  glLinkProgram(program);
  if(!printProgramLog(program)) LOG << "done." << endl;
}

void glsl::Shader::bind()const{

  glUseProgram(program);
}

void glsl::Shader::unbind()const{

  glUseProgram(0);
}

void glsl::Shader::setUniform(const string& name, const float& value){

  int id= getUniformBinding(name);
  glUniform1f(id, value);
}

void glsl::Shader::setUniform(const string& name, const int& value){

  int id= getUniformBinding(name);
  glUniform1i(id, value);
}

void glsl::Shader::setUniform(const string& name, const unsigned int& value){

  int id= getUniformBinding(name);
  glUniform1ui(id, value);
}

// Set a 2 component float vector uniform parameter
void glsl::Shader::setUniform(const string& name, const float& x, const float& y){

  int id= getUniformBinding(name);
  glUniform2f(id, x, y);
}

// Set a 2 component integer vector uniform parameter
void glsl::Shader::setUniform(const string& name, const int& x, const int& y){

  int id= getUniformBinding(name);
  glUniform2i(id, x, y);
}

// Set a 2 component undigned integer vector uniform parameter
void glsl::Shader::setUniform(const string& name, const unsigned int& x, const unsigned int& y){

  int id= getUniformBinding(name);
  glUniform2ui(id, x, y);
}


// Set a 3 component float vector uniform parameter
void glsl::Shader::setUniform(const string& name, const float& x, const float& y, const float& z){

  int id= getUniformBinding(name);
  glUniform3f(id, x, y, z);
}

// Set a 3 component integer vector uniform parameter
void glsl::Shader::setUniform(const string& name, const int& x, const int& y, const int& z){

  int id= getUniformBinding(name);
  glUniform3i(id, x, y, z);
}

// Set a 3 component unsigned integer vector uniform parameter
void glsl::Shader::setUniform(const string& name, const unsigned int& x, const unsigned int& y, const unsigned int& z){

  int id= getUniformBinding(name);
  glUniform3ui(id, x, y, z);
}

// Set a 4 component float vector uniform parameter
void glsl::Shader::setUniform(const string& name, const float& x, const float& y, const float& z, const float& w){

  int id= getUniformBinding(name);
  glUniform4f(id, x, y, z, w);
}

// Set a 4 component integer vector uniform parameter
void glsl::Shader::setUniform(const string& name, const int& x, const int& y, const int& z, const int& w){

  int id= getUniformBinding(name);
  glUniform4i(id, x, y, z, w);
}

// Set a 4 component unsigned integer vector uniform parameter
void glsl::Shader::setUniform(const string& name, const unsigned int& x, const unsigned int& y, const unsigned int& z, const unsigned int& w){

  int id= getUniformBinding(name);
  glUniform4ui(id, x, y, z, w);
}

void glsl::Shader::setUniform(const string& name, const glm::vec2& value){

  int id= getUniformBinding(name);
  glUniform2fv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::ivec2& value){

  int id= getUniformBinding(name);
  glUniform2iv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::uvec2& value){

  int id= getUniformBinding(name);
  glUniform2uiv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::vec3& value){

  int id= getUniformBinding(name);
  glUniform3fv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::ivec3& value){

  int id= getUniformBinding(name);
  glUniform3iv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::uvec3& value){

  int id= getUniformBinding(name);
  glUniform3uiv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::vec4& value){

  int id= getUniformBinding(name);
  glUniform4fv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::ivec4& value){

  int id= getUniformBinding(name);
  glUniform4iv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::uvec4& value){

  int id= getUniformBinding(name);
  glUniform4uiv(id, 1, &value[0]);
}

void glsl::Shader::setUniform(const string& name, const glm::mat2& value){

  int id= getUniformBinding(name);
  glUniformMatrix2fv(id, 1, false, &value[0][0]);
}

void glsl::Shader::setUniform(const string& name, const glm::mat2x3& value){

  int id= getUniformBinding(name);
  glUniformMatrix2x3fv(id, 2, false, &value[0][0]);
}

void glsl::Shader::setUniform(const string& name, const glm::mat3& value){

  int id= getUniformBinding(name);
  glUniformMatrix3fv(id, 1, false, &value[0][0]);
}

void glsl::Shader::setUniform(const string& name, const glm::mat4& value){

  int id= getUniformBinding(name);
  glUniformMatrix4fv(id, 1, false, &value[0][0]);
}

void glsl::Shader::bindVertexAttrib(const string& name, int id){

  glBindAttribLocation(program, id, name.c_str());
}

GLint glsl::Shader::getVertexAttribBinding(const string& name){

  return glGetAttribLocation(program, name.c_str());
}

void  glsl::Shader::bindOutput(const string& name, int id){
  glBindFragDataLocation(program, id, name.c_str());
}

GLint glsl::Shader::getUniformBinding(const string& name){
  return glGetUniformLocation(program, name.c_str());
}

string glsl::Shader::load(const std::string& file){

  ifstream input(file.c_str(), ios::binary);
  assert(input.is_open());

  string line, code;

  while(getline(input,line)){
    code += line;
    code += "\n";
  }
  assert(!code.empty());
  return code;
}

GLuint glsl::Shader::compile(GLuint type, const vector<string>& code){

  GLuint shader= glCreateShader(type);

  vector<const char*> codeFragments(code.size());
  for(unsigned int i= 0; i<code.size(); i++){
    codeFragments[i]= code[i].c_str();
  }
  
  glShaderSource(shader, code.size(), &codeFragments[0], 0);
  glCompileShader(shader);

  if(hasErrors(shader)){
    printShaderLog(shader);
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

bool glsl::Shader::hasErrors(GLuint shader){
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  return status != GL_TRUE;
}

bool glsl::Shader::printProgramLog(GLuint program){

  GLint infoLogLength = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

  // Some drivers return 1 as infoLog_length when the infoLog is an empty string
  if(infoLogLength > 1) {
    char* infoLog = new char[infoLogLength];
    glGetProgramInfoLog(program, infoLogLength, 0, infoLog);
    cerr << "\nprogram log:" << endl << infoLog << endl << endl;
    delete [] infoLog;
  }

  return infoLogLength > 1;
}

void glsl::Shader::printShaderLog(GLuint shader){

  GLint infoLogLength = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

  // Some drivers return 1 as infoLog_length when the infoLog is an empty string
  if(infoLogLength > 1){
    char* infoLog = new char[infoLogLength];
    glGetShaderInfoLog(shader, infoLogLength, 0, infoLog);
    cerr << "\nshader log:" << endl << infoLog << endl << endl;
    delete [] infoLog;
  }
}

void glsl::Shader::activateFeedbackVaryings(const std::vector< std::string > varyings){

  vector<const char*> varyings_raw(varyings.size());
  for(unsigned int i= 0; i<varyings.size(); i++){
    varyings_raw[i]= varyings[i].c_str();
  }
  glTransformFeedbackVaryings(program, varyings.size(), &varyings_raw[0], GL_INTERLEAVED_ATTRIBS);
}

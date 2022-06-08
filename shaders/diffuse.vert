uniform mat4 transformation;
uniform vec4 lightPosition;

attribute vec4 position;
attribute vec3 normal;
       
varying float shade;
       
void main(){

  vec3 lightDirection= normalize(lightPosition.xyz-position.xyz);

  shade= dot(normal, lightDirection);

  gl_Position= transformation*position;
}

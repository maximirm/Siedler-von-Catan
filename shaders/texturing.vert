// global variables
uniform mat4 modelViewProjectionMatrix;
uniform vec4 lightPosition;
uniform bool lighting;

// per-vertex attributes
attribute vec4 position;
attribute vec3 normal;
attribute vec2 texCoord;

// values sent to fragment shader
varying vec2 t;
varying float shade;

void main(void){

  t = texCoord;

  shade= 1.0;
  if(lighting){        
    vec3 lightDirection= normalize(lightPosition.xyz-position.xyz);
    shade= dot(normal, lightDirection);
  }
  gl_Position= modelViewProjectionMatrix*position;
}
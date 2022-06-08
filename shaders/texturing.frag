// global variables
uniform bool texturing;
uniform sampler2D texture;
// variables that are interpolated from the vertex shader
varying vec2 t;
varying float shade;

void main(void){

  vec4 color= vec4(vec3(shade), 1.0);
  if(texturing){
   color*= texture2D(texture, t);
  }
  gl_FragColor= color;
}
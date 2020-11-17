attribute highp vec3 posAttr;
uniform highp mat4 matrix;


void main(void)
{
    gl_Position = matrix * vec4(posAttr, 1.0);

}

attribute highp vec3 posAttr;

attribute highp vec3 normAttr;

uniform highp mat4 gWVP;
uniform highp mat4 gWorld;


out vec4 Normal0;
out vec4 WorldPos0;

void main()
{
        gl_Position = gWVP * vec4(posAttr, 1.0);
        //TexCoord0 = TexCoord;
        Normal0 = gWorld * vec4(normAttr, 0.0);
        WorldPos0 = gWorld * vec4(posAttr, 1.0);
}

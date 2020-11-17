//uniform sampler2D qt_Texture0;
//varying vec4 qt_TexCoord0;
uniform mediump vec4 colAttr;
void main(void)
{
    gl_FragColor = colAttr;
}

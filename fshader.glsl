#version 150

uniform sampler2D textureWorm;
uniform sampler2D textureTerrain;

in vec2 v_texcoord;
in vec4 v_position;
//! [0]
void main()
{
    // Set fragment color from texture
    if(texture2D(textureTerrain, v_texcoord).a != 1.0f)
        {
            discard;
        }
     gl_FragColor = texture2D(textureTerrain, v_texcoord);
}
//! [0]


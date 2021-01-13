#version 150

uniform sampler2D textureGrass;
uniform sampler2D textureRock;
uniform sampler2D textureSnow;
uniform sampler2D textureTerrain;
uniform bool has_heightmap;

in vec2 v_texcoord;
in vec4 v_position;
//! [0]
void main()
{
    // Set fragment color from texture
    if(has_heightmap){
        if(v_position.z <0.12f){
            gl_FragColor = texture2D(textureGrass, v_texcoord);
        }
        else if(v_position.z >0.25f){
            gl_FragColor = texture2D(textureSnow, v_texcoord);
        }
        else{
            gl_FragColor = texture2D(textureRock, v_texcoord);
        }
    }
    else{
            gl_FragColor = texture2D(textureTerrain, v_texcoord);

    }
}
//! [0]


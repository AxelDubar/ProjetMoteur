#version 150

uniform vec3 camera_pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 a_position;
in vec2 a_texcoord;

out vec2 v_texcoord;
out vec4 v_position;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    vec4 position=a_position;

    gl_Position = projection * view * model * position;


    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces

    v_texcoord = a_texcoord;
    v_position=position;
}
//! [0]

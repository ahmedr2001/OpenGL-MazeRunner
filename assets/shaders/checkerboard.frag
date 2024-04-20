#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    float row = floor(gl_FragCoord.x / size);
    float col = floor(gl_FragCoord.y / size);
    float sum = row + col;
    frag_color.xyz = (mod(sum, 2.0) == 0.0)? colors[0]:colors[1];
}
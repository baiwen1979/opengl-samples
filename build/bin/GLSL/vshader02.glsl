#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

void main() {

    gl_Position.xyz = vertexPosition_modelspace;
    //gl_Position.xyz = vec3(0.5 * vertexPosition_modelspace.x, 0.5 * vertexPosition_modelspace.y, vertexPosition_modelspace.z);
    gl_Position.w = 1.0;

}


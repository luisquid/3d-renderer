#include <stdio.h>
#include <string.h>
#include "array.h"
#include "mesh.h"
#include "utils.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0 },
    .scale = { 1.0, 1.0, 1.0 },
    .translation = { 0, 0, 0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

// face_t cube_faces[N_CUBE_FACES] = {
//     // front
//     { .a = 1, .b = 2, .c = 3 , .color = 0xFFFF0000},
//     { .a = 1, .b = 3, .c = 4, .color = 0xFFFF0000 },
//     // right
//     { .a = 4, .b = 3, .c = 5, .color = 0xFF00FF00 },
//     { .a = 4, .b = 5, .c = 6, .color = 0xFF00FF00 },
//     // back
//     { .a = 6, .b = 5, .c = 7, .color = 0xFF0000FF },
//     { .a = 6, .b = 7, .c = 8, .color = 0xFF0000FF },
//     // left
//     { .a = 8, .b = 7, .c = 2, .color = 0xFFFFFF00 },
//     { .a = 8, .b = 2, .c = 1, .color = 0xFFFFFF00 },
//     // top
//     { .a = 2, .b = 7, .c = 5, .color = 0xFFFF00FF },
//     { .a = 2, .b = 5, .c = 3, .color = 0xFFFF00FF },
//     // bottom
//     { .a = 6, .b = 8, .c = 1, .color = 0xFF00FFFF },
//     { .a = 6, .b = 1, .c = 4, .color = 0xFF00FFFF }
// };

// face_t cube_faces[N_CUBE_FACES] = {
//     // front
//     { .a = 1, .b = 2, .c = 3 , .color = 0xFFFFFFFF},
//     { .a = 1, .b = 3, .c = 4, .color = 0xFFFFFFFF },
//     // right
//     { .a = 4, .b = 3, .c = 5, .color = 0xFFFFFFFF },
//     { .a = 4, .b = 5, .c = 6, .color = 0xFFFFFFFF },
//     // back
//     { .a = 6, .b = 5, .c = 7, .color = 0xFFFFFFFF },
//     { .a = 6, .b = 7, .c = 8, .color = 0xFFFFFFFF },
//     // left
//     { .a = 8, .b = 7, .c = 2, .color = 0xFFFFFFFF },
//     { .a = 8, .b = 2, .c = 1, .color = 0xFFFFFFFF },
//     // top
//     { .a = 2, .b = 7, .c = 5, .color = 0xFFFFFFFF },
//     { .a = 2, .b = 5, .c = 3, .color = 0xFFFFFFFF },
//     // bottom
//     { .a = 6, .b = 8, .c = 1, .color = 0xFFFFFFFF },
//     { .a = 6, .b = 1, .c = 4, .color = 0xFFFFFFFF }
// };

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    { .a = 1, .b = 3, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    // right
    { .a = 4, .b = 3, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    { .a = 4, .b = 5, .c = 6, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    // back
    { .a = 6, .b = 5, .c = 7, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    { .a = 6, .b = 7, .c = 8, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    // left
    { .a = 8, .b = 7, .c = 2, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    { .a = 8, .b = 2, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    // top
    { .a = 2, .b = 7, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    { .a = 2, .b = 5, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    // bottom
    { .a = 6, .b = 8, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    { .a = 6, .b = 1, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF }
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }
    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void load_obj_file_data(char* filename)
{
    FILE* file; 
    file = fopen(filename, "r");

    char lines[4096]; 

    while(fgets(lines, 4096, file))
    {
        if(strncmp(lines, "v ", 2) == 0)
        {
            vec3_t vertex;

            sscanf(lines,
                "v %f %f %f", 
                &vertex.x, 
                &vertex.y, 
                &vertex.z);

            array_push(mesh.vertices, vertex);       
        }

        int vertexIndices[3];
        int textureIndices[3];
        int normalIndices[3];

        if(strncmp(lines, "f ", 2) == 0)
        {
            face_t face;
            sscanf(lines, 
                "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertexIndices[0], &textureIndices[0], &normalIndices[0],
                &vertexIndices[1], &textureIndices[1], &normalIndices[1],
                &vertexIndices[2], &textureIndices[2], &normalIndices[2]
            );

            
            face.a = vertexIndices[0];
            face.b = vertexIndices[1]; 
            face.c = vertexIndices[2];
            face.color = 0xFFFFFFFF;
            array_push(mesh.faces, face);
        }  
    }
}


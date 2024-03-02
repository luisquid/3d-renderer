#include <stdio.h>
#include <string.h>
#include "array.h"
#include "mesh.h"
#include "utils.h"

#define MAX_NUM_MESHES 10

static mesh_t meshes[MAX_NUM_MESHES];
static int mesh_count = 0;

int get_num_meshes(void){
    return mesh_count;
}

void load_mesh(char* mesh_filename, char* texture_filename, vec3_t scale, vec3_t position, vec3_t rotation){
    load_mesh_obj_data(&meshes[mesh_count], mesh_filename);
    load_mesh_texture_data(&meshes[mesh_count], texture_filename);

    meshes[mesh_count].scale = scale;
    meshes[mesh_count].translation = position;
    meshes[mesh_count].rotation = rotation;

    mesh_count++;
}

void load_mesh_obj_data(mesh_t* mesh, char* mesh_filename)
{
    FILE* file; 
    file = fopen(mesh_filename, "r");

    char lines[4096]; 
    
    tex2_t* texcoords = NULL;

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

            array_push(mesh->vertices, vertex);       
        }

        if(strncmp(lines, "vt ", 3) == 0){
            tex2_t texcoord;
            sscanf(lines, "vt %f %f", &texcoord.u, &texcoord.v);
            array_push(texcoords, texcoord);
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

            face.a = vertexIndices[0] - 1;
            face.b = vertexIndices[1] - 1; 
            face.c = vertexIndices[2] - 1;
            face.a_uv = texcoords[textureIndices[0] - 1];
            face.b_uv = texcoords[textureIndices[1] - 1];
            face.c_uv = texcoords[textureIndices[2] - 1];
            face.color = 0xFFFFFFFF;
            array_push(mesh->faces, face);
        }  
    }
    array_free(texcoords);
}

void load_mesh_texture_data(mesh_t* mesh, char* texture_filename){
    upng_t* png_texture = upng_new_from_file(texture_filename);
    if(png_texture != NULL){
        upng_decode(png_texture);
        if(upng_get_error(png_texture) == UPNG_EOK){
            mesh->texture = png_texture;
        }
    }
}

mesh_t* get_mesh(int index){
   return &meshes[index];
}

void free_meshes(void){
    for (int i = 0; i < mesh_count; i++)
    {
        array_free(meshes[i].faces);
        array_free(meshes[i].vertices);
        upng_free(meshes[i].texture);
    }
}

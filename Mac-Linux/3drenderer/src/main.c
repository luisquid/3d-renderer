#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "upng.h"
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "triangle.h"
#include "camera.h"

///////////////////////////////////////////////////////////////////////////////
// Array of triangles that should be rendered frame by frame
///////////////////////////////////////////////////////////////////////////////
#define MAX_TRIANGLES_PER_MESH 10000 
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];

int num_triangles_to_render = 0;

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0.0;

mat4_t proj_matrix;
mat4_t world_matrix;
mat4_t view_matrix;


///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
    render_mode = RENDER_TEXTURED;
    cull_method = CULL_BACKFACE;
    // Allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
    z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);

    // Creating a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

    // Initialize the perspective projection matrix
    float fov = M_PI / 3.0;
    float aspect = (float)window_height / (float)window_width;
    float znear = 0.1;
    float zfar = 100.0;
    proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

    // Loads the cube values in the mesh data structure
    //load_cube_mesh_data();

    // Loads the OBJ values to the mesh data structure
    load_obj_file_data("./assets/f22.obj");

    load_png_texture_data("./assets/f22.png");

}

///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            if (event.key.keysym.sym == SDLK_1)
                render_mode = RENDER_WIRE_VERTEX;
            if (event.key.keysym.sym == SDLK_2)
                render_mode = RENDER_WIRE;
            if (event.key.keysym.sym == SDLK_3)
                render_mode = RENDER_FILL_TRIANGLE;
            if (event.key.keysym.sym == SDLK_4)
                render_mode = RENDER_FILL_TRIANGLE_WIRE;
            if (event.key.keysym.sym == SDLK_5)
                render_mode = RENDER_TEXTURED;
            if (event.key.keysym.sym == SDLK_6)
                render_mode = RENDER_TEXTURED_WIRE;
            if (event.key.keysym.sym == SDLK_c)
                cull_method = CULL_BACKFACE;
            if (event.key.keysym.sym == SDLK_x)
                cull_method = CULL_NONE;
            if (event.key.keysym.sym == SDLK_a)
                camera.yaw -= 0.001 * delta_time;
            if (event.key.keysym.sym == SDLK_w) {
                camera.forward_velocity = vec3_mul(camera.direction, 0.005 * delta_time); 
                camera.position = vec3_add(camera.position, camera.forward_velocity);
            }
            if (event.key.keysym.sym == SDLK_s) {
                camera.forward_velocity = vec3_mul(camera.direction, 0.005 * delta_time); 
                camera.position = vec3_substract(camera.position, camera.forward_velocity);
            }
            if (event.key.keysym.sym == SDLK_d)
                camera.yaw += 0.001 * delta_time;
            if (event.key.keysym.sym == SDLK_q)
                camera.position.y -= 0.003 * delta_time;
            if (event.key.keysym.sym == SDLK_e)
                camera.position.y += 0.003 * delta_time;
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Wait some time until the reach the target frame time in milliseconds
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    // Only delay execution if we are running too fast
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    delta_time = SDL_GetTicks() - previous_frame_time;
    previous_frame_time = SDL_GetTicks();

    // Initialize the array of triangles to render
    num_triangles_to_render = 0;

    mesh.rotation.x += 0.00 * delta_time;    
    mesh.rotation.y += 0.00 * delta_time;    
    mesh.rotation.z += 0.00 * delta_time;    
    mesh.translation.z = 5.0;

    // Create view Matrix frame by frame
    
    vec3_t target = {0, 0, 1};
    mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw);
    camera.direction = vec3_from_vec4(mat4_mul_vec4(camera_yaw_rotation, vec4_from_vec3(target)));

    target = vec3_add(camera.position, camera.direction);
    vec3_t up_direction = {0, 1.0, 0};

    view_matrix = mat4_look_at(camera.position, target, up_direction);

    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

    // ORDER MATTERS!! Scale -> Rotate -> Translate
    world_matrix = mat4_identity();
    world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
    world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a];
        face_vertices[1] = mesh.vertices[mesh_face.b];
        face_vertices[2] = mesh.vertices[mesh_face.c];

        vec4_t transformed_vertices[3];

        // Loop all three vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++) {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

            // USE A MATRIX
            transformed_vertices[j] = transformed_vertex;
        }

            // Backface Culling
            vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
            vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
            vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

            vec3_t vector_ab = vec3_substract(vector_b, vector_a);
            vec3_normalize(&vector_ab);
            vec3_t vector_ac = vec3_substract(vector_c, vector_a);
            vec3_normalize(&vector_ac);

            // Cross product order depends on the coordinate system direction (left handed or right handed)
            vec3_t normal = vec3_cross(vector_ab, vector_ac);
            vec3_normalize(&normal);

            vec3_t origin = {0, 0, 0};
            vec3_t camera_ray = vec3_substract(origin, vector_a);

            float dot_normal_camera = vec3_dot(normal, camera_ray);
        if(cull_method == CULL_BACKFACE)
        {
            // Bypass triangle rendering if its not looking at the camera
            if(dot_normal_camera < 0)
            {
                continue; 
            }
        }
        
        vec4_t projected_points [3];
        for (int j = 0; j < 3; j++)
        {
            // Project the current vertex
            // projected_points[j] = project(vec3_from_vec4(transformed_vertices[j]));
            projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

            projected_points[j].y *= -1;    

            projected_points[j].x *= (window_width / 2.0);
            projected_points[j].y *= (window_height / 2.0);

            // Translate the projected points to the middle of the screen
            projected_points[j].x += (window_width / 2.0);
            projected_points[j].y += (window_height / 2.0);

            //projected_triangle.points[j] = projected_point;
        }

        float light_intensity_factor = -vec3_dot(normal, main_light.direction);

        uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);


        triangle_t projected_triangle = {
            .points = {
                { projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
                { projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
                { projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w }
            },
            .texcoords = {
                { mesh_face.a_uv.u, mesh_face.a_uv.v },
                { mesh_face.b_uv.u, mesh_face.b_uv.v },
                { mesh_face.c_uv.u, mesh_face.c_uv.v }
            },
            .color = triangle_color
        };


        // Save the projected triangle in the array of triangles to render
        //array_push(triangles_to_render, projected_triangle);
        if(num_triangles_to_render < MAX_TRIANGLES_PER_MESH){
            triangles_to_render[num_triangles_to_render] = projected_triangle;
            num_triangles_to_render++;
        }
    }   
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    draw_grid();

    // Loop all projected triangles and render them
    for (int i = 0; i < num_triangles_to_render; i++) {
        triangle_t triangle = triangles_to_render[i];       

        if(render_mode == RENDER_FILL_TRIANGLE || render_mode == RENDER_FILL_TRIANGLE_WIRE)
        {
            // Draw filled triangle
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, // vertex C
                triangle.color
            );
        }
        
        // Draw Textured Triangle
        if(render_mode == RENDER_TEXTURED || render_mode == RENDER_TEXTURED_WIRE){
            draw_textured_triangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,// vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,// vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,// vertex C
                mesh_texture
            );
        }


        if(render_mode == RENDER_WIRE || render_mode == RENDER_WIRE_VERTEX || render_mode == RENDER_FILL_TRIANGLE_WIRE || render_mode == RENDER_TEXTURED_WIRE)
        {
            // Draw unfilled triangle
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                0xFF00FFFF
            );
        }

        if(render_mode == RENDER_WIRE_VERTEX)
        {
            // Draw vertex points
            draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFF0000FF); // vertex A
            draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFF0000FF); // vertex B
            draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFF0000FF); // vertex C
        }

        
    }

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    clear_z_buffer();

    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {
    free(color_buffer);
    free(z_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
    upng_free(png_texture);
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(void) {
    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();

    return 0;
}

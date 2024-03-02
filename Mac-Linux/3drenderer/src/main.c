#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "upng.h"
#include "array.h"
#include "display.h"
#include "clipping.h"
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
    set_render_mode(RENDER_WIRE);
    set_culling_method(CULL_BACKFACE);

    init_light(vec3_new(0,0,1));

    // Initialize the perspective projection matrix
    float aspectx = (float)get_window_width() / (float)get_window_height();
    float aspecty = (float)get_window_height() / (float)get_window_width();
    float fovy = M_PI / 3.0;
    float fovx = atan(tan(fovy / 2) * aspectx) * 2;
    float znear = 0.1;
    float zfar = 100.0;
    proj_matrix = mat4_make_perspective(fovy, aspecty, znear, zfar);

    initialize_frustum_planes(fovx, fovy, znear, zfar);

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
    while(SDL_PollEvent(&event)){
        switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE){
                is_running = false;
                break;
            }
            if (event.key.keysym.sym == SDLK_1){
                set_render_mode(RENDER_WIRE_VERTEX);
                break;
            }
            if (event.key.keysym.sym == SDLK_2){
                set_render_mode(RENDER_WIRE);
                break;
            }
            if (event.key.keysym.sym == SDLK_3){
                set_render_mode(RENDER_FILL_TRIANGLE);
                break;
            }
            if (event.key.keysym.sym == SDLK_4){
                set_render_mode(RENDER_FILL_TRIANGLE_WIRE);
                break;
            }
            if (event.key.keysym.sym == SDLK_5){
                set_render_mode(RENDER_TEXTURED);
                break;
            }
            if (event.key.keysym.sym == SDLK_6){
                set_render_mode(RENDER_TEXTURED_WIRE);
                break;
            }
            if (event.key.keysym.sym == SDLK_c){
                set_culling_method(CULL_BACKFACE);
                break;
            }
            if (event.key.keysym.sym == SDLK_x){
                set_culling_method(CULL_NONE);
                break;
            }
            if (event.key.keysym.sym == SDLK_a){
                set_camera_yaw(get_camera_yaw() - 0.001 * delta_time);
                break;
            }
            if (event.key.keysym.sym == SDLK_w) {
                set_forward_velocity(vec3_mul(get_camera_direction(), 0.005 * delta_time)); 
                set_camera_position(vec3_add(get_camera_position(), get_camera_forward_velocity()));
                break;
            }
            if (event.key.keysym.sym == SDLK_s) {
                set_forward_velocity(vec3_mul(get_camera_direction(), 0.005 * delta_time)); 
                set_camera_position(vec3_substract(get_camera_position(), get_camera_forward_velocity()));
                break;
            }
            if (event.key.keysym.sym == SDLK_d){
                set_camera_yaw(get_camera_yaw() + 0.001 * delta_time);
                break;
            }
            if (event.key.keysym.sym == SDLK_q){
                set_camera_position(vec3_new(get_camera_position().x, (get_camera_position().y - 0.003 * delta_time), get_camera_position().z));
                break;
            }
            if (event.key.keysym.sym == SDLK_e){
                set_camera_position(vec3_new(get_camera_position().x, (get_camera_position().y + 0.003 * delta_time), get_camera_position().z));
                break;
            }
            if (event.key.keysym.sym == SDLK_r){
                set_camera_pitch(get_camera_pitch() + 0.001 * delta_time);
                break;
            }
            if (event.key.keysym.sym == SDLK_f){
                set_camera_pitch(get_camera_pitch() - 0.001 * delta_time);
                break;
            }
            break;
        }
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
    mesh.rotation.y += 0.002 * delta_time;    
    mesh.rotation.z += 0.00 * delta_time;    
    mesh.translation.z = 5.0;

    // Create view Matrix frame by frame
    
    vec3_t target = vec3_new(0,0,1);
    vec3_t up_direction = vec3_new(0,1,0);

    mat4_t camera_yaw_rotation = mat4_make_rotation_y(get_camera_yaw());
    mat4_t camera_pitch_rotation = mat4_make_rotation_x(get_camera_pitch());

    mat4_t camera_rotation = mat4_mul_mat4(camera_yaw_rotation, camera_pitch_rotation);
    set_camera_direction(vec3_from_vec4(mat4_mul_vec4(camera_rotation, vec4_from_vec3(target))));
    
    target = vec3_add(get_camera_position(), get_camera_direction());

    view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

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

            // ORDER MATTERS!! Scale -> Rotate -> Translate
            world_matrix = mat4_identity();
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

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

        if(is_cull_backface())
        {
            // Bypass triangle rendering if its not looking at the camera
            if(dot_normal_camera < 0)
            {
                continue; 
            }
        }

        polygon_t polygon = create_polygon_from_triangle(
            vec3_from_vec4(transformed_vertices[0]), 
            vec3_from_vec4(transformed_vertices[1]), 
            vec3_from_vec4(transformed_vertices[2]),
            mesh_face.a_uv, 
            mesh_face.b_uv, 
            mesh_face.c_uv
        );

        clip_polygon(&polygon);

        triangle_t triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
        int num_trianngles_after_clipping = 0;

        triangles_from_polygon(&polygon, triangles_after_clipping, &num_trianngles_after_clipping);

        for (int t = 0; t < num_trianngles_after_clipping; t++)
        {
            triangle_t triangle_after_clipping = triangles_after_clipping[t];
            vec4_t projected_points [3];
            for (int j = 0; j < 3; j++)
            {
                // Project the current vertex
                // projected_points[j] = project(vec3_from_vec4(transformed_vertices[j]));
                projected_points[j] = mat4_mul_vec4_project(proj_matrix, triangle_after_clipping.points[j]);

                projected_points[j].y *= -1;    

                projected_points[j].x *= (get_window_width() / 2.0);
                projected_points[j].y *= (get_window_height() / 2.0);

                // Translate the projected points to the middle of the screen
                projected_points[j].x += (get_window_width() / 2.0);
                projected_points[j].y += (get_window_height() / 2.0);

                //projected_triangle.points[j] = projected_point;
            }

            float light_intensity_factor = -vec3_dot(normal, get_light_direction());

            uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);


            triangle_t triangle_to_render = {
                .points = {
                    { projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
                    { projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
                    { projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w }
                },
                .texcoords = {
                    { triangle_after_clipping.texcoords[0].u, triangle_after_clipping.texcoords[0].v },
                    { triangle_after_clipping.texcoords[1].u, triangle_after_clipping.texcoords[1].v },
                    { triangle_after_clipping.texcoords[2].u, triangle_after_clipping.texcoords[2].v }
                },
                .color = triangle_color
            };


            // Save the projected triangle in the array of triangles to render
            //array_push(triangles_to_render, projected_triangle);
            if(num_triangles_to_render < MAX_TRIANGLES_PER_MESH){
                triangles_to_render[num_triangles_to_render] = triangle_to_render;
                num_triangles_to_render++;
            }
        }
    }   
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    clear_color_buffer(0xFF000000);
    clear_z_buffer();

    draw_grid();

    // Loop all projected triangles and render them
    for (int i = 0; i < num_triangles_to_render; i++) {
        triangle_t triangle = triangles_to_render[i];       

        if(should_render_filled_triangles())
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
        if(should_render_textured_triangles()){
            draw_textured_triangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,// vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,// vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,// vertex C
                mesh_texture
            );
        }


        if(should_render_wireframe())
        {
            // Draw unfilled triangle
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                0xFF00FFFF
            );
        }

        if(should_render_wire_vertex())
        {
            // Draw vertex points
            draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFF0000FF); // vertex A
            draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFF0000FF); // vertex B
            draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFF0000FF); // vertex C
        }

        
    }

    render_color_buffer();
}

///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {
    
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

#include "camera.h"

static camera_t camera = {
    .position = {0, 0, 0},
    .direction = {0, 0, 1},
    .forward_velocity = {0,0,0},
    .yaw = 0.0,
    .pitch = 0.0
};

float get_camera_yaw(void){
    return camera.yaw;
}

float get_camera_pitch(void){
    return camera.pitch;
}

vec3_t get_camera_position(void){
    return camera.position;
}

vec3_t get_camera_direction(void){
    return camera.direction;
}

vec3_t get_camera_forward_velocity(void){
    return camera.forward_velocity;
}

void set_camera_yaw(float yaw){
    camera.yaw = yaw;
}

void set_camera_pitch(float pitch){
    camera.pitch = pitch;
}

void set_camera_position(vec3_t position){
    camera.position = position;
}

void set_camera_direction(vec3_t direction){
    camera.direction = direction;
}

void set_forward_velocity(vec3_t velocity){
    camera.forward_velocity = velocity;
}
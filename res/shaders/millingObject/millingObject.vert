#version 400

flat out int instanceID;

void main() {
    gl_Position = vec4(0, 0, 0, 1.0);
    instanceID = gl_InstanceID;  // Pass the instance ID
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include <string>
#include "aitmesh.h"

using namespace std;
using namespace glm;

const int WINDOW_WIDTH = 1024; 
const int WINDOW_HEIGHT = 768;

GLuint vbo;
int numverts = 0;
const float DEG2RAD = 3.14159f / 180.0f;
// Transform uniforms location
GLuint g_model_to_world_transform_loc;
GLuint g_world_to_view_to_projection_transform_loc;

// Lighting uniforms location
GLuint g_ambient_light_intensity_loc;
GLuint g_directional_light_intensity_loc;
GLuint g_directional_light_direction_loc;

// Materials uniform location
GLuint g_ka_loc;
GLuint g_kd_loc;

//colour
GLuint g_color_location;
float g_colorVec[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float g_red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float g_blue[4] = { 0.3f, 0.3f, 1.1f, 1.0f };
float g_yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
float g_white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float g_gray[4] = { 0.7f, 0.7f, 0.7f, 0.7f };


void drawCircle(float radius)
{
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 10; j++)
	for (int i = 0; i < 360; i++)
	{
		float degInRad = i*DEG2RAD;
		glVertex2f(cos(degInRad)*(radius - (j*.1)), sin(degInRad)*(radius-(j*.1)));
	}
	
	glEnd();
}

static void render_scene_call_back()
{
	// Clear the back buffer and the z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create our world space to view space transformation matrix
	mat4 world_to_view_transform = lookAt(
		vec3(0.0f,10.0f,40.0f), // The position of your camera, in world space
		vec3(0.0f,0.0f,0.0f), // where you want to look at, in world space
		vec3(0.0f,1.0f,0.0f)  // Camera up direction (set to 0,-1,0 to look upside-down)
		); 

    // Create out projection transform
    mat4 projection_transform = perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 100.0f);

	// Combine the world space to view space transformation matrix and the projection transformation matrix
	mat4 world_to_view_to_projection_transform = projection_transform * world_to_view_transform;

    // Update the transforms in the shader program on the GPU
    glUniformMatrix4fv(g_world_to_view_to_projection_transform_loc, 1, GL_FALSE, &world_to_view_to_projection_transform[0][0]);

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ait_vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ait_vertex), (const GLvoid*)12);

    // Set the material properties
    glUniform1f(g_ka_loc, 0.8f); 
    glUniform1f(g_kd_loc, 0.8f);
	
	


	// Draw the model
	mat4 model_to_world_transform = mat4(1.0f);
	static float angle = 0.0f;
	angle+=1.0f;

	//----------------------------------------------
	static float y = 0.0f;
	static float p = 0.0f;
	static float p2 = 0.0f;

	y += 0.000f;
	p += 1.1f;
	p2 += 0.4f;

	mat4 trans = mat4(1.0f);
	mat4 trans2 = mat4(1.0f);
	trans = translate(trans, vec3(0.0f, y, 0.0f));
	trans2 = translate(trans2, vec3(5.0f, y, 5.0f));

	mat4 rot = rotate(mat4(1.0f), p, vec3(0.0f, 1.0f, 0.0f));
	mat4 rot2 = rotate(mat4(1.0f), p2, vec3(0.15f, 1.0f, 0.0f));

	mat4 modelToWorldTransform = trans*rot;
	//----------------------------------------------
	

	glUniform4fv(g_color_location,1, g_yellow);

	//star
	modelToWorldTransform =   scale(trans, vec3(2.5f, 2.5f, 2.5f));
	glUniformMatrix4fv(g_model_to_world_transform_loc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, numverts);

	glUniform4fv(g_color_location, 1, g_blue);
	//first planet
	trans = rot *translate(translate(scale(trans, vec3(0.5f, 0.5f, 0.5f)), vec3(10.0f, 0.0f, 0.0f)), vec3(0.0f, 0.0f, 0.0f))*rot;
	modelToWorldTransform = trans;
	glUniformMatrix4fv(g_model_to_world_transform_loc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, numverts);

	glUniform4fv(g_color_location, 1, g_gray);
	//first planet's moon
	modelToWorldTransform = scale(translate(trans, vec3(2.0f, 0.00f, 0.0f)), vec3(0.3f, 0.3f, 0.3f))*rot;
	glUniformMatrix4fv(g_model_to_world_transform_loc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, numverts);


	glUniform4fv(g_color_location, 1, g_red);
	//second planet
	trans2 = rot2 *translate(translate(scale(trans2, vec3(1.2f, 1.2f, 1.2f)), vec3(10.0f, 0.00f, 0.0f)), vec3(0.0f, 0.0f, 0.0f))*rot2; 
	modelToWorldTransform = trans2;
	glUniformMatrix4fv(g_model_to_world_transform_loc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, numverts);

	
	drawCircle(4.0f);
	
	
	//second planet's moon
	
	glUniform4fv(g_color_location, 1, g_white);
	modelToWorldTransform = scale(translate( trans2, vec3(2.0f, 0.00f, 0.0f)), vec3(0.3f, 0.3f, 0.3f))*rot2;	
	glUniformMatrix4fv(g_model_to_world_transform_loc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, numverts);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	

    glutSwapBuffers();
}

static void initialize_glut_callbacks()
{
    glutDisplayFunc(render_scene_call_back);
    glutIdleFunc(render_scene_call_back);
}

static void create_vertex_buffer()
{
	ait_mesh mesh;
	if(!mesh.load_from_obj("assets/sphere.obj"))
	{
		cerr<<"Error loading mesh from obj file."<<endl;
		system("pause");
		exit(0);
	}
	vector<ait_vertex> verts = mesh.get_vertices();
	numverts = verts.size(); 
    
 	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ait_vertex) * numverts, &verts[0], GL_STATIC_DRAW);
}

static void init_lights()
{
	// Setup the ambient light
	vec3 ambient_light_intensity = vec3(0.2f, 0.2f, 0.2f);
	glUniform3fv(g_ambient_light_intensity_loc, 1, &ambient_light_intensity[0]);
	
	// Setup the direactional light
	vec3 directional_light_direction = vec3(0.0f, 0.0f, -1.0f);
	normalize(directional_light_direction);
	glUniform3fv(g_directional_light_direction_loc, 1, &directional_light_direction[0]);
	vec3 directional_light_intensity = vec3(0.7f, 0.7f, 0.7f);
	glUniform3fv(g_directional_light_intensity_loc, 1, &directional_light_intensity[0]);
}

static void add_shader(GLuint shader_program, const char* p_shader_text, GLenum shader_type)
{
    GLuint shader_obj = glCreateShader(shader_type);

    if (shader_obj == 0) 
	{
        cerr<<"Error creating shader type "<<shader_type<<endl;
        exit(0);
    }

    const GLchar* p[1];
    p[0] = p_shader_text;
    GLint lengths[1];
    lengths[0]= strlen(p_shader_text);
    glShaderSource(shader_obj, 1, p, lengths);
    glCompileShader(shader_obj);
    GLint success;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);
    if (!success) 
	{
        GLchar info_log[1024];
        glGetShaderInfoLog(shader_obj, 1024, NULL, info_log);
        cerr<<"Error compiling shader type "<<shader_type<<": "<<info_log<<endl;
        exit(1);
    }

    glAttachShader(shader_program, shader_obj);
}

const string read_file_to_string(const char* filename)
{
	ifstream file (filename, ios::in);
	if (file.is_open())
	{
        stringstream continut;
        continut << file.rdbuf();
        continut << '\0';
		return continut.str();
	}
    return "";
}

static void build_shaders()
{
    GLuint shader_program = glCreateProgram();

    if (shader_program == 0) 
	{
        cerr<<"Error creating shader program\n";
        exit(1);
    }

	string vs = read_file_to_string("vertexShader.glsl");
	string fs = read_file_to_string("fragmentShader.glsl");

    add_shader(shader_program, vs.c_str(), GL_VERTEX_SHADER);
    add_shader(shader_program, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar error_log[1024] = { 0 };

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (success == 0) 
	{
		glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
		cerr<<"Error linking shader program: "<<error_log<<endl;
        exit(1);
	}

    glValidateProgram(shader_program);
    glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);
    if (!success) 
	{
        glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
        cerr<<"Error linking shader program: "<<error_log<<endl;
        exit(1);
    }

    glUseProgram(shader_program);

    g_model_to_world_transform_loc = glGetUniformLocation(shader_program, "gModelToWorldTransform");
    assert(g_model_to_world_transform_loc != 0xFFFFFFFF);
    g_world_to_view_to_projection_transform_loc = glGetUniformLocation(shader_program, "gWorldToViewToProjectionTransform");
    assert(g_world_to_view_to_projection_transform_loc != 0xFFFFFFFF);
	g_ambient_light_intensity_loc = glGetUniformLocation(shader_program, "gAmbientLightIntensity");
	assert(g_ambient_light_intensity_loc != 0xFFFFFFFF);
	g_directional_light_intensity_loc = glGetUniformLocation(shader_program, "gDirectionalLightIntensity");
	assert(g_directional_light_intensity_loc != 0xFFFFFFFF);
	g_directional_light_direction_loc = glGetUniformLocation(shader_program, "gDirectionalLightDirection");
	assert(g_directional_light_direction_loc != 0xFFFFFFFF);
    g_ka_loc = glGetUniformLocation(shader_program, "gKa");
	assert(g_directional_light_direction_loc != 0xFFFFFFFF);
    g_kd_loc = glGetUniformLocation(shader_program, "gKd");
	assert(g_directional_light_direction_loc != 0xFFFFFFFF);


	g_color_location = glGetUniformLocation(shader_program, "g_colorVec");
	assert(g_directional_light_direction_loc != 0xFFFFFFFF);
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("obj Loader Test");

    initialize_glut_callbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
		cerr<<"Error: "<<glewGetErrorString(res)<<"\n";
		return 1;
    }

	build_shaders();

	// Enable the z-buffer
	glEnable(GL_DEPTH_TEST);
	// GL_LESS - Passes if the incoming depth value is less than the stored depth value
	glDepthFunc(GL_LESS);

	//Enable backface culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); 

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 

	// Initialise lights
	init_lights();
	 
	// Create a vertex buffer
	create_vertex_buffer();

    glutMainLoop();
    
    return 0;
}
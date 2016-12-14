#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support. Compile with -fobjc-arc"
#endif

#define NUM_CUBE_VERTICES 108
#define NUM_CUBE_COLORS 144
#define NUM_GRID_VERTICES 72
#define NUM_GRID_COLORS 96

#import "TreasureHuntRenderer.h"

#import <AudioToolbox/AudioToolbox.h>
#import <GLKit/GLKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <QuartzCore/QuartzCore.h>

#import "GVRAudioEngine.h"
#import "GVRHeadTransform.h"

// Vertex shader implementation.
static const char *kVertexShaderString =
    "#version 100\n"
    "\n"
    "uniform mat4 uMVP; \n"
    "uniform vec3 uPosition; \n"
    "attribute vec3 aVertex; \n"
    "attribute vec4 aColor;\n"
    "varying vec4 vColor;\n"
    "varying vec3 vGrid;  \n"
    "void main(void) { \n"
    "  vGrid = aVertex + uPosition; \n"
    "  vec4 pos = vec4(vGrid, 1.0); \n"
    "  vColor = aColor;"
    "  gl_Position = uMVP * pos; \n"
    "    \n"
    "}\n";

// Simple pass-through fragment shader.
static const char *kPassThroughFragmentShaderString =
    "#version 100\n"
    "\n"
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "varying vec4 vColor;\n"
    "\n"
    "void main(void) { \n"
    "  gl_FragColor = vColor; \n"
    "}\n";

// Fragment shader for the floorplan grid.
// Line patters are generated based on the fragment's position in 3d.
static const char* kGridFragmentShaderString =
    "#version 100\n"
    "\n"
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "varying vec4 vColor;\n"
    "varying vec3 vGrid;\n"
    "\n"
    "void main() {\n"
    "    float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
    "    if ((mod(abs(vGrid.x), 10.0) < 0.1) ||\n"
    "        (mod(abs(vGrid.z), 10.0) < 0.1)) {\n"
    "      gl_FragColor = max(0.0, (90.0-depth) / 90.0) *\n"
    "                     vec4(1.0, 1.0, 1.0, 1.0) + \n"
    "                     min(1.0, depth / 90.0) * vColor;\n"
    "    } else {\n"
    "      gl_FragColor = vColor;\n"
    "    }\n"
    "}\n";

// Vertices for uniform cube mesh centered at the origin.
static const float kCubeVertices[NUM_CUBE_VERTICES] = {
  // Front face
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  // Right face
  0.5f, 0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
  // Back face
  0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  // Left face
  -0.5f, 0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f, 0.5f,
  -0.5f, 0.5f, 0.5f,
  // Top face
  -0.5f, 0.5f, -0.5f,
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, -0.5f,
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, -0.5f,
  // Bottom face
  0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
};

// Color of the cube's six faces.
static const float kCubeColors[NUM_CUBE_COLORS] = {
  // front, green
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,

  // right, blue
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,

  // back, also green
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,
  0.0f, 0.5273f, 0.2656f, 1.0f,

  // left, also blue
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,

  // top, red
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,

  // bottom, also red
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
  0.8359375f, 0.17578125f, 0.125f, 1.0f,
};

// Cube color when looking at it: Yellow.
static const float kCubeFoundColors[NUM_CUBE_COLORS] = {
  // front, yellow
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,

  // right, yellow
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,

  // back, yellow
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,

  // left, yellow
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,

  // top, yellow
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,

  // bottom, yellow
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
  1.0f, 0.6523f, 0.0f, 1.0f,
};

// The grid lines on the floor are rendered procedurally and large polygons cause floating point
// precision problems on some architectures. So we split the floor into 4 quadrants.
static const float kGridVertices[NUM_GRID_VERTICES] = {
  // +X, +Z quadrant
  200.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 200.0f,
  200.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 200.0f,
  200.0f, 0.0f, 200.0f,

  // -X, +Z quadrant
  0.0f, 0.0f, 0.0f,
  -200.0f, 0.0f, 0.0f,
  -200.0f, 0.0f, 200.0f,
  0.0f, 0.0f, 0.0f,
  -200.0f, 0.0f, 200.0f,
  0.0f, 0.0f, 200.0f,

  // +X, -Z quadrant
  200.0f, 0.0f, -200.0f,
  0.0f, 0.0f, -200.0f,
  0.0f, 0.0f, 0.0f,
  200.0f, 0.0f, -200.0f,
  0.0f, 0.0f, 0.0f,
  200.0f, 0.0f, 0.0f,

  // -X, -Z quadrant
  0.0f, 0.0f, -200.0f,
  -200.0f, 0.0f, -200.0f,
  -200.0f, 0.0f, 0.0f,
  0.0f, 0.0f, -200.0f,
  -200.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f,
};

static const float kGridColors[NUM_GRID_COLORS] = {
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
  0.0f, 0.3398f, 0.9023f, 1.0f,
};

// Cube size (scale).
static const float kCubeSize = 1.0f;

// Grid size (scale).
static const float kGridSize = 1.0f;

// Maximum cube distance for any of its axes from the origin.
static const float kMaxCubeDistance = 7.0f;

// Minimum cube distance for any of its axes from the origin.
static const float kMinCubeDistance = 2.0f;

// Maximum azimuth angle in radians to position the cube.
static const float kMaxCubeAzimuthRadians = 2.0f * M_PI;

// Maximum absolute elevation angle in radians to position the cube.
static const float kMaxCubeElevationRadians = 0.25f * M_PI;

// Cube focus angle threshold in radians.
static const float kFocusThresholdRadians = 0.5f;

// Sample sound file names.
static const NSString *kObjectSoundFile = @"cube_sound.wav";
static const NSString *kSuccessSoundFile = @"success.wav";

static GLuint LoadShader(GLenum type, const char *shader_src) {
  GLint compiled = 0;

  // Create the shader object
  const GLuint shader = glCreateShader(type);
  if (shader == 0) {
    return 0;
  }
  // Load the shader source
  glShaderSource(shader, 1, &shader_src, NULL);

  // Compile the shader
  glCompileShader(shader);
  // Check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    GLint info_len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);

    if (info_len > 1) {
      char *info_log = ((char *)malloc(sizeof(char) * info_len));
      glGetShaderInfoLog(shader, info_len, NULL, info_log);
      NSLog(@"Error compiling shader:%s", info_log);
      free(info_log);
    }
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

// Checks the link status of the given program.
static bool checkProgramLinkStatus(GLuint shader_program) {
  GLint linked = 0;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);

  if (!linked) {
    GLint info_len = 0;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_len);

    if (info_len > 1) {
      char *info_log = ((char *)malloc(sizeof(char) * info_len));
      glGetProgramInfoLog(shader_program, info_len, NULL, info_log);
      NSLog(@"Error linking program: %s", info_log);
      free(info_log);
    }
    glDeleteProgram(shader_program);
    return false;
  }
  return true;
}

@implementation TreasureHuntRenderer {
  // GL variables for the cube.
  GLfloat _cube_vertices[NUM_CUBE_VERTICES];
  GLfloat _cube_position[3];
  GLfloat _cube_colors[NUM_CUBE_COLORS];
  GLfloat _cube_found_colors[NUM_CUBE_COLORS];

  GLuint _cube_program;
  GLint _cube_vertex_attrib;
  GLint _cube_position_uniform;
  GLint _cube_mvp_matrix;
  GLuint _cube_vertex_buffer;
  GLint _cube_color_attrib;
  GLuint _cube_color_buffer;
  GLuint _cube_found_color_buffer;

  // GL variables for the grid.
  GLfloat _grid_vertices[NUM_GRID_VERTICES];
  GLfloat _grid_colors[NUM_GRID_COLORS];
  GLfloat _grid_position[3];

  GLuint _grid_program;
  GLint _grid_vertex_attrib;
  GLint _grid_color_attrib;
  GLint _grid_position_uniform;
  GLint _grid_mvp_matrix;
  GLuint _grid_vertex_buffer;
  GLuint _grid_color_buffer;

  GVRAudioEngine *_gvr_audio_engine;
  int _sound_object_id;
  int _success_source_id;
  bool _is_cube_focused;
}

#pragma mark - GVRCardboardViewDelegate overrides

- (void)cardboardView:(GVRCardboardView *)cardboardView
     willStartDrawing:(GVRHeadTransform *)headTransform {
  // Renderer must be created on GL thread before any call to drawFrame.
  // Load the vertex/fragment shaders.
  const GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, kVertexShaderString);
  NSAssert(vertex_shader != 0, @"Failed to load vertex shader");
  const GLuint fragment_shader = LoadShader(GL_FRAGMENT_SHADER, kPassThroughFragmentShaderString);
  NSAssert(fragment_shader != 0, @"Failed to load fragment shader");
  const GLuint grid_fragment_shader = LoadShader(GL_FRAGMENT_SHADER, kGridFragmentShaderString);
  NSAssert(grid_fragment_shader != 0, @"Failed to load grid fragment shader");

  /////// Create the program object for the cube.

  _cube_program = glCreateProgram();
  NSAssert(_cube_program != 0, @"Failed to create program");
  glAttachShader(_cube_program, vertex_shader);
  glAttachShader(_cube_program, fragment_shader);

  // Link the shader program.
  glLinkProgram(_cube_program);
  NSAssert(checkProgramLinkStatus(_cube_program), @"Failed to link _cube_program");

  // Get the location of our attributes so we can bind data to them later.
  _cube_vertex_attrib = glGetAttribLocation(_cube_program, "aVertex");
  NSAssert(_cube_vertex_attrib != -1, @"glGetAttribLocation failed for aVertex");
  _cube_color_attrib = glGetAttribLocation(_cube_program, "aColor");
  NSAssert(_cube_color_attrib != -1, @"glGetAttribLocation failed for aColor");

  // After linking, fetch references to the uniforms in our shader.
  _cube_mvp_matrix = glGetUniformLocation(_cube_program, "uMVP");
  _cube_position_uniform = glGetUniformLocation(_cube_program, "uPosition");
  NSAssert(_cube_mvp_matrix != -1 && _cube_position_uniform != -1,
           @"Error fetching uniform values for shader.");
  // Initialize the vertex data for the cube mesh.
  for (int i = 0; i < NUM_CUBE_VERTICES; ++i) {
    _cube_vertices[i] = (GLfloat)(kCubeVertices[i] * kCubeSize);
  }
  glGenBuffers(1, &_cube_vertex_buffer);
  NSAssert(_cube_vertex_buffer != 0, @"glGenBuffers failed for vertex buffer");
  glBindBuffer(GL_ARRAY_BUFFER, _cube_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_cube_vertices), _cube_vertices, GL_STATIC_DRAW);

  // Initialize the color data for the cube mesh.
  for (int i = 0; i < NUM_CUBE_COLORS; ++i) {
    _cube_colors[i] = (GLfloat)(kCubeColors[i] * kCubeSize);
  }
  glGenBuffers(1, &_cube_color_buffer);
  NSAssert(_cube_color_buffer != 0, @"glGenBuffers failed for color buffer");
  glBindBuffer(GL_ARRAY_BUFFER, _cube_color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_cube_colors), _cube_colors, GL_STATIC_DRAW);

  // Initialize the found color data for the cube mesh.
  for (int i = 0; i < NUM_CUBE_COLORS; ++i) {
    _cube_found_colors[i] = (GLfloat)(kCubeFoundColors[i] * kCubeSize);
  }
  glGenBuffers(1, &_cube_found_color_buffer);
  NSAssert(_cube_found_color_buffer != 0, @"glGenBuffers failed for color buffer");
  glBindBuffer(GL_ARRAY_BUFFER, _cube_found_color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_cube_found_colors), _cube_found_colors, GL_STATIC_DRAW);

  /////// Create the program object for the grid.

  _grid_program = glCreateProgram();
  NSAssert(_grid_program != 0, @"Failed to create program");
  glAttachShader(_grid_program, vertex_shader);
  glAttachShader(_grid_program, grid_fragment_shader);
  glLinkProgram(_grid_program);
  NSAssert(checkProgramLinkStatus(_grid_program), @"Failed to link _grid_program");

  // Get the location of our attributes so we can bind data to them later.
  _grid_vertex_attrib = glGetAttribLocation(_grid_program, "aVertex");
  NSAssert(_grid_vertex_attrib != -1, @"glGetAttribLocation failed for aVertex");
  _grid_color_attrib = glGetAttribLocation(_grid_program, "aColor");
  NSAssert(_grid_color_attrib != -1, @"glGetAttribLocation failed for aColor");

  // After linking, fetch references to the uniforms in our shader.
  _grid_mvp_matrix = glGetUniformLocation(_grid_program, "uMVP");
  _grid_position_uniform = glGetUniformLocation(_grid_program, "uPosition");
  NSAssert(_grid_mvp_matrix != -1 && _grid_position_uniform != -1,
           @"Error fetching uniform values for shader.");

  // Position grid below the camera.
  _grid_position[0] = 0;
  _grid_position[1] = -20.0f;
  _grid_position[2] = 0;

  for (int i = 0; i < NUM_GRID_VERTICES; ++i) {
    _grid_vertices[i] = (GLfloat)(kGridVertices[i] * kCubeSize);
  }
  glGenBuffers(1, &_grid_vertex_buffer);
  NSAssert(_grid_vertex_buffer != 0, @"glGenBuffers failed for vertex buffer");
  glBindBuffer(GL_ARRAY_BUFFER, _grid_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_grid_vertices), _grid_vertices, GL_STATIC_DRAW);

  // Initialize the color data for the grid mesh.
  for (int i = 0; i < NUM_GRID_COLORS; ++i) {
    _grid_colors[i] = (GLfloat)(kGridColors[i] * kGridSize);
  }
  glGenBuffers(1, &_grid_color_buffer);
  NSAssert(_grid_color_buffer != 0, @"glGenBuffers failed for color buffer");
  glBindBuffer(GL_ARRAY_BUFFER, _grid_color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_grid_colors), _grid_colors, GL_STATIC_DRAW);

  // Initialize GVRCardboardAudio engine.
  _gvr_audio_engine =
      [[GVRAudioEngine alloc] initWithRenderingMode:kRenderingModeBinauralHighQuality];
  [_gvr_audio_engine preloadSoundFile:kObjectSoundFile];
  [_gvr_audio_engine preloadSoundFile:kSuccessSoundFile];
  [_gvr_audio_engine start];

  // Generate seed for random number generation.
  srand48(time(0));

  // Spawn the first cube.
  _sound_object_id = [_gvr_audio_engine createSoundObject:kObjectSoundFile];

  [self spawnCube];
}

- (void)cardboardView:(GVRCardboardView *)cardboardView
     prepareDrawFrame:(GVRHeadTransform *)headTransform {
  // Update audio listener's head rotation.
  const GLKQuaternion head_rotation =
      GLKQuaternionMakeWithMatrix4(GLKMatrix4Transpose([headTransform headPoseInStartSpace]));
  [_gvr_audio_engine setHeadRotation:head_rotation.q[0]
                                   y:head_rotation.q[1]
                                   z:head_rotation.q[2]
                                   w:head_rotation.q[3]];
  // Update the audio engine.
  [_gvr_audio_engine update];

  // Check if the cube is focused.
  GLKVector3 source_cube_position =
      GLKVector3Make(_cube_position[0], _cube_position[1], _cube_position[2]);
  _is_cube_focused = [self isLookingAtObject:&head_rotation sourcePosition:&source_cube_position];

  // Clear GL viewport.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_SCISSOR_TEST);
}

- (void)cardboardView:(GVRCardboardView *)cardboardView
              drawEye:(GVREye)eye
    withHeadTransform:(GVRHeadTransform *)headTransform {
  CGRect viewport = [headTransform viewportForEye:eye];
  glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
  glScissor(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

  // Get the head matrix.
  const GLKMatrix4 head_from_start_matrix = [headTransform headPoseInStartSpace];

  // Get this eye's matrices.
  GLKMatrix4 projection_matrix = [headTransform projectionMatrixForEye:eye near:0.1f far:100.0f];
  GLKMatrix4 eye_from_head_matrix = [headTransform eyeFromHeadMatrix:eye];

  // Compute the model view projection matrix.
  GLKMatrix4 model_view_projection_matrix = GLKMatrix4Multiply(
      projection_matrix, GLKMatrix4Multiply(eye_from_head_matrix, head_from_start_matrix));

  // Render from this eye.
  [self renderWithModelViewProjectionMatrix:model_view_projection_matrix.m];
}

- (void)renderWithModelViewProjectionMatrix:(const float *)model_view_matrix {
  // Select our shader.
  glUseProgram(_cube_program);

  // Set the uniform values that will be used by our shader.
  glUniform3fv(_cube_position_uniform, 1, _cube_position);

  // Set the uniform matrix values that will be used by our shader.
  glUniformMatrix4fv(_cube_mvp_matrix, 1, false, model_view_matrix);

  // Set the cube colors.
  if (_is_cube_focused) {
    glBindBuffer(GL_ARRAY_BUFFER, _cube_found_color_buffer);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, _cube_color_buffer);
  }
  glVertexAttribPointer(_cube_color_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
  glEnableVertexAttribArray(_cube_color_attrib);

  // Draw our polygons.
  glBindBuffer(GL_ARRAY_BUFFER, _cube_vertex_buffer);
  glVertexAttribPointer(_cube_vertex_attrib, 3, GL_FLOAT, GL_FALSE,
                        sizeof(float) * 3, 0);
  glEnableVertexAttribArray(_cube_vertex_attrib);
  glDrawArrays(GL_TRIANGLES, 0, NUM_CUBE_VERTICES / 3);
  glDisableVertexAttribArray(_cube_vertex_attrib);
  glDisableVertexAttribArray(_cube_color_attrib);

  // Select our shader.
  glUseProgram(_grid_program);

  // Set the uniform values that will be used by our shader.
  glUniform3fv(_grid_position_uniform, 1, _grid_position);

  // Set the uniform matrix values that will be used by our shader.
  glUniformMatrix4fv(_grid_mvp_matrix, 1, false, model_view_matrix);

  // Set the grid colors.
  glBindBuffer(GL_ARRAY_BUFFER, _grid_color_buffer);
  glVertexAttribPointer(_grid_color_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
  glEnableVertexAttribArray(_grid_color_attrib);

  // Draw our polygons.
  glBindBuffer(GL_ARRAY_BUFFER, _grid_vertex_buffer);
  glVertexAttribPointer(_grid_vertex_attrib, 3, GL_FLOAT, GL_FALSE,
                        sizeof(float) * 3, 0);
  glEnableVertexAttribArray(_grid_vertex_attrib);
  glDrawArrays(GL_TRIANGLES, 0, NUM_GRID_VERTICES / 3);
  glDisableVertexAttribArray(_grid_vertex_attrib);
  glDisableVertexAttribArray(_grid_color_attrib);
}

- (void)cardboardView:(GVRCardboardView *)cardboardView
         didFireEvent:(GVRUserEvent)event {
  switch (event) {
    case kGVRUserEventBackButton:
      NSLog(@"User pressed back button");
      break;
    case kGVRUserEventTilt:
      NSLog(@"User performed tilt action");
      break;
    case kGVRUserEventTrigger:
      NSLog(@"User performed trigger action");
      // Check whether the object is found.
      if (_is_cube_focused) {
         _success_source_id = [_gvr_audio_engine createStereoSound:kSuccessSoundFile];
        [_gvr_audio_engine playSound:_success_source_id loopingEnabled:false];
        // Vibrate the device on success.
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
        // Generate the next cube.
        [self spawnCube];
      }
      break;
  }
}

- (void)cardboardView:(GVRCardboardView *)cardboardView shouldPauseDrawing:(BOOL)pause {
  if ([self.delegate respondsToSelector:@selector(shouldPauseRenderLoop:)]) {
    [self.delegate shouldPauseRenderLoop:pause];
  }
}

// Spawns the next cube at a new position.
- (void)spawnCube {
  // Set the new position and restart the playback.
  [self setRandomCubePosition:kMinCubeDistance maxLimit:kMaxCubeDistance];
  [_gvr_audio_engine setSoundObjectPosition:_sound_object_id
                                          x:_cube_position[0]
                                          y:_cube_position[1]
                                          z:_cube_position[2]];
  [_gvr_audio_engine playSound:_sound_object_id loopingEnabled:true];
}

// Sets a new position for the cube.
- (void)setRandomCubePosition:(float)min maxLimit:(float)max {
  // Choose random spherical coordinates to set the new position.
  const float distance = min + (float)((max - min) * drand48());
  const float azimuth = (float)(drand48() * kMaxCubeAzimuthRadians);
  const float elevation = (float)(2.0 * drand48() * kMaxCubeElevationRadians) -
                          kMaxCubeElevationRadians;
  _cube_position[0] = -cos(elevation) * sin(azimuth) * distance;
  _cube_position[1] = sin(elevation) * distance;
  _cube_position[2] = -cos(elevation) * cos(azimuth) * distance;
}

// Returns whether the object is currently on focus.
- (bool)isLookingAtObject:(const GLKQuaternion *)head_rotation
           sourcePosition:(GLKVector3 *)position {
  GLKVector3 source_direction = GLKQuaternionRotateVector3(
      GLKQuaternionInvert(*head_rotation), *position);
  return ABS(source_direction.v[0]) < kFocusThresholdRadians &&
         ABS(source_direction.v[1]) < kFocusThresholdRadians;
}

@end

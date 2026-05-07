// CS 247 - Scientific Visualization, KAUST
//
// Programming Assignment #5
#include <cstring>
#include "CS247_prog.h"

// cycle clear colors
static void nextClearColor()
{
    clearColor = (++clearColor) % 3;

    switch(clearColor)
    {
        case 0:
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 1:
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            break;
        default:
            glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
            break;
    }
}


// callbacks
// framebuffer to fix viewport
void frameBufferCallback(GLFWwindow* window, int width, int height)
{
    view_width = width;
    view_height = height;
    glViewport(0, 0, width, height);
}

// key callback to take user inputs for both windows
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE) {
        char* status[ 2 ];
        status[ 0 ] = "disabled";
        status[ 1 ] = "enabled";

        switch (key) {
            case '1':
                toggle_xy = 0;
                LoadData( filenames[ 0 ] );
                loaded_file = 0;
                fprintf( stderr, "Loading " );
                fprintf( stderr, filenames[ 0 ] );
                fprintf( stderr, " dataset.\n");
                break;
            case '2':
                toggle_xy = 0;
                LoadData(filenames[ 1 ] );
                loaded_file = 1;
                fprintf( stderr, "Loading " );
                fprintf( stderr, filenames[ 1 ] );
                fprintf( stderr, " dataset.\n");
                break;
            case '3':
                toggle_xy = 1;
                LoadData( filenames[ 2 ] );
                loaded_file = 2;
                fprintf( stderr, "Loading " );
                fprintf( stderr, filenames[ 2 ] );
                fprintf( stderr, " dataset.\n");
                break;
            case '0':
                if( num_timesteps > 1 ){
                    loadNextTimestep();
                    fprintf( stderr, "Timestep %d.\n", loaded_timestep );

                    std::vector<Streamline> old_streamlines = streamlines;
                    streamlines.clear();
                    for (const Streamline& sl : old_streamlines) {
                        computeStreamline(sl.seed_x, sl.seed_y);
                    }
                    fprintf(stderr, "recalculated streamlines for new timestep.\n");


                }
                break;
            case GLFW_KEY_A:
                en_arrow = !en_arrow;
                fprintf(stderr, "%s drawing arrows.\n", en_arrow? "enabling" : "disabling");
                break;
            case GLFW_KEY_S:
                current_scalar_field = (current_scalar_field + 1)%num_scalar_fields;
                DownloadScalarFieldAsTexture();
                fprintf( stderr, "Scalar field changed.\n");
                break;
            case GLFW_KEY_B:
                nextClearColor();
                fprintf( stderr, "Next clear color.\n");
                break;
            case GLFW_KEY_EQUAL:
                sampling_rate = std::min(sampling_rate + 5, 100);
                fprintf(stderr, "Increasing sampling rate to %d.\n", sampling_rate);
                break;
            case GLFW_KEY_MINUS:
                sampling_rate = std::max(sampling_rate - 5, 5);
                fprintf(stderr, "Decreasing sampling rate to: %d.\n", sampling_rate);
                break;
            case GLFW_KEY_I:
                dt = std::min(dt + 0.005, 1.);
                fprintf(stderr, "Increase dt: %.2f\n", dt);
                break;
            case GLFW_KEY_K:
                dt = std::max(dt - 0.005, 0.0001);
                fprintf(stderr, "Decrease dt: %.2f\n", dt);
                break;
            case GLFW_KEY_T:
                en_streamline = !en_streamline;
                fprintf(stderr, "%s drawing streamlines.\n", en_streamline? "enabling" : "disabling");
                break;
            case GLFW_KEY_P:
                en_pathline = !en_pathline;
                fprintf(stderr, "%s drawing pathlines.\n", en_pathline? "enabling" : "disabling");
                break;
            // TODO: add keyboard controls for:
            //   - toggle colormap mode (cycle off/rainbow/cool-warm)
            //   - adjust blend factor (increase/decrease between 0.0 and 1.0)
            //   - toggle integration method (Euler/RK2)
            //   - clear all seeds


            case GLFW_KEY_C:
                colormap_mode = (colormap_mode + 1) % 3;
                {
                    const char* mode_names[] = { "off (grayscale)", "rainbow", "cool-warm" };
                    fprintf(stderr, "choosen colormap mode: %s.\n", mode_names[colormap_mode]);
                }
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                blend_factor = std::min(blend_factor + 0.1f, 1.0f);
                fprintf(stderr, "Blend factor: %.2f\n", blend_factor);
                break;
            case GLFW_KEY_LEFT_BRACKET:
                blend_factor = std::max(blend_factor - 0.1f, 0.0f);
                fprintf(stderr, "Blend factor: %.2f\n", blend_factor);
                break;

            case GLFW_KEY_L:
                glyph_scaled_length = !glyph_scaled_length;
                fprintf(stderr, "Glyph length mode: %s.\n",
                    glyph_scaled_length ? "scaled by magnitude" : "constant");
                break;

            case GLFW_KEY_R:
                use_rk2 = !use_rk2;
                fprintf(stderr, "Integration method: %s\n", use_rk2 ? "RK2" : "Euler");
                break;
            case GLFW_KEY_X:
                streamlines.clear();
                pathlines.clear(); 
                fprintf(stderr, "Cleared all streamlines & pathlines.\n");
                break;

            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                exit( 0 );
                break;
            default:
                fprintf( stderr, "\nKeyboard commands:\n\n"
                                 "1, load %s dataset\n"
                                 "2, load %s dataset\n"
                                 "3, load %s dataset\n"
                                 "0, cycle through timesteps\n"
                                 "b, switch backgropund color\n"
                                 "a, en-/disable arrows.\n"
                                 "t, en-/disable streamlines.\n"
                                 "p, en-/disable pathlines.\n"
                                 "+, increase sampling rate.\n"
                                 "-, decrease sampling rate.\n"
                                 "c, colormap mode : grayscale , rainbow , or cool-warm \n"
                                 "[ increase bkend factor  \n"
                                 "] decrease bkend factor  \n"
                                 "l, Glyph length mode:  scaled by magnitude or constant \n"
                                 "r: Integration method:  RK2 or  Euler \n "
                                 "x, clear all streamlines.\n"
                                 "i, increase dt.\n"
                                 "k, decrease dt.\n"
                                 "q, <esc> - Quit\n",
                         filenames[0], filenames[1], filenames[2] );
                break;
        }
    }
}

// mouse callback to seed streamlines/pathlines
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        // TODO: seed streamlines & pathlines using mouse clicks
        // Hint: convert screen coords to grid coords (y-flip needed),
        //       then call computeStreamline/computePathline when enabled

        if (!grid_data_loaded) 
            return;

        int win_w, win_h;
        glfwGetWindowSize(window, &win_w, &win_h);

        int grid_x = (int)((xpos / (double)win_w) * (double)(vol_dim[0] - 1));
        int grid_y = (int)(((win_h - ypos) / (double)win_h) * (double)(vol_dim[1] - 1));

        if (grid_x < 0 || grid_x >= vol_dim[0] ||
            grid_y < 0 || grid_y >= vol_dim[1]) 
            return;

        fprintf(stderr, "Click at pixel (%d, %d) -> grid (%d, %d)\n",
            (int)xpos, (int)ypos, grid_x, grid_y);

        if (en_streamline) {
            computeStreamline(grid_x, grid_y);
        }

        if (en_pathline) {
            computePathline(grid_x, grid_y, loaded_timestep);
        }

    }
}

// glfw error callback
static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// data

void loadNextTimestep( void )
{
    loaded_timestep = ( loaded_timestep + 1 ) % num_timesteps;
    DownloadScalarFieldAsTexture();
}


/*
 * load .gri dataset
 * This only reads the header information and calls the dat loader
 * For now we ignore the grid data and assume a rectangular grid
 */
void LoadData( char* base_filename )
{
    //reset
    reset_rendering_props();

    char filename[ 80 ];
    strcpy( filename, base_filename );
    strcat( filename, ".gri");

    fprintf( stderr, "loading grid file %s\n", filename );

    // open grid file, read only, binary mode
    FILE* fp = fopen( filename, "rb" );
    if ( fp == NULL ) {
        fprintf( stderr, "Cannot open file %s for reading.\n", filename );
        return;
    }

    // read header
    char header[ 40 ];
    fread( header, sizeof( char ), 40, fp );
    sscanf( header, "SN4DB %d %d %d %d %d %f",
            &vol_dim[ 0 ], &vol_dim[ 1 ], &vol_dim[ 2 ],
            &num_scalar_fields, &num_timesteps ,&timestep );

    fprintf( stderr, "dimensions: x: %d, y: %d, z: %d.\n", vol_dim[ 0 ], vol_dim[ 1 ], vol_dim[ 2 ] );
    fprintf( stderr, "additional info: # scalar fields: %d, # timesteps: %d, timestep: %f.\n", num_scalar_fields, num_timesteps, timestep );

    // read data
    char dat_filename[ 80 ];
    strcpy( dat_filename, base_filename );

    if( num_timesteps <= 1 ){

        strcat( dat_filename, ".dat");

    } else {

        strcat( dat_filename, ".00000.dat");

    }

    loaded_timestep = 0;
    LoadVectorData( base_filename );

    glfwSetWindowSize(window, vol_dim[ 0 ], vol_dim[ 1 ] );
    grid_data_loaded = true;
}

/*
 * load .dat dataset
 * loads vector and scalar fields
 */
void LoadVectorData( const char* filename )
{
    fprintf( stderr, "loading scalar file %s\n", filename );

    // open data file, read only, binary mode
    char ts_name[ 80 ];
    if( num_timesteps > 1 )
    {
        sprintf( ts_name, "%s.%.5d.dat", filename, 0 );
    }
    else
        sprintf( ts_name, "%s.dat",filename);

    FILE* fp = fopen( ts_name, "rb" );
    if ( fp == NULL ) {
        fprintf( stderr, "Cannot open file %s for reading.\n", filename );
        return;
    }
    else
    {
        fclose( fp );
    }

    data_size = vol_dim[ 0 ] * vol_dim[ 1 ] * vol_dim[ 2 ];

    if (!vector_array) {
        delete[] vector_array;
        vector_array = NULL;
    }
    // dim.xyz * vector.xyz * timesteps
    vector_array = new float[ data_size * 3 * num_timesteps];

    // read data
    if (!scalar_fields) {
        delete[] scalar_fields;
        scalar_fields = NULL;
        delete[] scalar_bounds;
        scalar_bounds = NULL;
    }
    // dim.xyz * scalarfields(2) * timesteps
    scalar_fields = new float[ data_size * num_scalar_fields * num_timesteps ];
    scalar_bounds = new float[ 2 * num_scalar_fields * num_timesteps ];

    int num_total_fields = num_scalar_fields + 3; // scalar fields + vec.xyz
    float *tmp = new float[ data_size * num_total_fields * num_timesteps ];

    for( int k = 0 ; k < num_timesteps; k++ )
    {
        char times_name[ 80 ];
        if( num_timesteps > 1 )
        {
            sprintf( times_name, "%s.%.5d.dat", filename, k );
            fp = fopen( times_name, "rb" );
        }
        else
            fp = fopen( ts_name, "rb" );
        // read scalar data
        fread( &tmp[k*data_size*num_total_fields], sizeof( float ), ( data_size * num_total_fields ), fp );

        // close file
        fclose( fp );

        // copy and scan for min and max values
        for( int  i = 0; i < num_scalar_fields; i++ ){

            float min_val = 99999.9f;
            float max_val = 0.0f;

            float avg = 0.0;

            int offset = i * data_size * num_timesteps;

            for( int j = 0; j < data_size; j++ ){

                float val = tmp[ j * num_total_fields + 3 + i + k*data_size*num_total_fields ];

                scalar_fields[ j + k*data_size + offset ] = val;

                if( toggle_xy ) {
                    // overwrite
                    if( i == 0 ){
                        vector_array[ 3*j + 0 + 3*k*data_size ] = tmp[ j * num_total_fields + 1 + k*data_size*num_total_fields ];//toggle x and y components in vector field
                        vector_array[ 3*j + 1 + 3*k*data_size ] = tmp[ j * num_total_fields + 0 + k*data_size*num_total_fields ];
                        vector_array[ 3*j + 2 + 3*k*data_size ] = tmp[ j * num_total_fields + 2 + k*data_size*num_total_fields ];
                    }
                } else {
                    // overwrite
                    if( i == 0 ){
                        vector_array[ 3*j + 0 + 3*k*data_size ] = tmp[ j * num_total_fields + 0 + k*data_size*num_total_fields ];
                        vector_array[ 3*j + 1 + 3*k*data_size ] = tmp[ j * num_total_fields + 1 + k*data_size*num_total_fields ];
                        vector_array[ 3*j + 2 + 3*k*data_size ] = tmp[ j * num_total_fields + 2 + k*data_size*num_total_fields ];
                    }
                }

                min_val = std::min( val, min_val );
                max_val = std::max( val, max_val );

                avg += scalar_fields[ offset + j + k*data_size ] / data_size;
            }
            scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] = min_val;
            scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] = max_val;
        }

        // normalize
        for( int  i = 0; i < num_scalar_fields; i++ ){

            int offset = i * data_size * num_timesteps;

            float lower_bound = scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ];
            float upper_bound = scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ];

            // scale between [0..1] where 1 is original zero
            // the boundary of the bigger abs border will be used to scale
            // meaning one boundary will likely not be hit i.e real scale
            // for -50..100 will be [0.25..1.0]
            if( lower_bound < 0.0 && upper_bound > 0.0 ){

                float scale = 0.5f / std::max( -lower_bound, upper_bound );

                for( int j = 0; j < data_size; j++ ){

                    scalar_fields[ offset + j + k*data_size ] = 0.5f + scalar_fields[ offset + j + k*data_size ] * scale;
                }
                scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] = 0.5f + scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] * scale;
                scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] = 0.5f + scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] * scale;


                // scale between [0..1]
            } else {

                float sign = upper_bound <= 0.0 ? -1.0f : 1.0f;

                float scale = 1.0f / ( upper_bound - lower_bound ) * sign;

                for( int j = 0; j < data_size; j++ ){

                    scalar_fields[ offset + j + k*data_size ] = ( scalar_fields[ offset + j + k*data_size ] - lower_bound ) * scale;
                }
                scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] = ( scalar_bounds[ 2 * i     + k*num_scalar_fields*2 ] + lower_bound ) * scale; //should be 0.0
                scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] = ( scalar_bounds[ 2 * i + 1 + k*num_scalar_fields*2 ] + lower_bound ) * scale; //should be 1.0
            }
        }
    }
    delete[] tmp;
    DownloadScalarFieldAsTexture();

    scalar_data_loaded = true;
}


void DownloadScalarFieldAsTexture( void )
{
    fprintf( stderr, "downloading scalar field to 2D texture\n" );

    glEnable( GL_TEXTURE_2D );

    // generate and bind 2D texture
    glGenTextures( 1, &scalar_field_texture );
    glBindTexture( GL_TEXTURE_2D, scalar_field_texture );

    // set necessary texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    int datasize = vol_dim[0] * vol_dim[1];
    //download texture in correct format
    glTexImage2D( GL_TEXTURE_2D, 0,  GL_INTENSITY16, vol_dim[ 0 ], vol_dim[ 1 ], 0, GL_LUMINANCE, GL_FLOAT, &scalar_fields[ (loaded_timestep + current_scalar_field * num_timesteps)*datasize ] );


    glDisable( GL_TEXTURE_2D );
}

char *textFileRead( char *fn ){

    FILE *fp;
    char *content = NULL;

    int count=0;

    if (!fn) {
        fp = fopen(fn,"rt");

        if (!fp) {

            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}


// initializations
// init application
bool initApplication(int argc, char **argv)
{

    std::string version((const char *)glGetString(GL_VERSION));
    std::stringstream stream(version);
    unsigned major, minor;
    char dot;

    stream >> major >> dot >> minor;

    assert(dot == '.');
    if (major > 3 || (major == 2 && minor >= 0)) {
        std::cout << "OpenGL Version " << major << "." << minor << std::endl;
    } else {
        std::cout << "The minimum required OpenGL version is not supported on this machine. Supported is only " << major << "." << minor << std::endl;
        return false;
    }

    return true;
}

void reset_rendering_props( void )
{
    num_scalar_fields = 0;
    streamlines.clear();
    pathlines.clear();


}

// set up the scene
void setup() {
    LoadData( filenames[ 0 ] );
    loaded_file = 0;

    DownloadScalarFieldAsTexture();


    // compile & link shader
    vectorProgram.compileShader("../../../shaders/vertex.vs");
    vectorProgram.compileShader("../../../shaders/fragment.fs");
    vectorProgram.link();

    // make quad to render texture
    // see: vboquad.h and vboquad.cpp
    quad.init();

    // TODO: glyph/streamlines/pathlines VAO and VBO

    glGenVertexArrays(1, &glyph_vao);
    glGenBuffers(1, &glyph_vbo);

    glBindVertexArray(glyph_vao);
    glBindBuffer(GL_ARRAY_BUFFER, glyph_vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    glGenVertexArrays(1, &streamline_vao);
    glGenBuffers(1, &streamline_vbo);

    glBindVertexArray(streamline_vao);
    glBindBuffer(GL_ARRAY_BUFFER, streamline_vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    glGenVertexArrays(1, &pathline_vao);
    glGenBuffers(1, &pathline_vbo);

    glBindVertexArray(pathline_vao);
    glBindBuffer(GL_ARRAY_BUFFER, pathline_vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// rendering
void render() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_TEXTURE_2D );

    // draw the texture
    glBindTexture(GL_TEXTURE_2D, scalar_field_texture);
    vectorProgram.use();

    model = mat4(1);

    vectorProgram.setUniform("vertexColor", glm::vec4(0));
    vectorProgram.setUniform("model", model);

    // TODO: pass colormap uniforms to shader before drawing the quad
    // Hint: set colormapMode and blendFactor uniforms here

    vectorProgram.setUniform("colormapMode", colormap_mode);
    vectorProgram.setUniform("blendFactor", blend_factor);
    quad.render();
    glDisable( GL_TEXTURE_2D );

    // TODO: reset colormap mode to 0 before drawing overlays
    // so that glyphs/streamlines/pathlines use solid colors
    vectorProgram.setUniform("colormapMode", 0);
    // TODO: draw glyphs, streamlines, pathlines



    if (en_arrow) {
        drawGlyphs();
    }


    if (en_streamline) {
        drawStreamlines();
    }
    
    if (en_pathline) {
        drawPathlines();
    }
    
}

// entry point
int main(int argc, char** argv)
{
    // init variables
    view_width = 0;
    view_height = 0;

    toggle_xy = 0;

    en_arrow = false;
    en_streamline = false;
    en_pathline = false;
    sampling_rate = 15;
    dt = 0.1;

    reset_rendering_props();

    vector_array = NULL;
    scalar_fields = NULL;
    scalar_bounds = NULL;
    grid_data_loaded = false;
    scalar_data_loaded = false;
    current_scalar_field = 0;
    clearColor = 0;

    colormap_mode = 0;
    blend_factor = 1.0f;


    glyph_scaled_length = false;


    use_rk2 = true;    

    filenames[ 0 ] = "../../../data/block/c_block";
    filenames[ 1 ] = "../../../data/tube/tube";
    filenames[ 2 ] = "../../../data/hurricane/hurricane_p_tc";



    // set glfw error callback
    glfwSetErrorCallback(errorCallback);

    // init glfw
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // init glfw window
    window = glfwCreateWindow(gWindowWidth, gWindowHeight, "AMCS/CS247 Scientific Visualization", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set GLFW callback functions
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, frameBufferCallback);

    // make context current (once is sufficient)
    glfwMakeContextCurrent(window);

    // get the frame buffer size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // init the OpenGL API (we need to do this once before any calls to the OpenGL API)
    gladLoadGL();

    // init our application
    if (!initApplication(argc, argv)) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    // set up the scene
    setup();

    // print menu
    keyCallback(window, GLFW_KEY_BACKSLASH, 0, GLFW_PRESS, 0);

    // start traversing the main loop
    // loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // clear frame buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render one frame
        render();

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll and process input events (keyboard, mouse, window, ...)
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

// TODO: define any useful functions you might need.
//  e.g., indexing, linear interpolation ..etc

glm::vec2 getVector(int gi, int gj) {
    int W = vol_dim[0];
    int H = vol_dim[1];

    if (gi < 0 || gi >= W || gj < 0 || gj >= H) {
        return glm::vec2(0.0f, 0.0f);
    }

    int idx = (loaded_timestep * data_size + gj * W + gi) * 3;
    return glm::vec2(vector_array[idx], vector_array[idx + 1]);
}

glm::vec2 sampleBilinear(float x, float y) {
    int i = (int)std::floor(x);
    int j = (int)std::floor(y);

    float fx = x - (float)i;
    float fy = y - (float)j;

    glm::vec2 v00 = getVector(i, j);
    glm::vec2 v10 = getVector(i + 1, j);
    glm::vec2 v01 = getVector(i, j + 1);
    glm::vec2 v11 = getVector(i + 1, j + 1);

    glm::vec2 bottom = (1.0f - fx) * v00 + fx * v10;
    glm::vec2 top = (1.0f - fx) * v01 + fx * v11;

    return (1.0f - fy) * bottom + fy * top;
}



glm::vec2 streamlineStep(glm::vec2 p, float direction) {
    glm::vec2 v1 = sampleBilinear(p.x, p.y);

    if (!use_rk2) {
        return p + direction * (float)dt * v1;
    }
    else {
        glm::vec2 mid = p + direction * (float)dt * 0.5f * v1;
        glm::vec2 v2 = sampleBilinear(mid.x, mid.y);
        return p + direction * (float)dt * v2;
    }
}

std::vector<glm::vec2> integrateOneSide(glm::vec2 seed, float direction) {
    std::vector<glm::vec2> path;
    path.push_back(seed);

    glm::vec2 p = seed;
    float acc_len = 0.0f;
    const float MAX_LEN = 500.0f;
    const float EPS = 1e-5f;
    const int MAX_STEPS = 5000;

    for (int i = 0; i < MAX_STEPS; ++i) {

        glm::vec2 v_here = sampleBilinear(p.x, p.y);
        if (glm::length(v_here) < EPS) 
            break;

        glm::vec2 next = streamlineStep(p, direction);

        if (next.x < 0 || next.x >= vol_dim[0] - 1 
            ||
            next.y < 0 || next.y >= vol_dim[1] - 1) 
            
            break;

        acc_len += glm::length(next - p);
        if (acc_len > MAX_LEN) 
            break;

        p = next;
        path.push_back(p);
    }

    return path;
}


void computeStreamline(int x, int y)
{
// TODO: compute streamlines starting from x,y position. enable switching between euler and runge kutta
// Hint: implement bilinear interpolation of vectors, forward+backward integration,
//       and stopping conditions (boundary, zero vector, max accumulated length)

// TODO: set any useful uniforms & update VBO & draw
    if (!grid_data_loaded) 
        return;

    glm::vec2 seed((float)x, (float)y);

    std::vector<glm::vec2> backward = integrateOneSide(seed, -1.0f);
    std::vector<glm::vec2> forward = integrateOneSide(seed, +1.0f);

    Streamline sl;
    sl.seed_x = x;
    sl.seed_y = y;

    int W = vol_dim[0];
    int H = vol_dim[1];

    auto pushNDC = [&](glm::vec2 p) {
        float nx = (p.x / (float)(W - 1)) * 2.0f - 1.0f;
        float ny = (p.y / (float)(H - 1)) * 2.0f - 1.0f;
        sl.vertices.push_back(nx);
        sl.vertices.push_back(ny);
        };

    for (auto it = backward.rbegin(); it != backward.rend(); ++it) {
        pushNDC(*it);
    }
    for (size_t i = 1; i < forward.size(); ++i) {
        pushNDC(forward[i]);
    }

    if (sl.vertices.size() >= 4) {  
        streamlines.push_back(sl);
    }
}



void drawStreamlines() {
    if (streamlines.empty()) return;

    vectorProgram.use();
    vectorProgram.setUniform("model", glm::mat4(1.0f));
    vectorProgram.setUniform("vertexColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); 
    vectorProgram.setUniform("colormapMode", 0);  

    glBindVertexArray(streamline_vao);
    glBindBuffer(GL_ARRAY_BUFFER, streamline_vbo);

    for (const Streamline& sl : streamlines) {
        if (sl.vertices.size() < 4) 
            continue;

        glBufferData(GL_ARRAY_BUFFER,
            sl.vertices.size() * sizeof(float),
            sl.vertices.data(),
            GL_DYNAMIC_DRAW);

        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(sl.vertices.size() / 2));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


glm::vec2 sampleBilinearAtTime(float x, float y, int t) {
    int i = (int)std::floor(x);
    int j = (int)std::floor(y);

    float fx = x - (float)i;
    float fy = y - (float)j;

    int W = vol_dim[0];
    int H = vol_dim[1];


    auto getAt = [&](int gi, int gj) {
        if (gi < 0 || gi >= W || gj < 0 || gj >= H) 
            return glm::vec2(0.0f, 0.0f);
        int idx = (t * data_size + gj * W + gi) * 3;
        return glm::vec2(vector_array[idx], vector_array[idx + 1]);
        };

    glm::vec2 v00 = getAt(i, j);
    glm::vec2 v10 = getAt(i + 1, j);
    glm::vec2 v01 = getAt(i, j + 1);
    glm::vec2 v11 = getAt(i + 1, j + 1);

    glm::vec2 bottom = (1.0f - fx) * v00 + fx * v10;
    glm::vec2 top = (1.0f - fx) * v01 + fx * v11;
    return (1.0f - fy) * bottom + fy * top;
}


glm::vec2 sampleTrilinear(float x, float y, float t) {
    int t0 = (int)std::floor(t);
    int t1 = t0 + 1;

    if (t0 < 0 || t1 >= num_timesteps) {
        return glm::vec2(0.0f, 0.0f);
    }

    float ft = t - (float)t0;

    glm::vec2 v_t0 = sampleBilinearAtTime(x, y, t0);
    glm::vec2 v_t1 = sampleBilinearAtTime(x, y, t1);

    return (1.0f - ft) * v_t0 + ft * v_t1;
}



glm::vec2 pathlineStep(glm::vec2 p, float t, float direction) {
    glm::vec2 v1 = sampleTrilinear(p.x, p.y, t);

    if (!use_rk2) {
        return p + direction * (float)dt * v1;
    }
    else {

        glm::vec2 mid_p = p + direction * (float)dt * 0.5f * v1;
        float mid_t = t + direction * (float)dt * 0.5f;
        glm::vec2 v2 = sampleTrilinear(mid_p.x, mid_p.y, mid_t);
        return p + direction * (float)dt * v2;
    }
}


std::vector<glm::vec2> integrateOnePathSide(glm::vec2 seed, float t_start, float direction) {
    std::vector<glm::vec2> path;
    path.push_back(seed);

    glm::vec2 p = seed;
    float t = t_start;
    float acc_len = 0.0f;
    const float MAX_LEN = 500.0f;
    const float EPS = 1e-5f;
    const int MAX_STEPS = 5000;

    for (int i = 0; i < MAX_STEPS; ++i) {
 
        glm::vec2 v_here = sampleTrilinear(p.x, p.y, t);
        if (glm::length(v_here) < EPS) 
            break;

    
        glm::vec2 next = pathlineStep(p, t, direction);
        float t_next = t + direction * (float)dt;

        if (next.x < 0 || next.x >= vol_dim[0] - 1 ||
            next.y < 0 || next.y >= vol_dim[1] - 1) 
            break;

        if (t_next < 0 || t_next >= (float)(num_timesteps - 1)) 
            break;

        acc_len += glm::length(next - p);
        if (acc_len > MAX_LEN) 
            break;

        p = next;
        t = t_next;
        path.push_back(p);
    }

    return path;
}


void drawPathlines() {
    if (pathlines.empty()) 
        return;

    vectorProgram.use();
    vectorProgram.setUniform("model", glm::mat4(1.0f));
    vectorProgram.setUniform("vertexColor", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)); 
    vectorProgram.setUniform("colormapMode", 0);

    glBindVertexArray(pathline_vao);
    glBindBuffer(GL_ARRAY_BUFFER, pathline_vbo);

    for (const Pathline& pl : pathlines) {
        if (pl.vertices.size() < 4) 
            continue;

        glBufferData(GL_ARRAY_BUFFER,
            pl.vertices.size() * sizeof(float),
            pl.vertices.data(),
            GL_DYNAMIC_DRAW);

        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(pl.vertices.size() / 2));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void computePathline(int x, int y, int t)
{
    // TODO: compute pathlines starting from x,y position and time step t. enable switching between euler and runge kutta
    // Hint: implement trilinear interpolation (bilinear in space + linear in time),
    //       forward+backward integration advancing in both space and time
     
    // TODO: set any useful uniforms & update VBO & draw



    if (!grid_data_loaded) 
        return;
    if (num_timesteps <= 1) {
        return;
    }

    glm::vec2 seed((float)x, (float)y);
    float t_start = (float)t;


    std::vector<glm::vec2> backward = integrateOnePathSide(seed, t_start, -1.0f);
    std::vector<glm::vec2> forward = integrateOnePathSide(seed, t_start, +1.0f);

    Pathline pl;

    int W = vol_dim[0];
    int H = vol_dim[1];


    auto pushNDC = [&](glm::vec2 p) {
        float nx = (p.x / (float)(W - 1)) * 2.0f - 1.0f;
        float ny = (p.y / (float)(H - 1)) * 2.0f - 1.0f;
        pl.vertices.push_back(nx);
        pl.vertices.push_back(ny);
        };

    for (auto it = backward.rbegin(); it != backward.rend(); ++it) {
        pushNDC(*it);
    }
    for (size_t i = 1; i < forward.size(); ++i) {
        pushNDC(forward[i]);
    }

    if (pl.vertices.size() >= 4) {
        pathlines.push_back(pl);
    }
}

void drawGlyphs() {
    // TODO: draw arrows/glyphs
    // Hint: iterate over grid with sampling_rate stride, compute arrow geometry
    // (shaft + arrowhead) in NDC, upload to VBO, draw with GL_LINES

        if (!grid_data_loaded) 
            return;

        int W = vol_dim[0];
        int H = vol_dim[1];


        glyph_vertices.clear();

        const float ARROW_LEN_CONST = 0.04f;   
        const float ARROW_SCALE = 0.05f;   
        const float HEAD_FRACTION = 0.3f;     
        const float HEAD_ANGLE_DEG = 150.0f;  

        const float head_angle_rad = HEAD_ANGLE_DEG * 3.14159265f / 180.0f;
        const float cos_a = std::cos(head_angle_rad);
        const float sin_a = std::sin(head_angle_rad);

        for (int j = 0; j < H; j += sampling_rate) {
            for (int i = 0; i < W; i += sampling_rate) {

                int idx = (loaded_timestep * data_size + j * W + i) * 3;
                float vx = vector_array[idx + 0];
                float vy = vector_array[idx + 1];

                float mag = std::sqrt(vx * vx + vy * vy);
                if (mag < 1e-6f) 
                    continue;   

                float dx = vx / mag;
                float dy = vy / mag;

                float L = glyph_scaled_length ? mag * ARROW_SCALE : ARROW_LEN_CONST;

                float px = (float(i) / float(W - 1)) * 2.0f - 1.0f;
                float py = (float(j) / float(H - 1)) * 2.0f - 1.0f;

                float tx = px + L * dx;
                float ty = py + L * dy;

                float head_len = L * HEAD_FRACTION;

                float left_dx = cos_a * dx - sin_a * dy;
                float left_dy = sin_a * dx + cos_a * dy;
                float right_dx = cos_a * dx + sin_a * dy;
                float right_dy = -sin_a * dx + cos_a * dy;

                float lx = tx + head_len * left_dx;
                float ly = ty + head_len * left_dy;
                float rx = tx + head_len * right_dx;
                float ry = ty + head_len * right_dy;


                glyph_vertices.push_back(px); glyph_vertices.push_back(py);
                glyph_vertices.push_back(tx); glyph_vertices.push_back(ty);

                glyph_vertices.push_back(tx); glyph_vertices.push_back(ty);
                glyph_vertices.push_back(lx); glyph_vertices.push_back(ly);

                glyph_vertices.push_back(tx); glyph_vertices.push_back(ty);
                glyph_vertices.push_back(rx); glyph_vertices.push_back(ry);
            }
        }

        if (glyph_vertices.empty()) return;

        glBindVertexArray(glyph_vao);
        glBindBuffer(GL_ARRAY_BUFFER, glyph_vbo);
        glBufferData(GL_ARRAY_BUFFER,
            glyph_vertices.size() * sizeof(float),
            glyph_vertices.data(),
            GL_DYNAMIC_DRAW);

        vectorProgram.setUniform("vertexColor", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        vectorProgram.setUniform("model", glm::mat4(1.0f));

        glDrawArrays(GL_LINES, 0, (GLsizei)(glyph_vertices.size() / 2));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
   
}

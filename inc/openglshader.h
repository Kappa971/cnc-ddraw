#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

/* OpenGL 2.0 */

static char PASSTHROUGH_VERT_SHADER_110[] =
    "#version 110\n"
    "varying vec2 TEX0; \n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = ftransform(); \n"
    "    TEX0 = gl_MultiTexCoord0.xy; \n"
    "}\n";


static char PALETTE_FRAG_SHADER_110[] =
    "#version 110\n"
    "uniform sampler2D Texture; \n"
    "uniform sampler2D PaletteTexture; \n"
    "varying vec2 TEX0; \n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec4 pIndex = texture2D(Texture, TEX0); \n"
    "   gl_FragColor = texture2D(PaletteTexture, vec2(pIndex.r * (255.0/256.0) + (0.5/256.0), 0)); \n"
    "}\n";


static char PASSTHROUGH_FRAG_SHADER_110[] =
    "#version 110\n"
    "uniform sampler2D Texture; \n"
    "varying vec2 TEX0; \n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec4 texel = texture2D(Texture, TEX0); \n"
    "   gl_FragColor = texel; \n"
    "}\n";

/* OpenGL 3.0 */

static char PASSTHROUGH_VERT_SHADER[] =
    "#version 130\n"
    "in vec4 VertexCoord;\n"
    "in vec4 COLOR;\n"
    "in vec4 TexCoord;\n"
    "out vec4 COL0;\n"
    "out vec4 TEX0;\n"
    "uniform mat4 MVPMatrix;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVPMatrix * VertexCoord;\n"
    "    COL0 = COLOR;\n"
    "    TEX0.xy = TexCoord.xy;\n"
    "}\n";


static char PALETTE_FRAG_SHADER[] =
    "#version 130\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D Texture;\n"
    "uniform sampler2D PaletteTexture;\n"
    "in vec4 TEX0;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 pIndex = texture(Texture, TEX0.xy);\n"
    "    FragColor = texture(PaletteTexture, vec2(pIndex.r * (255.0/256.0) + (0.5/256.0), 0));\n"
    "}\n";


static char PASSTHROUGH_FRAG_SHADER[] =
    "#version 130\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D Texture;\n"
    "in vec4 TEX0;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 texel = texture(Texture, TEX0.xy);\n"
    "    FragColor = texel;\n"
    "}\n";


/*   
//    The following code is licensed under the MIT license: https://gist.github.com/TheRealMJP/bc503b0b87b643d3505d41eab8b332ae
//    Ported from code: https://gist.github.com/TheRealMJP/c83b8c0f46b63f3a88a5986f4fa982b1
//    Samples a texture with Catmull-Rom filtering, using 9 texture fetches instead of 16.
//    See http://vec3.ca/bicubic-filtering-in-fewer-taps/ for more details
//    Modified to use 5 texture fetches
*/

static char CATMULL_ROM_FRAG_SHADER[] =
    "#version 130\n"
    "out mediump vec4 FragColor;\n"
    "uniform int FrameDirection;\n"
    "uniform int FrameCount;\n"
    "uniform vec2 OutputSize;\n"
    "uniform vec2 TextureSize;\n"
    "uniform vec2 InputSize;\n"
    "uniform sampler2D Texture;\n"
    "in vec4 TEX0;\n"
    "\n"
    "#define Source Texture\n"
    "#define vTexCoord TEX0.xy\n"
    "\n"
    "#define SourceSize vec4(TextureSize, 1.0 / TextureSize)\n"
    "#define outsize vec4(OutputSize, 1.0 / OutputSize)\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec2 samplePos = vTexCoord * SourceSize.xy;\n"
    "    vec2 texPos1 = floor(samplePos - 0.5) + 0.5;\n"
    "\n"
    "    vec2 f = samplePos - texPos1;\n"
    "\n"
    "    vec2 w0 = f * (-0.5 + f * (1.0 - 0.5 * f));\n"
    "    vec2 w1 = 1.0 + f * f * (-2.5 + 1.5 * f);\n"
    "    vec2 w2 = f * (0.5 + f * (2.0 - 1.5 * f));\n"
    "    vec2 w3 = f * f * (-0.5 + 0.5 * f);\n"
    "\n"
    "    vec2 w12 = w1 + w2;\n"
    "    vec2 offset12 = w2 / (w1 + w2);\n"
    "\n"
    "    vec2 texPos0  = texPos1 - 1.;\n"
    "    vec2 texPos3  = texPos1 + 2.;\n"
    "    vec2 texPos12 = texPos1 + offset12;\n"
    "\n"
    "    texPos0  *= SourceSize.zw;\n"
    "    texPos3  *= SourceSize.zw;\n"
    "    texPos12 *= SourceSize.zw;\n"
    "\n"
    "    float wtm = w12.x * w0.y;\n"
    "    float wml = w0.x * w12.y;\n"
    "    float wmm = w12.x * w12.y;\n"
    "    float wmr = w3.x * w12.y;\n"
    "    float wbm = w12.x * w3.y;\n"
    "\n"
    "    vec3 result = vec3(0.0f);\n"
    "\n"
    "    result += texture(Source, vec2(texPos12.x, texPos0.y)).rgb * wtm;\n"
    "    result += texture(Source, vec2(texPos0.x, texPos12.y)).rgb * wml;\n"
    "    result += texture(Source, vec2(texPos12.x, texPos12.y)).rgb * wmm;\n"
    "    result += texture(Source, vec2(texPos3.x, texPos12.y)).rgb * wmr;\n"
    "    result += texture(Source, vec2(texPos12.x, texPos3.y)).rgb * wbm;\n"
    "\n"
    "    FragColor = vec4(result * (1. / (wtm + wml + wmm + wmr + wbm)), 1.0);\n"
    "}\n";


/*
// The following code is licensed under the MIT license:
// Hyllian's jinc windowed-jinc 2-lobe sharper with anti-ringing Shader
// Copyright (C) 2011-2016 Hyllian/Jararaca - sergiogdb@gmail.com
// https://github.com/libretro/glsl-shaders/blob/09e2942efbab2f51b60ff0b93b7761b0b0570910/windowed/shaders/lanczos2-sharp.glsl
*/

static char LANCZOS2_FRAG_SHADER[] =
    "#version 130\n"
    "#define JINC2_WINDOW_SINC 0.5\n"
    "#define JINC2_SINC 1.0\n"
    "#define JINC2_AR_STRENGTH 0.8\n"
    "\n"
    "out vec4 FragColor;\n"
    "uniform int FrameDirection;\n"
    "uniform int FrameCount;\n"
    "uniform vec2 OutputSize;\n"
    "uniform vec2 TextureSize;\n"
    "uniform vec2 InputSize;\n"
    "uniform sampler2D Texture;\n"
    "in vec4 TEX0;\n"
    "\n"
    "const   float pi                = 3.1415926535897932384626433832795;\n"
    "const   float wa                = JINC2_WINDOW_SINC*pi;\n"
    "const   float wb                = JINC2_SINC*pi;\n"
    "\n"
    "// Calculates the distance between two points\n"
    "float d(vec2 pt1, vec2 pt2)\n"
    "{\n"
    "  vec2 v = pt2 - pt1;\n"
    "  return sqrt(dot(v,v));\n"
    "}\n"
    "\n"
    "vec3 min4(vec3 a, vec3 b, vec3 c, vec3 d)\n"
    "{\n"
    "    return min(a, min(b, min(c, d)));\n"
    "}\n"
    "\n"
    "vec3 max4(vec3 a, vec3 b, vec3 c, vec3 d)\n"
    "{\n"
    "    return max(a, max(b, max(c, d)));\n"
    "}\n"
    "\n"
    "vec4 resampler(vec4 x)\n"
    "{\n"
    "   vec4 res;\n"
    "\n"
    "   res.x = (x.x==0.0) ?  wa*wb  :  sin(x.x*wa)*sin(x.x*wb)/(x.x*x.x);\n"
    "   res.y = (x.y==0.0) ?  wa*wb  :  sin(x.y*wa)*sin(x.y*wb)/(x.y*x.y);\n"
    "   res.z = (x.z==0.0) ?  wa*wb  :  sin(x.z*wa)*sin(x.z*wb)/(x.z*x.z);\n"
    "   res.w = (x.w==0.0) ?  wa*wb  :  sin(x.w*wa)*sin(x.w*wb)/(x.w*x.w);\n"
    "\n"
    "   return res;\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 color;\n"
    "    vec4 weights[4];\n"
    "\n"
    "    vec2 dx = vec2(1.0, 0.0);\n"
    "    vec2 dy = vec2(0.0, 1.0);\n"
    "\n"
    "    vec2 pc = TEX0.xy*TextureSize;\n"
    "\n"
    "    vec2 tc = (floor(pc-vec2(0.5,0.5))+vec2(0.5,0.5));\n"
    "     \n"
    "    weights[0] = resampler(vec4(d(pc, tc    -dx    -dy), d(pc, tc           -dy), d(pc, tc    +dx    -dy), d(pc, tc+2.0*dx    -dy)));\n"
    "    weights[1] = resampler(vec4(d(pc, tc    -dx       ), d(pc, tc              ), d(pc, tc    +dx       ), d(pc, tc+2.0*dx       )));\n"
    "    weights[2] = resampler(vec4(d(pc, tc    -dx    +dy), d(pc, tc           +dy), d(pc, tc    +dx    +dy), d(pc, tc+2.0*dx    +dy)));\n"
    "    weights[3] = resampler(vec4(d(pc, tc    -dx+2.0*dy), d(pc, tc       +2.0*dy), d(pc, tc    +dx+2.0*dy), d(pc, tc+2.0*dx+2.0*dy)));\n"
    "\n"
    "    dx = dx/TextureSize;\n"
    "    dy = dy/TextureSize;\n"
    "    tc = tc/TextureSize;\n"
    "\n"
    "    vec3 c00 = texture(Texture, tc    -dx    -dy).xyz;\n"
    "    vec3 c10 = texture(Texture, tc           -dy).xyz;\n"
    "    vec3 c20 = texture(Texture, tc    +dx    -dy).xyz;\n"
    "    vec3 c30 = texture(Texture, tc+2.0*dx    -dy).xyz;\n"
    "    vec3 c01 = texture(Texture, tc    -dx       ).xyz;\n"
    "    vec3 c11 = texture(Texture, tc              ).xyz;\n"
    "    vec3 c21 = texture(Texture, tc    +dx       ).xyz;\n"
    "    vec3 c31 = texture(Texture, tc+2.0*dx       ).xyz;\n"
    "    vec3 c02 = texture(Texture, tc    -dx    +dy).xyz;\n"
    "    vec3 c12 = texture(Texture, tc           +dy).xyz;\n"
    "    vec3 c22 = texture(Texture, tc    +dx    +dy).xyz;\n"
    "    vec3 c32 = texture(Texture, tc+2.0*dx    +dy).xyz;\n"
    "    vec3 c03 = texture(Texture, tc    -dx+2.0*dy).xyz;\n"
    "    vec3 c13 = texture(Texture, tc       +2.0*dy).xyz;\n"
    "    vec3 c23 = texture(Texture, tc    +dx+2.0*dy).xyz;\n"
    "    vec3 c33 = texture(Texture, tc+2.0*dx+2.0*dy).xyz;\n"
    "	\n"
    "    //  Get min/max samples\n"
    "    vec3 min_sample = min4(c11, c21, c12, c22);\n"
    "    vec3 max_sample = max4(c11, c21, c12, c22);\n"
    "	\n"
    "    color = vec3(dot(weights[0], vec4(c00.x, c10.x, c20.x, c30.x)), dot(weights[0], vec4(c00.y, c10.y, c20.y, c30.y)), dot(weights[0], vec4(c00.z, c10.z, c20.z, c30.z)));\n"
    "    color+= vec3(dot(weights[1], vec4(c01.x, c11.x, c21.x, c31.x)), dot(weights[1], vec4(c01.y, c11.y, c21.y, c31.y)), dot(weights[1], vec4(c01.z, c11.z, c21.z, c31.z)));\n"
    "    color+= vec3(dot(weights[2], vec4(c02.x, c12.x, c22.x, c32.x)), dot(weights[2], vec4(c02.y, c12.y, c22.y, c32.y)), dot(weights[2], vec4(c02.z, c12.z, c22.z, c32.z)));\n"
    "    color+= vec3(dot(weights[3], vec4(c03.x, c13.x, c23.x, c33.x)), dot(weights[3], vec4(c03.y, c13.y, c23.y, c33.y)), dot(weights[3], vec4(c03.z, c13.z, c23.z, c33.z)));\n"
    "    color = color/(dot(weights[0], vec4(1,1,1,1)) + dot(weights[1], vec4(1,1,1,1)) + dot(weights[2], vec4(1,1,1,1)) + dot(weights[3], vec4(1,1,1,1)));\n"
    "\n"
    "    // Anti-ringing\n"
    "    vec3 aux = color;\n"
    "    color = clamp(color, min_sample, max_sample);\n"
    "    color = mix(aux, color, JINC2_AR_STRENGTH);\n"
    "\n"
    "    // final sum and weight normalization\n"
    "    FragColor.xyz = color;\n"
    "}\n";

/*
// The following code is licensed under the MIT license:
// Hyllian's xBR-lv2 Shader
// Copyright (C) 2011-2016 Hyllian - sergiogdb@gmail.com
// https://github.com/libretro/glsl-shaders/blob/09e2942efbab2f51b60ff0b93b7761b0b0570910/xbr/shaders/xbr-lv2.glsl
*/

static char XBR_LV2_VERT_SHADER[] =
    "#version 130\n"
    " \n"
    "#define texCoord TEX0\n"
    "#define t1 TEX1\n"
    "#define t2 TEX2\n"
    "#define t3 TEX3\n"
    "#define t4 TEX4\n"
    "#define t5 TEX5\n"
    "#define t6 TEX6\n"
    "#define t7 TEX7\n"
    "\n"
    "in  vec4 VertexCoord;\n"
    "in  vec4 Color;\n"
    "in  vec2 TexCoord;\n"
    "\n"
    "out vec4 color;\n"
    "out vec2 texCoord;\n"
    "out vec4 t1;\n"
    "out vec4 t2;\n"
    "out vec4 t3;\n"
    "out vec4 t4;\n"
    "out vec4 t5;\n"
    "out vec4 t6;\n"
    "out vec4 t7;\n"
    "\n"
    "uniform mat4 MVPMatrix;\n"
    "uniform int FrameDirection;\n"
    "uniform int FrameCount;\n"
    "uniform vec2 OutputSize;\n"
    "uniform vec2 TextureSize;\n"
    "uniform vec2 InputSize;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVPMatrix * VertexCoord;\n"
    "    color = Color;\n"
    "\n"
    "    float dx = (1.0/TextureSize.x);\n"
    "    float dy = (1.0/TextureSize.y);\n"
    "\n"
    "    texCoord     = TexCoord;\n"
    "	texCoord.x *= 1.00000001;\n"
    "    t1 = TexCoord.xxxy + vec4( -dx, 0, dx,-2.0*dy); // A1 B1 C1\n"
    "    t2 = TexCoord.xxxy + vec4( -dx, 0, dx,    -dy); //  A  B  C\n"
    "    t3 = TexCoord.xxxy + vec4( -dx, 0, dx,      0); //  D  E  F\n"
    "    t4 = TexCoord.xxxy + vec4( -dx, 0, dx,     dy); //  G  H  I\n"
    "    t5 = TexCoord.xxxy + vec4( -dx, 0, dx, 2.0*dy); // G5 H5 I5\n"
    "    t6 = TexCoord.xyyy + vec4(-2.0*dx,-dy, 0,  dy); // A0 D0 G0\n"
    "    t7 = TexCoord.xyyy + vec4( 2.0*dx,-dy, 0,  dy); // C4 F4 I4\n"
    "}\n";

static char XBR_LV2_FRAG_SHADER[] =
    "#version 130\n"
    "\n"
    "//#pragma parameter XBR_SCALE \"xBR Scale\" 3.0 1.0 5.0 1.0\n"
    "#pragma parameter XBR_Y_WEIGHT \"Y Weight\" 48.0 0.0 100.0 1.0\n"
    "#pragma parameter XBR_EQ_THRESHOLD \"Eq Threshold\" 15.0 0.0 50.0 1.0\n"
    "#pragma parameter XBR_LV1_COEFFICIENT \"Lv1 Coefficient\" 0.5 0.0 30.0 0.5\n"
    "#pragma parameter XBR_LV2_COEFFICIENT \"Lv2 Coefficient\" 2.0 1.0 3.0 0.1\n"
    "#pragma parameter small_details \"Preserve Small Details\" 0.0 0.0 1.0 1.0\n"
    "\n"
    "#define mul(a,b) (b*a)\n"
    "\n"
    "// Uncomment just one of the three params below to choose the corner detection\n"
    "//#define CORNER_A\n"
    "//#define CORNER_B\n"
    "#define CORNER_C\n"
    "//#define CORNER_D\n"
    "\n"
    "#ifndef CORNER_A\n"
    "  #define SMOOTH_TIPS\n"
    "#endif\n"
    "\n"
    "#define XBR_SCALE 3.0\n"
    "\n"
    "#define lv2_cf XBR_LV2_COEFFICIENT\n"
    " \n"
    "#define texCoord TEX0\n"
    "#define t1 TEX1\n"
    "#define t2 TEX2\n"
    "#define t3 TEX3\n"
    "#define t4 TEX4\n"
    "#define t5 TEX5\n"
    "#define t6 TEX6\n"
    "#define t7 TEX7\n"
    "\n"
    "\n"
    "//#pragma parameter XBR_SCALE \"xBR Scale\" 3.0 1.0 5.0 1.0\n"
    "#pragma parameter XBR_Y_WEIGHT \"Y Weight\" 48.0 0.0 100.0 1.0\n"
    "#pragma parameter XBR_EQ_THRESHOLD \"Eq Threshold\" 15.0 0.0 50.0 1.0\n"
    "#pragma parameter XBR_LV1_COEFFICIENT \"Lv1 Coefficient\" 0.5 0.0 30.0 0.5\n"
    "#pragma parameter XBR_LV2_COEFFICIENT \"Lv2 Coefficient\" 2.0 1.0 3.0 0.1\n"
    "#pragma parameter small_details \"Preserve Small Details\" 0.0 0.0 1.0 1.0\n"
    "\n"
    "#define mul(a,b) (b*a)\n"
    "\n"
    "// Uncomment just one of the three params below to choose the corner detection\n"
    "//#define CORNER_A\n"
    "//#define CORNER_B\n"
    "#define CORNER_C\n"
    "//#define CORNER_D\n"
    "\n"
    "#ifndef CORNER_A\n"
    "  #define SMOOTH_TIPS\n"
    "#endif\n"
    "\n"
    "#define XBR_SCALE 3.0\n"
    "\n"
    "#define lv2_cf XBR_LV2_COEFFICIENT\n"
    " \n"
    "#define texCoord TEX0\n"
    "#define t1 TEX1\n"
    "#define t2 TEX2\n"
    "#define t3 TEX3\n"
    "#define t4 TEX4\n"
    "#define t5 TEX5\n"
    "#define t6 TEX6\n"
    "#define t7 TEX7\n"
    "\n"
    "out vec4 FragColor;\n"
    "uniform int FrameDirection;\n"
    "uniform int FrameCount;\n"
    "uniform vec2 OutputSize;\n"
    "uniform vec2 TextureSize;\n"
    "uniform vec2 InputSize;\n"
    "uniform sampler2D Texture;\n"
    "in vec2 texCoord;\n"
    "in vec4 t1;\n"
    "in vec4 t2;\n"
    "in vec4 t3;\n"
    "in vec4 t4;\n"
    "in vec4 t5;\n"
    "in vec4 t6;\n"
    "in vec4 t7;\n"
    "\n"
    "#ifdef PARAMETER_UNIFORM\n"
    "uniform float XBR_Y_WEIGHT;\n"
    "uniform float XBR_EQ_THRESHOLD;\n"
    "uniform float XBR_LV1_COEFFICIENT;\n"
    "uniform float XBR_LV2_COEFFICIENT;\n"
    "uniform float small_details;\n"
    "#else\n"
    "#define XBR_Y_WEIGHT 48.0\n"
    "#define XBR_EQ_THRESHOLD 15.0\n"
    "#define XBR_LV1_COEFFICIENT 0.5\n"
    "#define XBR_LV2_COEFFICIENT 2.0\n"
    "#define small_details 0.0\n"
    "#endif\n"
    "// END PARAMETERS //\n"
    "\n"
    "const float coef         = 2.0;\n"
    "const vec3 rgbw          = vec3(14.352, 28.176, 5.472);\n"
    "const vec4 eq_threshold  = vec4(15.0, 15.0, 15.0, 15.0);\n"
    "\n"
    "vec4 delta   = vec4(1.0/XBR_SCALE, 1.0/XBR_SCALE, 1.0/XBR_SCALE, 1.0/XBR_SCALE);\n"
    "vec4 delta_l = vec4(0.5/XBR_SCALE, 1.0/XBR_SCALE, 0.5/XBR_SCALE, 1.0/XBR_SCALE);\n"
    "vec4 delta_u = delta_l.yxwz;\n"
    "\n"
    "const  vec4 Ao = vec4( 1.0, -1.0, -1.0, 1.0 );\n"
    "const  vec4 Bo = vec4( 1.0,  1.0, -1.0,-1.0 );\n"
    "const  vec4 Co = vec4( 1.5,  0.5, -0.5, 0.5 );\n"
    "const  vec4 Ax = vec4( 1.0, -1.0, -1.0, 1.0 );\n"
    "const  vec4 Bx = vec4( 0.5,  2.0, -0.5,-2.0 );\n"
    "const  vec4 Cx = vec4( 1.0,  1.0, -0.5, 0.0 );\n"
    "const  vec4 Ay = vec4( 1.0, -1.0, -1.0, 1.0 );\n"
    "const  vec4 By = vec4( 2.0,  0.5, -2.0,-0.5 );\n"
    "const  vec4 Cy = vec4( 2.0,  0.0, -1.0, 0.5 );\n"
    "const  vec4 Ci = vec4(0.25, 0.25, 0.25, 0.25);\n"
    "\n"
    "const vec3 Y = vec3(0.2126, 0.7152, 0.0722);\n"
    "\n"
    "// Difference between vector components.\n"
    "vec4 df(vec4 A, vec4 B)\n"
    "{\n"
    "    return vec4(abs(A-B));\n"
    "}\n"
    "\n"
    "// Compare two vectors and return their components are different.\n"
    "vec4 diff(vec4 A, vec4 B)\n"
    "{\n"
    "    return vec4(notEqual(A, B));\n"
    "}\n"
    "\n"
    "// Determine if two vector components are equal based on a threshold.\n"
    "vec4 eq(vec4 A, vec4 B)\n"
    "{\n"
    "    return (step(df(A, B), vec4(XBR_EQ_THRESHOLD)));\n"
    "}\n"
    "\n"
    "// Determine if two vector components are NOT equal based on a threshold.\n"
    "vec4 neq(vec4 A, vec4 B)\n"
    "{\n"
    "    return (vec4(1.0, 1.0, 1.0, 1.0) - eq(A, B));\n"
    "}\n"
    "\n"
    "// Weighted distance.\n"
    "vec4 wd(vec4 a, vec4 b, vec4 c, vec4 d, vec4 e, vec4 f, vec4 g, vec4 h)\n"
    "{\n"
    "    return (df(a,b) + df(a,c) + df(d,e) + df(d,f) + 4.0*df(g,h));\n"
    "}\n"
    "\n"
    "vec4 weighted_distance(vec4 a, vec4 b, vec4 c, vec4 d, vec4 e, vec4 f, vec4 g, vec4 h, vec4 i, vec4 j, vec4 k, vec4 l)\n"
    "{\n"
    "	return (df(a,b) + df(a,c) + df(d,e) + df(d,f) + df(i,j) + df(k,l) + 2.0*df(g,h));\n"
    "}\n"
    "\n"
    "float c_df(vec3 c1, vec3 c2) \n"
    "{\n"
    "      vec3 df = abs(c1 - c2);\n"
    "      return df.r + df.g + df.b;\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 edri, edr, edr_l, edr_u, px; // px = pixel, edr = edge detection rule\n"
    "    vec4 irlv0, irlv1, irlv2l, irlv2u, block_3d;\n"
    "    vec4 fx, fx_l, fx_u; // inequations of straight lines.\n"
    "\n"
    "    vec2 fp  = fract(texCoord*TextureSize);\n"
    "\n"
    "    vec3 A1 = texture(Texture, t1.xw ).xyz;\n"
    "    vec3 B1 = texture(Texture, t1.yw ).xyz;\n"
    "    vec3 C1 = texture(Texture, t1.zw ).xyz;\n"
    "    vec3 A  = texture(Texture, t2.xw ).xyz;\n"
    "    vec3 B  = texture(Texture, t2.yw ).xyz;\n"
    "    vec3 C  = texture(Texture, t2.zw ).xyz;\n"
    "    vec3 D  = texture(Texture, t3.xw ).xyz;\n"
    "    vec3 E  = texture(Texture, t3.yw ).xyz;\n"
    "    vec3 F  = texture(Texture, t3.zw ).xyz;\n"
    "    vec3 G  = texture(Texture, t4.xw ).xyz;\n"
    "    vec3 H  = texture(Texture, t4.yw ).xyz;\n"
    "    vec3 I  = texture(Texture, t4.zw ).xyz;\n"
    "    vec3 G5 = texture(Texture, t5.xw ).xyz;\n"
    "    vec3 H5 = texture(Texture, t5.yw ).xyz;\n"
    "    vec3 I5 = texture(Texture, t5.zw ).xyz;\n"
    "    vec3 A0 = texture(Texture, t6.xy ).xyz;\n"
    "    vec3 D0 = texture(Texture, t6.xz ).xyz;\n"
    "    vec3 G0 = texture(Texture, t6.xw ).xyz;\n"
    "    vec3 C4 = texture(Texture, t7.xy ).xyz;\n"
    "    vec3 F4 = texture(Texture, t7.xz ).xyz;\n"
    "    vec3 I4 = texture(Texture, t7.xw ).xyz;\n"
    "\n"
    "    vec4 b  = vec4(dot(B ,rgbw), dot(D ,rgbw), dot(H ,rgbw), dot(F ,rgbw));\n"
    "    vec4 c  = vec4(dot(C ,rgbw), dot(A ,rgbw), dot(G ,rgbw), dot(I ,rgbw));\n"
    "    vec4 d  = b.yzwx;\n"
    "    vec4 e  = vec4(dot(E,rgbw));\n"
    "    vec4 f  = b.wxyz;\n"
    "    vec4 g  = c.zwxy;\n"
    "    vec4 h  = b.zwxy;\n"
    "    vec4 i  = c.wxyz;\n"
    "    \n"
    "	vec4 i4, i5, h5, f4;\n"
    "	\n"
    "	float y_weight = XBR_Y_WEIGHT;\n"
    "	\n"
    "	if (small_details < 0.5)\n"
    "	{\n"
    "		i4 = vec4(dot(I4,rgbw), dot(C1,rgbw), dot(A0,rgbw), dot(G5,rgbw));\n"
    "		i5 = vec4(dot(I5,rgbw), dot(C4,rgbw), dot(A1,rgbw), dot(G0,rgbw));\n"
    "		h5 = vec4(dot(H5,rgbw), dot(F4,rgbw), dot(B1,rgbw), dot(D0,rgbw));\n"
    "	}\n"
    "	else\n"
    "	{\n"
    "		i4 = mul( mat4x3(I4, C1, A0, G5), y_weight * Y );\n"
    "		i5 = mul( mat4x3(I5, C4, A1, G0), y_weight * Y );\n"
    "		h5 = mul( mat4x3(H5, F4, B1, D0), y_weight * Y );\n"
    "	}\n"
    "\n"
    "    // These inequations define the line below which interpolation occurs.\n"
    "    fx   = (Ao*fp.y+Bo*fp.x); \n"
    "    fx_l = (Ax*fp.y+Bx*fp.x);\n"
    "    fx_u = (Ay*fp.y+By*fp.x);\n"
    "\n"
    "    irlv1 = irlv0 = diff(e,f) * diff(e,h);\n"
    "\n"
    "#ifdef CORNER_B\n"
    "    irlv1      = (irlv0 * ( neq(f,b) * neq(h,d) + eq(e,i) * neq(f,i4) * neq(h,i5) + eq(e,g) + eq(e,c) ) );\n"
    "#endif\n"
    "#ifdef CORNER_D\n"
    "    vec4 c1 = i4.yzwx;\n"
    "    vec4 g0 = i5.wxyz;\n"
    "    irlv1     = (irlv0  *  ( neq(f,b) * neq(h,d) + eq(e,i) * neq(f,i4) * neq(h,i5) + eq(e,g) + eq(e,c) ) * (diff(f,f4) * diff(f,i) + diff(h,h5) * diff(h,i) + diff(h,g) + diff(f,c) + eq(b,c1) * eq(d,g0)));\n"
    "#endif\n"
    "#ifdef CORNER_C\n"
    "    irlv1     = (irlv0  * ( neq(f,b) * neq(f,c) + neq(h,d) * neq(h,g) + eq(e,i) * (neq(f,f4) * neq(f,i4) + neq(h,h5) * neq(h,i5)) + eq(e,g) + eq(e,c)) );\n"
    "#endif\n"
    "\n"
    "    irlv2l = diff(e,g) * diff(d,g);\n"
    "    irlv2u = diff(e,c) * diff(b,c);\n"
    "\n"
    "    vec4 fx45i = clamp((fx   + delta   -Co - Ci)/(2.0*delta  ), 0.0, 1.0);\n"
    "    vec4 fx45  = clamp((fx   + delta   -Co     )/(2.0*delta  ), 0.0, 1.0);\n"
    "    vec4 fx30  = clamp((fx_l + delta_l -Cx     )/(2.0*delta_l), 0.0, 1.0);\n"
    "    vec4 fx60  = clamp((fx_u + delta_u -Cy     )/(2.0*delta_u), 0.0, 1.0);\n"
    "\n"
    "    vec4 wd1, wd2;\n"
    "	if (small_details < 0.5)\n"
    "	{\n"
    "		wd1 = wd( e, c,  g, i, h5, f4, h, f);\n"
    "		wd2 = wd( h, d, i5, f, i4,  b, e, i);\n"
    "	}\n"
    "	else\n"
    "	{\n"
    "		wd1 = weighted_distance( e, c, g, i, f4, h5, h, f, b, d, i4, i5);\n"
    "		wd2 = weighted_distance( h, d, i5, f, b, i4, e, i, g, h5, c, f4);\n"
    "	}\n"
    "\n"
    "    edri  = step(wd1, wd2) * irlv0;\n"
    "    edr   = step(wd1 + vec4(0.1, 0.1, 0.1, 0.1), wd2) * step(vec4(0.5, 0.5, 0.5, 0.5), irlv1);\n"
    "    edr_l = step( lv2_cf*df(f,g), df(h,c) ) * irlv2l * edr;\n"
    "    edr_u = step( lv2_cf*df(h,c), df(f,g) ) * irlv2u * edr;\n"
    "\n"
    "    fx45  = edr   * fx45;\n"
    "    fx30  = edr_l * fx30;\n"
    "    fx60  = edr_u * fx60;\n"
    "    fx45i = edri  * fx45i;\n"
    "\n"
    "    px = step(df(e,f), df(e,h));\n"
    "\n"
    "#ifdef SMOOTH_TIPS\n"
    "    vec4 maximos = max(max(fx30, fx60), max(fx45, fx45i));\n"
    "#endif\n"
    "#ifndef SMOOTH_TIPS\n"
    "    vec4 maximos = max(max(fx30, fx60), fx45);\n"
    "#endif\n"
    "\n"
    "    vec3 res1 = E;\n"
    "    res1 = mix(res1, mix(H, F, px.x), maximos.x);\n"
    "    res1 = mix(res1, mix(B, D, px.z), maximos.z);\n"
    "    \n"
    "    vec3 res2 = E;\n"
    "    res2 = mix(res2, mix(F, B, px.y), maximos.y);\n"
    "    res2 = mix(res2, mix(D, H, px.w), maximos.w);\n"
    "    \n"
    "    vec3 res = mix(res1, res2, step(c_df(E, res1), c_df(E, res2)));\n"
    "\n"
    "    FragColor.xyz = res;\n"
    "}\n";

#endif

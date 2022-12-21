#ifndef CONSTANTS
#define CONSTANTS

#include "stb_image.h"
#include <string>

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 1;
const int DIRECTIONAL_SHADOW_MAP_SIZE = 1024;

const std::string MAIN_VERTEX_SHADER = "Shaders/vertexShader.txt";
const std::string MAIN_FRAGMENT_SHADER = "Shaders/fragmentShader.txt";

const std::string DIRECTIONAL_SHADOW_VERTEX_SHADER = "Shaders/directionalShadowMapVertexShader.txt";
const std::string DIRECTIONAL_SHADOW_FRAGMENT_SHADER = "Shaders/directionalShadowMapFragmentShader.txt";

const std::string OMNIDIRECTIONAL_SHADOW_VERTEX_SHADER = "Shaders/omniShadowMapVertexShader.txt";
const std::string OMNIDIRECTIONAL_SHADOW_GEOMETRY_SHADER = "Shaders/omniShadowMapGeometryShader.txt";
const std::string OMNIDIRECTIONAL_SHADOW_FRAGMENT_SHADER = "Shaders/omniShadowMapFragmentShader.txt";

const std::string BACKGROUND_MUSIC = "Music/forest.wav";
const std::string FLASHLIGHT_SOUND = "Music/flashlight.wav";

const std::string PLANE_TEXTURE = "Textures/dirt.png";
const std::string ERROR_TEXTURE = "Textures/plain.png";

const std::string HOUSE_MODEL = "Models/Old House 2 3D Models.obj";
const std::string FLASHLIGHT_MODEL = "Models/Linterna.obj";
const std::string WELL_MODEL = "Models/old_well.obj";

const std::string SKYBOX_FT = "Textures/Skybox/space_ft.png";
const std::string SKYBOX_BK = "Textures/Skybox/space_bk.png";
const std::string SKYBOX_UP = "Textures/Skybox/space_up.png";
const std::string SKYBOX_DN = "Textures/Skybox/space_dn.png";
const std::string SKYBOX_RT = "Textures/Skybox/space_rt.png";
const std::string SKYBOX_LF = "Textures/Skybox/space_lf.png";
#endif
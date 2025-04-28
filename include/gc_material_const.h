#ifndef GC_MATERIALS_CONSTANTS_H
#define GC_MATERIALS_CONSTANTS_H

/*
 * Material constants based on values from:
 * "OpenGL Materials" by Eric Devernay
 * Source: http://devernay.free.fr/cours/opengl/materials.html
 *
 * These values are used for defining ambient, diffuse, specular,
 * and shininess properties of materials in OpenGL.
 *
 * The following materials are available:
 *
 *   Gemstones:
 *     emerald, jade, obsidian, pearl, ruby, turquoise
 *
 *   Metals:
 *     brass, bronze, chrome, copper, gold, silver
 *
 *   Plastics:
 *     plastic_black, plastic_cyan, plastic_green, plastic_red, plastic_white, plastic_yellow
 *
 *   Rubbers:
 *     rubber_black, rubber_cyan, rubber_green, rubber_red, rubber_white, rubber_yellow
 *
 * Naming Convention:
 *
 *     GC_MTL_(MATERIAL FAMILY)_(COLOR IF ANY)_(PROPERTY)
 *
 * Examples:
 *
 *     GC_MTL_EMERALD_AMBIENT
 *     GC_MTL_PLASTIC_BLACK_DIFFUSE
 *     GC_MTL_RUBBER_RED_SPECULAR
 *     GC_MTL_GOLD_SHININESS
 *
 * Available Properties:
 *
 *     - AMBIENT   (vec3: {r g b})
 *     - DIFFUSE   (vec3: {r g b})
 *     - SPECULAR  (vec3: {r g b})
 *     - SHININESS (float)
 *
 * Notes:
 * - For materials without a color modifier (like gold, ruby, pearl), just use the material name.
 * - For plastics and rubbers, the **material** comes first ("plastic", "rubber"), and the **color** second.
 */

// emerald
#define GC_MTL_EMERALD_AMBIENT {0.0215, 0.1745, 0.0215}
#define GC_MTL_EMERALD_DIFFUSE {0.07568, 0.61424, 0.07568}
#define GC_MTL_EMERALD_SPECULAR {0.633, 0.727811, 0.633}
#define GC_MTL_EMERALD_SHININESS 0.6

// jade 
#define GC_MTL_JADE_AMBIENT {0.135, 0.2225, 0.1575}
#define GC_MTL_JADE_DIFFUSE {0.54, 0.89, 0.63}
#define GC_MTL_JADE_SPECULAR {0.316228, 0.316228, 0.316228}
#define GC_MTL_JADE_SHININESS 0.1

// obsidian 
#define GC_MTL_OBSIDIAN_AMBIENT {0.05375, 0.05, 0.06625}
#define GC_MTL_OBSIDIAN_DIFFUSE {0.18275, 0.17, 0.22525}
#define GC_MTL_OBSIDIAN_SPECULAR {0.332741, 0.328634, 0.346435}
#define GC_MTL_OBSIDIAN_SHININESS 0.3

//pearl
#define GC_MTL_PEARL_AMBIENT {0.25, 0.20725, 0.20725}
#define GC_MTL_PEARL_DIFFUSE {1, 0.829, 0.829}
#define GC_MTL_PEARL_SPECULAR {0.296648, 0.296648, 0.296648}
#define GC_MTL_PEARL_SHININESS 0.088

//ruby
#define GC_MTL_RUBY_AMBIENT {0.1745, 0.01175, 0.01175}
#define GC_MTL_RUBY_DIFFUSE {0.61424, 0.04136, 0.04136}
#define GC_MTL_RUBY_SPECULAR {0.727811, 0.626959, 0.626959}
#define GC_MTL_RUBY_SHININESS 0.6

//turquoise
#define GC_MTL_TURQUOISE_AMBIENT{0.1, 0.18725, 0.1745}
#define GC_MTL_TURQUOISE_DIFFUSE {0.396, 0.74151, 0.69102}
#define GC_MTL_TURQUOISE_SPECULAR {0.297254, 0.30829, 0.306678}
#define GC_MTL_TURQUOISE_SHININESS 0.1

// brass
#define GC_MTL_BRASS_AMBIENT {0.329412, 0.223529, 0.027451}
#define GC_MTL_BRASS_DIFFUSE {0.780392, 0.568627, 0.113725}
#define GC_MTL_BRASS_SPECULAR {0.992157, 0.941176, 0.807843}
#define GC_MTL_BRASS_SHININESS 0.21794872

// bronze
#define GC_MTL_BRONZE_AMBIENT {0.2125, 0.1275, 0.054}
#define GC_MTL_BRONZE_DIFFUSE {0.714, 0.4284, 0.18144}
#define GC_MTL_BRONZE_SPECULAR {0.393548, 0.271906, 0.166721}
#define GC_MTL_BRONZE_SHININESS 0.2

// chrome, 
#define GC_MTL_CHROME_AMBIENT {0.25, 0.25, 0.25}
#define GC_MTL_CHROME_DIFFUSE {0.4, 0.4, 0.4}
#define GC_MTL_CHROME_SPECULAR {0.774597, 0.774597, 0.774597}
#define GC_MTL_CHROME_SHININESS 0.6

// copper
#define GC_MTL_COPPER_AMBIENT {0.19125, 0.0735, 0.0225}
#define GC_MTL_COPPER_DIFFUSE {0.7038, 0.27048, 0.0828}
#define GC_MTL_COPPER_SPECULAR {0.256777, 0.137622, 0.086014}
#define GC_MTL_COPPER_SHININESS 0.1

// gold
#define GC_MTL_GOLD_AMBIENT {0.24725, 0.1995, 0.0745}
#define GC_MTL_GOLD_DIFFUSE {0.75164, 0.60648, 0.22648}
#define GC_MTL_GOLD_SPECULAR {0.628281, 0.555802, 0.366065}
#define GC_MTL_GOLD_SHININESS 0.4

//silver
#define GC_MTL_SILVER_AMBIENT{0.19225, 0.19225, 0.19225}
#define GC_MTL_SILVER_DIFFUSE {0.50754, 0.50754, 0.50754}
#define GC_MTL_SILVER_SPECULAR {0.508273, 0.508273, 0.508273}
#define GC_MTL_SILVER_SHININESS 0.4

//black plastic
#define GC_MTL_PLASTIC_AMBIENT{0.0, 0.0, 0.0}
#define GC_MTL_PLASTIC_DIFFUSE {0.01, 0.01, 0.01}
#define GC_MTL_PLASTIC_SPECULAR {0.50, 0.50, 0.50}
#define GC_MTL_PLASTIC_SHININESS .25

//cyan plastic
#define GC_MTL_PLASTIC_CYAN_AMBIENT{0.0, 0.1, 0.06}
#define GC_MTL_PLASTIC_CYAN_DIFFUSE {0.0, 0.50980392, 0.50980392}
#define GC_MTL_PLASTIC_CYAN_SPECULAR {0.50196078, 0.50196078, 0.50196078}
#define GC_MTL_PLASTIC_CYAN_SHININESS .25

//green plastic
#define GC_MTL_PLASTIC_GREEN_AMBIENT {0.0, 0.0, 0.0}
#define GC_MTL_PLASTIC_GREEN_DIFFUSE {0.1, 0.35, 0.1}
#define GC_MTL_PLASTIC_GREEN_SPECULAR {0.45, 0.55, 0.45}
#define GC_MTL_PLASTIC_GREEN_SHININESS .25

//red plastic
#define GC_MTL_PLASTIC_RED_AMBIENT {0.0, 0.0, 0.0}
#define GC_MTL_PLASTIC_RED_DIFFUSE {0.5, 0.0, 0.0}
#define GC_MTL_PLASTIC_RED_SPECULAR {0.7, 0.6, 0.6}
#define GC_MTL_PLASTIC_RED_SHININESS .25

//white plastic
#define GC_MTL_PLASTIC_WHITE_AMBIENT{0.0, 0.0, 0.0}
#define GC_MTL_PLASTIC_WHITE_DIFFUSE {0.55, 0.55, 0.55}
#define GC_MTL_PLASTIC_WHITE_SPECULAR {0.70, 0.70, 0.70}
#define GC_MTL_PLASTIC_WHITE_SHININESS .25

//yellow plastic
#define GC_MTL_PLASTIC_YELLOW_AMBIENT {0.0, 0.0, 0.0}
#define GC_MTL_PLASTIC_YELLOW_DIFFUSE {0.5, 0.5, 0.0}
#define GC_MTL_PLASTIC_YELLOW_SPECULAR {0.60, 0.60, 0.50}
#define GC_MTL_PLASTIC_YELLOW_SHININESS .25

//black rubber
#define GC_MTL_RUBBER_BLACK_AMBIENT {0.02, 0.02, 0.02}
#define GC_MTL_RUBBER_BLACK_DIFFUSE {0.01, 0.01, 0.01}
#define GC_MTL_RUBBER_BLACK_SPECULAR {0.4, 0.4, 0.4}
#define GC_MTL_RUBBER_BLACK_SHININESS .078125

//cyan rubber
#define GC_MTL_RUBBER_CYAN_AMBIENT {0.0, 0.05, 0.05}
#define GC_MTL_RUBBER_CYAN_DIFFUSE {0.4, 0.5, 0.5}
#define GC_MTL_RUBBER_CYAN_SPECULAR {0.04, 0.7, 0.7}
#define GC_MTL_RUBBER_CYAN_SHININESS .078125

//green rubber
#define GC_MTL_RUBBER_GREEN_AMBIENT {0.0, 0.05, 0.0}
#define GC_MTL_RUBBER_GREEN_DIFFUSE {0.4, 0.5, 0.4}
#define GC_MTL_RUBBER_GREEN_SPECULAR {0.04, 0.7, 0.04}
#define GC_MTL_RUBBER_GREEN_SHININESS .078125

//red rubber
#define GC_MTL_RUBBER_RED_AMBIENT {0.05, 0.0, 0.0}
#define GC_MTL_RUBBER_RED_DIFFUSE{0.5, 0.4, 0.4}
#define GC_MTL_RUBBER_RED_SPECULAR {0.7, 0.04, 0.04}
#define GC_MTL_RUBBER_RED_SHININESS .078125

//white rubber
#define GC_MTL_RUBBER_WHITE_AMBIENT {0.05, 0.05, 0.05}
#define GC_MTL_RUBBER_WHITE_DIFFUSE {0.5, 0.5, 0.5}
#define GC_MTL_RUBBER_WHITE_SPECULAR {0.7, 0.7, 0.7}
#define GC_MTL_RUBBER_WHITE_SHININESS .078125

// yellow rubber
#define GC_MTL_RUBBER_YELLOW_AMBIENT {0.05, 0.05, 0.0}
#define GC_MTL_RUBBER_YELLOW_DIFFUSE {0.5, 0.5, 0.4}
#define GC_MTL_RUBBER_YELLOW_SPECULAR {0.7, 0.7, 0.04}
#define GC_MTL_RUBBER_YELLOW_SHININESS .078125



#endif
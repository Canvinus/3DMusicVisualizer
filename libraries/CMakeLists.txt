cmake_minimum_required(VERSION 3.17)
project(libraries)

set(CMAKE_CXX_STANDARD 14)

#GLFW

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw-3.3.2)

#GLEW

add_subdirectory(glew-2.1.0/build/cmake)

#GLM

add_subdirectory(glm)

#stb

add_subdirectory(stb)

#Assimp

SET (ASSIMP_BUILD_ASSIMP_TOOLS CACHE BOOL OFF)

SET (ASSIMP_BUILD_TESTS CACHE BOOL OFF)

SET (ASSIMP_BUILD_3DS_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3MF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_AC_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_AMF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASE_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASSBIN_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_B3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_BLEND_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_BVH_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_C4D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_COB_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_COLLADA_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_CSM_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_DXF_IMPORTER CACHE BOOL OFF)
#SET (ASSIMP_BUILD_FBX_IMPORTER CACHE BOOL OFF)
#SET (ASSIMP_BUILD_GLTF_IMPORTER CACHE BOOL OFF)
#SET (ASSIMP_BUILD_GLTF2_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_HMP_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_IFC_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_IRR_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_IRRMESH_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_LWO_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_LWS_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MD2_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MD3_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MD4_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MD5_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MDC_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MDL_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MMD_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_MS3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_NDO_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_NFF_IMPORTER CACHE BOOL OFF)
#SET (ASSIMP_BUILD_OBJ_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OFF_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OGRE_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OPENGEX_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_PLY_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_Q3BSP_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_Q3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_RAW_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_SIB_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_SMD_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STEP_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STL_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_TERRAGEN_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X3D_IMPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_XGL_IMPORTER CACHE BOOL OFF)

SET (ASSIMP_BUILD_3DS_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_3MF_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASSBIN_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASSJSON_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_ASSXML_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_COLLADA_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_FBX_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_GLTF_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_GLTF2_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OBJ_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_OPENGEX_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_PLY_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STEP_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_STL_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X_EXPORTER CACHE BOOL OFF)
SET (ASSIMP_BUILD_X3D_EXPORTER CACHE BOOL OFF)

add_subdirectory(assimp-5.0.1)

#FreeType

add_subdirectory(freetype-2.10.4)
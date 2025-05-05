#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>

#include "gc_logs.h"
#include "mesh.h"
#include "model.h"

#include "shader_variables.h"

typedef struct aiScene aiScene;
typedef struct aiNode aiNode;
typedef struct aiMesh aiMesh;
typedef struct aiFace aiFace;
typedef struct aiMaterial aiMaterial;
typedef enum aiTextureType aiTextureType;

static void GS_Model_ProcessNode(GSModel *model, aiNode *node, aiScene *scene);
static void GS_Model_ProcessMesh(GSMesh *gsmesh, aiMesh *aimesh, aiScene *scene);
//static void _GS_Model_ProcessNode_internals(GSModel *model, aiNode *node, aiScene *scene, GSMesh *gsmesh);
static GSModel *GS_Model_Init();

void GS_Model_Free(GSModel *model)
{
    for (unsigned int i =0; i < gcv_size(model->meshes); i++)
    {
        GS_Mesh_Static_Free(&gcv_at(model->meshes,i));
    }
    free(model);
}

void GS_Model_Draw(GSModel *model, GS_ShaderHandle handle)
{
    for (unsigned int i = 0; i < gcv_size(model->meshes); i++)
    {
        GS_Mesh_Draw(&gcv_at(model->meshes, i), handle);
    }
}

GSModel *GS_Model_Load(const char* path) {
    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = aiImportFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs
    );
  
    // If the import failed, report it
    if(NULL == scene) {
      GC_LOG(aiGetErrorString());
      return NULL;
    }

    GSModel * model = GS_Model_Init();
  
    // Now we can access the file's contents
    GS_Model_ProcessNode(model, scene->mRootNode, scene);
  
    // We're done. Release all resources associated with this import
    aiReleaseImport(scene);
    return model;
}

static void GS_Model_ProcessNode(GSModel *model, aiNode *node, aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        GSMesh gsmesh;
        GS_Mesh_Create(&gsmesh);

        aiMesh *aimesh = scene->mMeshes[node->mMeshes[i]];
        GS_Model_ProcessMesh(&gsmesh, aimesh, scene);
        gcv_push(model->meshes, gsmesh);
    }
    

    for (unsigned int i =0; i < node->mNumChildren; i++)
    {
        GS_Model_ProcessNode(model, node->mChildren[i], scene);
    }
}

static void GS_Model_ProcessMesh(GSMesh *gsmesh, aiMesh *aimesh, aiScene *scene)
{
    GSVertex vertex;
    for (unsigned int i =0; i < aimesh->mNumVertices; i++)
    {
        vertex.position[0] = aimesh->mVertices[i].x;
        vertex.position[1] = aimesh->mVertices[i].y;
        vertex.position[2] = aimesh->mVertices[i].z;

        vertex.normal[0] = aimesh->mNormals[i].x;
        vertex.normal[1] = aimesh->mNormals[i].y;
        vertex.normal[2] = aimesh->mNormals[i].z;
        
        if (aimesh->mTextureCoords[0])
        {
            vertex.texCoords[0] = aimesh->mTextureCoords[0][i].x;
            vertex.texCoords[1] = aimesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.texCoords[0] = 0; 
            vertex.texCoords[1] = 0;
        }

        gcv_push(gsmesh->vertices, vertex);
    }
    
    for (unsigned int i =0; i < aimesh->mNumFaces; i++)
    {
        aiFace *face = &aimesh->mFaces[i];
        for (unsigned int j =0; j < face->mNumIndices; j++)
        {
            gcv_push(gsmesh->indices, face->mIndices[j]);
        }
    }

    if (aimesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];
        GS_Model_Material_Load(gsmesh, material, aiTextureType_DIFFUSE, GS_TexDiffuse);
        GS_Model_Material_Load(gsmesh, material, aiTextureType_SPECULAR, GS_TexSpecular);
    }

    GS_Mesh_Setup(gsmesh);

}

void GS_Model_Material_Load(GSMesh *mesh, aiMaterial *material, 
                            aiTextureType aiType, GSTextureType type)
{

}

static GSModel *GS_Model_Init()
{
    GSModel *model = malloc(sizeof(GSModel));
    model->meshes = gcv_create(model->meshes);
    return model;
}

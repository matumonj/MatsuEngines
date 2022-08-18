#include "FbxLoader.h"

#include <cassert>


using namespace DirectX;

/// <summary>
/// �ÓI�����o��ϐ��̎���
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize()
{
    //�ď������`�F�b�N
    assert(fbxManager == nullptr);
    //�������烁���o�ϐ��ɑ��
    this->device = device;
    //FBX�}�l�[�W���̐���
    fbxManager = FbxManager::Create();
    //FBX�}�l�[�W���̓��o�͐ݒ�
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBX�C���|�[�^�̐���
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
    //�e��FBX�C���X�^���X�̔j��
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

f_Model* FbxLoader::LoadModelFromFile(const string ModelName)
{
    //���f���Ɠ������O�̃t�@�C������ǂݍ���
    const string directoryPath = baseDirectory + ModelName + "/";
    //�g���q,FBX��t�^
    const string fileName = ModelName + ".fbx";
    //�A�����ăt���p�X�𓾂�
    const string fullpath = directoryPath + fileName;

    //�t�@�C�������w�肵��FBX�t�@�C����ǂݍ���

    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }

    //�V�[������
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //�t�@�C�����烍�[�h����FBX�ɏ����V�[���ɃC���|�[�g
    fbxImporter->Import(fbxScene);

    //���f������
    f_Model* fbxModel = new f_Model();
    fbxModel->name = ModelName;
    //FBX�m�[�h�̐����擾
    int nodeCount = fbxScene->GetNodeCount();
    //���炩���ߕK�v�����̃��������m�ۂ��邱�ƂŁA�A�h���X�������̂�\�h
    fbxModel->nodes.reserve(nodeCount);
    //���[�g�m�[�h���珇�ɉ�͂��Ă��ł�ɗ�������
    ParseNodeRecursive(fbxModel, fbxScene->GetRootNode());
    //FBX�V�[�����
    fbxModel->fbxScene = fbxScene;

    //�o�b�t�@����
    fbxModel->CreateBuffers();

    return fbxModel;
}

void FbxLoader::ParseNodeRecursive(f_Model* f_Model, FbxNode* fbxNode, Node* parent)
{
    //�m�[�h�����擾
    string name = fbxNode->GetName();
    //���f���Ƀm�[�h��ǉ��iTodo�j
    f_Model->nodes.emplace_back();
    Node& node = f_Model->nodes.back();
    //�m�[�h�����擾
    node.name = fbxNode->GetName();

    //FBX�m�[�h�̃��[�J���ړ����
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    //�`���ϊ����đ��
    node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f };
    node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
    node.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 1.0f };

    //�X�P�[���A��]���s�ړ��s��̌v�Z
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //���[�J���ό`�s��̌v�Z
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling;   //���[���h�s��ɃX�P�[�����O�𔽉f
    node.transform *= matRotation;  //���[���h�s��ɉ�]�𔽉f
    node.transform *= matTranslation;   //���[���h�s��ɕ��s�ړ��𔽉f
    //FBX�m�[�h�̏�����͂��ăm�[�h�ɋL�^�iTodo�j

    //�O���[�o���ό`�s��̌v�Z
    node.globalTransform = node.transform;
    if (parent) {
        node.parent = parent;
        //�e�̕ό`����Z
        node.globalTransform *= parent->globalTransform;
    }
//    Hand= f_Model[0].GetModelTransform();
    //FBX�m�[�h�̃��b�V��������́iTodo�j
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute) {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
            f_Model->meshNode = &node;
            ParseMesh(f_Model, fbxNode);
        }
    }

    //�q�m�[�h�ɑ΂��čċA�Ăяo��
    for (int i = 0; i < fbxNode->GetChildCount(); i++) {
        ParseNodeRecursive(f_Model, fbxNode->GetChild(i));
    }
}

void FbxLoader::ParseMesh(f_Model* f_Model, FbxNode* fbxNode)
{
    //�m�[�h�̃��b�V�����擾
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    //���_���W�ǂݎ��
    ParseMeshVertices(f_Model, fbxMesh);
    //�ʂ��\������Ł[���̓ǂݎ��
    ParseMeshFaces(f_Model, fbxMesh);
    //�}�e���A���̓ǂݎ��
    ParseMaterial(f_Model, fbxNode);
    //�X�L�j���O���̓ǂݎ��
    ParseSkin(f_Model, fbxMesh);
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
    //�s
    for (int i = 0; i < 4; i++) {
        //��
        for (int j = 0; j < 4; j++) {
            //1�v�f�R�s�[
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }
}

void FbxLoader::ParseMeshVertices(f_Model* f_Model, FbxMesh* fbxMesh)
{
    auto& vertices = f_Model->vertices;

    //���_���W�f�[�^�̐�
    const int PolygonVertexCount = fbxMesh->GetPolygonVertexCount(); //�ύX�OfbxMesh->GetControlPointsCount() : ���_�� / fbxMesh->GetPolygonVertexCount() : �|���S�����_�C���f�b�N�X�� 
    fbxMesh->GetPolygonVertexCount();
    //�K�v���������_�f�[�^�z����m��
    f_Model::VertexPosNormalUvSkin vert{};
    f_Model->vertices.resize(PolygonVertexCount, vert);
}

void FbxLoader::ParseMeshFaces(f_Model* f_Model, FbxMesh* fbxMesh)
{
    auto& vertices = f_Model->vertices; //564prin
    auto& indices = f_Model->indices;

    //���_���W�f�[�^���ő�
    const int PolygonVertexCountMax = fbxMesh->GetPolygonVertexCount() - 1;
    //�J�E���g�p
    int PolygonVertexCount = 0;

    //1�t�@�C���ɕ������b�V���̃��f���͔�Ή�
    assert(indices.size() == 0);
    //�ʂ̐�
    const int polygonCount = fbxMesh->GetPolygonCount();
    //UV�f�[�^��
    const int textureUVCount = fbxMesh->GetTextureUVCount();
    //UV�����X�g
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);
    //�ʂ��Ƃ̏��ǂݎ��
    for (int i = 0; i < polygonCount; i++)
    {
        //�ʂ��\�����钸�_�̐����擾 (3�Ȃ�O�p�`�|���S��)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);

        //1���_������
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX���_�z��̃C���f�b�N�X
            int index = fbxMesh->GetPolygonVertex(i, j); //�|���S���ԍ�(i)�ƁA�|���S�����̒��_�C���f�b�N�X(j)����A���_�ԍ����擾����
            assert(index >= 0);

            //���_�@���ǂݍ���
            f_Model::VertexPosNormalUvSkin& vertex = vertices[PolygonVertexCount];
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }

            //�e�N�X�`��UV�ǂݍ���
            if (textureUVCount > 0)
            {
                FbxVector2 uvs;
                bool lUnmappedUV;
                // o�Ԗڌ��ߑł��œǂݍ���
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = 1.0f - (float)uvs[1];
                }
            }

            //FBX���b�V���̒��_���W�z����擾
            FbxVector4* pCoord = fbxMesh->GetControlPoints();

            //FBX���b�V���̑S���_���W�����f�����̔z��ɃR�s�[����
            //���W���R�s�[
            vertex.pos.x = (float)pCoord[index][0];
            vertex.pos.y = (float)pCoord[index][1];
            vertex.pos.z = (float)pCoord[index][2];



            //�C���f�b�N�X�z��ɒ��_�C���f�b�N�X�ǉ�
            //3���_�܂łȂ�
            if (j < 3)
            {
                //1���_�ǉ����A����2���_�ƎO�p�`���\�z����
                indices.push_back(PolygonVertexCount);
            } else//4���_��
            {
                //3���_�ǉ���
                //�l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }
            if (PolygonVertexCount == PolygonVertexCountMax)
            {
            } else
            {
                PolygonVertexCount++;
            }

        }
    }
}

void FbxLoader::ParseMaterial(f_Model* f_Model, FbxNode* fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();
    if (materialCount > 0) {
        //�擪�̃}�e���A�����擾
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
        //�e�N�X�`����ǂݍ��񂾂��ǂ�����\���t���O
        bool textureLoaded = false;

        if (material) {
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                //�����W��
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                f_Model->ambient.x = (float)ambient.Get()[0];
                f_Model->ambient.y = (float)ambient.Get()[1];
                f_Model->ambient.z = (float)ambient.Get()[2];

                //�g�U���ˌ��W��
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                f_Model->diffuse.x = (float)diffuse.Get()[0];
                f_Model->diffuse.y = (float)diffuse.Get()[1];
                f_Model->diffuse.z = (float)diffuse.Get()[2];

                //�f�B�t���[�Y�e�N�X�`�������o��
                const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                if (diffuseProperty.IsValid())
                {
                    const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                    if (texture) {
                        const char* filepath = texture->GetFileName();
                        //�t�@�C���p�X����t�@�C�������o
                        string path_str(filepath);
                        string name = ExtractFileName(path_str);
                        //�e�N�X�`���ǂݍ���
                        LoadTexture(f_Model, baseDirectory + f_Model->name + "/" + name);
                        textureLoaded = true;
                    }
                }

            }
        }
        //�e�N�X�`�����Ȃ��ꍇ�͔��e�N�X�`����\��
        if (!textureLoaded) {
            LoadTexture(f_Model, baseDirectory + defaultTextureFileName);
        }
    }
}

void FbxLoader::LoadTexture(f_Model* f_Model, const std::string& fullpath)
{
    HRESULT result = S_FALSE;
    //WIC�e�N�X�`���̃��[�h
    TexMetadata& metadata = f_Model->metadata;
    ScratchImage& scratchImg = f_Model->scratchImg;
    //���j�R�[�h������ɕϊ�
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
    result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
    if (FAILED(result)) {
        assert(0);
    }
}

void FbxLoader::ParseSkin(f_Model* f_Model, FbxMesh* fbxMesh)
{
    FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
    //�X�L�j���O��񂪂Ȃ���ΏI��
    if (fbxSkin == nullptr) {
        //�e���_�ɂ��Ă̏���
        for (int i = 0; i < f_Model->vertices.size(); i++)
        {
            //�ŏ��̃{�[��(�P�ʍs��)�̉e��100%�ɂ���
            f_Model->vertices[i].boneIndex[0] = 0;
            f_Model->vertices[i].boneWeight[0] = 1.0f;
        }
        return;
    }

    //�{�[���z��̎Q��
    std::vector<f_Model::Bone>& bones = f_Model->bones;

    //�{�[���̐�
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);

    //�S�Ẵ{�[���ɂ���
    for (int i = 0; i < clusterCount; i++) {
        //FBX�{�[�����
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

        //�{�[�����̂̃m�[�h�̖��O���擾
        const char* boneName = fbxCluster->GetLink()->GetName();

        //�V�����{�[����ǉ����A�ǉ������{�[���̎Q�Ƃ𓾂�
        bones.emplace_back(f_Model::Bone(boneName));
        f_Model::Bone& bone = bones.back();
        //����{�[����FBX�̃{�[����R�Â���
        bone.fbxCluster = fbxCluster;
        //FBX���珉���p���s����擾����
        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);

        //XMMatrix�^�ɕϊ�����
        XMMATRIX initialPose;
        ConvertMatrixFromFbx(&initialPose, fbxMat);

        //�����p���t�s��𓾂�
        bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
    }

    //�{�[���ԍ��ƃX�L�j���O�E�F�C�g�̃y�A
    struct WeightSet {
        UINT index;
        float weight;
    };

    int verticesMax = fbxMesh->GetControlPointsCount();

    //�񎟌��z��i�W���b�N�z��j
    //list:���_���e�����󂯂�{�[���̑S���X�g
    //vector:�����S���_��
    std::vector<std::list<WeightSet>>weightLists(verticesMax);

    //�S�Ẵ{�[���ɂ���
    for (int i = 0; i < clusterCount; i++) {
        //FBX�{�[�����
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //���̃{�[���ɉe�����󂯂钸�_�̐�
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        //���̃{�[���ɉe�����󂯂钸�_�̔z��
        int* controlPointIndices = fbxCluster->GetControlPointIndices();
        double* controlPointWeights = fbxCluster->GetControlPointWeights();

        //�e�����󂯂�S���_�ɂ���
        for (int j = 0; j < controlPointIndicesCount; j++) {
            //���_�ԍ�
            int controlPointIndex = controlPointIndices[j];
            //�X�L���E�F�C�g
            float weight = (float)controlPointWeights[j];
            //���̒��_�̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��ǉ�
            weightLists[controlPointIndex].emplace_back(WeightSet{ (UINT)i, weight });
        }
    }

    //�Ăіʂ̏����擾����K�v������
    //�ʂ̐�
    const int polygonCount = fbxMesh->GetPolygonCount();
    //�J�E���g�p
    int PolygonVertexCount = 0;
    //���_���W�f�[�^���ő�
    const int PolygonVertexCountMax = fbxMesh->GetPolygonVertexCount();
    //�e���_�̃R���g���[���|�C���g
    std::vector<int> vertexControlPoint(PolygonVertexCountMax);
    //�z��Ŏ���
    for (int i = 0; i < polygonCount; i++)
    {
        //�ʂ��\�����钸�_�̐����擾 (3�Ȃ�O�p�`�|���S��)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);

        //1���_������
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX���_�z��̃C���f�b�N�X
            int index = fbxMesh->GetPolygonVertex(i, j); //�|���S���ԍ�(i)�ƁA�|���S�����̒��_�C���f�b�N�X(j)����A���_�ԍ����擾����
            assert(index >= 0);
            vertexControlPoint[PolygonVertexCount] = index;
            PolygonVertexCount++;
        }
    }

    //���_�z�񏑂������p�̎Q��
    auto& vertices = f_Model->vertices;

    //�e���_�ɂ��Ă̏���
    for (int i = 0; i < verticesMax; i++) {
        //���_�̃E�F�C�g����ł��傫��4��I��
        auto& weightList = weightLists[i];
        //�召��r�p�̃����_�����w�肵�č~���Ƀ\�[�g
        weightList.sort(
            [](auto const& lhs, auto const& rhs) {
                //���̗v�f�̕����傫�����ture�@�����łȂ����false��Ԃ�
                return lhs.weight > rhs.weight;
            });

        int weightArrrayIndex = 0;
        //�~���\�[�g�ς݂̃E�F�C�g���X�g����
        for (auto& WeightSet : weightList) {

            for (int j = 0; j < PolygonVertexCountMax; j++) {
                //�Ή����钸�_�������
                if (vertexControlPoint[j] == i) {
                    //���_�f�[�^�ɏ�������
                    vertices[j].boneIndex[weightArrrayIndex] = WeightSet.index;
                    vertices[j].boneWeight[weightArrrayIndex] = WeightSet.weight;
                }
            }
            //4�ɒB������I��
            if (++weightArrrayIndex >= f_Model::MAX_BONE_INDICES) {
                float weight = 0.0f;
                //2�Ԗڈȍ~�̃E�F�C�g�����v
                for (int j = 1; j < f_Model::MAX_BONE_INDICES; j++) {
                    weight += vertices[i].boneWeight[j];
                }
                //���v��1.0f�i100%�j�ɂȂ�悤�ɒ���
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;
    //��؂蕶���@'\\'�@���o�Ă����ԍŌ�̕���������
    pos1 = path.rfind('\\');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);

    }
    //��؂蕶���@'/'�@���o�Ă����ԍŌ�̕���������
    pos1 = path.rfind('/');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);

    }

    return path;
}

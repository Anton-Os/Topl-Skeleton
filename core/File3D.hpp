#include <vector>

#include "FileIO.hpp"

#include "primitives/Geo_Model3D.hpp"

typedef std::vector<std::string> fileDataExtracts_t; // type for 3D file extracted string data

#define TAG_TARGET_COUNT 3
#define OPEN_TAG_OFFSET 0
#define INNER_LABEL_OFFSET 1
#define CLOSE_TAG_OFFSET 2
typedef std::string tagTargets_t[TAG_TARGET_COUNT]; // opening tag, inner label, and closing tag

enum F3D_Format {
	F3D_ObjFile, // OBJ file format support
	F3D_DaeFile // Collada file format support
};

enum F3D_ParseType {
    F3D_CheckLine, // checks for cooresponding line start sequence
    F3D_CheckTag // checks for cooresponding xml tag and attributes
};

class File3D_Node {
public:
    File3D_Node() : mName("node"){}
	File3D_Node(std::string name) : mName(name){}

	std::string getName() { return mName; }
    Geo_Model3D genRenderObj();
	/* void appendPosExtractData(std::string data) { mPosData.push_back(data); }
    void appendNormalsExtractData(std::string data){ mNormalsData.push_back(data); }
    void appendTexCoordExtractData(std::string data){ mTexCoordData.push_back(data); }
    void appendIndexExtractData(std::string data){ mIndexData.push_back(data); } */
private:
	const std::string mName; // name of Node
    /* fileDataExtracts_t mPosData;
    fileDataExtracts_t mNormalsData;
    fileDataExtracts_t mTexCoordData;
    fileDataExtracts_t mIndexData; */
};

class File3D_DocumentTree {
public:
	File3D_DocumentTree(const char* source);
	// File3D_DocumentTree(const char* source) : lineParseAttribs({"", "", "", ""}); // mNodeData allocation performed here
	~File3D_DocumentTree() {
        for(unsigned n = 0; n < mNodeNames.size(); n++) if(mNodeData + n != nullptr) delete *(mNodeData + n);
        if (mNodeData != nullptr) free(mNodeData); 
    }
private:
	void readNode(unsigned nodeNum); // reads File3D file in ascii encoding mode

    F3D_Format mFormat; // type of file detected
    F3D_ParseType mParseType; // type of parsing to perform for model data
    // TODO: Utilize the parsing attributes union
    // union File3D_ParseAttribs {
        struct File3D_LineParseAttribs { // used for parsing data based on start of line, see F3D_CheckLine enumeration
            File3D_LineParseAttribs(){}
            File3D_LineParseAttribs(const std::string& p, const std::string& n, const std::string& t, const std::string& i)
                : posData_seq(p), normalsData_seq(n), texCoordData_seq(t), indexData_seq(i){}
            std::string posData_seq;
            std::string normalsData_seq;
            std::string texCoordData_seq;
            std::string indexData_seq;
        } mLineParseAttribs;
        struct File3D_TagParseAttribs { // used for parsing data based on xml tags, see F3D_CheckTag enumeration
            File3D_TagParseAttribs(){}
            // File3D_TagParseAttribs(tagTargets_t p, tagTargets_t n, tagTargets_t t, tagTargets_t i)
            File3D_TagParseAttribs(
                const std::string& pTag, const std::string& pInner,
                const std::string& nTag, const std::string& nInner,
                const std::string& tTag, const std::string& tInner,
                const std::string& iTag, const std::string& iInner
                )
            {
                // TODO: Look for a more concise way to perform array copying operation
                posData_tags[OPEN_TAG_OFFSET] = pTag; posData_tags[INNER_LABEL_OFFSET] = pInner; posData_tags[CLOSE_TAG_OFFSET] = pTag;
                normalsData_tags[OPEN_TAG_OFFSET] = nTag; normalsData_tags[INNER_LABEL_OFFSET] = nInner; normalsData_tags[CLOSE_TAG_OFFSET] = nTag;
                texCoordData_tags[OPEN_TAG_OFFSET] = tTag; texCoordData_tags[INNER_LABEL_OFFSET] = tInner; texCoordData_tags[CLOSE_TAG_OFFSET] = tTag;
                indexData_tags[OPEN_TAG_OFFSET] = iTag; indexData_tags[INNER_LABEL_OFFSET] = iInner; indexData_tags[CLOSE_TAG_OFFSET] = iTag;
                /* posData_tags[OPEN_TAG_OFFSET] = p[OPEN_TAG_OFFSET]; posData_tags[INNER_LABEL_OFFSET] = p[INNER_LABEL_OFFSET]; posData_tags[CLOSE_TAG_OFFSET] = p[CLOSE_TAG_OFFSET];
                normalsData_tags[OPEN_TAG_OFFSET] = n[OPEN_TAG_OFFSET]; normalsData_tags[INNER_LABEL_OFFSET] = n[INNER_LABEL_OFFSET]; normalsData_tags[CLOSE_TAG_OFFSET] = n[CLOSE_TAG_OFFSET];
                texCoordData_tags[OPEN_TAG_OFFSET] = t[OPEN_TAG_OFFSET]; texCoordData_tags[INNER_LABEL_OFFSET] = t[INNER_LABEL_OFFSET]; texCoordData_tags[CLOSE_TAG_OFFSET] = t[CLOSE_TAG_OFFSET];
                indexData_tags[OPEN_TAG_OFFSET] = i[OPEN_TAG_OFFSET]; indexData_tags[INNER_LABEL_OFFSET] = i[INNER_LABEL_OFFSET]; indexData_tags[CLOSE_TAG_OFFSET] = i[CLOSE_TAG_OFFSET]; */
            }
            tagTargets_t posData_tags; //= { "", "", "" };
            tagTargets_t normalsData_tags;
            tagTargets_t texCoordData_tags;
            tagTargets_t indexData_tags;
        } mTagParseAttribs;
	// };
    // } mParseAttribs;

    std::string mNodeNameLabel; // label used to search for valid node name
    std::vector<std::string> mNodeNames; // list of the names of detected nodes

	std::string mFileSource;
	File3D_Node** mNodeData = nullptr;
	bool mIsValidFile; // used for error checking!
};
#ifndef TZW_TECHNIQUE_H
#define TZW_TECHNIQUE_H
#include <vector>
#include <string>
#include <map>
#include "../Shader/ShaderProgram.h"
#include "TechniqueVar.h"
#include "../Math/Matrix44.h"
#include "EngineSrc/3D/Effect/Effect.h"
namespace tzw {
class StdMaterial;
class Node;
class Drawable;
class Material
{
public:
    Material ();
	void loadFromTemplate(std::string name);
	void loadFromFile(std::string filePath);
    static Material * createFromTemplate(std::string name);
	static Material * createFromFile(std::string matPath);
    void setVar(std::string name, const Matrix44 &value);
    void setVar(std::string name,const float& value);
    void setVar(std::string name,const int& value);
	void setVar(std::string name, const vec2 & value);
    void setVar(std::string name, const vec3& value);
    void setVar(std::string name, const vec4 &value);
    void setVar(std::string name, const TechniqueVar & value);
    void setTex(std::string name,Texture * texture,int id = 0);
    void use(ShaderProgram * extraProgram = nullptr);
	unsigned int getMapSlot(std::string mapName);
	unsigned int getMapSlotWithAlias(std::string mapName);
	std::string getAlias(std::string theName);
	ShaderProgram *getProgram() const;
	bool isExist(std::string name);
    Material * clone();

	bool getIsCullFace();

	void setIsCullFace(bool newVal);

	TechniqueVar * get(std::string);

	void inspectIMGUI(std::string name, float min, float max, const char * fmt = "%.2f");

private:
    std::string m_vsPath;
    std::string m_fsPath;
    std::map<std::string,TechniqueVar *> m_varList;

	bool m_isCullFace;
	std::string m_name;
	std::map<std::string, unsigned int> m_texSlotMap;
	std::map<std::string, std::string> m_aliasMap;
	ShaderProgram * m_program;
};

} // namespace tzw

#endif // TZW_TECHNIQUE_H

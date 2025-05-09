#pragma once

#include "Core/Exports.h"
#include "Core/Compilable.h"

class Mesh;
class Material;

class CORE_API Node : public Compilable
{
public:
    Node(const std::string& name);
    ~Node() override;

    void setParent(Node* parent);

    void addChild(std::shared_ptr<Node> child);

    void removeChild(std::shared_ptr<Node> child);

    const std::string& getName() const;

    void setMesh(std::shared_ptr<Mesh> mesh);

    std::shared_ptr<Mesh> getMesh() const;

    void setMaterial(std::shared_ptr<Material> material);

    std::shared_ptr<Material> getMaterial() const;

    void record() override;

    void compile() override;

private:
	std::string m_name;
	std::vector<std::shared_ptr<Node>> m_children;

    Node* m_parent;

protected:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
};

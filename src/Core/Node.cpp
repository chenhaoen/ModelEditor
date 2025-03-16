#include "Core/Node.h"
#include "Core/Mesh.h"
#include "Core/Material.h"

Node::Node(const std::string& name)
    :m_parent(nullptr)
    ,m_name(name)
{
}

Node::~Node()
{
}

void Node::setParent(Node* parent)
{
    m_parent = parent;
}

void Node::addChild(std::shared_ptr<Node> child)
{
    auto where = std::find_if(m_children.begin(), m_children.end(), [&](const std::shared_ptr<Node>& node) {
        return node == child;
        });

    if (where != m_children.end())
    {
        return;
    }

    m_children.emplace_back(child);
    child->setParent(this);
}

void Node::removeChild(std::shared_ptr<Node> child)
{
    auto where = std::find_if(m_children.begin(), m_children.end(), [&](const std::shared_ptr<Node>& node) {
        return node == child;
        });

    if (where == m_children.end())
    {
        return;
    }

    child->setParent(nullptr);
    m_children.erase(where);
}

const std::string& Node::getName() const
{
    return m_name;
}

void Node::setMesh(std::shared_ptr<Mesh> mesh)
{
    m_mesh = mesh;
}

std::shared_ptr<Mesh> Node::getMesh() const
{
    return m_mesh;
}

void Node::setMaterial(std::shared_ptr<Material> material)
{
    m_material = material;
}

std::shared_ptr<Material> Node::getMaterial() const
{
    return m_material;
}

void Node::record()
{
    if (m_mesh)
    {
        m_mesh->record();
    }

    if (m_material)
    {
        m_material->record();
    }

    for (auto child : m_children)
    {
        child->record();
    }
}

void Node::compile()
{
}

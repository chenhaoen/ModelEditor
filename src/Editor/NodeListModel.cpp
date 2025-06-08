#include "Editor/NodeListModel.h"

#include "Core/NodeTree.h"

NodeListModel::NodeListModel(std::shared_ptr<NodeTree> nodeTree, QObject* parent)
    :m_nodeTree(nodeTree)
{
}

void NodeListModel::addNode(std::shared_ptr<Node> node)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_nodeTree->addNode(node);
    endInsertRows();
}

int NodeListModel::rowCount(const QModelIndex& parent) const
{
    return m_nodeTree->size(); // 返回数据容器的大小
}

QVariant NodeListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_nodeTree->size())
        return QVariant();

    if (role == Qt::DisplayRole )
        return QVariant("cube");

    return QVariant();
}

QHash<int, QByteArray> NodeListModel::roleNames() const
{
    QHash<int, QByteArray> result = QAbstractListModel::roleNames();
    result.insert(0, "cube");
    return result;
}

//bool NodeListModel::setData(const QModelIndex& index, const QVariant& value, int role)
//{
//    return false;
//}
//
//Qt::ItemFlags NodeListModel::flags(const QModelIndex& index) const
//{
//    return Qt::ItemFlags();
//}
//
//bool NodeListModel::insertRows(int row, int count, const QModelIndex& parent)
//{
//    return false;
//}

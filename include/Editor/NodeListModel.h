#pragma once

class Node;
class NodeTree;

class NodeListModel :
    public QAbstractListModel
{
public:
    NodeListModel(std::weak_ptr<NodeTree> nodeTree, QObject* parent = nullptr);

    void addNode(std::shared_ptr<Node>);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // ø…—°÷ÿ–¥
    //bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    //Qt::ItemFlags flags(const QModelIndex& index) const override;
    //bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

private:
    std::weak_ptr<NodeTree> m_nodeTree;
};


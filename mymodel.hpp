#ifndef MYMODEL_HPP
#define MYMODEL_HPP

// mymodel.h
#include <QAbstractTableModel>
#include <QString>
#include <QWidget>
#include <QPlainTextEdit>
#include <QTableView>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "instruction.hpp"
#include "simulator.hpp"

class QAbstractTableModel;
class QModelIndex;

class MyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MyModel(int r, int c, QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(int r, int c, const QVariant& value);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    std::string getData(int r, int c);
    QString** getModel();

private:
    int tableRow;
    int tableCol;
    QString** m_gridData;  //holds text entered into QTableView
};

#endif
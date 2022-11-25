// mymodel.cpp
#include "mymodel.hpp"
#include<iostream>

MyModel::MyModel(const int r, const int c, QObject* parent)
    : QAbstractTableModel(parent)
{
    tableRow = r;
    tableCol = c;

    m_gridData = new QString*[tableRow];
    for (int i = 0; i < tableRow; i++) {
        m_gridData[i] = new QString[tableCol];

    }
  
        
}

int MyModel::rowCount(const QModelIndex& /*parent*/) const
{
    return tableRow;
}

int MyModel::columnCount(const QModelIndex& /*parent*/) const
{
    return tableCol;
}


QVariant MyModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (m_gridData[index.row()][index.column()].size() != 0) {
            return m_gridData[index.row()][index.column()];
        }
        else {
            return " ";
        }
    }


    return QVariant();
}

std::string MyModel::getData(int r, int c)
{
    if ((r >= 0 && r < tableRow) && (c >= 0 && c < tableCol)) {
        return m_gridData[r][c].toUtf8().constData();
    }
    else {
        return "";
    }
}

//role = Qt::EditRole
bool MyModel::setData(int r, int c, const QVariant& value)
{
    

    if ((r >= 0 && r < tableRow) && (c >= 0 && c < tableCol)) {

        m_gridData[r][c] = value.toString();
       
        return true;
    }
    
    return false;
}




QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (tableCol == 3) {

            switch (section) {
            case 0:
                return QString("Number");
            case 1:
                return QString("Alias");
            case 2:
                return QString("Value (Hex)");
            }
        }
        else if (tableCol == 2) {
            switch (section) {
            case 0:
                return QString("Address (Hex)");
            case 1:
                return QString("Value (Hex)");

            }
        }
    }
    return QVariant();
}

QString** MyModel::getModel() {
    return m_gridData;
}
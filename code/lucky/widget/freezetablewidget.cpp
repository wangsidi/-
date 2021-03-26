#include "freezetablewidget.h"

#include <QStandardItemModel>
#include <QScrollBar>
#include <QHeaderView>
#include <QDebug>

FreezeTableWidget::FreezeTableWidget(QWidget * parent, int freezeColCounts):
    QTableView(parent)
{
    //只读
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    //去除垂直表头
    this->verticalHeader()->setVisible(false);
    //自适应列宽
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //创建初始化模型
    QStandardItemModel* model = new QStandardItemModel;
    //创建表头
    QStringList header;
    //赋值
    header <<"合约代码          " << "合约名称          " << " 开盘                            " << " 最高                            " << " 最低                            " << " 最新                            " << " 涨停                            " << " 跌停                            "/*<<"测试次数"*/;
    //加入表头
    model->setHorizontalHeaderLabels(header);
    //设置模型
    this->setModel(model);
    //设置滚动条
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                             "{"
                                             "width:8px;"
                                             "background:rgba(0,0,0,0%);"
                                             "margin:0px,0px,0px,0px;"
                                             "padding-top:9px;"
                                             "padding-bottom:9px;"
                                             "}"
                                             "QScrollBar::handle:vertical"
                                             "{"
                                             "width:8px;"
                                             "background:rgba(0,0,0,25%);"
                                             " border-radius:4px;"
                                             "min-height:20;"
                                             "}"
                                             "QScrollBar::handle:vertical:hover"
                                             "{"
                                             "width:8px;"
                                             "background:rgba(0,0,0,50%);"
                                             " border-radius:4px;"
                                             "min-height:20;"
                                             "}"
                                             "QScrollBar::add-line:vertical"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/3.png);"
                                             "subcontrol-position:bottom;"
                                             "}"
                                             "QScrollBar::sub-line:vertical"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/1.png);"
                                             "subcontrol-position:top;"
                                             "}"
                                             "QScrollBar::add-line:vertical:hover"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/4.png);"
                                             "subcontrol-position:bottom;"
                                             "}"
                                             "QScrollBar::sub-line:vertical:hover"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/2.png);"
                                             "subcontrol-position:top;"
                                             "}"
                                             "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                             "{"
                                             "background:rgba(0,0,0,10%);"
                                             "border-radius:4px;"
                                             "}"
                                             );
    //setSelectionBehavior(QAbstractItemView::SelectRows);
    frozenTableView = new QTableView(this);

    m_iFreezeColCounts = freezeColCounts;

    init();

    //connect the headers and scrollbars of both tableviews together
    connect(horizontalHeader(),&QHeaderView::sectionResized, this,
            &FreezeTableWidget::updateSectionWidth);
    connect(verticalHeader(),&QHeaderView::sectionResized, this,
            &FreezeTableWidget::updateSectionHeight);

    connect(frozenTableView->verticalScrollBar(), &QAbstractSlider::valueChanged,
            verticalScrollBar(), &QAbstractSlider::setValue);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged,
            frozenTableView->verticalScrollBar(), &QAbstractSlider::setValue);


}
FreezeTableWidget::~FreezeTableWidget()
{
    delete frozenTableView;
}

//! [init part1]
void FreezeTableWidget::init()
{
    frozenTableView->setModel(model());
    frozenTableView->setFocusPolicy(Qt::NoFocus);
    frozenTableView->verticalHeader()->hide();
    //自适应列宽
    frozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //frozenTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    viewport()->stackUnder(frozenTableView);
    //! [init part1]

    //! [init part2]
/*    frozenTableView->setStyleSheet("QTableView { border: none;"
                                   "background-color: #8EDE21;"
                                   "selection-background-color: #999}"); *///for demo purposes
    //设置和父table同步的光标选择单元格(必要)
    frozenTableView->setSelectionModel(selectionModel());

    for (int col = m_iFreezeColCounts; col < model()->columnCount(); ++col)
        frozenTableView->setColumnHidden(col, true);

    for(int i = 0; i <m_iFreezeColCounts; i++)
    {
        frozenTableView->setColumnWidth(i, columnWidth(0));
    }

    frozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frozenTableView->show();

    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollMode(ScrollPerPixel);
    frozenTableView->setVerticalScrollMode(ScrollPerPixel);
}
//! [init part2]


//! [sections]
void FreezeTableWidget::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
    if (logicalIndex == m_iFreezeColCounts-1){
        int width = 0;
        for(int i = 0; i< m_iFreezeColCounts-1; i++)
        {
            width += columnWidth(i);
        }

        for(int i = 0; i< m_iFreezeColCounts; i++)
        {
            frozenTableView->setColumnWidth(i, (newSize+width)/m_iFreezeColCounts);
        }


        updateFrozenTableGeometry();
    }
}

void FreezeTableWidget::updateSectionHeight(int logicalIndex, int /* oldSize */, int newSize)
{
    frozenTableView->setRowHeight(logicalIndex, newSize);
}
//! [sections]


//! [resize]
void FreezeTableWidget::resizeEvent(QResizeEvent * event)
{
    QTableView::resizeEvent(event);
    updateFrozenTableGeometry();
}
//! [resize]


//! [navigate]
//QModelIndex FreezeTableWidget::moveCursor(CursorAction cursorAction,
//                                          Qt::KeyboardModifiers modifiers)
//{
//      QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

//      if (cursorAction == MoveLeft && current.column() > 0
//              && visualRect(current).topLeft().x() < columnWidth(0) ){
//            const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x()
//                                 - frozenTableView->columnWidth(0);
//            horizontalScrollBar()->setValue(newValue);
//      }

//      return current;
//}
//! [navigate]

void FreezeTableWidget::scrollTo (const QModelIndex & index, ScrollHint hint){
    //if (index.column() > 0)
    QTableView::scrollTo(index, hint);
}

//! [geometry]
void FreezeTableWidget::updateFrozenTableGeometry()
{
    int width = 0;
    for(int i = 0; i< m_iFreezeColCounts; i++)
    {
        width += columnWidth(i);
    }
    frozenTableView->setGeometry(verticalHeader()->width() + frameWidth(),
                                 frameWidth(),width ,
                                 viewport()->height()+horizontalHeader()->height());
}
//! [geometry]

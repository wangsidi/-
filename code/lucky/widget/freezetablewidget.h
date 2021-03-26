#ifndef FREEZETABLEWIDGET_H
#define FREEZETABLEWIDGET_H

#include <QTableView>

//! [Widget definition]
class FreezeTableWidget : public QTableView {
     Q_OBJECT

public:
    FreezeTableWidget(QWidget * parent = nullptr, int freezeColCounts = 2);
    ~FreezeTableWidget();
    void init();
protected:
      void resizeEvent(QResizeEvent *event) override;
      //QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
      void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible) override;

private:
      QTableView *frozenTableView;
      void updateFrozenTableGeometry();
      int m_iFreezeColCounts = 1;


private slots:
      void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
      void updateSectionHeight(int logicalIndex, int oldSize, int newSize);

};
//! [Widget definition]
#endif // FREEZETABLEWIDGET_H

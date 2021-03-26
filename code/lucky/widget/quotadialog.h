#ifndef QUOTADIALOG_H
#define QUOTADIALOG_H

#include <QDialog>

namespace Ui {
class QuotaDialog;
}

class QuotaDialog : public QDialog
{
    Q_OBJECT
Q_SIGNALS:
    void confirmValue(QString nIssueType, QString exchangeType, QString contractCode);
    void allValue(QString nIssueType);

public:
    explicit QuotaDialog(QWidget *parent = nullptr);
    ~QuotaDialog();

    void showError(QString errorInfo);

protected:
    void keyPressEvent(QKeyEvent *) override;

private:
    Ui::QuotaDialog *ui;
};

#endif // QUOTADIALOG_H

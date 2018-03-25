#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class WelcomeWidget;
}

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = 0);
    ~WelcomeWidget();

    void showMessage(const char* str);

private slots:
    void onMove();
    void onStay();
    void onClose();

    void on_pb_close_clicked();

private:
    Ui::WelcomeWidget *ui;
    QTimer * m_pShowTimer;
    QTimer * m_pStayTimer;
    QTimer * m_pCloseTimer;
    QPoint   m_point;
    int      m_nDesktopHeight;
    double   m_dTransparent;

};

#endif // WELCOMEWIDGET_H

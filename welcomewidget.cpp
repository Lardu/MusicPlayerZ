#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include <QDesktopWidget>

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

    m_nDesktopHeight = QApplication::desktop()->height();

    m_dTransparent = 1.0;

    m_pShowTimer = new QTimer(this);
    m_pStayTimer = new QTimer(this);
    m_pCloseTimer = new QTimer(this);

    connect(m_pShowTimer, SIGNAL(timeout()), this, SLOT(onMove()));
    connect(m_pStayTimer, SIGNAL(timeout()), this, SLOT(onStay()));
    connect(m_pCloseTimer, SIGNAL(timeout()), this, SLOT(onClose()));

    QRect rect = QApplication::desktop()->availableGeometry();
    m_point.setX(rect.width() - width());
    m_point.setY(rect.height() - height());
    move(m_point.x(), m_point.y());
    m_pShowTimer->start(5);
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::onMove()
{
    m_nDesktopHeight--;
    move(m_point.x(), m_nDesktopHeight);
    if (m_nDesktopHeight <= m_point.y())
    {
        m_pShowTimer->stop();
        m_pStayTimer->start(5000);
    }


}

void WelcomeWidget::onStay()
{
    m_pStayTimer->stop();
    m_pCloseTimer->start(100);

}

void WelcomeWidget::onClose()
{
    m_dTransparent -= 0.1;
    if (m_dTransparent <= 0.0)
    {
        m_pCloseTimer->stop();
        close();
    }
    else
    {
        setWindowOpacity(m_dTransparent);
    }


}

void WelcomeWidget::on_pb_close_clicked()
{
    this->close();
}

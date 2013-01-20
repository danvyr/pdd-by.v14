#include "MainWindow.h"

#include "ui_MainWindow.h"

class MainWindowImpl : public QObject
{
    Q_OBJECT

public:
    MainWindowImpl(MainWindow& window);

private:
    MainWindow& m_window;
    Ui::MainWindow m_ui;
};

#include "MainWindow.moc"

MainWindowImpl::MainWindowImpl(MainWindow& window) :
    QObject(&window),
    m_window(window)
{
    m_ui.setupUi(&m_window);

    m_window.layout()->setSizeConstraint(QLayout::SetFixedSize);

#ifdef Q_OS_MAC
    m_ui.sectionDescriptionLabel->setAttribute(Qt::WA_MacSmallSize);
    m_ui.topicDescriptionLabel->setAttribute(Qt::WA_MacSmallSize);
    m_ui.ticketDescriptionLabel->setAttribute(Qt::WA_MacSmallSize);
    m_ui.randomTicketDescriptionLabel->setAttribute(Qt::WA_MacSmallSize);
#endif

    connect(m_ui.sectionTrainingButton, SIGNAL(clicked()), m_ui.sectionTrainingAction, SLOT(trigger()));
    connect(m_ui.topicTrainingButton, SIGNAL(clicked()), m_ui.topicTrainingAction, SLOT(trigger()));
    connect(m_ui.ticketTrainingButton, SIGNAL(clicked()), m_ui.ticketTrainingAction, SLOT(trigger()));
    connect(m_ui.randomTicketTrainingButton, SIGNAL(clicked()), m_ui.randomTicketTrainingAction, SLOT(trigger()));
    connect(m_ui.topicExamButton, SIGNAL(clicked()), m_ui.topicExamAction, SLOT(trigger()));
    connect(m_ui.ticketExamButton, SIGNAL(clicked()), m_ui.ticketExamAction, SLOT(trigger()));
    connect(m_ui.randomTicketExamButton, SIGNAL(clicked()), m_ui.randomTicketExamAction, SLOT(trigger()));

    connect(m_ui.aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(m_ui.quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags),
    m_impl(new MainWindowImpl(*this))
{
    //
}

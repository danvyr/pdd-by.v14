#include "MainWindow.h"

#include "ui_MainWindow.h"

#include "SectionDialog.h"
#include "TicketDialog.h"

#include "Forest.h"
#include "Leaf/QuestionLeaf.h"
#include "Limb/IQuestionLimb.h"

#include <QSignalMapper>

class MainWindowImpl : public QObject
{
    Q_OBJECT

    typedef QMap<QAction*, QPair<QPushButton*, char const*> > ActionMap;

public:
    MainWindowImpl(MainWindow& window);

private slots:
    void OnSection(bool isExam = false);
    void OnTopic(bool isExam = false);
    void OnTicket(bool isExam = false);
    void OnRandomTicket(bool isExam = false);
    void OnTopicExam();
    void OnTicketExam();
    void OnRandomTicketExam();

    void ShowTicket(QDialog* intermediateDialog, PddBy::IQuestionCrawler& questions, bool isExam);

    void ActionChanged(QObject* action);

private:
    MainWindow& m_window;
    Ui::MainWindow m_ui;
    ActionMap m_actionMap;
    QSignalMapper m_actionSignalMapper;
    PddBy::IOakPtr m_oak;
};

#include "MainWindow.moc"

MainWindowImpl::MainWindowImpl(MainWindow& window) :
    QObject(&window),
    m_window(window),
    m_oak(PddBy::Forest::CreateOak("/Users/mikedld/Repo/pdd-by.v14.obj/Pdd32_cd"))
{
    m_ui.setupUi(&m_window);

    m_window.layout()->setSizeConstraint(QLayout::SetFixedSize);

    m_actionMap.insert(m_ui.sectionTrainingAction, qMakePair(m_ui.sectionTrainingButton, SLOT(OnSection())));
    m_actionMap.insert(m_ui.topicTrainingAction, qMakePair(m_ui.topicTrainingButton, SLOT(OnTopic())));
    m_actionMap.insert(m_ui.ticketTrainingAction, qMakePair(m_ui.ticketTrainingButton, SLOT(OnTicket())));
    m_actionMap.insert(m_ui.randomTicketTrainingAction, qMakePair(m_ui.randomTicketTrainingButton, SLOT(OnRandomTicket())));
    m_actionMap.insert(m_ui.topicExamAction, qMakePair(m_ui.topicExamButton, SLOT(OnTopicExam())));
    m_actionMap.insert(m_ui.ticketExamAction, qMakePair(m_ui.ticketExamButton, SLOT(OnTicketExam())));
    m_actionMap.insert(m_ui.randomTicketExamAction, qMakePair(m_ui.randomTicketExamButton, SLOT(OnRandomTicketExam())));

    connect(&m_actionSignalMapper, SIGNAL(mapped(QObject*)), this, SLOT(ActionChanged(QObject*)));

    for (ActionMap::const_iterator it = m_actionMap.begin(), end = m_actionMap.end(); it != end; ++it)
    {
        connect(it.value().first, SIGNAL(clicked()), it.key(), SLOT(trigger()));
        connect(it.key(), SIGNAL(triggered()), this, it.value().second);
        connect(it.key(), SIGNAL(changed()), &m_actionSignalMapper, SLOT(map()));
        m_actionSignalMapper.setMapping(it.key(), it.key());

        //it.key()->setEnabled(false);
    }

    connect(m_ui.aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(m_ui.quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindowImpl::OnSection(bool isExam)
{
    SectionDialog* sectionDialog = new SectionDialog(m_oak->GetSectionLimb(), m_oak->GetQuestionLimb(), isExam, &m_window);
    sectionDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(sectionDialog, SIGNAL(TicketPrepared(QDialog*, PddBy::IQuestionCrawler&, bool)),
        SLOT(ShowTicket(QDialog*, PddBy::IQuestionCrawler&, bool)));

    sectionDialog->open();
}

void MainWindowImpl::OnTopic(bool isExam)
{
    Q_UNUSED(isExam);
}

void MainWindowImpl::OnTicket(bool isExam)
{
    Q_UNUSED(isExam);
}

void MainWindowImpl::OnRandomTicket(bool isExam)
{
    PddBy::IQuestionCrawlerPtr questions = m_oak->GetQuestionLimb().ListQuestionsByRandomTicket();
    ShowTicket(0, *questions, isExam);
}

void MainWindowImpl::OnTopicExam()
{
    OnTopic(true);
}

void MainWindowImpl::OnTicketExam()
{
    OnTicket(true);
}

void MainWindowImpl::OnRandomTicketExam()
{
    OnRandomTicket(true);
}

void MainWindowImpl::ShowTicket(QDialog* intermediateDialog, PddBy::IQuestionCrawler& questions, bool isExam)
{
    TicketDialog ticketDialog(m_oak->GetImageLimb(), questions, isExam);

    if (intermediateDialog != 0)
    {
        intermediateDialog->hide();
    }

    m_window.hide();

    ticketDialog.exec();

    m_window.show();

    if (intermediateDialog != 0)
    {
        intermediateDialog->show();
    }
}

void MainWindowImpl::ActionChanged(QObject* action)
{
    ActionMap::const_iterator it = m_actionMap.find(static_cast<QAction*>(action));
    Q_ASSERT(it != m_actionMap.end());

    if (it == m_actionMap.end())
    {
        return;
    }

    it.value().first->setEnabled(it.key()->isEnabled());
}

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags),
    m_impl(new MainWindowImpl(*this))
{
    //
}

#include "TicketDialog.h"

#include "ui_TicketDialog.h"

#include "Leaf/ImageLeaf.h"
#include "Leaf/QuestionLeaf.h"
#include "Limb/IImageLimb.h"
#include "Limb/IQuestionLimb.h"

#include <QKeyEvent>

class TicketDialogImpl : public QObject
{
    Q_OBJECT

    struct QuestionInfo
    {
        PddBy::QuestionLeaf Question;
        QPixmap Pixmap;
        std::size_t AnswerIndex;
    };

public:
    TicketDialogImpl(TicketDialog& dialog, PddBy::IImageLimb const& imageLimb, PddBy::IQuestionCrawler& questions, bool isExam);

    virtual bool eventFilter(QObject* object, QEvent* event);

private:
    void SelectAnswer(int index);
    void SkipQuestion();
    void ShowRegulations();
    void ShowComments();

    void SetupCurrentQuestion();

private:
    TicketDialog& m_dialog;
    bool const m_isExam;
    Ui::TicketDialog m_ui;
    QList<QuestionInfo> m_questions;
    int m_currentQuestion;
};

#include "TicketDialog.moc"

TicketDialogImpl::TicketDialogImpl(TicketDialog& dialog, PddBy::IImageLimb const& imageLimb, PddBy::IQuestionCrawler& questions,
    bool isExam) :
    QObject(&dialog),
    m_dialog(dialog),
    m_isExam(isExam),
    m_currentQuestion(0)
{
    m_ui.setupUi(&m_dialog);

    QuestionInfo questionInfo;
    while (questions.GetNext(questionInfo.Question))
    {
        if (!questionInfo.Question.ImageId.empty())
        {
            PddBy::ImageLeaf const imageLeaf = imageLimb.GetImage(questionInfo.Question.ImageId);
            questionInfo.Pixmap = QPixmap::fromImage(QImage::fromData(&imageLeaf.Data[0], imageLeaf.Data.size()));
        }
        else
        {
            questionInfo.Pixmap = QPixmap();
        }

        questionInfo.AnswerIndex = static_cast<std::size_t>(-1);

        m_questions << questionInfo;
    }

    if (m_isExam)
    {
        m_ui.hotkeysLabel->setText(tr("<b>Return</b> - answer, <b>Space</b> - skip"));
    }
    else
    {
        m_ui.hotkeysLabel->setText(tr("<b>Return</b> - answer, <b>Space</b> - skip, <b>F1</b> - regulations, "
            "<b>F2</b> - comments"));
    }

    m_ui.hotkeysLabel->setAttribute(Qt::WA_MacSmallSize);

    m_dialog.installEventFilter(this);

    SetupCurrentQuestion();
}

bool TicketDialogImpl::eventFilter(QObject* /*object*/, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            SelectAnswer(keyEvent->key() - Qt::Key_1);
            return true;

        case Qt::Key_Space:
            SkipQuestion();
            return true;

        case Qt::Key_F1:
            ShowRegulations();
            return true;

        case Qt::Key_F2:
            ShowComments();
            return true;
        }
    }
    else if (event->type() == QEvent::Resize && m_dialog.isVisible())
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        m_dialog.move(m_dialog.x() + (resizeEvent->oldSize().width() - m_dialog.size().width()) / 2, m_dialog.y());
    }

    return false;
}

void TicketDialogImpl::SelectAnswer(int index)
{
    Q_UNUSED(index);
}

void TicketDialogImpl::SkipQuestion()
{
    m_currentQuestion = (m_currentQuestion + 1) % m_questions.size();
    SetupCurrentQuestion();
}

void TicketDialogImpl::ShowRegulations()
{
    //
}

void TicketDialogImpl::ShowComments()
{
    //
}

void TicketDialogImpl::SetupCurrentQuestion()
{
    QuestionInfo const& questionInfo = m_questions[m_currentQuestion];

    m_ui.imageLabel->setVisible(!questionInfo.Pixmap.isNull());
    if (!questionInfo.Pixmap.isNull())
    {
        m_ui.imageLabel->setPixmap(questionInfo.Pixmap);
        m_ui.imageLabel->setFixedHeight(questionInfo.Pixmap.height() * m_ui.imageLabel->width() / questionInfo.Pixmap.width());
    }

    m_ui.questionLabel->setText(QString::fromStdString(questionInfo.Question.Text));

    for (std::size_t i = 0, count = questionInfo.Question.Answers.size(); i < count; i++)
    {
        m_ui.answersLayout->itemAtPosition(i, 0)->widget()->show();
        m_ui.answersLayout->itemAtPosition(i, 1)->widget()->show();

        QString const answerText = QString::fromStdString(questionInfo.Question.Answers[i]);
        static_cast<QLabel*>(m_ui.answersLayout->itemAtPosition(i, 1)->widget())->setText(answerText);
    }

    for (std::size_t i = questionInfo.Question.Answers.size(), count = m_ui.answersLayout->rowCount() - 1; i < count; i++)
    {
        m_ui.answersLayout->itemAtPosition(i, 0)->widget()->hide();
        m_ui.answersLayout->itemAtPosition(i, 1)->widget()->hide();
    }

    m_dialog.setWindowTitle(tr("%0 / %1").arg(m_currentQuestion + 1).arg(m_questions.size()));

    m_dialog.adjustSize();
}

TicketDialog::TicketDialog(PddBy::IImageLimb const& imageLimb, PddBy::IQuestionCrawler& questions, bool isExam, QWidget* parent,
    Qt::WindowFlags flags) :
    QDialog(parent, flags),
    m_impl(new TicketDialogImpl(*this, imageLimb, questions, isExam))
{
    //
}

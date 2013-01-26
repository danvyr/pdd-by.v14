#pragma once

#include "ICrawler.h"

#include <QDialog>

namespace PddBy
{

struct QuestionLeaf;
typedef ICrawler<QuestionLeaf> IQuestionCrawler;

class IQuestionLimb;
class ISectionLimb;

} // namespace PddBy

class SectionDialogImpl;

class SectionDialog : public QDialog
{
    Q_OBJECT

    friend class SectionDialogImpl;

public:
    SectionDialog(PddBy::ISectionLimb const& sectionLimb, PddBy::IQuestionLimb const& questionLimb, bool isExam,
        QWidget* parent = 0, Qt::WindowFlags flags = 0);

signals:
    void TicketPrepared(QDialog* intermediateDialog, PddBy::IQuestionCrawler& questions, bool isExam);

private:
    SectionDialogImpl* m_impl;
};

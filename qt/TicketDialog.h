#pragma once

#include "ICrawler.h"

#include <QDialog>

namespace PddBy
{

struct QuestionLeaf;
typedef ICrawler<QuestionLeaf> IQuestionCrawler;

class IImageLimb;

} // namespace PddBy

class TicketDialogImpl;

class TicketDialog : public QDialog
{
    Q_OBJECT

public:
    TicketDialog(PddBy::IImageLimb const& imageLimb, PddBy::IQuestionCrawler& questions, bool isExam, QWidget* parent = 0,
        Qt::WindowFlags flags = 0);

private:
    TicketDialogImpl* m_impl;
};

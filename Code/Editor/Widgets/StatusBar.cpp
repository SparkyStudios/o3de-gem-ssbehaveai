#include <Editor/Widgets/StatusBar.h>

#include <QPixmap>

#include <Editor/Widgets/ui_StatusBar.h>
#include <Editor/Windows/MainWindow.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets
{
    StatusBar::StatusBar(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::StatusBar)
    {
        ui->setupUi(this);

        Windows::MainWindow* main_win = static_cast<Windows::MainWindow*>(parent);
        connect(main_win, &Windows::MainWindow::ValidTree, this, &StatusBar::OnValidTree);
        connect(main_win, &Windows::MainWindow::InvalidTree, this, &StatusBar::OnInvalidTree);
    }

    StatusBar::~StatusBar()
    {
        delete ui;
    }

    void StatusBar::SetOpenedFileName(const QString& filename)
    {
        ui->openFileName->setText(filename);
    }

    void StatusBar::OnValidTree()
    {
        QPixmap pix;
        pix.load(":/icons/green-circle.png");
        ui->validityIcon->setToolTip("Valid Tree");

        ui->validityIcon->setPixmap(pix);
        ui->validityIcon->setScaledContents(true);
    }

    void StatusBar::OnInvalidTree()
    {
        QPixmap pix;
        pix.load(":/icons/red-circle.png");
        ui->validityIcon->setToolTip("Invalid Tree");

        ui->validityIcon->setPixmap(pix);
        ui->validityIcon->setScaledContents(true);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Widgets

#include <Editor/Widgets/moc_StatusBar.cpp>

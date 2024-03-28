#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(SpriteEditor &editor, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startdialog.setModal(true);
    startdialog.show();

    colorDialog.setModal(true);

    connect(&startdialog, &StartDialog::onProjectAccepted, &editor, &SpriteEditor::onNewProject);
    connect(&editor, &SpriteEditor::displayFrame, this, &MainWindow::onDisplayCurrentFrame);
    connect(ui->AddFrameButton, &QPushButton::clicked, &editor, &SpriteEditor::addFrame);
    connect(ui->frameAdjustor, &QSpinBox::valueChanged, &editor, &SpriteEditor::adjustFrame);
    connect(&editor, &SpriteEditor::updateFrameBox, this, &MainWindow::updateMaxFrames);
    connect(&editor, &SpriteEditor::updateFrameBox, ui->frameAdjustor, &QSpinBox::setValue);
    connect(&colorDialog, &QColorDialog::colorSelected, [&editor](const QColor& newColor){
        editor.setCurrentColor(newColor);
    });
    connect(ui->ColorButton, &QPushButton::clicked, this, [this](){ colorDialog.show(); });

    connect(ui->action_new, &QAction::triggered, &editor, [this](bool) { startdialog.show(); });
    //delete
    connect(ui->DeleteFrameButton, &QPushButton::clicked, &editor, &SpriteEditor::deleteFrame);
    connect(&editor, SpriteEditor::deleteModelFrame, this,MainWindow::deleteViewFrame);



    // toolbar

    connect (ui->DrawButton, &QPushButton::clicked, &editor, &SpriteEditor::onDrawPressed);
    connect (ui->EraseButton, &QPushButton::clicked, &editor, &SpriteEditor::onErasePressed);

    // mouse

    connect(this, &MainWindow::mousePress, &editor, &SpriteEditor::onMousePressed);
    connect(this, &MainWindow::mouseMove, &editor, &SpriteEditor::onMouseMoved);

    // canvas

    connect(this, &MainWindow::setCanvasPosition, &editor, &SpriteEditor::currentCanvasPosition);
    connect(&editor, &SpriteEditor::updateCanvasSize, this, &MainWindow::onUpdateCanvasSize);

    auto canvasPostion = ui->MainEditorCanvas->pos();
    emit setCanvasPosition(canvasPostion.x(), canvasPostion.y());

}

void MainWindow::onDisplayCurrentFrame(QImage *frame)
{

    ui->MainEditorCanvas->setPixmap(QPixmap::fromImage(*frame));
}

void MainWindow::updateMaxFrames(int max)
{
    ui->frameAdjustor->setMaximum(max);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//delete
void MainWindow::deleteViewFrame(int value){
    ui->frameAdjustor->setMaximum(ui->frameAdjustor->maximum()-1);
    ui->frameAdjustor->setValue(value);
}


// mouse

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {

        emit mousePress((int)event->position().x(), (int)event->position().y(), true);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton))
        emit mouseMove((int)event->position().x(), (int)event->position().y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit mousePress((int)event->position().x(), (int)event->position().y(), false);
}

// void MainWindow::paintEvent(QPaintEvent *event)
// {
//     // QPainter painter(this);
//     // QRect dirtyRect = event->rect();
//     // painter.drawImage(dirtyRect, image, dirtyRect);
// }

// void MainWindow::drawLineTo(const QPoint &endPoint)
// {
//     // QPainter painter(&image);
//     // painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
//     //                     Qt::RoundJoin));
//     // painter.drawLine(lastPoint, endPoint);
//     // modified = true;

//     // int rad = (myPenWidth / 2) + 2;
//     // update(QRect(lastPoint, endPoint).normalized()
//     //            .adjusted(-rad, -rad, +rad, +rad));
//     // lastPoint = endPoint;
// }

void MainWindow::onUpdateCanvasSize(int x, int y) {
    auto canvasPostion = ui->MainEditorCanvas->pos();
    ui->MainEditorCanvas->setGeometry(canvasPostion.x(), canvasPostion.y(), x, y);
}

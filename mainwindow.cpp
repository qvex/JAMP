#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMoveEvent>
#include <QCloseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTime>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    play_flag = 0;
    menu_visible_flag = 0;
    ui->setupUi(this);

    ui->tableWidget->hide();

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //connect(ui->pushButton_7,SIGNAL(clicked()),this,SLOT(close()));
    //connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(showMinimized()));

    ui->horizontalSlider_2->setRange(0,100);

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    player->setPlaylist(playlist);

    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(playTime_slot(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(currentTime_slot(qint64)));

    connect(player,SIGNAL(volumeChanged(int)),this,SLOT(volumeChanged_slot(int)));
    player->setVolume(50);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    ui->pushButton_4->setText("Play Again");

    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(updateSongList(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ReleaseCapture())
      SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}

void MainWindow::playTime_slot(qint64 finishTime)
{
    QTime playFinishTime(0,finishTime/60000,finishTime/1000%60);
    ui->label_2->setText(playFinishTime.toString("mm:ss"));
    ui->horizontalSlider->setMaximum(finishTime);
}

void MainWindow::currentTime_slot(qint64 position)
{
    QTime playCurrentTime(0, position/60000,position/1000%60);
    ui->label->setText(playCurrentTime.toString("mm:ss"));
    ui->horizontalSlider->setValue(position);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    QTime time(0, position/60000,position/1000%60);
    ui->label->setText(time.toString("mm:ss"));
    ui->horizontalSlider->setValue(position);
}

void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    player->setVolume(position);
}

void MainWindow::on_volumeChanged_slot(int volume)
{
    player->setVolume(volume);
    ui->horizontalSlider_2->setValue(volume);
}

void MainWindow::on_pushButton_clicked()
{
    if(!playlist->isEmpty())
    {
        ui->pushButton->hide();
        play_flag = 1;
        player->play();
    }
}

void MainWindow::on_pushButton_9_clicked()
{
        ui->pushButton->show();
        play_flag = 0;
        player->pause();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!playlist->isEmpty())
        ui->pushButton->hide();
    int currentIndex = playlist->currentIndex();
    if(--currentIndex<0)
        currentIndex = 0;
    playlist->setCurrentIndex(currentIndex);
    player->play();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!playlist->isEmpty())
        ui->pushButton->hide();
    int currentIndex = playlist->currentIndex();
    if(++currentIndex == playlist->mediaCount())
        currentIndex = 0;
    player->play();
}

void MainWindow::on_pushButton_6_clicked()
{
    QStringList fileName = QFileDialog::getOpenFileNames(this,"Open the music you want to play",QDir::homePath(),"name(*.mp3)");
    if(fileName.count()>0)
    {
        foreach(QString fileItem, fileName)
        {
            if(!fileItem.isEmpty())
            {
                playlist->addMedia(QUrl::fromLocalFile(fileItem));
                QString tablet_file_name = fileItem.split("/").last();
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(tablet_file_name.split(".").front()));
            }
        }
        playlist->setCurrentIndex(0);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QMenu menu;
    menu.addAction("Single Player", this, SLOT(currentItemOnce_slot()));
    menu.addAction("Single Cycle", this, SLOT(currentItemOnce_slot()));
    menu.addAction("Loop", this, SLOT(Loop_slot()));
    menu.addAction("Shuffle Playback", this, SLOT(Random_slot()));
    menu.exec(QCursor::pos());
}

void MainWindow::currentItemOnce_slot()
{
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    ui->pushButton_4->setText("Single \nBroadcast");
}

void MainWindow::currentItemInLoop_slot()
{
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ui->pushButton_4->setText("Single \nCycle");
}

void MainWindow::Loop_slot()
{
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    ui->pushButton_4->setText("Cycle \nPlay");
}

void MainWindow::Random_slot()
{
    playlist->setPlaybackMode(QMediaPlaylist::Random);
    ui->pushButton_4->setText("Random \nPlay");
}

//Control playback menu disappears and reappears
void MainWindow::on_pushButton_5_clicked()
{
    if(menu_visible_flag == 0)
    {
        ui->tableWidget->show();
        menu_visible_flag = 1;
    }
    else
    {
        ui->tableWidget->hide();
        menu_visible_flag = 0;
    }
}

void MainWindow::updateSongList(int i)
{
    ui->tableWidget->selectRow(i);
    ui->label_3->setText(tr("Now Playing: %1").arg(ui->tableWidget->item(i,0)->text()));
}



void MainWindow::on_pushButton_7_clicked()
{
    connect(ui->pushButton_7,SIGNAL(clicked()),this,SLOT(close()));
}


void MainWindow::on_pushButton_8_clicked()
{
    connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(showMinimized()));
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define NOMINMAX
#include <QMainWindow>
#include <Windows.h>

class QMediaPlayer;
class QMediaPlaylist;
class QMouseEvent;
class QSystemTrayIcon;
class QAction;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    int play_flag;
    int menu_visible_flag;

private slots:

    void playTime_slot(qint64 finishTime);

    void currentTime_slot(qint64 position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_volumeChanged_slot(int volume);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();
    void on_pushButton_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();

    void currentItemOnce_slot();

    void currentItemInLoop_slot();

    void Loop_slot();

    void Random_slot();

    void updateSongList(int i);

    void on_pushButton_5_clicked();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "ObelixUdpSimThread.h"



typedef struct _T_SimTrack
{
  double Bearing;
  double Distance;
  double Course;
  double GroundSpeed;
}T_SimTrack;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
    void OnTimerTick();
        void OnSimTimerTick();
  void on_pbxRunSimulator_clicked();

  void on_pbxRunPlot_clicked();

  void on_sbxAnntenaSped_valueChanged(int arg1);

  void on_sbxBeamWidth_valueChanged(double arg1);

  void on_cbxVideoMode_currentIndexChanged(int index);

  void on_sbxIntensty_valueChanged(double arg1);

  void on_pbxClearScope_clicked();

  void on_ckxDispAntenna_stateChanged(int arg1);

  void on_ckxDispRangeLimit_stateChanged(int arg1);

  void on_ckxDispRangeRings_stateChanged(int arg1);

  void on_ckxDispCompas_stateChanged(int arg1);

  void on_ckxDispVideo_stateChanged(int arg1);

  void on_ckxDispTracks_stateChanged(int arg1);

  void on_sbxPresistance_valueChanged(double arg1);

  void on_sbxDisplayPxKtsRatio_valueChanged(double arg1);

  void on_sbxScopeDisplayRange_valueChanged(double arg1);

  void on_sbxSimNoise_valueChanged(double arg1);

  void on_sbxRange_valueChanged(int arg1);


  void on_pbxStopSimulator_clicked();

  void on_pbxDispSource_clicked(bool checked);

  void on_pbxDispSim_clicked(bool checked);

  void on_pbxDispScope_clicked(bool checked);



  void on_pbxBuildSimGenerated_clicked();

  void on_pbxClearSimGenerated_clicked();

  void on_pbxRunSim_clicked();

  void on_pbxStopSim_clicked();

private:
  void BuildSimTrackTable();
  void PushSimTrackTable();
private:
  Ui::MainWindow *ui;

  ObelixUdpSimThread* mObelixSimThread;
   QTimer* mMainTimer;
   QTimer* mSimTimer;

   T_ObelixTrack* mlTrackTable;
   T_SimTrack* mSimTable;
   int mSimTableSize;
};

#endif // MAINWINDOW_H
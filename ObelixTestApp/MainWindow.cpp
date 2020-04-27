#include "MainWindow.h"
#include "ui_MainWindow.h"

#define KNOT_TO_MS 0.514444
#define NM_TO_M 1852
#define DEG_TO_RAD 0.0174533

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  qDebug("MainWindows is executed in thread : %X", QThread::currentThreadId());

  ui->setupUi(this);
  mObelixSimThread = new ObelixUdpSimThread();

  // SIM
  ui->sbxAnntenaSped->setValue(mObelixSimThread->AntennaRpm());
  ui->sbxSimTimer->setValue(mObelixSimThread->TimerPeridod());
  ui->sbxBeamWidth->setValue(mObelixSimThread->BeamWidth());
  ui->sbxRange->setValue(mObelixSimThread->Range());
  ui->sbxIntensty->setValue(mObelixSimThread->VideoIntensity());
  ui->sbxSimNoise->setValue(mObelixSimThread->VideoNoise());

  // DISPLAY
  ui->sbxScopeDisplayRange->setValue(ui->olxPlot->RangeNm());
  ui->sbxDisplayPxKtsRatio->setValue(ui->olxPlot->DisplayPxKtsRatio());
  ui->ckxDispVideo->setChecked(ui->olxPlot->DisplayVideo());
  ui->ckxDispTracks->setChecked(ui->olxPlot->DisplayTracks());
  ui->ckxDispCompas->setChecked(ui->olxPlot->DisplayCompas());
  ui->ckxDispAntenna->setChecked(ui->olxPlot->DisplayAntenna());
  ui->ckxDispRangeLimit->setChecked(ui->olxPlot->DisplayRangeLimit());
  ui->ckxDispRangeRings->setChecked(ui->olxPlot->DisplayRangeRings());

  mMainTimer = new QTimer(this);
  connect(mMainTimer,&QTimer::timeout,this,&MainWindow::OnTimerTick);
  mMainTimer->start(100);

  mSimTimer = new QTimer(this);
  connect(mSimTimer,&QTimer::timeout,this,&MainWindow::OnSimTimerTick);


  BuildSimTrackTable();



}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pbxRunSimulator_clicked()
{
  // Lock contols
  ui->sbxNbLevel->setEnabled(false);
  ui->sbxNbCells->setEnabled(false);
  ui->pbxRunSimulator->setEnabled(false);
  ui->pbxStopSimulator->setEnabled(true);

  // Set parameters
  mObelixSimThread->SetVideoBeamParameters(ui->sbxNbLevel->value(), ui->sbxNbCells->value());
  mObelixSimThread->SetUdpReaderParameters(ui->tbxIpVideo->text(), ui->sbxPortVideo->value(),
                                           ui->tbxIpTrack->text(), ui->sbxPortTrack->value());

  // Run
  mObelixSimThread->start();
}


void MainWindow::on_pbxStopSimulator_clicked()
{
  // Stop
  mObelixSimThread->AskForStop();

  // unlock contols
  ui->sbxNbLevel->setEnabled(true);
  ui->sbxNbCells->setEnabled(true);
  ui->pbxRunSimulator->setEnabled(true);
  ui->pbxStopSimulator->setEnabled(false);
}




void MainWindow::on_pbxRunPlot_clicked()
{
  //
  ui->olxPlot->SetUdpReaderVideoParameters(ui->tbxIpVideo->text(), ui->sbxPortVideo->value());
  ui->olxPlot->SetUdpReaderTrackParameters(ui->tbxIpTrack->text(), ui->sbxPortTrack->value());

  // Run
  ui->olxPlot->Start(ui->sbxPlotTimer->value());
}


void MainWindow::OnTimerTick()
{
  ui->pgxFifoLoad->setValue(ui->olxPlot->FifoObelixVideoLoad());
  ui->pgxFifoTrackLoad->setValue(ui->olxPlot->FifoObelixTrackLoad());
  ui->tbxAverageTime->setText(QString("%1").arg(ui->olxPlot->ElapsedAverageTimeMs()));









}

void MainWindow::OnSimTimerTick()
{
  PushSimTrackTable();
}

void MainWindow::on_sbxAnntenaSped_valueChanged(int arg1)
{
  mObelixSimThread->SetAntennaRpm(arg1);
}

void MainWindow::on_sbxBeamWidth_valueChanged(double arg1)
{
  mObelixSimThread->SetBeamWidth(arg1);
}

void MainWindow::on_cbxVideoMode_currentIndexChanged(int index)
{
  mObelixSimThread->SetVideoMode(index);
}

void MainWindow::on_sbxIntensty_valueChanged(double arg1)
{
  mObelixSimThread->SetVideoIntensity(arg1);
}

void MainWindow::on_pbxClearScope_clicked()
{
  ui->olxPlot->ClearScope();
}

void MainWindow::on_ckxDispAntenna_stateChanged(int arg1)
{
  Q_UNUSED(arg1);
  ui->olxPlot->SetDisplayAntenna(ui->ckxDispAntenna->isChecked());
  ui->olxPlot->RefreshScope();
}

void MainWindow::on_ckxDispRangeLimit_stateChanged(int arg1)
{
  Q_UNUSED(arg1);
  ui->olxPlot->SetDisplayRangeLimit(ui->ckxDispRangeLimit->isChecked());
  ui->olxPlot->RefreshScope();
}

void MainWindow::on_ckxDispRangeRings_stateChanged(int arg1)
{
  Q_UNUSED(arg1);
  ui->olxPlot->SetDisplayRangeRings(ui->ckxDispRangeRings->isChecked());
  ui->olxPlot->RefreshScope();
}

void MainWindow::on_ckxDispCompas_stateChanged(int arg1)
{
  Q_UNUSED(arg1);
  ui->olxPlot->SetDisplayCompas(ui->ckxDispCompas->isChecked());
  ui->olxPlot->RefreshScope();
}

void MainWindow::on_ckxDispVideo_stateChanged(int arg1)
{
  Q_UNUSED(arg1);
  ui->olxPlot->SetDisplayVideo(ui->ckxDispVideo->isChecked());
  ui->olxPlot->RefreshScope();
}

void MainWindow::on_ckxDispTracks_stateChanged(int arg1)
{
  Q_UNUSED(arg1);
  ui->olxPlot->SetDisplayTracks(ui->ckxDispTracks->isChecked());
  ui->olxPlot->RefreshScope();
}

void MainWindow::on_sbxPresistance_valueChanged(double arg1)
{
  ui->olxPlot->SetPresistenceRatio(arg1);
}

void MainWindow::on_sbxDisplayPxKtsRatio_valueChanged(double arg1)
{
  ui->olxPlot->SetDisplayPxKtsRatio(arg1);
}

void MainWindow::on_sbxScopeDisplayRange_valueChanged(double arg1)
{
  ui->olxPlot->SetRangeNm(arg1);
}

void MainWindow::on_sbxSimNoise_valueChanged(double arg1)
{
  mObelixSimThread->SetVideoNoise(arg1);
}

void MainWindow::on_sbxRange_valueChanged(int arg1)
{
  mObelixSimThread->SetRange(arg1);
}






void MainWindow::on_pbxDispSource_clicked(bool checked)
{
  ui->gbxUdp->setVisible(checked);
  ui->gbxSource->setVisible(checked);
}

void MainWindow::on_pbxDispSim_clicked(bool checked)
{
  ui->gbxSimulator->setVisible(checked);
}

void MainWindow::on_pbxDispScope_clicked(bool checked)
{
  ui->gbxScope->setVisible(checked);
}

void MainWindow::BuildSimTrackTable()
{
  if (mSimTable)
  {
    free (mSimTable);
  }

  //
  mSimTableSize = ui->sbxNbSimTracks->value();
  mSimTable  = (T_SimTrack*)calloc(mSimTableSize, sizeof(T_SimTrack));

  //
  mlTrackTable = (T_ObelixTrack*)calloc(mSimTableSize, sizeof(T_ObelixTrack));

  //
  for (int i=0; i<mSimTableSize;  i++)
  {
    //
    if (ui->ckxRandomSimAz->isChecked() == true)
    {
      mSimTable[i].Bearing = rand()%static_cast<int>(ui->sbxSimAz->value());
    }
    else
    {
      mSimTable[i].Bearing = ui->sbxSimAz->value();
    }

    //
    if (ui->ckxRandomSimDist->isChecked() == true)
    {
      mSimTable[i].Distance = rand()%static_cast<int>(ui->sbxSimDist->value());
    }
    else
    {
      mSimTable[i].Distance = ui->sbxSimDist->value();
    }

    //
    if (ui->ckxRandomSimCourse->isChecked() == true)
    {
      mSimTable[i].Course = rand()%static_cast<int>(ui->sbxSimCourse->value());
    }
    else
    {
      mSimTable[i].Course = ui->sbxSimCourse->value();
    }

    //
    if (ui->ckxRandomSimSpeed->isChecked() == true)
    {
      mSimTable[i].GroundSpeed = rand()%static_cast<int>(ui->sbxSimSpeed->value());
    }
    else
    {
      mSimTable[i].GroundSpeed = ui->sbxSimSpeed->value();
    }
  }
}

void MainWindow::PushSimTrackTable()
{
  // Compute trajectory
  for (int i=0; i<mSimTableSize;  i++)
  {
    double lTrackBearing  = static_cast<double>(mSimTable[i].Bearing);
    double lTrackDistance = static_cast<double>(mSimTable[i].Distance);
    double lTrackCourse   = static_cast<double>(mSimTable[i].Course);
    double lTrackSpeed    = static_cast<double>(mSimTable[i].GroundSpeed);


    double lDeltaT = ui->sbxSimPeriod->value()*0.001*ui->sbxSimSpeedFactor->value();

    double lTrackX = lTrackDistance * NM_TO_M * sin(lTrackBearing*DEG_TO_RAD);
    double lTrackY = lTrackDistance * NM_TO_M * cos(lTrackBearing*DEG_TO_RAD);

    double lSpeedX = lTrackSpeed * KNOT_TO_MS * sin(lTrackCourse*DEG_TO_RAD);
    double lSpeedY = lTrackSpeed * KNOT_TO_MS * cos(lTrackCourse*DEG_TO_RAD);


    lTrackX = (lTrackX + lDeltaT * lSpeedX)/NM_TO_M;
    lTrackY = (lTrackY + lDeltaT * lSpeedY)/NM_TO_M;

    lTrackBearing = atan2(lTrackX,lTrackY)/DEG_TO_RAD;
    lTrackDistance = sqrt(lTrackX*lTrackX + lTrackY*lTrackY);



    if (lTrackDistance >= 100)
    {
      mSimTable[i].Bearing = rand()%static_cast<int>(ui->sbxSimAz->value());
      mSimTable[i].Distance   = rand()%static_cast<int>(ui->sbxSimDist->value());
      mSimTable[i].Course  = rand()%static_cast<int>(ui->sbxSimCourse->value());
      mSimTable[i].GroundSpeed   = rand()%static_cast<int>(ui->sbxSimSpeed->value());
    }
    else
    {
      mSimTable[i].Bearing = lTrackBearing;
      mSimTable[i].Distance   = lTrackDistance;
    }
  }

  // Push to sim thread
  for (int i=0; i<mSimTableSize;  i++)
  {
    mlTrackTable[i].Id = i;
    mlTrackTable[i].Bearing = static_cast<quint16>(mSimTable[i].Bearing/OBX_TRK_BEARINGCOURSE_LSB);
    mlTrackTable[i].Distance   = static_cast<quint16>(mSimTable[i].Distance/OBX_TRK_DISTANCE_LSB);
    mlTrackTable[i].Course  = static_cast<quint16>(mSimTable[i].Course/OBX_TRK_BEARINGCOURSE_LSB);
    mlTrackTable[i].GroundSpeed   = static_cast<quint16>(mSimTable[i].GroundSpeed/OBX_TRK_SPEED_LSB);
  }

  mObelixSimThread->PushTracks(mlTrackTable, mSimTableSize);
}

void MainWindow::on_pbxBuildSimGenerated_clicked()
{
  BuildSimTrackTable();
}

void MainWindow::on_pbxClearSimGenerated_clicked()
{

}

void MainWindow::on_pbxRunSim_clicked()
{
    mSimTimer->start(ui->sbxSimPeriod->value());
}

void MainWindow::on_pbxStopSim_clicked()
{
    mSimTimer->stop();
}

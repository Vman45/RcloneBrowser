#pragma once

#include "hours_spinbox.h"
#include "minutes_spinbox.h"
#include "pch.h"
#include "ui_scheduler_widget.h"

class SchedulerWidget : public QWidget {
  Q_OBJECT

public:
  /*
    StreamWidget(QProcess *rclone, QProcess *player, const QString &remote,
                 const QString &stream, const QStringList &args,
                 QWidget *parent = nullptr);
    ~StreamWidget();
  */

  SchedulerWidget(const QString &taskId, const QString &taskName,
                  const QStringList &args, QWidget *parent = nullptr);
  ~SchedulerWidget();

  bool isRunning = true;
  // return all scheduler parameters so we can store it
  QStringList getSchedulerParameters();
  QString getSchedulerTaskId();
  void updateTaskName(const QString newTaskName);

public slots:
  //  void cancel();

signals:
  //  void finished();
  void closed();
  void save();

private:
  Ui::SchedulerWidget ui;
  QStringList mArgs;

  void applySettingsToScreen();
  void applyArgsToScheduler(QStringList args);

  QString enhanceCron(QString cron);

  // list of scheduler parameters to be persistent in file
  QString mSchedulerStatus = "paused";

  QString mTaskId = "";
  QString mTaskName = "";      // b64
  QString mSchedulerName = ""; // b64
  QString mSchedulerId = QUuid::createUuid().toString();

  QString mLastRun = "never"; // b64
  QString mRequestId = "";
  QString mLastRunFinished = ""; // b64
  QString mLastRunStatus = "";   // b64

  bool mDailyState = true;
  bool mDailyMon = true;
  bool mDailyTue = true;
  bool mDailyWed = true;
  bool mDailyThu = true;
  bool mDailyFri = true;
  bool mDailySat = true;
  bool mDailySun = true;
  QString mDailyHour = "00";
  QString mDailyMinute = "00";

  bool mCronState = false;
  QString mCron = "30 6,18 * * MON-FRI"; // b64

  QString mExecutionMode = "0"; // 1,2,3
};

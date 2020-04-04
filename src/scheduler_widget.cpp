#include "scheduler_widget.h"
#include "qcron.h"
#include "utils.h"

SchedulerWidget::SchedulerWidget(const QString &taskId, const QString &taskName,
                                 const QStringList &args, QWidget *parent)
    : QWidget(parent) {

  mTaskId = taskId;
  mTaskName = taskName;

  ui.setupUi(this);
  auto settings = GetSettings();

  QString newInfo = "Scheduled task: " + taskName;

  if (newInfo.length() > 140) {
    mSchedulerName = newInfo.left(57) + "..." + newInfo.right(80);
  } else {
    mSchedulerName = newInfo;
  }

  if (args.at(0) == "NewScheduler") {
    // new scheduler - apply defaults
    applySettingsToScreen();
    //    ui.info->setEnabled(false);
    //    ui.nextRun->setEnabled(false);


  if (mIconsColour == "white") {
    ui.showDetails->setStyleSheet(
        "QToolButton { border: 0; font-weight: bold;}");
  } else {
    ui.showDetails->setStyleSheet(
        "QToolButton { border: 0; color: black; font-weight: bold;}");
  }

  ui.showDetails->setText("  Paused");



  } else {

applyArgsToScheduler(args);

if (mGlobalStop) {
    mNextRun = "";
} else {

applySettingsToScreen();

if (mSchedulerStatus == "activated") {
    mNextRun = nextRun().toString("ddd, dd-MMM-yyyy HH:mm t");
} else {

mNextRun = "";
}

}

    applySettingsToScreen();
  }

  ui.saveSchedule->setEnabled(false);
  ui.saveStatus->hide();
  ui.cancelScheduleEdit->setEnabled(false);

  /*
    QString remoteTrimmed;

    auto settings = GetSettings();
    mArgs.append(QDir::toNativeSeparators(GetRclone()));
    mArgs.append(args);
    mArgs.append(" | ");
    mArgs.append(stream);

    ui.showOutput->setToolTip(mArgs.join(" "));

    if (remote.length() > 140) {
      remoteTrimmed = remote.left(57) + "..." + remote.right(80);
    } else {
      remoteTrimmed = remote;
    }

    ui.info->setText(remoteTrimmed);
    ui.info->setCursorPosition(0);

    ui.stream->setText(stream);
    ui.stream->setCursorPosition(0);
    ui.stream->setToolTip(stream);

    ui.remote->setText(remote);
    ui.remote->setCursorPosition(0);
    ui.remote->setToolTip(remote);

    ui.details->setVisible(false);

    ui.output->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui.output->setVisible(false);
  */

  QString mIconsColour = settings->value("Settings/iconsColour").toString();

  QString img_add = "";

  if (mIconsColour == "white") {
    img_add = "_inv";
  }

  ui.showDetails->setIcon(
      QIcon(":media/images/qbutton_icons/vrightarrow" + img_add + ".png"));
  ui.showDetails->setIconSize(QSize(24, 24));

  ui.details->setVisible(false);

  /*
    ui.showOutput->setIcon(
        QIcon(":media/images/qbutton_icons/vrightarrow" + img_add + ".png"));
    ui.showOutput->setIconSize(QSize(24, 24));
  */

  /*
    ui.cancel->setToolTip("Stop streaming");
    ui.cancel->setStatusTip("Stop streaming");
  */

  ui.runTask->setIcon(
      QIcon(":media/images/qbutton_icons/run" + img_add + ".png"));
  ui.runTask->setIconSize(QSize(24, 24));

  ui.stopTask->setIcon(
      QIcon(":media/images/qbutton_icons/stop" + img_add + ".png"));
  ui.stopTask->setIconSize(QSize(24, 24));

  ui.editTask->setIcon(
      QIcon(":media/images/qbutton_icons/edit" + img_add + ".png"));
  ui.editTask->setIconSize(QSize(24, 24));

  ui.cancel->setIcon(
      QIcon(":media/images/qbutton_icons/cancel" + img_add + ".png"));
  ui.cancel->setIconSize(QSize(24, 24));

  ui.pause->setIcon(
      QIcon(":media/images/qbutton_icons/pause" + img_add + ".png"));
  ui.pause->setIconSize(QSize(24, 24));

  ui.start->setIcon(
      QIcon(":media/images/qbutton_icons/run" + img_add + ".png"));
  ui.start->setIconSize(QSize(24, 24));

  ui.saveStatus->hide();

  QObject::connect(ui.start, &QPushButton::clicked, this, [=]() {
    mSchedulerStatus = "activated";
    mNextRun = nextRun().toString("ddd, dd-MMM-yyyy HH:mm t");
    applySettingsToScreen();
    emit save();
  });

  QObject::connect(ui.pause, &QPushButton::clicked, this, [=]() {
    mSchedulerStatus = "paused";
    mNextRun = "";
    applySettingsToScreen();
    emit save();
  });

  QObject::connect(ui.validate, &QPushButton::clicked, this, [=]() {
    QString cronExp = enhanceCron(ui.cron->text().trimmed()) + " *";

    QCron cron(cronExp);

    if (!cron.isValid()) {

      QMessageBox::warning(this, "Cron validation",
                           QString("Your cron definition:\n\n" +
                                   ui.cron->text().trimmed() +
                                   "\n\nis not valid."),
                           QMessageBox::Ok);
    } else {
      QDateTime nowDateTime = QDateTime::currentDateTime();

      QMessageBox msgBox;

      msgBox.setText("Cron validation");
      msgBox.setInformativeText(QString(
          "Your cron definition is valid. Next five schedules:\n\nnext: " +
          cron.next(nowDateTime).toString("ddd, dd-MMM-yyyy HH:mm t") +
          +"\nthen: " +
          cron.next(cron.next(nowDateTime))
              .toString("ddd, dd-MMM-yyyy HH:mm t") +
          "\nthen: " +
          cron.next(cron.next(cron.next(nowDateTime)))
              .toString("ddd, dd-MMM-yyyy HH:mm t") +
          "\nthen: " +
          cron.next(cron.next(cron.next(cron.next(nowDateTime))))
              .toString("ddd, dd-MMM-yyyy HH:mm t") +
          "\nthen: " +
          cron.next(cron.next(cron.next(cron.next(cron.next(nowDateTime)))))
              .toString("ddd, dd-MMM-yyyy HH:mm t")));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
      // msgBox.setStyleSheet("QLabel{min-width: 200px;}");
      // msgBox.setBaseSize(QSize(600, 120));
      msgBox.exec();
    }
  });

  //  QObject::connect(ui.cancel, &QToolButton::clicked, this, [=]() {});

  QObject::connect(
      ui.showDetails, &QToolButton::toggled, this, [=](bool checked) {
        ui.details->setVisible(checked);
        if (checked) {
          ui.showDetails->setIcon(QIcon(
              ":media/images/qbutton_icons/vdownarrow" + img_add + ".png"));
          ui.showDetails->setIconSize(QSize(24, 24));

          if (mDailyState) {
            ui.tabWidget->setCurrentIndex(0);
          }

          if (mCronState) {
            ui.tabWidget->setCurrentIndex(1);
          }
        } else {
          ui.showDetails->setIcon(QIcon(
              ":media/images/qbutton_icons/vrightarrow" + img_add + ".png"));
          ui.showDetails->setIconSize(QSize(24, 24));
        }
      });

  QObject::connect(ui.cancel, &QToolButton::clicked, this, [=]() {
    int button = QMessageBox::question(
        this, "Stop",
        QString("Do you want to delete\n\n %1 \n\nscheduler?")
            .arg(mSchedulerName),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (button == QMessageBox::Yes) {
      emit closed();
    }
  });

  QObject::connect(ui.dailyState, &QToolButton::clicked, this, [=]() {
    if (ui.dailyState->isChecked()) {
      ui.cronState->setChecked(false);
      ui.saveSchedule->setEnabled(true);
      ui.saveStatus->show();
      ui.cancelScheduleEdit->setEnabled(true);
    } else {
      ui.dailyState->setChecked(true);
    }
  });

  QObject::connect(ui.cronState, &QToolButton::clicked, this, [=]() {
    if (ui.cronState->isChecked()) {
      ui.dailyState->setChecked(false);
      ui.saveSchedule->setEnabled(true);
      ui.saveStatus->show();
      ui.cancelScheduleEdit->setEnabled(true);
    } else {
      ui.cronState->setChecked(true);
    }
  });

  QObject::connect(ui.everyday, &QToolButton::clicked, this, [=]() {
    if (ui.everyday->isChecked()) {

      ui.cb_mon->setChecked(true);
      ui.cb_tue->setChecked(true);
      ui.cb_wed->setChecked(true);
      ui.cb_thu->setChecked(true);
      ui.cb_fri->setChecked(true);
      ui.cb_sat->setChecked(true);
      ui.cb_sun->setChecked(true);
      ui.saveSchedule->setEnabled(true);
      ui.saveStatus->show();
      ui.cancelScheduleEdit->setEnabled(true);
    } else {

      ui.everyday->setChecked(true);
    }
  });

  QObject::connect(ui.cb_mon, &QToolButton::clicked, this, [=]() {
    if (ui.cb_mon->isChecked()) {
      if (ui.cb_tue->isChecked() && ui.cb_wed->isChecked() &&
          ui.cb_thu->isChecked() && ui.cb_fri->isChecked() &&
          ui.cb_sat->isChecked() && ui.cb_sun->isChecked()) {
        ui.everyday->setChecked(true);
      }
    } else {
      ui.everyday->setChecked(false);
      if (!ui.cb_tue->isChecked() && !ui.cb_wed->isChecked() &&
          !ui.cb_thu->isChecked() && !ui.cb_fri->isChecked() &&
          !ui.cb_sat->isChecked() && !ui.cb_sun->isChecked()) {
        ui.cb_mon->setChecked(true);
      }
    }
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.cb_tue, &QToolButton::clicked, this, [=]() {
    if (ui.cb_tue->isChecked()) {
      if (ui.cb_mon->isChecked() && ui.cb_wed->isChecked() &&
          ui.cb_thu->isChecked() && ui.cb_fri->isChecked() &&
          ui.cb_sat->isChecked() && ui.cb_sun->isChecked()) {
        ui.everyday->setChecked(true);
      }
    } else {
      ui.everyday->setChecked(false);
      if (!ui.cb_mon->isChecked() && !ui.cb_wed->isChecked() &&
          !ui.cb_thu->isChecked() && !ui.cb_fri->isChecked() &&
          !ui.cb_sat->isChecked() && !ui.cb_sun->isChecked()) {
        ui.cb_tue->setChecked(true);
      }
    }
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.cb_wed, &QToolButton::clicked, this, [=]() {
    if (ui.cb_wed->isChecked()) {
      if (ui.cb_mon->isChecked() && ui.cb_tue->isChecked() &&
          ui.cb_thu->isChecked() && ui.cb_fri->isChecked() &&
          ui.cb_sat->isChecked() && ui.cb_sun->isChecked()) {
        ui.everyday->setChecked(true);
      }
    } else {
      ui.everyday->setChecked(false);
      if (!ui.cb_mon->isChecked() && !ui.cb_tue->isChecked() &&
          !ui.cb_thu->isChecked() && !ui.cb_fri->isChecked() &&
          !ui.cb_sat->isChecked() && !ui.cb_sun->isChecked()) {
        ui.cb_wed->setChecked(true);
      }
    }
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.cb_thu, &QToolButton::clicked, this, [=]() {
    if (ui.cb_thu->isChecked()) {
      if (ui.cb_mon->isChecked() && ui.cb_tue->isChecked() &&
          ui.cb_wed->isChecked() && ui.cb_fri->isChecked() &&
          ui.cb_sat->isChecked() && ui.cb_sun->isChecked()) {
        ui.everyday->setChecked(true);
      }
    } else {
      ui.everyday->setChecked(false);
      if (!ui.cb_mon->isChecked() && !ui.cb_tue->isChecked() &&
          !ui.cb_wed->isChecked() && !ui.cb_fri->isChecked() &&
          !ui.cb_sat->isChecked() && !ui.cb_sun->isChecked()) {
        ui.cb_thu->setChecked(true);
      }
    }
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.cb_fri, &QToolButton::clicked, this, [=]() {
    if (ui.cb_fri->isChecked()) {
      if (ui.cb_mon->isChecked() && ui.cb_tue->isChecked() &&
          ui.cb_wed->isChecked() && ui.cb_thu->isChecked() &&
          ui.cb_sat->isChecked() && ui.cb_sun->isChecked()) {
        ui.everyday->setChecked(true);
      }
    } else {
      ui.everyday->setChecked(false);
      if (!ui.cb_mon->isChecked() && !ui.cb_tue->isChecked() &&
          !ui.cb_wed->isChecked() && !ui.cb_thu->isChecked() &&
          !ui.cb_sat->isChecked() && !ui.cb_sun->isChecked()) {
        ui.cb_fri->setChecked(true);
      }
    }
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.cb_sat, &QToolButton::clicked, this, [=]() {
    if (ui.cb_sat->isChecked()) {
      if (ui.cb_mon->isChecked() && ui.cb_tue->isChecked() &&
          ui.cb_wed->isChecked() && ui.cb_thu->isChecked() &&
          ui.cb_fri->isChecked() && ui.cb_sun->isChecked()) {
        ui.everyday->setChecked(true);
      }
    } else {
      ui.everyday->setChecked(false);
      if (!ui.cb_mon->isChecked() && !ui.cb_tue->isChecked() &&
          !ui.cb_wed->isChecked() && !ui.cb_thu->isChecked() &&
          !ui.cb_fri->isChecked() && !ui.cb_sun->isChecked()) {
        ui.cb_sat->setChecked(true);
      }
    }
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.cb_sun, &QToolButton::clicked, this, [=]() {
    if (ui.cb_sun->isChecked()) {
      if (ui.cb_mon->isChecked() && ui.cb_tue->isChecked() &&
          ui.cb_wed->isChecked() && ui.cb_thu->isChecked() &&
          ui.cb_fri->isChecked() && ui.cb_sat->isChecked()) {
        ui.everyday->setChecked(true);
      }
    } else {
      ui.everyday->setChecked(false);
      if (!ui.cb_mon->isChecked() && !ui.cb_tue->isChecked() &&
          !ui.cb_wed->isChecked() && !ui.cb_thu->isChecked() &&
          !ui.cb_fri->isChecked() && !ui.cb_sat->isChecked()) {
        ui.cb_sun->setChecked(true);
      }
    }
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.cron, &QLineEdit::textChanged, this, [=]() {
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.schedulerName, &QLineEdit::textChanged, this, [=]() {
    ui.saveSchedule->setEnabled(true);
    ui.saveStatus->show();
    ui.cancelScheduleEdit->setEnabled(true);
  });

  QObject::connect(ui.hours, QOverload<int>::of(&HoursSpinBox::valueChanged),
                   [=]() {
                     ui.saveSchedule->setEnabled(true);
                     ui.saveStatus->show();
                     ui.cancelScheduleEdit->setEnabled(true);
                   });

  QObject::connect(ui.minutes,
                   QOverload<int>::of(&MinutesSpinBox::valueChanged), [=]() {
                     ui.saveSchedule->setEnabled(true);
                     ui.saveStatus->show();
                     ui.cancelScheduleEdit->setEnabled(true);
                   });

  QObject::connect(ui.cb_executionMode, &QComboBox::currentTextChanged, this,
                   [=]() {
                     ui.saveSchedule->setEnabled(true);
                     ui.saveStatus->show();
                     ui.cancelScheduleEdit->setEnabled(true);
                   });

  QObject::connect(ui.cancelScheduleEdit, &QPushButton::clicked, this, [=]() {
    applySettingsToScreen();
    ui.saveSchedule->setEnabled(false);
    ui.saveStatus->hide();
    ui.cancelScheduleEdit->setEnabled(false);
  });

  QObject::connect(ui.saveSchedule, &QPushButton::clicked, this, [=]() {
    if (ui.schedulerName->text().isEmpty()) {
      QMessageBox::warning(this, "Warning",
                           "Please enter scheduler name to save.");
      ui.schedulerName->setFocus(Qt::FocusReason::OtherFocusReason);
      return;
    }

    if (ui.dailyState->isChecked()) {
    }

    if (ui.cronState->isChecked()) {

      QString cronExp = enhanceCron(ui.cron->text().trimmed()) + " *";

      QCron cron(cronExp);

      if (!cron.isValid()) {
        QMessageBox::warning(this, "Cron validation",
                             QString("Your cron definition:\n\n" +
                                     ui.cron->text().trimmed() +
                                     "\n\nis not valid."),
                             QMessageBox::Ok);
        ui.tabWidget->setCurrentIndex(1);
        ui.cron->setFocus(Qt::FocusReason::OtherFocusReason);
        return;
      }
    }

    // save task
    applyScreenToSettings();
    emit save();
    applySettingsToScreen();
    ui.saveSchedule->setEnabled(false);
    ui.saveStatus->hide();
    ui.cancelScheduleEdit->setEnabled(false);
    // start new schedule
  });

  QObject::connect(ui.editTask, &QToolButton::clicked, this,
                   [=]() { emit editTask(); });

  QObject::connect(ui.stopTask, &QToolButton::clicked, this,
                   [=]() { emit stopTask(); });

  QObject::connect(ui.runTask, &QToolButton::clicked, this, [=]() {
    //!!!

    //    QDateTime nowDateTime = QDateTime::currentDateTime();

    //    mLastRun = nowDateTime.toString("ddd, dd-MMM-yyyy HH:mm t");
    mRequestId = QUuid::createUuid().toString();
    //    emit save();
    mManualStart = true;
    emit runTask();
  });

}

SchedulerWidget::~SchedulerWidget() {}

/*
void SchedulerWidget::cancel() {
  if (!isRunning) {
    return;
  }
}
*/

void SchedulerWidget::applyScreenToSettings() {
  //!!!
  mSchedulerName = ui.schedulerName->text();

  mDailyState = ui.dailyState->isChecked();

  mDailyMon = ui.cb_mon->isChecked();
  mDailyTue = ui.cb_tue->isChecked();
  mDailyWed = ui.cb_wed->isChecked();
  mDailyThu = ui.cb_thu->isChecked();
  mDailyFri = ui.cb_fri->isChecked();
  mDailySat = ui.cb_sat->isChecked();
  mDailySun = ui.cb_sun->isChecked();

  mDailyHour = QString::number(ui.hours->value());
  mDailyMinute = QString::number(ui.minutes->value());

  mCronState = ui.cronState->isChecked();

  mCron = ui.cron->text();

  mExecutionMode = QString::number(ui.cb_executionMode->currentIndex());
}

void SchedulerWidget::applySettingsToScreen() {

qDebug() << "mGlobalStop: " << mGlobalStop;
qDebug() << "mSchedulerStatus: " << mSchedulerStatus;



ui.nextRun->setText(mNextRun);

qDebug() << "mNextRun: " << mNextRun;

  if (mGlobalStop) {

    if (mIconsColour == "white") {
      ui.showDetails->setStyleSheet(
          "QToolButton { border: 0; font-weight: bold;}");
    } else {
      ui.showDetails->setStyleSheet(
          "QToolButton { border: 0; color: black; font-weight: bold;}");
    }

    ui.showDetails->setText("  Stopped");

    if (mSchedulerStatus == "paused") {

      ui.start->setEnabled(true);
      ui.pause->setEnabled(false);
    }

    if (mSchedulerStatus == "activated") {

      ui.start->setEnabled(false);
      ui.pause->setEnabled(true);
    }

  } else {

    if (mSchedulerStatus == "paused") {

      if (mIconsColour == "white") {
        ui.showDetails->setStyleSheet(
            "QToolButton { border: 0; font-weight: bold;}");
      } else {
        ui.showDetails->setStyleSheet(
            "QToolButton { border: 0; color: black; font-weight: bold;}");
      }

      ui.showDetails->setText("  Paused");

      ui.start->setEnabled(true);
      ui.pause->setEnabled(false);

    }

    if (mSchedulerStatus == "activated") {

      ui.showDetails->setStyleSheet(
          "QToolButton { border: 0; color: darkgreen; font-weight: bold;}");

      ui.showDetails->setText("  Active");

      ui.start->setEnabled(false);
      ui.pause->setEnabled(true);
    }
  }

  //  QString mTaskId;
  //  QString mTaskName; //b64
  ui.taskName->setText(mTaskName);

  //  QString mSchedulerName; //b64
  ui.schedulerName->setText(mSchedulerName);
  ui.schedulerName->setCursorPosition(0);
  ui.info->setText(mSchedulerName);

  //  QString mSchedulerId = QUuid::createUuid().toString();

  //  QString mLastRun; //b64
  ui.lastRun->setText(mLastRun);

  //  QString mRequestId;
  //  QString mLastRunFinished; //b64
  ui.lastRunFinished->setText(mLastRunFinished);

  if (mLastRunStatus == "running" || mLastRunStatus == "in the queue") {
    ui.lastRunStatus->setStyleSheet(
        "QLabel { color: green; font-weight: bold;}");
    ui.runTask->setEnabled(false);
    ui.stopTask->setEnabled(true);
  }

  if (mLastRunStatus == "finished") {
    ui.lastRunStatus->setStyleSheet(
        "QLabel { color: green; font-weight: bold;}");
    ui.runTask->setEnabled(true);
    ui.stopTask->setEnabled(false);
  }

  if (mLastRunStatus == "removed from the queue" ||
      mLastRunStatus == "stopped" || mLastRunStatus == "error") {
    ui.lastRunStatus->setStyleSheet("QLabel { color: red; font-weight: bold;}");
    ui.runTask->setEnabled(true);
    ui.stopTask->setEnabled(false);
  }

  //  QString mLastRunStatus; //b64
  ui.lastRunStatus->setText(mLastRunStatus);

  //  bool mDailyState = true;
  //  bool mDailyMon = true;
  //  bool mDailyTue = true;
  //  bool mDailyWed = true;
  //  bool mDailyThu = true;
  //  bool mDailyFri = true;
  //  bool mDailySat = true;
  //  bool mDailySun = true;
  ui.dailyState->setChecked(mDailyState);
  ui.cb_mon->setChecked(mDailyMon);
  ui.cb_tue->setChecked(mDailyTue);
  ui.cb_wed->setChecked(mDailyWed);
  ui.cb_thu->setChecked(mDailyThu);
  ui.cb_fri->setChecked(mDailyFri);
  ui.cb_sat->setChecked(mDailySat);
  ui.cb_sun->setChecked(mDailySun);

  if (mDailyMon && mDailyTue && mDailyWed && mDailyThu && mDailyFri &&
      mDailySat && mDailySun) {
    ui.everyday->setChecked(true);
  } else {
    ui.everyday->setChecked(false);
  }

  // QString mDailyHour = "00";
  ui.hours->setValue(mDailyHour.toInt());
  // QString mDailyMinute = "00";
  ui.minutes->setValue(mDailyMinute.toInt());

  //  bool mCronState = false;
  ui.cronState->setChecked(mCronState);

  //  QString mCron = "30 6,18 */2 * MON-FRI";
  ui.cron->setText(mCron);

  //  QString executionMode = "1"; //1,2,3
  ui.cb_executionMode->setCurrentIndex(mExecutionMode.toInt());

  if (mGlobalStop) {
    ui.runTask->setEnabled(false);
    ui.stopTask->setEnabled(false);
  } else {

    if (mLastRunStatus == "running" || mLastRunStatus == "in the queue") {
      ui.runTask->setEnabled(false);
      ui.stopTask->setEnabled(true);
    } else {
      ui.runTask->setEnabled(true);
      ui.stopTask->setEnabled(false);
    }
  }
}

QStringList SchedulerWidget::getSchedulerParameters() {

  QStringList schedulerArgs;

  schedulerArgs << "mSchedulerId" << mSchedulerId;
  schedulerArgs << "mSchedulerName" << mSchedulerName.toUtf8().toBase64();
  schedulerArgs << "mTaskId" << mTaskId;
  schedulerArgs << "mTaskName" << mTaskName.toUtf8().toBase64();
  schedulerArgs << "mLastRun" << mLastRun.toUtf8().toBase64();
  schedulerArgs << "mRequestId" << mRequestId;
  schedulerArgs << "mLastRunFinished" << mLastRunFinished.toUtf8().toBase64();
  schedulerArgs << "mLastRunStatus" << mLastRunStatus.toUtf8().toBase64();
  schedulerArgs << "mSchedulerStatus" << mSchedulerStatus;

  schedulerArgs << "mDailyState" << QVariant(mDailyState).toString();
  schedulerArgs << "mDailyMon" << QVariant(mDailyMon).toString();
  schedulerArgs << "mDailyTue" << QVariant(mDailyTue).toString();
  schedulerArgs << "mDailyWed" << QVariant(mDailyWed).toString();
  schedulerArgs << "mDailyThu" << QVariant(mDailyThu).toString();
  schedulerArgs << "mDailyFri" << QVariant(mDailyFri).toString();
  schedulerArgs << "mDailySat" << QVariant(mDailySat).toString();
  schedulerArgs << "mDailySun" << QVariant(mDailySun).toString();
  schedulerArgs << "mDailyHour" << mDailyHour;
  schedulerArgs << "mDailyMinute" << mDailyMinute;

  schedulerArgs << "mCronState" << QVariant(mCronState).toString();
  schedulerArgs << "mCron" << mCron.toUtf8().toBase64();
  schedulerArgs << "mExecutionMode" << mExecutionMode;

  return schedulerArgs;
}

void SchedulerWidget::applyArgsToScheduler(QStringList args) {

  int argsLength = args.count();

  QString arg;
  QString argValue;

  for (int i = 0; i < argsLength; i = i + 2) {

    arg = args.at(i);
    argValue = args.at(i + 1);

    if (arg == "mSchedulerId") {
      mSchedulerId = argValue;
    }
    if (arg == "mSchedulerName") {
      mSchedulerName = (QString)QByteArray::fromBase64(argValue.toUtf8());
    }
    if (arg == "mTaskId") {
      mTaskId = argValue;
    }
    if (arg == "mTaskName") {
      mTaskName = (QString)QByteArray::fromBase64(argValue.toUtf8());
    }
    if (arg == "mLastRun") {
      mLastRun = (QString)QByteArray::fromBase64(argValue.toUtf8());
    }
    if (arg == "mRequestId") {
      mRequestId = argValue;
    }
    if (arg == "mLastRunFinished") {
      mLastRunFinished = (QString)QByteArray::fromBase64(argValue.toUtf8());
    }
    if (arg == "mLastRunStatus") {
      mLastRunStatus = (QString)QByteArray::fromBase64(argValue.toUtf8());
    }

    if (arg == "mSchedulerStatus") {
      mSchedulerStatus = argValue;
      qDebug() << "mSchedulerStatus: " << mSchedulerStatus;
    }

    if (arg == "mDailyState") {
      mDailyState = QVariant(argValue).toBool();
    }

    if (arg == "mDailyMon") {
      mDailyMon = QVariant(argValue).toBool();
    }
    if (arg == "mDailyTue") {
      mDailyTue = QVariant(argValue).toBool();
    }
    if (arg == "mDailyWed") {
      mDailyWed = QVariant(argValue).toBool();
    }
    if (arg == "mDailyThu") {
      mDailyThu = QVariant(argValue).toBool();
    }
    if (arg == "mDailyFri") {
      mDailyFri = QVariant(argValue).toBool();
    }
    if (arg == "mDailySat") {
      mDailySat = QVariant(argValue).toBool();
    }
    if (arg == "mDailySun") {
      mDailySun = QVariant(argValue).toBool();
    }

    if (arg == "mDailyHour") {
      mDailyHour = argValue;
    }

    if (arg == "mDailyMinute") {
      mDailyMinute = argValue;
    }

    if (arg == "mCronState") {
      mCronState = QVariant(argValue).toBool();
    }
    if (arg == "mCron") {
      mCron = (QString)QByteArray::fromBase64(argValue.toUtf8());
    }
    if (arg == "mExecutionMode") {
      mExecutionMode = argValue;
    }
  }
  return;
}

void SchedulerWidget::updateTaskName(const QString newTaskName) {

  mTaskName = newTaskName;
  ui.taskName->setText(mTaskName);
}

void SchedulerWidget::stopScheduler() {

  if (mIconsColour == "white") {
    ui.showDetails->setStyleSheet(
        "QToolButton { border: 0; font-weight: bold;}");
  } else {
    ui.showDetails->setStyleSheet(
        "QToolButton { border: 0; color: black; font-weight: bold;}");
  }

  ui.showDetails->setText("  Stopped");

  emit stopTask();

  ui.runTask->setEnabled(false);
  ui.stopTask->setEnabled(false);

  mGlobalStop = true;

  ui.nextRun->setText("");
}

void SchedulerWidget::startScheduler() {

  mGlobalStop = false;
  applySettingsToScreen();
  // restart !!!
}

QString SchedulerWidget::getSchedulerTaskId() { return mTaskId; }

QString SchedulerWidget::getSchedulerRequestId() { return mRequestId; }

int SchedulerWidget::getExecutionMode() { return mExecutionMode.toInt(); }

QString SchedulerWidget::enhanceCron(QString cron) {

  QString enhancedCron = cron.toUpper();

  enhancedCron.replace("MON", "1");
  enhancedCron.replace("TUE", "2");
  enhancedCron.replace("WED", "3");
  enhancedCron.replace("THU", "4");
  enhancedCron.replace("FRI", "5");
  enhancedCron.replace("SAT", "6");
  enhancedCron.replace("SUN", "7");

  enhancedCron.replace("JAN", "1");
  enhancedCron.replace("FEB", "2");
  enhancedCron.replace("MAR", "3");
  enhancedCron.replace("APR", "4");
  enhancedCron.replace("MAY", "5");
  enhancedCron.replace("JUN", "6");
  enhancedCron.replace("JUL", "7");
  enhancedCron.replace("AUG", "8");
  enhancedCron.replace("SEP", "9");
  enhancedCron.replace("OCT", "10");
  enhancedCron.replace("NOV", "11");
  enhancedCron.replace("DEC", "12");

  return enhancedCron;
}

void SchedulerWidget::updateTaskStatus(const QString requestID,
                                       const QString taskStatus) {

  if (requestID == mRequestId) {

    if (taskStatus == "already running") {
      if (mManualStart) {
        QMessageBox::warning(
            this, "Warning",
            QString("This task can't be started.\nIt is already running."),
            QMessageBox::Ok);
        mManualStart = false;
      }
    }

    if (taskStatus == "running") {
      QDateTime nowDateTime = QDateTime::currentDateTime();
      mLastRun = nowDateTime.toString("ddd, dd-MMM-yyyy HH:mm:ss t");
      mLastRunFinished = "";
      mLastRunStatus = "running";
      mTaskRunning = true;
    }

    if (taskStatus == "in the queue") {
      QDateTime nowDateTime = QDateTime::currentDateTime();
      mLastRun = nowDateTime.toString("ddd, dd-MMM-yyyy HH:mm:ss t");
      mLastRunFinished = "";
      mLastRunStatus = "in the queue";
      mTaskRunning = true;
    }

    if (taskStatus == "removed from the queue") {
      QDateTime nowDateTime = QDateTime::currentDateTime();
      mLastRunFinished = nowDateTime.toString("ddd, dd-MMM-yyyy HH:mm:ss t");
      mLastRunStatus = "removed from the queue";
      mTaskRunning = false;
    }

    if (taskStatus == "finished") {
      QDateTime nowDateTime = QDateTime::currentDateTime();
      mLastRunFinished = nowDateTime.toString("ddd, dd-MMM-yyyy HH:mm:ss t");
      mLastRunStatus = "finished";
      mTaskRunning = false;
    }

    if (taskStatus == "stopped") {
      QDateTime nowDateTime = QDateTime::currentDateTime();
      mLastRunFinished = nowDateTime.toString("ddd, dd-MMM-yyyy HH:mm:ss t");
      mLastRunStatus = "stopped";
      mTaskRunning = false;
    }

    if (taskStatus == "error") {
      QDateTime nowDateTime = QDateTime::currentDateTime();
      mLastRunFinished = nowDateTime.toString("ddd, dd-MMM-yyyy HH:mm:ss t");
      mLastRunStatus = "error";
      mTaskRunning = false;
    }

    applySettingsToScreen();
    emit save();
  }
}


QDateTime SchedulerWidget::nextRun() {


if (mCronState) {

QString cronExp = enhanceCron(ui.cron->text().trimmed()) + " *";
QDateTime nowDateTime = QDateTime::currentDateTime();
QCron cron(cronExp);

return (cron.next(nowDateTime));

}


return (QDateTime::fromString ("Fri, 07-Jan-2022 17:00:00 GMT Summer Time", "ddd, dd-MMM-yyyy HH:mm:ss t"));



}

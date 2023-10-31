#include "main_dlg.h"
#include <QDebug>
#include <QIntValidator>

#define M_PI (4.0f*std::atan(1.0f))

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint),
    work_params({"COM3", 0, 0, 0, 0, 0, 25, 0}),
    left_wheel(40 * 60),
    right_wheel(40 * 60){
  setupUi(this);
  work_thread = new WorkThread(this, mutex, queue, work_params);
  left_widget->setParams({0.0, 60, 12, -25, 25, 10, "Left Wheel"});
  right_widget->setParams({0.0, 60, 12, -25, 25, 10, "Right Wheel"});
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(start_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  connect(sp_button, SIGNAL(clicked()), this, SLOT(spButtonClicked()));
}

void MainDlg::buttonClicked() {
  work_params.port_name = edit_port->text();
  work_params.kp = edit_kp->text().toInt();
  work_params.ki = edit_ki->text().toInt();
  work_params.kd = edit_kd->text().toInt();
  work_params.left_sp = left_sp->value();
  work_params.right_sp = right_sp->value();
  work_params.sampling_time_ms = edit_st->text().toInt();
  if (start_button->text() == "Start") {
    const size_t buffer_size = 60000 / work_params.sampling_time_ms;
    left_widget->reserve(buffer_size, work_params.sampling_time_ms);
    right_widget->reserve(buffer_size, work_params.sampling_time_ms);
    left_wheel.reserve(buffer_size);
    right_wheel.reserve(buffer_size);
    queue.clear();
    timer.start(work_params.sampling_time_ms);
    work_thread->start(QThread::HighestPriority);
  } else {
    work_thread->stop();
    timer.stop();
  }
  controls->setDisabled(work_thread->isRunning());
  gains_group->setDisabled(work_thread->isRunning());
  start_button->setText(work_thread->isRunning() ? "Stop" : "Start");
}

void MainDlg::timeout() {
    {
        QMutexLocker lock(&mutex);
        while (!queue.empty()) {
            const auto data = queue.front();
            queue.pop_front();
            left_wheel.v.push_back(data.left_v);
            right_wheel.v.push_back(data.right_v);

            left_wheel.v_sp.push_back(data.left_sp);
            right_wheel.v_sp.push_back(data.right_sp);

            left_wheel.err.push_back(data.left_sp - data.left_v);
            right_wheel.err.push_back(data.right_sp - data.right_v);

            left_wheel.t.push_back(data.timestamp);
            right_wheel.t.push_back(data.timestamp);

            if (data.left_sp == work_params.left_sp && data.right_sp == work_params.right_sp) {
                work_params.sp_changed = false;
            }
        }
    }
    left_widget->setData(left_wheel);
    right_widget->setData(right_wheel);
    left_wheel.clear();
    right_wheel.clear();
    controls->setDisabled(work_thread->isRunning());
    gains_group->setDisabled(work_thread->isRunning());
    start_button->setText(work_thread->isRunning() ? "Stop" : "Start");
}

void MainDlg::spButtonClicked() {
    work_params.left_sp = left_sp->value();
    work_params.right_sp = right_sp->value();
    work_params.sp_changed = true;
}
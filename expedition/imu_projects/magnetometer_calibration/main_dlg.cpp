#include "main_dlg.h"
#include <iostream>
#include <fstream>

#include <Eigen/Eigenvalues>
#include <QDebug>
#include <QFileDialog>

Eigen::Matrix3d generateMatrixSqrt(const Eigen::Matrix3d& A) {
    const Eigen::EigenSolver<Eigen::Matrix<double, 3, 3>> solver(A, true);
    const Eigen::Matrix3d P = solver.eigenvectors().real();
    const Eigen::Matrix3d D = solver.eigenvalues().real().cwiseSqrt().asDiagonal();
    return P * D * P.transpose();
}

void ellipsoidFit(const std::vector<Eigen::Vector3f>& mag_data, Eigen::Matrix3d& M, Eigen::Vector3d& origin) {
    // Implementation based on this paper: https://www.researchgate.net/publication/4070857_Least_squares_ellipsoid_specific_fitting

    // Creates D matrix
    Eigen::MatrixXd D(mag_data.size(), 10);
    for (int row = 0; row < mag_data.size(); row++) {
        const auto& v = mag_data[row];
        D(row, 0) = v.x() * v.x();
        D(row, 1) = v.y() * v.y();
        D(row, 2) = v.z() * v.z();
        D(row, 3) = 2.0 * v.y() * v.z();
        D(row, 4) = 2.0 * v.x() * v.z();
        D(row, 5) = 2.0 * v.x() * v.y();
        D(row, 6) = 2.0 * v.x();
        D(row, 7) = 2.0 * v.y();
        D(row, 8) = 2.0 * v.z();
        D(row, 9) = 1.0;
    }
    const Eigen::Matrix<double, 10, 10> DTD = D.transpose() * D;

    // Creates C and S11...S22 matrix
    const double k = 4;
    Eigen::Matrix<double, 6, 6> C;
    C << -1, k / 2 - 1, k / 2 - 1, 0, 0, 0,
        k / 2 - 1, -1, k / 2 - 1, 0, 0, 0,
        k / 2 - 1, k / 2 - 1, -1, 0, 0, 0,
        0, 0, 0, -k, 0, 0,
        0, 0, 0, 0, -k, 0,
        0, 0, 0, 0, 0, -k;
    const auto S11 = DTD.block<6, 6>(0, 0);
    const auto S12 = DTD.block<6, 4>(0, 6);
    const auto S22 = DTD.block<4, 4>(6, 6);

    // Creates V1 & V2 vectors
    const Eigen::Matrix<double, 6, 6> E = C.inverse() * (S11 - S12 * S22.inverse() * S12.transpose());
    Eigen::EigenSolver<Eigen::Matrix<double, 6, 6>> solver(E);
    Eigen::Index i;
    solver.eigenvalues().real().maxCoeff(&i);
    Eigen::Matrix<double, 6, 1> V1 = solver.eigenvectors().real().col(i);
    if (V1[0] < 0.0) {
        V1 *= -1;
    }
    const Eigen::Matrix<double, 4, 1> V2 = -S22.inverse() * S12.transpose() * V1;

    // Creates A, M, and origin matrices/vectors
    Eigen::Matrix3d A;
    A << V1(0), V1(5), V1(4),
        V1(5), V1(1), V1(3),
        V1(4), V1(3), V1(2);
    origin = -A.inverse() * Eigen::Vector3d(V2(0), V2(1), V2(2));
    const double r = std::sqrt(origin.transpose() * A * origin - V2(3)); // Scales M to r to be
    M = generateMatrixSqrt(A) / r;
}

MainDlg::MainDlg()
  : QDialog(nullptr, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint),
    work_params({"COM3", 25}) {
  setupUi(this);
  mag_data.reserve(10000);
  work_thread = new WorkThread(this, mutex, queue, work_params);
  //edit_duration->setValidator(new QIntValidator(0, 60 * 60, this));

  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(start_btn, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  connect(spin_x, SIGNAL(valueChanged(int)), widget, SLOT(angleXChanged(int)));
  connect(spin_y, SIGNAL(valueChanged(int)), widget, SLOT(angleYChanged(int)));
  connect(spin_z, SIGNAL(valueChanged(int)), widget, SLOT(angleZChanged(int)));
  connect(save_calib_btn, SIGNAL(clicked()), this, SLOT(saveCalibration()));
  //connect(shift_center, SIGNAL(stateChanged(int)), widget, SLOT(shiftCenter(int)));
}

void MainDlg::buttonClicked() {
  work_params.port_name = port_edit->text();
  if (start_btn->text() == "Start") {
    widget->clearRingBuffers();
    timer.start(25);
    work_thread->start(QThread::HighestPriority);
  } else {
    work_thread->stop();
    timer.stop();
  }
  port_edit->setDisabled(work_thread->isRunning());
  start_btn->setText(work_thread->isRunning() ? "Stop" : "Start");
}

void MainDlg::timeout() {
    timeout_count++;
    size_t index = mag_data.size();
    {
        QMutexLocker lock(&mutex);
        while (!queue.empty()) {
            const auto data = queue.front();
            queue.pop_front();
            if (mag_data.size() < mag_data.capacity()) {
                mag_data.push_back(data.mag);
            }
            min_point = min_point.cwiseMin(data.mag);
            max_point = max_point.cwiseMax(data.mag);
        }
    }
    Eigen::Vector3f center{0.0f, 0.0f, 0.0f};
    if (shift_center->isChecked()) {
        center = (min_point + max_point) / 2.0f;
    }
    if (timeout_count % 40 == 0) {
        std::array<int, 8> octants{0, 0, 0, 0, 0, 0, 0, 0};
        for (int i = 0; i < mag_data.size(); i++) {
            const Eigen::Vector3f p = mag_data[i] - center;
            if (p.x() > 0) {
                //0, 1, 2, 3
                if (p.y() > 0) {
                    //0, 1
                    if (p.z() > 0) {
                        //0
                        octants[0]++;
                    }
                    else {
                        //1
                        octants[1]++;
                    }
                }
                else {
                    //2,3
                    if (p.z() > 0) {
                        //2
                        octants[2]++;
                    }
                    else {
                        //3
                        octants[3]++;
                    }
                }
            }
            else {
                //4, 5, 6, 7
                if (p.y() > 0) {
                    //4, 5
                    if (p.z() > 0) {
                        //4
                        octants[4]++;
                    }
                    else {
                        //5
                        octants[5]++;
                    }
                }
                else {
                    //6,7
                    if (p.z() > 0) {
                        //6
                        octants[6]++;
                    }
                    else {
                        //7
                        octants[7]++;
                    }
                }
            }
        }
        oct_0_edit->setText(QString::number(octants[0]));
        oct_1_edit->setText(QString::number(octants[1]));
        oct_2_edit->setText(QString::number(octants[2]));
        oct_3_edit->setText(QString::number(octants[3]));
        oct_4_edit->setText(QString::number(octants[4]));
        oct_5_edit->setText(QString::number(octants[5]));
        oct_6_edit->setText(QString::number(octants[6]));
        oct_7_edit->setText(QString::number(octants[7]));
    }
    int status = (check_0->isChecked() ? 1 : 0) << 0 |
        (check_1->isChecked() ? 1 : 0) << 1 |
        (check_2->isChecked() ? 1 : 0) << 2 |
        (check_3->isChecked() ? 1 : 0) << 3 |
        (check_4->isChecked() ? 1 : 0) << 4 |
        (check_5->isChecked() ? 1 : 0) << 5 |
        (check_6->isChecked() ? 1 : 0) << 6 |
        (check_7->isChecked() ? 1 : 0) << 7;
    widget->setData(index, mag_data, center, status);
    port_edit->setDisabled(work_thread->isRunning());
    start_btn->setText(work_thread->isRunning() ? "Stop" : "Start");
}

void MainDlg::saveCalibration() {
    Eigen::Matrix3d M;
    Eigen::Vector3d origin;
    ellipsoidFit(mag_data, M, origin);
    const QString filepath = QFileDialog::getSaveFileName(this, "Save calibration: ", QString(), "*.mag_cal");
    std::FILE* file = std::fopen(filepath.toStdString().c_str(), "w");
    std::fprintf(file, "%lf %lf %lf\n", origin.x(), origin.y(), origin.z());
    std::fprintf(file, "%lf %lf %lf\n%lf %lf %lf\n%lf %lf %lf\n", M(0, 0), M(0, 1), M(0, 2), M(1,0), M(1, 1), M(1, 2), M(2,0), M(2,1), M(2,2));
    std::fclose(file);
    {
        file = std::fopen("../test.txt", "w");
        for (const auto& i : mag_data) {
            std::fprintf(file, "%f, %f, %f\n", i.x(), i.y(), i.z());
        }
        std::fclose(file);
    }
}
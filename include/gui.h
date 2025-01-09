#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include "manager.h"

class TaskMonitor : public QMainWindow {
    Q_OBJECT

public:
    TaskMonitor(TaskManager* manager, QWidget* parent = nullptr);

private slots:
    void updateTaskTable();
    void appendLog(const QString& logMessage);
    void addTask();
    void startTasks();
    void cancelTasks();

private:
    TaskManager* taskManager;
    QTableWidget* taskTable;
    QTextEdit* logViewer; // Widget to display logs
    QPushButton* addTaskButton;
    QPushButton* startButton;
    QPushButton* cancelButton;

    void setupUI();
    void connectSignals();
};

#endif // TASKMONITOR_H

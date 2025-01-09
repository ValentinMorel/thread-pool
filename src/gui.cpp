#include "gui.h"
#include <QHeaderView>
#include <QTimer>
#include <QInputDialog>
#include "task.h"

TaskMonitor::TaskMonitor(TaskManager* manager, QWidget* parent)
    : QMainWindow(parent), taskManager(manager) {
    setupUI();
    connectSignals();

    // Periodic update of task information
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TaskMonitor::updateTaskTable);
    timer->start(1000); // Update every second
}

void TaskMonitor::setupUI() {
    this->setWindowTitle("Thread Pool Monitor");

    // Task table
    taskTable = new QTableWidget(this);
    taskTable->setColumnCount(3);
    taskTable->setHorizontalHeaderLabels({"Task Name", "Duration (s)", "Status"});
    taskTable->horizontalHeader()->setStretchLastSection(true);

    // Log viewer
    logViewer = new QTextEdit(this);
    logViewer->setReadOnly(true); // Prevent editing

    // Buttons
    addTaskButton = new QPushButton("Add Task", this);
    startButton = new QPushButton("Start Tasks", this);
    cancelButton = new QPushButton("Cancel Tasks", this);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(taskTable);
    layout->addWidget(logViewer);
    layout->addWidget(addTaskButton);
    layout->addWidget(startButton);
    layout->addWidget(cancelButton);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void TaskMonitor::connectSignals() {
    connect(addTaskButton, &QPushButton::clicked, this, &TaskMonitor::addTask);
    connect(startButton, &QPushButton::clicked, this, &TaskMonitor::startTasks);
    connect(cancelButton, &QPushButton::clicked, this, &TaskMonitor::cancelTasks);

    // Connect logging signal
    connect(taskManager, &TaskManager::logMessage, this, &TaskMonitor::appendLog);
}

void TaskMonitor::addTask() {
    QString name = QInputDialog::getText(this, "Add Task", "Enter task name:");
    if (!name.isEmpty()) {
        int duration = QInputDialog::getInt(this, "Add Task", "Enter task duration (seconds):", 2, 1, 3600);
        taskManager->addTask(std::make_shared<Task>(name.toStdString(), duration));

        // Update table immediately
        int row = taskTable->rowCount();
        taskTable->insertRow(row);
        taskTable->setItem(row, 0, new QTableWidgetItem(name));
        taskTable->setItem(row, 1, new QTableWidgetItem(QString::number(duration)));
        taskTable->setItem(row, 2, new QTableWidgetItem("Pending"));
    }
}

void TaskMonitor::startTasks() {
    taskManager->startExecution();
    appendLog("Task execution started.");
}

void TaskMonitor::cancelTasks() {
    taskManager->cancelExecution();
    appendLog("Task execution canceled.");
}

void TaskMonitor::updateTaskTable() {
    taskTable->clearContents();
    taskTable->setRowCount(0);

    auto statuses = taskManager->getTaskStatuses(); // Updated to return name, duration, status
    for (const auto& [name, duration, status] : statuses) {
        int row = taskTable->rowCount();
        taskTable->insertRow(row);
        taskTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(name)));
        taskTable->setItem(row, 1, new QTableWidgetItem(QString::number(duration)));
        taskTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(status)));
    }
}


void TaskMonitor::appendLog(const QString& logMessage) {
    logViewer->append(logMessage); // Add the log message to the QTextEdit
}

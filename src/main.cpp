#include <QApplication>
#include "manager.h"
#include "gui.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create a TaskManager with a pool of 4 threads
    TaskManager taskManager(4);

    // Create the TaskMonitor GUI
    TaskMonitor taskMonitor(&taskManager);
    taskMonitor.show();

    return app.exec();
}

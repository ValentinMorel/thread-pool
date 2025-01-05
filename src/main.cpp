#include "manager.h"
#include "task.h"

int main(){

  TaskManager manager(4);

  manager.addTask(std::make_shared<Task>("Task1", 2));
  manager.addTask(std::make_shared<Task>("Task2", 3));
  manager.addTask(std::make_shared<Task>("Task3", 1));
  manager.addTask(std::make_shared<Task>("Task4", 4));
  manager.addTask(std::make_shared<Task>("Task5", 2));
  manager.addTask(std::make_shared<Task>("Task6", 3));
  return 0;
}

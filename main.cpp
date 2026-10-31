#include <iostream>
#include <string>

//  Абстрактная задача
class Task {
public:
    virtual void execute() = 0; 
};

//  Наследники 
class PrintTask : public Task {
    std::string text;
public:
    PrintTask(std::string t) : text(t) {}
    void execute() override {
        std::cout << "[Печать] " << text << "\n";
    }
};

class MathTask : public Task {
    int a, b;
public:
    MathTask(int x, int y) : a(x), b(y) {}
    void execute() override {
        std::cout << "[Математика] " << a << " + " << b << " = " << (a + b) << "\n";
    }
};

class DownloadTask : public Task {
    std::string filename;
public:
    DownloadTask(std::string f) : filename(f) {}
    void execute() override {
        std::cout << "[Скачивание] Файл " << filename << " загружен.\n";
    }
};

// Очередь с приоритетом (двоичная куча)
class TaskQueue {
private:
    struct Node {
        Task* task;
        int priority;
    };
    
    static const int MAX = 100; // Максимальный размер
    Node heap[MAX];
    int size;

    // поменять местами
    void swap(int i, int j) {
        Node temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    // Просеивание вверх (при добавлении)
    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[i].priority > heap[parent].priority) {
                swap(i, parent);
                i = parent;
            } else {
                break;
            }
        }
    }

    // Просеивание вниз (при извлечении)
    void siftDown(int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;

            if (left < size && heap[left].priority > heap[largest].priority)
                largest = left;
            if (right < size && heap[right].priority > heap[largest].priority)
                largest = right;

            if (largest != i) {
                swap(i, largest);
                i = largest;
            } else {
                break;
            }
        }
    }

public:
    TaskQueue() { size = 0; }

    // Добавить задачу с приоритетом
    void push(Task* task, int priority) {
        heap[size].task = task;
        heap[size].priority = priority;
        siftUp(size);
        size++;
    }

    // Выполнить задачу с максимальным приоритетом
    void pop() {
        if (size == 0) return;
        
        heap[0].task->execute();
        delete heap[0].task; // Удаляем задачу из памяти
        
        heap[0] = heap[size - 1]; // На место корня ставим последний
        size--;
        siftDown(0);
    }

    bool empty() {
        return size == 0;
    }
};

// Демонстрация работы
int main() {
    TaskQueue queue;

    // Добавляем задачи с разным приоритетом
    queue.push(new PrintTask("Привет, мир!"), 2);
    queue.push(new MathTask(10, 20), 5);
    queue.push(new DownloadTask("file.txt"), 1);
    queue.push(new PrintTask("Важное сообщение"), 10);

    std::cout << "Выполнение очереди (по убыванию приоритета)\n";
    while (!queue.empty()) {
        queue.pop();
    }

    return 0;
}
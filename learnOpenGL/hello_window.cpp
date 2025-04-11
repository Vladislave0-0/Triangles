//==================================================================
// Ссылка на урок: https://habr.com/ru/articles/311234/
//
// Функция glfwWindowShouldClose проверяет в начале каждой 
// итерации цикла, получил ли GLFW инструкцию к закрытию, 
// если так — то функция вернет true и игровой цикл перестанет 
// работать, после чего мы сможем закрыть наше приложение.
//
// Функция glfwPollEvents проверяет были ли вызваны какие либо 
// события (вроде ввода с клавиатуры или перемещение мыши) и 
// вызывает установленные функции (которые мы можем установить 
// через функции обратного вызова (callback)). Обычно мы 
// вызываем функции обработки событий в начале итерации цикла.
//
// Функция glfwSwapBuffers заменяет цветовой буфер (большой буфер, 
// содержащий значения цвета для каждого пикселя в GLFW окне), 
// который использовался для отрисовки во время текущей итерации 
// и показывает результат на экране.
//
// Для достижения некоего контроля над вводом, мы можем 
// воспользоваться функциями обратного вызова в GLFW. Функции 
// обратного вызова это указатели на функции, которые можно передать 
// в GLFW, чтобы они были вызваны в нужное время. Одной из таких 
// функций является KeyCallback, которая будет вызываться каждый 
// раз, когда пользователь использует клавиатуру.
//==================================================================

#include <GLFW/glfw3.h>
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
  // и приложение после этого закроется
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  // Передали key_callback в функцию в GLFW.
  glfwSetKeyCallback(window, key_callback);

  // Настроили размеры окна
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Покрасили содержимое окна в другой цвет
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  // Очистили выделенные ресурсы
  glfwTerminate();
}

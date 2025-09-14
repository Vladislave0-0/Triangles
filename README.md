# Оглавление

Проект создан в рамках курса по $C$++ Константина Владимирова.

- [Level 1: Intersection of triangles](#hw3d-level-1-intersection-of-triangles-in-r3)

- [Level 2: Visualization in OpenGL](#hw3d-level-2-visualization-in-opengl)

<br><br>
***
# HW3D. Level 1: Intersection of triangles in $R^3$

## Описание
Первый уровень проекта реализует пересечение треугольников в пространстве. Важно также отметить, что если треугольник вырожденный, то это всё ещё треугольник (то есть треугольник может быть и прямой, и точкой).

## Требования
CMake с версией не меньше 3.11

## Ввод/вывод
Со стандартного ввода программа получает число $0 < N \leqslant 10^6$, а потом $N$ наборов из $9$ точек, представляющих вершины трёхмерных треугольников:
```
2

1 0 1
1 0 5
5 0 4

5 0 2
2.1 0 8
1 0 -1
```

На выходе получаем номера всех треугольников, которые пересекаются хотя бы с одним другим треугольником.

```
0
1
```

## Компиляция
```bash
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/
```

Тестирование проекта (интеграция google- и end2end-тестов):
```bash
cd build/
ctest --output-on-failure
```

Запуск проекта:
```bash
cd build/
./triag < path_to_test
```

Запуск отдельно google-тестов и end2end-тестов соответственно:
```bash
cd build/
./google_test
../end2end/run_e2e.sh
```

<br><br><br>
***

# HW3D. Level 2: Visualization in OpenGL

## Описание
Второй  уровень проекта реализует визуализацию плоских треугольников в $R^3$ с помощью OpenGL. 

Трёхмерная сцена должна состоять из:
- Фиксированного объёма, где располагаются все треугольники;
- Всех треугольников, нарисованных синим цветом;
- Треугольников, пересекающихся с другими, нарисованных красным цветом;
- Должно быть освещение;
- Должна быть возможность управлять камерой, облетая сцену вокруг, приближая и отдаляя камеру (мышкой или клавиатурой).
***

## Требования к зависимостям
Для корректной работы программы необходимо установить следующие библиотеки:
```bash
libgl1-mesa-dev, libglfw3, libglfw3-dev, libglew-dev, libgtest-dev, libglm-dev, libxi-dev
```

## Компиляция
```bash
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/
```

## Тестирование
```bash
cd build/
# Все тесты
ctest --output-on-failure

# Только google-тесты
./google_test

# Только end2end-тесты
../end2end/run_e2e.sh
```

## Использование флага --help или -h
```bash
Usage: triag [OPTIONS] < input_file

Options:
  -v, --visualize   # Enable visualization mode
  -h, --help        # Show this help message
  --version         # Show version information

Examples:
  triag < input.txt          # Calculation mode (default)
  triag -v < input.txt       # Visualization mode 
```


## Управление
    W/S: Движение вперед/назад
    A/D: Движение влево/вправо
    Space/Shift: Движение вверх/вниз
    Мышь: Поворот камеры
    ESC: Пауза/меню настроек
    Q: Выход

## Меню настроек (ESC)
    Цвета треугольников (пересекающиеся/обычные)
    Скорость камеры и поле зрения
    Отображение координатной сетки
    Параметры освещения

---
<br><br>
![alt text](./images/debug.png)

# Triangles Project: intersection of triangles in $R^3$

## Описание
Проект создан в рамках курса по $C$++ Константина Владимирова и реализует пересечение треугольников в пространстве. Важно также отметить, что если треугольник вырожденный, то это всё ещё треугольник (то есть треугольник может быть и прямой, и точкой).

## Компиляция
```
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cmake --build build --target all
```

Тестирование проекта (интеграция google- и end2end-тестов):
```
cmake --build build --target test
```

Запуск проекта:
```
cd build/
./triag < path_to_test
```

Запуск отдельно google-тестов end2end-тестов соответственно:
```
./google_test
../end2end/run_e2e.sh
```


## Структура проекта
- **src/**: содержит исходные файлы с запуском программы и google-тестами
  - `lfu_cache.cpp`
  - `perfect_cache.cpp`
  - `google_test.cpp`
- **include/**: содержит заголовочные файлы с реализацией основных классов и методов:
  - `config.hpp`   
  - `interval.hpp` 
  - `line.hpp`
  - `octotree.hpp` 
  - `plane.hpp`
  - `point.hpp`   
  - `triangles.hpp`
  - `vector.hpp`
- **end2end/**: содержит конечные тесты и их результаты:
  - `run_e2e.sh`
  - `answers/`  
  - `tests/`
- **geogebra_tests/**: содержит архивы для визуализации конкретного google-теста в geogebra-3D:
  - `Intersection3D_1.ggb`
  - `Intersection3D_2.ggb`
  - `...`
- **images/**: для мема в конце README:
  - `debug.png`

## Ввод/вывод
Со стандартного ввода программа получает число $0 < N \leqslant 10^6$, а потом $N$ наборов из $9$ точек, представляющих вершины трёхмерных треугольников. 

На выходе получаем номера всех треугольников, которые пересекаются хотя бы с одним другим треугольником.

![alt text](./images/debug.png)

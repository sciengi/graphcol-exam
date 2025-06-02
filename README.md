### Сборка (Ветка со сборкой под Windows)

Для запуска визуализатора необходимо открыть в браузере `visualizer/visualizer.html`.

Для сборки алгоритма используется [cmake](https://cmake.org/), для чего в корне репозитория подготовлен CMakeLists.txt.
```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --config Release
$ cd \build\algorithm\Release
$ .\demo.exe # Запуск демонстрации
```

> Протестировано в стандартной vscode под Windows 10


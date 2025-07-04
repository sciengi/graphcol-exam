
### О работе

Данный репозиторий содержит реализацию алгоритма рёберной раскраски Мисры и Гриза, 
способную генерировать протокол для приложенного визуализатора.

Алгоритм предназначен для правильной раскраски неориентированных графов без петель и кратных ребёр.
Является конструктивным доказательством [теоремы Визинга](https://ru.wikipedia.org/wiki/%D0%A2%D0%B5%D0%BE%D1%80%D0%B5%D0%BC%D0%B0_%D0%92%D0%B8%D0%B7%D0%B8%D0%BD%D0%B3%D0%B0).

Содержимое директорий:
- `algorithm`  - исходный код алгоритма, тесты
- `examples`   - примеры протоколов, сгенерированные алгоритмом
- `visualizer` - визуализатор и инструкция для него

**Материалы для рецензентов прикреплены в общий файл экзамена!**


### Сборка и запуск

Визуализатор не требует предварительных действий: для его запуска необходимо открыть в браузере `visualizer/visualizer.html`.

Алгоритм написан на C++. 
Для его сборки используется [cmake](https://cmake.org/), для чего в корне репозитория подготовлен `CMakeLists.txt`.
> Пользователь Windows - см. [ветку](https://github.com/sciengi/graphcol-exam/tree/Windows-building-changes)

Для сборки алгоритма выполнить следующие команды в терминале:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
После этого можно запустить демонстрацию работы алгоритма: `./algorithm/demo`.

Если в системе есть [googletest](https://github.com/google/googletest), то будут собраны тесты.
Их можно запустить командой `./algorithm/test/tester`, код тестов находится в `algorithm/test/`.


### Демонстрация

Имея собранный алгоритм, можно с помощью `demo` подавать граф в виде матрицы смежности на раскраску алгоритму.
`demo` в той же директории сгенерирует протокол - `report.json`.
Его  можно загрузить в визуализатор с помощью соответствующей кнопки.

Так выглядит взаимодействие с `demo` на примере ввода $K_{4}$:
```bash
[build directory]$ ./algorithm/demo
Please, enter graph as a row count and adjacency matrix:
(Use 1 for connections and 0 for not)
4
0 1 1 1
1 0 1 1
1 1 0 1
1 1 1 0

'Colored' adjacency matrix:
0 4 2 1 
4 0 1 2 
2 1 0 3 
1 2 3 0 

Using 4 colors, when degree of graph is 3
Coloring is correct
```

После "раскрашенной" матрицы смежности выводится количество использованных цветов, максимальная степень графа, и результат проверки раскраски.


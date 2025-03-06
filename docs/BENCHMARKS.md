# Benchmarks

## Новый benchmarking

Benchmarking происходит в `playground` на двух сценариях исполнения, для каждого дерева отдельно.

### Описание

Сценарии:
1. `Random`: Генерирует псевдослучайные наборы данных элементов: 1. вставка (элементы вставляются в дерево), 2. удаление (элементы удаляются из дерева); значения ограничены, что повышает вероятность появления одинаковых элементов; наборы данных не хранятся в памяти отдельно, а вставляются в дерево поэлементно, что способствует точному анализу потребления памяти.
2. `Sorted`: Генерирует отсортированный массив уникальных входных значений (в порядке возрастания) и вставляет их в дерево, а затем удаляет в псевдослучайном порядке.

Для каждого сценария считается общее время исполнения и время исполнения (среднее и общее) для операций `insert`/`remove`.

**Кол-во элементов в сценарии: 200'000**.


### Результаты

1. **Darwin 24.3.0 macOS-15.3.1, Apple M3 Max**:
```txt
Operating System: Darwin 24.3.0
Platform: macOS-15.3.1-arm64-arm-64bit-Mach-O
Processor: arm
RAM: 64.00 GB
Reading configuration from: ./configs/benchmarking.yaml
Configured Trees: ['RBTree', 'AVLTree', 'SplayTree', 'TreapTree']
Benchmark Mode: all
Memcheck Mode: False

Running benchmark for tree: RBTree with mode: all
==== Output for RBTree ====
=== Report for 'RBTree<int>', scenario 'RANDOM' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 113.971ms
        Avg. time per operation: 0.000569854ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 195815
        Total time elapsed: 96.7281ms
        Avg. time per operation: 0.000483641ms
Total time: 310.805ms


=== Report for 'RBTree<int>', scenario 'SORTED' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 92.6408ms
        Avg. time per operation: 0.000463204ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 200000
        Total time elapsed: 109.031ms
        Avg. time per operation: 0.000545153ms
Total time: 285.509ms



Running benchmark for tree: AVLTree with mode: all
==== Output for AVLTree ====
=== Report for 'AVLTree<int>', scenario 'RANDOM' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 461.769ms
        Avg. time per operation: 0.00230884ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 195815
        Total time elapsed: 407.602ms
        Avg. time per operation: 0.00203801ms
Total time: 978.697ms


=== Report for 'AVLTree<int>', scenario 'SORTED' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 467.915ms
        Avg. time per operation: 0.00233958ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 200000
        Total time elapsed: 452.141ms
        Avg. time per operation: 0.0022607ms
Total time: 1015.61ms



Running benchmark for tree: SplayTree with mode: all
==== Output for SplayTree ====
=== Report for 'SplayTree<int>', scenario 'RANDOM' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 120.231ms
        Avg. time per operation: 0.000601154ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 195815
        Total time elapsed: 124.114ms
        Avg. time per operation: 0.000620568ms
Total time: 347.616ms


=== Report for 'SplayTree<int>', scenario 'SORTED' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 76.093ms
        Avg. time per operation: 0.000380465ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 200000
        Total time elapsed: 179.566ms
        Avg. time per operation: 0.000897832ms
Total time: 344ms



Running benchmark for tree: TreapTree with mode: all
==== Output for TreapTree ====
=== Report for 'TreapTree<int>', scenario 'RANDOM' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 196.359ms
        Avg. time per operation: 0.000981793ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 195815
        Total time elapsed: 140.659ms
        Avg. time per operation: 0.000703294ms
Total time: 441.85ms


=== Report for 'TreapTree<int>', scenario 'SORTED' ===
INSERTION:
        Operations executed: 200000
        Total time elapsed: 119.857ms
        Avg. time per operation: 0.000599285ms
REMOVAL:
        Operations executed: 200000
        Succeeded (removed an element): 200000
        Total time elapsed: 107.614ms
        Avg. time per operation: 0.00053807ms
Total time: 315.737ms
```
    
**Выводы**: верны выводы в результатах старого бенчмаркинга ниже. Помимо этого, как ожидалось, Red-Black Tree наиболее эффективный в обоих сценариях, в особенности оно хорошо справляется с вставкой и произвольным удалением отсортированных элементов.

*ScapeGoat Tree было протестировано на другой ОС, поэтому не совсем корректно сравнивать с данными замерами, но опираясь на сделанные можно сказать, что новое дерево также проигрывает Red-Black Tree, но его плюс в возможности ускорить одну из операций, путем изменения коэффициента балансировки, но при этом увеличится время работы другой операции*

## Старый benchmarking

Бенчмарк прогоняется как часть юнит-тестов (таргет `test_tree_algorithms`.
Смотреть файл [`tests/test_tree_algorithms.cpp`](../tests/test_tree_algorithms.cpp) на тест `TEST(TreesBenchmark, MeasureExecutionTimes)`.

### Описание

- Для каждого дерева вызываются операции `insert` и `remove` равновероятностно, суммарное число операций: 1M.
- Выводится:
    - Среднее время выполнение операций для каждого дерева
    - Общее суммарное время исполнения
- Запуск с флагами cmake release сборки


1. **Linux: WSL2 Ubuntu 22.04** (Intel(R) Core(TM) i7-8550U CPU @ 1.80GH)
    ```txt
    === Report for 'AVLTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 8588ms
    Total insert time: 4721.42ms, Average insert time: 0.00944551ms
    Total remove time: 2630.33ms, Average remove time: 0.00525918ms
    
    === Report for 'SplayTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 5584.9ms
    Total insert time: 2284.1ms, Average insert time: 0.0045695ms
    Total remove time: 2117.74ms, Average remove time: 0.00423428ms
    
    === Report for 'TreapTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 4336.97ms
    Total insert time: 1886.56ms, Average insert time: 0.00377418ms
    Total remove time: 1339.78ms, Average remove time: 0.0026788ms
    ```
   
   Выводы: все имплементации написаны на указателях, причем в Splay и AVL деревьях разыменований существенно больше, чем в Treap,
   из-за этого они работают медленнее. При этом AVL в своей реализации методов `insert` и `remove` использует `std::vector`
   для сохранения пути от корня до нужной вершины, что сильно замедляет работу из-за динамических переаллокаций внутри вектора.
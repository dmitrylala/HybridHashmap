# HybridHashTable
Реализация хэш-таблицы с закрытым хэшированием (в каждой корзинке - указатель на список элементов), при этом в каждой корзинке есть небольшой “запас” места, чтобы помещать туда небольшое число элементов (а не в список).

## Реализация
Основная логика корзинки с "запасом" реализована в классе Bucket<Key, T>, в которой хранятся пары ключ-значения хэш-таблицы. Класс содержить два атрибута - m_cache и m_list, m_cache и представляет собой тот самый "запас" места. При удалении пары из этого кэша, если есть элементы в списке, то они копируются в освободившееся место в кэше. Предоставляемый интерфейс: методы add, remove, contains, clear и тд.

```c++
template<typename Key, typename T>
class Bucket {
  ...
  void add(const std::pair<Key, T> &);
  bool remove(const Key &);
  bool contains(const Key &) const;
  ...
  std::vector< std::pair<Key, T> > m_cache;
  std::list< std::pair<Key, T> > m_list;
};
```

Класс Hashmap реализован как набор корзинок, куда помещаются пары ключ-значение, при этом номер корзинки определяется с помощью хэша от ключа. Также предоставляет схожий интерфейс для добавления, удаления и модификации существующих элементов.

```c++
template<
    typename Key,
    typename T,
    typename Hash = std::hash<Key>
> class Hashmap {
  ...
  void add(const std::pair<Key, T> &);
  void remove(const_key_reference key);
  bool contains(const Key &) const;
  T& operator[](const Key &);
  T operator[](const Key &) const;
  ...
  std::hash<Key> m_hash;
  std::vector< Bucket<Key, T> > m_buckets;
};
```

## Кодстайл
C++ кодстайл - [Google Style Guide](https://google.github.io/styleguide/cppguide.html), python кодстайл - PEP8, исключения указаны в CPPLINT.cfg и в .pylintrc.

## Установка зависимостей и сборка
Из зависимостей понадобится cmake, clang10 (либо другая версия, нужно ее позже указать conan) и poetry:
```
pip install poetry
```

Сборку и запуск тестов нужно выполнять под venv, который устанавливается и активируется с помощью poetry:
```
poetry install && poetry shell
mkdir build && cd build
conan install .. --build missing -s compiler=clang -s compiler.version=10 -s compiler.libcxx=libstdc++11
cd ..
cmake . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build build --target all
```
Имеются следующие таргеты (помещаются в build/):
- hashmap - демонстрационная программа, интерфейс взаимодействия доступен по help
- hashmap_test - юнит тесты

Запуск интеграционных тестов:
```
pytest test/integr/hashmap.py
```

Запуск линтеров:
```
cpplint include/* bin/* test/unit/*
pylint test/integr/hashmap.py
```

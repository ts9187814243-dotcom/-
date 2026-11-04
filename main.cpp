#include <iostream>
#include <limits>
#include <string>   // для std::string
#include <iomanip>  // для setw / fixed / setprecision при выводе таблиц

using namespace std;

// ==================== Структуры для хранения данных ====================
// Product — базовая сущность проекта 
struct Product {
    int id;                     // уникальный идентификатор товара
    string name;                // название товара
    string category;            // категория товара
    int quantity;               // количество на складе
    double price;               // цена за единицу
    string supplier;            // поставщик
    int typeCode;               // тип товара: 1 — обычный, 2 — скоропортящийся, 3 — электроника, 4 — одежда

    // поля для скоропортящегося товара
    string expiryDate;          // срок годности (например, "2026-12-31")
    string storageConditions;   // условия хранения (например, "холодильник +2...+6°C")

    // поля для электроники
    string brand;               // бренд производителя
    int warrantyMonths;         // гарантия в месяцах
    string serialNumber;        // серийный номер

    // поля для одежды
    string size;                // размер (например, "M", "L", "42")
    string color;               // цвет
    string fabricComposition;   // состав ткани (например, "80% хлопок, 20% полиэстер")
};

// SupplyOperation — операция на складе
struct SupplyOperation {
    int operationId;            // ID операции
    int operationType;          // тип операции: 1 — поступление, 2 — отгрузка
    int productId;              // ID связанного товара
    int amount;                 // количество в операции
    string date;                // дата операции (например, "2026-10-01")
    string reason;              // причина (например, "закупка", "продажа", "брак", "возврат")
};

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Система управления складом" << '\n';

    // ==================== ОБЪЯВЛЕНИЕ И ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ ====================

    // Product — базовый класс
    int id = 0;                     // уникальный идентификатор товара
    string name = "";               // название товара
    string category = "";           // категория (выбирается из списка по типу товара, включая пункт "Другое")
    int quantity = 0;               // количество на складе
    double price = 0.0;             // цена за единицу
    string supplier = "";           // поставщик

    //  — скоропортящийся товар
    string expiryDate = "";         // срок годности (например, "2026-12-31")
    string storageConditions = "";  // условия хранения (например, "холодильник +2...+6°C")

    //  — электроника
    string brand = "";              // бренд производителя
    int warrantyMonths = 0;         // гарантия в месяцах
    string serialNumber = "";       // серийный номер

    //  — одежда
    string size = "";               // размер (например, "M", "L", "42")
    string color = "";              // цвет
    string fabricComposition = "";  // состав ткани (например, "80% хлопок, 20% полиэстер")

    //  — операция на складе
    int operationId = 0;            // ID операции
    int operationType = 0;          // тип операции: 1 — поступление, 2 — отгрузка
    int productId = 0;              // ID связанного товара
    int amount = 0;                 // количество в операции
    string date = "";               // дата операции (например, "2026-10-01")
    string reason = "";             // причина (например, "закупка", "продажа", "брак", "возврат")

    //  Переменные для меню и управления складом
    int menuChoice = -1;            // выбранный пункт главного меню
    int productCount = 0;           // текущее количество товаров в списке
    int operationCount = 0;         // текущее количество записей в истории операций
    int searchChoice = 0;           // вариант поиска (1 — по названию, 2 — по категории)
    int filterChoice = 0;           // вариант фильтрации списка товаров
    int sortChoice = 0;             // вариант сортировки (1 — по количеству, 2 — по цене)
    string searchQuery = "";        // строка поискового запроса

    const int MAX_PRODUCTS = 100;      // максимальное количество товаров
    const int MAX_OPERATIONS = 1000;   // максимальное количество операций
    const int LOW_STOCK_THRESHOLD = 5; // порог для уведомления о недостатке товара

    // Массивы товаров и операций — основное хранилище данных программы
    Product products[MAX_PRODUCTS];             // массив товаров (до 100)
    SupplyOperation operations[MAX_OPERATIONS]; // история операций (до 1000)

    // Счётчики ID для новых записей
    int nextProductId = 1;      // следующий свободный ID товара
    int nextOperationId = 1;    // следующий свободный ID операции

    // Вспомогательные переменные для циклов и проверок
    int i = 0;                  // счётчик циклов
    int foundIndex = -1;        // индекс найденного товара (-1 — не найден)
    bool found = false;         // признак успешного поиска
    bool idExists = false;      // признак того, что ID уже занят

    // =====================================================================
    // ============================ ГЛАВНОЕ МЕНЮ ===========================
    // Все действия выполняются только через меню
    // =====================================================================

    while (true) { // бесконечный цикл — программа работает до пункта «Выход»

        cout << "\n========== ГЛАВНОЕ МЕНЮ ==========" << '\n';
        cout << "1  — Добавить товар" << '\n';
        cout << "2  — Показать список товаров" << '\n';
        cout << "3  — Поиск товаров" << '\n';
        cout << "4  — Сортировка товаров" << '\n';
        cout << "5  — Поступление / отгрузка товара" << '\n';
        cout << "6  — Отчёт по остаткам" << '\n';
        cout << "7  — История операций" << '\n';
        cout << "8  — Расчёты по товару" << '\n';
        cout << "9  — Таблица стоимости партии (табуляция)" << '\n';
        cout << "10 — Матрица остатков по складам" << '\n';
        cout << "11 — Полное описание товара" << '\n';
        cout << "0  — Выход" << '\n';
        cout << "Выберите пункт: ";
        cin >> menuChoice;
        while (cin.fail() || menuChoice < 0 || menuChoice > 11) {
            cout << "Введите число от 0 до 11: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> menuChoice;
        }

        switch (menuChoice) {

            // ==================== 1. Добавление товара ====================
        case 1: {
            // Вложенное меню: выбор типа товара
            int typeChoice = 0;
            cout << "\n--- Добавление товара ---" << '\n';
            cout << "Выберите тип товара:" << '\n';
            cout << "1 — обычный товар" << '\n';
            cout << "2 — скоропортящийся товар" << '\n';
            cout << "3 — электроника" << '\n';
            cout << "4 — одежда" << '\n';
            cout << "Тип: ";
            cin >> typeChoice;
            while (cin.fail() || typeChoice < 1 || typeChoice > 4) {
                cout << "Введите число от 1 до 4: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> typeChoice;
            }

            char addMore = '0'; // повтор добавления нескольких товаров
            do {
                if (productCount >= MAX_PRODUCTS) {
                    cout << "Склад переполнен (максимум " << MAX_PRODUCTS << " товаров)." << '\n';
                    break;
                }

                Product p;              // временная переменная нового товара
                p.typeCode = typeChoice; // запоминаем выбранный тип

                // --- ввод основных полей товара ---
                cout << "ID товара: ";
                cin >> p.id;
                while (cin.fail() || p.id <= 0) {
                    cout << "Введите корректный ID (целое положительное число): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> p.id;
                }

                // Проверка уникальности ID
                idExists = false;
                for (i = 0; i < productCount; i++) {
                    if (products[i].id == p.id) { idExists = true; break; }
                }
                while (idExists) {
                    cout << "Такой ID уже есть! Введите другой: ";
                    cin >> p.id;
                    while (cin.fail() || p.id <= 0) {
                        cout << "Введите корректный ID (целое положительное число): ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> p.id;
                    }
                    idExists = false;
                    for (i = 0; i < productCount; i++) {
                        if (products[i].id == p.id) { idExists = true; break; }
                    }
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка буфера перед getline

                cout << "Название товара: ";
                getline(cin, p.name);
                while (p.name.empty()) {
                    cout << "Название не может быть пустым. Введите название: ";
                    getline(cin, p.name);
                }

                // --- выбор категории из фиксированного списка (зависит от типа товара) ---
                int categoryChoice = 0;

                cout << "Категория товара:" << '\n';

                switch (typeChoice) {
                case 1: // обычный товар — универсальный список категорий
                    cout << "1 — Бытовая химия" << '\n';
                    cout << "2 — Канцелярия" << '\n';
                    cout << "3 — Мебель" << '\n';
                    cout << "4 — Инструменты" << '\n';
                    cout << "5 — Другое" << '\n';
                    cout << "Категория: ";
                    cin >> categoryChoice;
                    while (cin.fail() || categoryChoice < 1 || categoryChoice > 5) {
                        cout << "Введите число от 1 до 5: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> categoryChoice;
                    }
                    if (categoryChoice == 1)      p.category = "Бытовая химия";
                    else if (categoryChoice == 2) p.category = "Канцелярия";
                    else if (categoryChoice == 3) p.category = "Мебель";
                    else if (categoryChoice == 4) p.category = "Инструменты";
                    else                          p.category = "Другое";
                    break;

                case 2: // скоропортящийся товар — продуктовые категории
                    cout << "1 — Молочные продукты" << '\n';
                    cout << "2 — Овощи и фрукты" << '\n';
                    cout << "3 — Мясо и рыба" << '\n';
                    cout << "4 — Заморозка" << '\n';
                    cout << "5 — Другое" << '\n';
                    cout << "Категория: ";
                    cin >> categoryChoice;
                    while (cin.fail() || categoryChoice < 1 || categoryChoice > 5) {
                        cout << "Введите число от 1 до 5: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> categoryChoice;
                    }
                    if (categoryChoice == 1)      p.category = "Молочные продукты";
                    else if (categoryChoice == 2) p.category = "Овощи и фрукты";
                    else if (categoryChoice == 3) p.category = "Мясо и рыба";
                    else if (categoryChoice == 4) p.category = "Заморозка";
                    else                          p.category = "Другое";
                    break;

                case 3: // электроника
                    cout << "1 — Смартфоны" << '\n';
                    cout << "2 — Ноутбуки" << '\n';
                    cout << "3 — Бытовая техника" << '\n';
                    cout << "4 — Аксессуары" << '\n';
                    cout << "5 — Другое" << '\n';
                    cout << "Категория: ";
                    cin >> categoryChoice;
                    while (cin.fail() || categoryChoice < 1 || categoryChoice > 5) {
                        cout << "Введите число от 1 до 5: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> categoryChoice;
                    }
                    if (categoryChoice == 1)      p.category = "Смартфоны";
                    else if (categoryChoice == 2) p.category = "Ноутбуки";
                    else if (categoryChoice == 3) p.category = "Бытовая техника";
                    else if (categoryChoice == 4) p.category = "Аксессуары";
                    else                          p.category = "Другое";
                    break;

                case 4: // одежда
                    cout << "1 — Верхняя одежда" << '\n';
                    cout << "2 — Обувь" << '\n';
                    cout << "3 — Трикотаж" << '\n';
                    cout << "4 — Аксессуары" << '\n';
                    cout << "5 — Другое" << '\n';
                    cout << "Категория: ";
                    cin >> categoryChoice;
                    while (cin.fail() || categoryChoice < 1 || categoryChoice > 5) {
                        cout << "Введите число от 1 до 5: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> categoryChoice;
                    }
                    if (categoryChoice == 1)      p.category = "Верхняя одежда";
                    else if (categoryChoice == 2) p.category = "Обувь";
                    else if (categoryChoice == 3) p.category = "Трикотаж";
                    else if (categoryChoice == 4) p.category = "Аксессуары";
                    else                          p.category = "Другое";
                    break;
                }

                cout << "Количество товара: ";
                cin >> p.quantity;
                while (cin.fail() || p.quantity <= 0) {
                    cout << "Введите корректное количество (целое положительное число): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> p.quantity;
                }

                cout << "Цена товара: ";
                cin >> p.price;
                while (cin.fail() || p.price <= 0) {
                    cout << "Введите корректную цену (положительное число): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> p.price;
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Поставщик: ";
                getline(cin, p.supplier);
                while (p.supplier.empty()) {
                    cout << "Поставщик не может быть пустым. Введите поставщика: ";
                    getline(cin, p.supplier);
                }

                // --- ввод полей в зависимости от типа товара (вложенное ветвление) ---
                switch (typeChoice) {
                case 2: // скоропортящийся товар
                    cout << "Введите срок годности (гггг-мм-дд): ";
                    getline(cin, p.expiryDate);
                    while (p.expiryDate.empty()) {
                        cout << "Срок годности не может быть пустым. Введите срок годности: ";
                        getline(cin, p.expiryDate);
                    }
                    cout << "Введите условия хранения: ";
                    getline(cin, p.storageConditions);
                    while (p.storageConditions.empty()) {
                        cout << "Условия хранения не могут быть пустыми. Введите условия: ";
                        getline(cin, p.storageConditions);
                    }
                    break;

                case 3: // электроника
                    cout << "Введите бренд: ";
                    getline(cin, p.brand);
                    while (p.brand.empty()) {
                        cout << "Бренд не может быть пустым. Введите бренд: ";
                        getline(cin, p.brand);
                    }
                    cout << "Введите гарантию (в месяцах): ";
                    cin >> p.warrantyMonths;
                    while (cin.fail() || p.warrantyMonths <= 0) {
                        cout << "Введите корректный срок гарантии (целое положительное число): ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> p.warrantyMonths;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Введите серийный номер: ";
                    getline(cin, p.serialNumber);
                    while (p.serialNumber.empty()) {
                        cout << "Серийный номер не может быть пустым. Введите серийный номер: ";
                        getline(cin, p.serialNumber);
                    }
                    break;

                case 4: // одежда
                    cout << "Введите размер: ";
                    getline(cin, p.size);
                    while (p.size.empty()) {
                        cout << "Размер не может быть пустым. Введите размер: ";
                        getline(cin, p.size);
                    }
                    cout << "Введите цвет: ";
                    getline(cin, p.color);
                    while (p.color.empty()) {
                        cout << "Цвет не может быть пустым. Введите цвет: ";
                        getline(cin, p.color);
                    }
                    cout << "Введите состав ткани: ";
                    getline(cin, p.fabricComposition);
                    while (p.fabricComposition.empty()) {
                        cout << "Состав ткани не может быть пустым. Введите состав: ";
                        getline(cin, p.fabricComposition);
                    }
                    break;

                default: // обычный товар — дополнительных полей нет
                    break;
                }

                // Запись товара в массив
                products[productCount] = p;
                productCount++;
                nextProductId = p.id + 1;
                cout << "Товар добавлен! Всего товаров: " << productCount << '\n';

                cout << "Добавить ещё товар такого же типа? (1 — да, 0 — нет): ";
                cin >> addMore;
                while (cin.fail() || (addMore != '1' && addMore != '0')) {
                    cout << "Введите 1 или 0: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> addMore;
                }
            } while (addMore == '1');
            break;
        }

              // ==================== 2. Список товаров с фильтрацией ====================
        case 2: {
            if (productCount == 0) {
                cout << "Список пуст. Сначала добавьте товары." << '\n';
                break;
            }

            // Вложенное меню фильтрации
            cout << "\n--- Список товаров ---" << '\n';
            cout << "1 — все товары" << '\n';
            cout << "2 — только с низким остатком (<" << LOW_STOCK_THRESHOLD << ")" << '\n';
            cout << "3 — по категории" << '\n';
            cout << "Вариант: ";
            cin >> filterChoice;
            while (cin.fail() || filterChoice < 1 || filterChoice > 3) {
                cout << "Введите число от 1 до 3: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> filterChoice;
            }

            string filterCategory = "";
            if (filterChoice == 3) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Введите категорию: ";
                getline(cin, filterCategory);
                while (filterCategory.empty()) {
                    cout << "Категория не может быть пустой. Введите категорию: ";
                    getline(cin, filterCategory);
                }
            }

            cout << "\n" << setw(5) << "ID" << setw(20) << "Название"
                << setw(15) << "Категория" << setw(10) << "Кол-во"
                << setw(12) << "Цена" << setw(15) << "Поставщик" << '\n';

            int shown = 0; // сколько строк выведено
            for (i = 0; i < productCount; i++) {
                // условия фильтрации
                if (filterChoice == 2 && products[i].quantity >= LOW_STOCK_THRESHOLD) continue;
                if (filterChoice == 3 && products[i].category != filterCategory) continue;

                cout << setw(5) << products[i].id
                    << setw(20) << products[i].name
                    << setw(15) << products[i].category
                    << setw(10) << products[i].quantity
                    << setw(12) << fixed << setprecision(2) << products[i].price
                    << setw(15) << products[i].supplier << '\n';
                shown++;
            }
            if (shown == 0) cout << "Под фильтр ничего не подошло." << '\n';
            break;
        }

              // ==================== 3. Поиск товаров ====================
        case 3: {
            if (productCount == 0) {
                cout << "Список пуст. Сначала добавьте товары." << '\n';
                break;
            }

            // Вложенное меню поиска
            cout << "\n--- Поиск товаров ---" << '\n';
            cout << "1 — по названию" << '\n';
            cout << "2 — по категории" << '\n';
            cout << "Вариант: ";
            cin >> searchChoice;
            while (cin.fail() || (searchChoice != 1 && searchChoice != 2)) {
                cout << "Введите корректный вариант (1 или 2): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> searchChoice;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Введите текст для поиска (можно часть слова): ";
            getline(cin, searchQuery);
            while (searchQuery.empty()) {
                cout << "Запрос не может быть пустым. Введите текст: ";
                getline(cin, searchQuery);
            }

            found = false;
            for (i = 0; i < productCount; i++) {
                // поиск по подстроке через string::find
                string field = (searchChoice == 1) ? products[i].name : products[i].category;
                if (field.find(searchQuery) != string::npos) {
                    cout << "Найден: ID=" << products[i].id << ", " << products[i].name
                        << ", категория: " << products[i].category
                        << ", кол-во: " << products[i].quantity
                        << ", цена: " << fixed << setprecision(2) << products[i].price << " руб." << '\n';
                    found = true;
                }
            }
            if (!found) cout << "Ничего не найдено." << '\n';
            break;
        }

              // ==================== 4. Сортировка товаров ====================
        case 4: {
            if (productCount == 0) {
                cout << "Список пуст. Сначала добавьте товары." << '\n';
                break;
            }

            // Вложенное меню сортировки
            cout << "\n--- Сортировка ---" << '\n';
            cout << "1 — по количеству (по возрастанию)" << '\n';
            cout << "2 — по цене (по возрастанию)" << '\n';
            cout << "Вариант: ";
            cin >> sortChoice;
            while (cin.fail() || (sortChoice != 1 && sortChoice != 2)) {
                cout << "Введите корректный вариант сортировки (1 или 2): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> sortChoice;
            }

            // Сортировка пузырьком по массиву структур
            for (int a = 0; a < productCount - 1; a++) {
                for (int b = 0; b < productCount - 1 - a; b++) {
                    bool needSwap = (sortChoice == 1)
                        ? (products[b].quantity > products[b + 1].quantity)
                        : (products[b].price > products[b + 1].price);
                    if (needSwap) {
                        Product tmp = products[b];
                        products[b] = products[b + 1];
                        products[b + 1] = tmp;
                    }
                }
            }

            cout << "Список после сортировки:" << '\n';
            for (i = 0; i < productCount; i++) {
                cout << products[i].id << ") " << products[i].name
                    << " — кол-во: " << products[i].quantity
                    << ", цена: " << fixed << setprecision(2) << products[i].price << '\n';
            }
            break;
        }

              // ==================== 5. Поступление / отгрузка ====================
        case 5: {
            if (productCount == 0) {
                cout << "Список пуст. Сначала добавьте товары." << '\n';
                break;
            }

            cout << "Тип операции (1 — поступление, 2 — отгрузка): ";
            cin >> operationType;
            while (cin.fail() || (operationType != 1 && operationType != 2)) {
                cout << "Введите корректный тип операции (1 или 2): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> operationType;
            }

            cout << "ID товара для операции: ";
            cin >> productId;
            while (cin.fail() || productId <= 0) {
                cout << "Введите корректный ID товара (целое положительное число): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> productId;
            }

            // Ищем товар в массиве
            foundIndex = -1;
            for (i = 0; i < productCount; i++) {
                if (products[i].id == productId) { foundIndex = i; break; }
            }
            if (foundIndex == -1) {
                cout << "Товар с ID " << productId << " не найден!" << '\n';
                break;
            }

            cout << "Количество в операции: ";
            cin >> amount;
            while (cin.fail() || amount <= 0) {
                cout << "Введите корректное количество (целое положительное число): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> amount;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Дата операции (гггг-мм-дд): ";
            getline(cin, date);
            while (date.empty()) {
                cout << "Дата не может быть пустой. Введите дату: ";
                getline(cin, date);
            }

            cout << "Причина операции (закупка / продажа / брак / возврат): ";
            getline(cin, reason);
            while (reason.empty()) {
                cout << "Причина не может быть пустой. Введите причину: ";
                getline(cin, reason);
            }

            // Изменение остатка с защитой от ухода в минус
            if (operationType == 1) {
                products[foundIndex].quantity += amount;
                cout << "Поступление выполнено. Новый остаток: " << products[foundIndex].quantity << '\n';
            }
            else if (amount > products[foundIndex].quantity) {
                cout << "ОТКАЗ: на складе только " << products[foundIndex].quantity
                    << " шт., отгрузить " << amount << " нельзя." << '\n';
                break;
            }
            else {
                products[foundIndex].quantity -= amount;
                cout << "Отгрузка выполнена. Новый остаток: " << products[foundIndex].quantity << '\n';
            }

            // Записываем операцию в историю
            if (operationCount < MAX_OPERATIONS) {
                operations[operationCount].operationId = nextOperationId;
                operations[operationCount].operationType = operationType;
                operations[operationCount].productId = productId;
                operations[operationCount].amount = amount;
                operations[operationCount].date = date;
                operations[operationCount].reason = reason;
                operationCount++;
                nextOperationId++;
            }

            // Автоматическое уведомление о недостатке товара
            if (products[foundIndex].quantity < LOW_STOCK_THRESHOLD) {
                cout << "УВЕДОМЛЕНИЕ: остаток \"" << products[foundIndex].name
                    << "\" ниже порога " << LOW_STOCK_THRESHOLD << " шт.!" << '\n';
            }
            break;
        }

              // ==================== 6. Отчёт по остаткам ====================
        case 6: {
            if (productCount == 0) {
                cout << "Склад пуст." << '\n';
                break;
            }

            int totalItems = 0;      // суммарное количество единиц
            double totalValue = 0.0; // суммарная стоимость запаса
            int maxIndex = 0, minIndex = 0;

            for (i = 0; i < productCount; i++) {
                totalItems += products[i].quantity;
                totalValue += products[i].quantity * products[i].price;
                if (products[i].quantity > products[maxIndex].quantity) maxIndex = i;
                if (products[i].quantity < products[minIndex].quantity) minIndex = i;
            }

            cout << "\n----- ОТЧЁТ ПО ОСТАТКАМ -----" << '\n';
            cout << "Позиций на складе: " << productCount << '\n';
            cout << "Всего единиц товара: " << totalItems << '\n';
            cout << "Общая стоимость запаса: " << fixed << setprecision(2) << totalValue << " руб." << '\n';
            cout << "Средняя цена единицы: " << totalValue / totalItems << " руб." << '\n';
            cout << "Больше всего: \"" << products[maxIndex].name << "\" ("
                << products[maxIndex].quantity << " шт.)" << '\n';
            cout << "Меньше всего: \"" << products[minIndex].name << "\" ("
                << products[minIndex].quantity << " шт.)" << '\n';

            cout << "Позиции с низким остатком (<" << LOW_STOCK_THRESHOLD << "):" << '\n';
            for (i = 0; i < productCount; i++) {
                if (products[i].quantity < LOW_STOCK_THRESHOLD) {
                    cout << "  - " << products[i].name << " (" << products[i].quantity << " шт.)" << '\n';
                }
            }
            break;
        }

              // ==================== 7. История операций ====================
        case 7: {
            if (operationCount == 0) {
                cout << "Операций пока нет." << '\n';
                break;
            }
            cout << "\n----- ИСТОРИЯ ОПЕРАЦИЙ -----" << '\n';
            for (i = 0; i < operationCount; i++) {
                string typeText = (operations[i].operationType == 1) ? "ПОСТУПЛЕНИЕ" : "ОТГРУЗКА";
                cout << "#" << operations[i].operationId << " [" << typeText << "] "
                    << "товар ID " << operations[i].productId
                    << ", кол-во: " << operations[i].amount
                    << ", дата: " << operations[i].date
                    << ", причина: " << operations[i].reason << '\n';
            }
            break;
        }

              // ==================== 8. Расчёты по товару ====================
        case 8: {
            if (productCount == 0) {
                cout << "Список пуст. Сначала добавьте товары." << '\n';
                break;
            }

            // Выбираем товар, по которому считаем
            cout << "ID товара для расчётов: ";
            cin >> productId;
            while (cin.fail() || productId <= 0) {
                cout << "Введите корректный ID товара (целое положительное число): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> productId;
            }

            foundIndex = -1;
            for (i = 0; i < productCount; i++) {
                if (products[i].id == productId) { foundIndex = i; break; }
            }
            if (foundIndex == -1) {
                cout << "Товар с ID " << productId << " не найден!" << '\n';
                break;
            }

            // Арифметические расчёты
            double inventoryValue = products[foundIndex].quantity * products[foundIndex].price; // стоимость запаса
            double priceWithVat = products[foundIndex].price + products[foundIndex].price * 0.20;     // цена с НДС 20%
            double bulkDiscountPrice = products[foundIndex].price - products[foundIndex].price * 0.10; // оптовая цена (скидка 10%)

            cout << "\n----- РАСЧЁТЫ ПО ТОВАРУ \"" << products[foundIndex].name << "\" -----" << '\n';
            cout << "Стоимость запаса: " << fixed << setprecision(2) << inventoryValue << " руб." << '\n';
            cout << "Цена с НДС (20%): " << priceWithVat << " руб." << '\n';
            cout << "Цена при опте (скидка 10%): " << bulkDiscountPrice << " руб." << '\n';
            // Целочисленное деление: раскладка запаса по ящикам
            cout << "Полных ящиков по 10 шт. из запаса: " << products[foundIndex].quantity / 10
                << ", остаток: " << products[foundIndex].quantity % 10 << " шт." << '\n';
            break;
        }

              // ==================== 9. Табуляция стоимости партии ====================
        case 9: {
            if (productCount == 0) {
                cout << "Список пуст. Сначала добавьте товары." << '\n';
                break;
            }

            cout << "ID товара для таблицы стоимости: ";
            cin >> productId;
            while (cin.fail() || productId <= 0) {
                cout << "Введите корректный ID товара (целое положительное число): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> productId;
            }

            foundIndex = -1;
            for (i = 0; i < productCount; i++) {
                if (products[i].id == productId) { foundIndex = i; break; }
            }
            if (foundIndex == -1) {
                cout << "Товар с ID " << productId << " не найден!" << '\n';
                break;
            }

            // Таблица: стоимость партии в зависимости от количества.
            // Скидка 10% применяется при покупке от 5 шт.
            double unitPrice = products[foundIndex].price;
            cout << "\nТовар: \"" << products[foundIndex].name << "\", цена за штуку: "
                << fixed << setprecision(2) << unitPrice << " руб." << '\n';
            cout << "\nКол-во | Стоимость | Со скидкой (от 5 шт.)" << '\n';
            for (int k = 1; k <= 10; k++) {
                double cost = k * unitPrice;
                double discounted = (k >= 5) ? cost * 0.9 : cost;
                cout << setw(6) << k << " | " << setw(9) << cost
                    << " | " << setw(9) << discounted << '\n';
            }
            break;
        }

              // ==================== 10. Матрица остатков по складам ====================
        case 10: {
            // Матрица: строки — категории товаров (до 5), столбцы — склады (3 шт.)
            const int MAX_CATEGORIES = 5;
            const int MAX_WAREHOUSES = 3;
            int stockMatrix[MAX_CATEGORIES][MAX_WAREHOUSES];
            int catCount = 0;

            cout << "Сколько категорий учесть (1–5): ";
            cin >> catCount;
            while (cin.fail() || catCount < 1 || catCount > MAX_CATEGORIES) {
                cout << "Введите число от 1 до 5: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> catCount;
            }

            // Ввод матрицы
            for (int r = 0; r < catCount; r++) {
                for (int c = 0; c < MAX_WAREHOUSES; c++) {
                    cout << "Категория " << (r + 1) << ", склад " << (c + 1) << " — количество: ";
                    cin >> stockMatrix[r][c];
                    while (cin.fail() || stockMatrix[r][c] < 0) {
                        cout << "Введите неотрицательное целое: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> stockMatrix[r][c];
                    }
                }
            }

            // Вывод матрицы
            cout << "\nМатрица остатков (строки — категории, столбцы — склады):" << '\n';
            for (int r = 0; r < catCount; r++) {
                for (int c = 0; c < MAX_WAREHOUSES; c++) {
                    cout << setw(8) << stockMatrix[r][c];
                }
                cout << '\n';
            }

            // Суммы по строкам и столбцам, максимум и минимум
            int grandTotal = 0;
            int maxValue = stockMatrix[0][0];
            int minValue = stockMatrix[0][0];

            cout << "\nСуммы по категориям (строкам): ";
            for (int r = 0; r < catCount; r++) {
                int rowSum = 0;
                for (int c = 0; c < MAX_WAREHOUSES; c++) {
                    rowSum += stockMatrix[r][c];
                }
                grandTotal += rowSum;
                cout << rowSum << " ";
            }
            cout << '\n';

            cout << "Суммы по складам (столбцам): ";
            for (int c = 0; c < MAX_WAREHOUSES; c++) {
                int colSum = 0;
                for (int r = 0; r < catCount; r++) {
                    colSum += stockMatrix[r][c];
                }
                cout << colSum << " ";
            }
            cout << '\n';

            for (int r = 0; r < catCount; r++) {
                for (int c = 0; c < MAX_WAREHOUSES; c++) {
                    if (stockMatrix[r][c] > maxValue) maxValue = stockMatrix[r][c];
                    if (stockMatrix[r][c] < minValue) minValue = stockMatrix[r][c];
                }
            }
            cout << "Всего на всех складах: " << grandTotal << '\n';
            cout << "Максимум в ячейке: " << maxValue << ", минимум: " << minValue << '\n';

            // Изменение элемента матрицы
            char changeMatrix = '0';
            cout << "Изменить элемент матрицы? (1 — да, 0 — нет): ";
            cin >> changeMatrix;
            while (cin.fail() || (changeMatrix != '1' && changeMatrix != '0')) {
                cout << "Введите 1 или 0: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> changeMatrix;
            }
            if (changeMatrix == '1') {
                int rIdx = 0, cIdx = 0, newValue = 0;
                cout << "Номер категории (1–" << catCount << "): ";
                cin >> rIdx;
                while (cin.fail() || rIdx < 1 || rIdx > catCount) {
                    cout << "Введите корректный номер: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> rIdx;
                }
                cout << "Номер склада (1–" << MAX_WAREHOUSES << "): ";
                cin >> cIdx;
                while (cin.fail() || cIdx < 1 || cIdx > MAX_WAREHOUSES) {
                    cout << "Введите корректный номер: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> cIdx;
                }
                cout << "Новое значение: ";
                cin >> newValue;
                while (cin.fail() || newValue < 0) {
                    cout << "Введите неотрицательное целое: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> newValue;
                }
                stockMatrix[rIdx - 1][cIdx - 1] = newValue; // переводим 1-based в 0-based индексы

                cout << "Матрица после изменения:" << '\n';
                for (int r = 0; r < catCount; r++) {
                    for (int c = 0; c < MAX_WAREHOUSES; c++) {
                        cout << setw(8) << stockMatrix[r][c];
                    }
                    cout << '\n';
                }
            }
            break;
        }

               // ==================== 11. Полное описание товара (строки) ====================
        case 11: {
            if (productCount == 0) {
                cout << "Список пуст. Сначала добавьте товары." << '\n';
                break;
            }

            cout << "ID товара для полного описания: ";
            cin >> productId;
            while (cin.fail() || productId <= 0) {
                cout << "Введите корректный ID товара (целое положительное число): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> productId;
            }

            foundIndex = -1;
            for (i = 0; i < productCount; i++) {
                if (products[i].id == productId) { foundIndex = i; break; }
            }
            if (foundIndex == -1) {
                cout << "Товар не найден." << '\n';
                break;
            }

            // Конкатенация строк — формирование полного описания через + и +=
            string fullDescription = products[foundIndex].name;
            fullDescription += " (категория: " + products[foundIndex].category + ")";
            fullDescription += " — остаток " + to_string(products[foundIndex].quantity) + " шт.";
            fullDescription += ", цена " + to_string(products[foundIndex].price) + " руб.";
            fullDescription += ", поставщик: " + products[foundIndex].supplier;

            // Добавляем сведения в зависимости от типа товара
            if (products[foundIndex].typeCode == 2) {
                fullDescription += ", срок годности: " + products[foundIndex].expiryDate
                    + ", условия хранения: " + products[foundIndex].storageConditions;
            }
            else if (products[foundIndex].typeCode == 3) {
                fullDescription += ", бренд: " + products[foundIndex].brand
                    + ", гарантия: " + to_string(products[foundIndex].warrantyMonths)
                    + " мес., серийный номер: " + products[foundIndex].serialNumber;
            }
            else if (products[foundIndex].typeCode == 4) {
                fullDescription += ", размер: " + products[foundIndex].size
                    + ", цвет: " + products[foundIndex].color
                    + ", состав ткани: " + products[foundIndex].fabricComposition;
            }

            cout << "\nПолное описание:\n" << fullDescription << '\n';
            // Базовые операции со строками
            cout << "Длина описания: " << fullDescription.length() << " символов." << '\n';
            cout << "Первая буква названия: " << products[foundIndex].name[0] << '\n';

            // Поиск по подстроке в полном описании
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Подстрока для поиска в описании: ";
            getline(cin, searchQuery);
            while (searchQuery.empty()) {
                cout << "Запрос не может быть пустым. Введите текст: ";
                getline(cin, searchQuery);
            }

            if (fullDescription.find(searchQuery) != string::npos) {
                cout << "Подстрока \"" << searchQuery << "\" найдена в описании." << '\n';
            }
            else {
                cout << "Подстрока \"" << searchQuery << "\" не найдена." << '\n';
            }
            break;
        }

               // ==================== Выход ====================
        case 0:
            cout << "Завершение работы. Товаров на складе: " << productCount
                << ", операций в истории: " << operationCount << '\n';
            return 0;

            // Обработка некорректного пункта меню
        default:
            cout << "Функция в разработке или неверный пункт меню." << '\n';
            break;
        }
    }

    return 0;
}

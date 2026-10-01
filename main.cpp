#include <iostream>
#include <limits>
int main() {
	setlocale(LC_ALL, "Russian");
	std::cout << "Система управления складом" << '\n';
	// Product — базовый класс 
	int id;                     // уникальный идентификатор товара
	string name;                // название товара
	string category;            // категория товара
	int quantity;               // количество на складе
	double price;               // цена за единицу
	string supplier;             // поставщик

	//  — скоропортящийся товар 
	string expiryDate;          // срок годности (например, "2026-12-31")
	string storageConditions;   // условия хранения (например, " холодильник +2...+6°C")

	//  — электроника 
	string brand;               // бренд производителя
	int warrantyMonths;         // гарантия в месяцах
	string serialNumber;        // серийный номер

	//  — одежда
	string size;                // размер (например, "M", "L", "42")
	string color;               // цвет
	string fabricComposition;  // состав ткани (например, "80% хлопок, 20% полиэстер")

	//  — операция на складе 
	int operationId;            // ID операции
	int operationType;          // тип операции: 1 — поступление, 2 — отгрузка
	int productId;              // ID связанного товара
	int amount;                 // количество в операции
	string date;                // дата операции (например, "2026-10-01")
	string reason;              // причина (например, "закупка", "продажа", "брак", "возврат")

	//  Переменные для меню и управления складом 
	int menuChoice;             // выбранный пункт главного меню
	int productCount;           // текущее количество товаров в списке
	int operationCount;         // текущее количество записей в истории операций
	int searchChoice;           // вариант поиска (1 — по названию, 2 — по категории)
	int filterChoice;            // вариант фильтрации списка товаров
	int sortChoice;             // вариант сортировки (1 — по количеству, 2 — по цене)
	string searchQuery;         // строка поискового запроса
	const int MAX_PRODUCTS = 100;    // максимальное количество товаров
	const int MAX_OPERATIONS = 1000; // максимальное количество операций
	const int LOW_STOCK_THRESHOLD = 5; // порог для уведомления о недостатке товара



		// ==================== ЦЕЛЫЕ ЧИСЛА (int) ====================

	// id — идентификатор товара
	cout << "Введите ID товара: ";
	cin >> id;
	while (cin.fail() || id <= 0) {
    	cout << "Введите корректный ID (целое положительное число): ";
   	 	cin.clear();
  	 	cin.ignore(numeric_limits<streamsize>::max(), '\n');
   		cin >> id;
	}

	// quantity — количество товара
	cout << "Введите количество товара: ";
	cin >> quantity;
	while (cin.fail() || quantity <= 0) {
    	cout << "Введите корректное количество (целое положительное число): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> quantity;
	}

	// warrantyMonths — гарантия в месяцах
	cout << "Введите гарантию (в месяцах): ";
	cin >> warrantyMonths;
	while (cin.fail() || warrantyMonths <= 0) {
    	cout << "Введите корректный срок гарантии (целое положительное число): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> warrantyMonths;
	}

	// operationType — тип операции (1 — поступление, 2 — отгрузка)
	cout << "Введите тип операции (1 — поступление, 2 — отгрузка): ";
	cin >> operationType;
	while (cin.fail() || (operationType != 1 && operationType != 2)) {
    	cout << "Введите корректный тип операции (1 или 2): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> operationType;
	}

	// productId — ID связанного товара
	cout << "Введите ID товара для операции: ";
	cin >> productId;
	while (cin.fail() || productId <= 0) {
    	cout << "Введите корректный ID товара (целое положительное число): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> productId;
	}

	// amount — количество в операции
	cout << "Введите количество в операции: ";
	cin >> amount;
	while (cin.fail() || amount <= 0) {
    	cout << "Введите корректное количество (целое положительное число): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> amount;
	}

	// menuChoice — выбор пункта главного меню
	cout << "Выберите пункт меню: ";
	cin >> menuChoice;
	while (cin.fail()) {
    	cout << "Введите корректный пункт меню: ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> menuChoice;
	}

	// searchChoice — вариант поиска (1 — по названию, 2 — по категории)
	cout << "Выберите вариант поиска (1 — по названию, 2 — по категории): ";
	cin >> searchChoice;
	while (cin.fail() || (searchChoice != 1 && searchChoice != 2)) {
    	cout << "Введите корректный вариант (1 или 2): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> searchChoice;
	}

	// sortChoice — вариант сортировки (1 — по количеству, 2 — по цене)
	cout << "Выберите сортировку (1 — по количеству, 2 — по цене): ";
	cin >> sortChoice;
	while (cin.fail() || (sortChoice != 1 && sortChoice != 2)) {
    	cout << "Введите корректный вариант сортировки (1 или 2): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> sortChoice;
	}

	// ==================== ВЕЩЕСТВЕННЫЕ ЧИСЛА (double) ====================

	// price — цена товара
	cout << "Введите цену товара: ";
	cin >> price;
	while (cin.fail() || price <= 0) {
    	cout << "Введите корректную цену (положительное число): ";
    	cin.clear();
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    	cin >> price;
	}

	// ==================== СТРОКИ (string) ====================
	// Для строк cin.fail() не сработает, поэтому используем getline и проверку на пустоту

	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка буфера перед getline

	// name — название товара
	cout << "Введите название товара: ";
	getline(cin, name);
	while (name.empty()) {
    	cout << "Название не может быть пустым. Введите название: ";
    	getline(cin, name);
	}

	// category — категория товара
	cout << "Введите категорию товара: ";
	getline(cin, category);
	while (category.empty()) {
    	cout << "Категория не может быть пустой. Введите категорию: ";
    	getline(cin, category);
	}

	// supplier — поставщик
	cout << "Введите поставщика: ";
	getline(cin, supplier);
	while (supplier.empty()) {
    	cout << "Поставщик не может быть пустым. Введите поставщика: ";
    	getline(cin, supplier);
	}

	// expiryDate — срок годности
	cout << "Введите срок годности (гггг-мм-дд): ";
	getline(cin, expiryDate);
	while (expiryDate.empty()) {
    	cout << "Срок годности не может быть пустым. Введите срок годности: ";
    	getline(cin, expiryDate);
	}

	// storageConditions — условия хранения
	cout << "Введите условия хранения: ";
	getline(cin, storageConditions);
	while (storageConditions.empty()) {
    	cout << "Условия хранения не могут быть пустыми. Введите условия: ";
    	getline(cin, storageConditions);
	}

	// brand — бренд
	cout << "Введите бренд: ";
	getline(cin, brand);
	while (brand.empty()) {
    	cout << "Бренд не может быть пустым. Введите бренд: ";
    	getline(cin, brand);
	}

	// serialNumber — серийный номер
	cout << "Введите серийный номер: ";
	getline(cin, serialNumber);
	while (serialNumber.empty()) {
    	cout << "Серийный номер не может быть пустым. Введите серийный номер: ";
    	getline(cin, serialNumber);
	}

	// size — размер одежды
	cout << "Введите размер: ";
	getline(cin, size);
	while (size.empty()) {
    	cout << "Размер не может быть пустым. Введите размер: ";
    	getline(cin, size);
	}

	// color — цвет
	cout << "Введите цвет: ";
	getline(cin, color);
	while (color.empty()) {
    	cout << "Цвет не может быть пустым. Введите цвет: ";
    	getline(cin, color);
	}

	// fabricComposition — состав ткани
	cout << "Введите состав ткани: ";
	getline(cin, fabricComposition);
	while (fabricComposition.empty()) {
    	cout << "Состав ткани не может быть пустым. Введите состав: ";
    	getline(cin, fabricComposition);
	}

	// date — дата операции
	cout << "Введите дату операции (гггг-мм-дд): ";
	getline(cin, date);
	while (date.empty()) {
    	cout << "Дата не может быть пустой. Введите дату: ";
    	getline(cin, date);
	}

	// reason — причина операции
	cout << "Введите причину операции: ";
	getline(cin, reason);
	while (reason.empty()) {
    	cout << "Причина не может быть пустой. Введите причину: ";
    	getline(cin, reason);
	}

	// searchQuery — поисковый запрос
	cout << "Введите текст для поиска: ";
	getline(cin, searchQuery);
	while (searchQuery.empty()) {
    	cout << "Запрос не может быть пустым. Введите текст: ";
    	getline(cin, searchQuery);
	}
	
	return 0;
}

